#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <thread>
#include <unistd.h>

#include "WallDetector.h"
//#include "..\include\WallDetector.h"

using namespace std;

// コンストラクタ
//WallDetector::WallDetector()
//{
	// 初期ファイル読込
	// 
	// 壁検知のキャリブレーション
//}

// iniファイルの読み込み
void WallDetector::initWallDetector()
{
	ifstream ifs(".\\WallDetector.ini");

	// 分割パラメタ取得
	for (size_t i = 0; i < tag2::SensorAccessenum::Max; i++)
	{
		string str;
		int num1, num2, num3, num4;
		cout << str << ":" << num1 << " " << num2 << " " << num3 << " " << num4 << endl;
		this->coeffDistanceDivision[tag2::SensorAccessenum::Left] = num1;
		this->coeffDistanceDivision[tag2::SensorAccessenum::Forward1] = num2;
		this->coeffDistanceDivision[tag2::SensorAccessenum::Forward2] = num3;
		this->coeffDistanceDivision[tag2::SensorAccessenum::Right] = num4;
	}

	// 差分パラメタ取得
	for (size_t i = 0; i < tag2::SensorAccessenum::Max; i++)
	{
		string str;
		int num1, num2, num3, num4;
		cout << str << ":" << num1 << " " << num2 << " " << num3 << " " << num4 << endl;
		this->coeffDistanceSubtraction[tag2::SensorAccessenum::Left] = num1;
		this->coeffDistanceSubtraction[tag2::SensorAccessenum::Forward1] = num2;
		this->coeffDistanceSubtraction[tag2::SensorAccessenum::Forward2] = num3;
		this->coeffDistanceSubtraction[tag2::SensorAccessenum::Right] = num4;
	}
	ifs.close();
}

// 延々とセンサ計測と壁情報の更新を行うメソッド
void WallDetector::startLightSensor()
{
	this->isContinueSense = true;
	int i = 0;
	// フラグが立ち続ける限り計測＆壁情報更新
	while (this->isContinueSense)
	{ i++;
		if(i > 100) break;
		// 距離計測
		this->calcDistances();

		// 壁情報の更新
		this->updateWallInfo();

		// スリープ
		//std::this_thread::sleep_for(std::chrono::microseconds(this->chkInterval));
		usleep(100000);
		
	}
}

// 現在の壁情報を返す
WallDetector::Wall WallDetector::chkWall()
{
	return this->wall;
}

// 光センサの計測を中止する関数（中身はフラグの変更のみ）
int WallDetector::stopLightSensor()
{
	try
	{
		this->isContinueSense = false;
	}
	catch (const std::exception&)
	{
		return -1;
	}

	return 0;
}

// 距離を求める
void WallDetector::calcDistances()
{
	// 変数の初期化
	int lumidata[4] = { 0 }; // 輝度値[4]
	
	this->getlumidata(lumidata);
	// 各方向の距離を求める
	this->distdata[tag1::WallDirectionenum::Left] = this->calcOneDistance(tag2::SensorAccessenum::Left, lumidata[tag2::SensorAccessenum::Left]);
	this->distdata[tag1::WallDirectionenum::Right] = this->calcOneDistance(tag2::SensorAccessenum::Right, lumidata[tag2::SensorAccessenum::Right]);
	this->distdata[tag1::WallDirectionenum::Forward] = this->calcOneDistance(tag2::SensorAccessenum::Forward1, lumidata[tag2::SensorAccessenum::Forward1]);
	this->distdata[tag1::WallDirectionenum::Forward] += this->calcOneDistance(tag2::SensorAccessenum::Forward2, lumidata[tag2::SensorAccessenum::Forward2]);

	// 前方は2回あるので、平均を求める
	this->distdata[tag1::WallDirectionenum::Forward] /= 2;

	if (this->isDebug)
	{
		this->writeDistancefromEachSensor();
	}
}

// センサの取得値を返す
void WallDetector::getlumidata(int lumidata[4]) {
	
	// 取得データをそのまま返す
	get_sensor_sts(lumidata);
	lumidata[tag2::SensorAccessenum::Left] = this->lumihistory[0].lumidata[tag2::SensorAccessenum::Left];
	lumidata[tag2::SensorAccessenum::Right] = this->lumihistory[0].lumidata[tag2::SensorAccessenum::Right];
	lumidata[tag2::SensorAccessenum::Forward1] = this->lumihistory[0].lumidata[tag2::SensorAccessenum::Forward1];
	lumidata[tag2::SensorAccessenum::Forward2] = this->lumihistory[0].lumidata[tag2::SensorAccessenum::Forward2];
}


// 取得データをそのまま返す
void WallDetector::get_sensor_sts(int* lumidata) {
	std::ifstream ifs("/dev/rtlightsensor0");
	ifs >> lumidata[0] >> lumidata[1] >> lumidata[2] >> lumidata[3];
}

// 取得履歴からデータを返す
void get_sensor_sts2(int* lumidata) {
	std::ifstream ifs("/dev/rtlightsensor0");
	ifs >> lumidata[0] >> lumidata[1] >> lumidata[2] >> lumidata[3];
	
	
}

// 距離計算(1つ)
double WallDetector::calcOneDistance(tag2::SensorAccessenum direction, int lumi)
{
	double dist = 0;
	double coeffSub = this->coeffDistanceSubtraction[direction];
	double coeffDiv = this->coeffDistanceDivision[direction];

	dist = sqrt((lumi - coeffSub) / coeffDiv);

	return dist;
}

// 壁情報の更新
void WallDetector::updateWallInfo()
{
	for (int i = 0; i < tag1::WallDirectionenum::Max; i++)
	{
		if (this->distdata[i] < this->wallDetectThreshold)
		{
			this->wall.data[i] = 1;
		}
		else
		{
			this->wall.data[i] = 0;
		}
	}
}


// センサからの距離をファイル出力(dist_history)
void WallDetector::writeDistancefromEachSensor()
{
	ofstream ofs("dist_history.txt", ios::app);
	ofs << this->distdata[tag2::SensorAccessenum::Left] << " ";
	ofs << this->distdata[tag2::SensorAccessenum::Forward1] << " ";
	ofs << this->distdata[tag2::SensorAccessenum::Forward2] << " ";
	ofs << this->distdata[tag2::SensorAccessenum::Right] << endl;
	ofs.close();
}

// 光センサの取得値をファイル出力(light_history.txt)
void WallDetector::writeLightfromEachSensor()
{
	ofstream ofs("dist_history.txt", ios::app);
	ofs << this->distdata[tag2::SensorAccessenum::Left] << " ";
	ofs << this->distdata[tag2::SensorAccessenum::Forward1] << " ";
	ofs << this->distdata[tag2::SensorAccessenum::Forward2] << " ";
	ofs << this->distdata[tag2::SensorAccessenum::Right] << endl;
	ofs.close();
}

