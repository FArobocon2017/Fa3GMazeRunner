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
	for (size_t i = 0; i < LightSensor::Sensor::Max; i++)
	{
		string str;
		int num1, num2, num3, num4;
		cout << str << ":" << num1 << " " << num2 << " " << num3 << " " << num4 << endl;
		this->coeffDistanceDivision[LightSensor::Sensor::Left] = num1;
		this->coeffDistanceDivision[LightSensor::Sensor::Forward1] = num2;
		this->coeffDistanceDivision[LightSensor::Sensor::Forward2] = num3;
		this->coeffDistanceDivision[LightSensor::Sensor::Right] = num4;
	}

	// 差分パラメタ取得
	for (size_t i = 0; i < LightSensor::Sensor::Max; i++)
	{
		string str;
		int num1, num2, num3, num4;
		cout << str << ":" << num1 << " " << num2 << " " << num3 << " " << num4 << endl;
		this->coeffDistanceSubtraction[LightSensor::Sensor::Left] = num1;
		this->coeffDistanceSubtraction[LightSensor::Sensor::Forward1] = num2;
		this->coeffDistanceSubtraction[LightSensor::Sensor::Forward2] = num3;
		this->coeffDistanceSubtraction[LightSensor::Sensor::Right] = num4;
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

// 現在の壁情報を返す(Minimum構造)
WallDetector::Wall WallDetector::chkWall()
{
	// 距離計測
	this->calcDistances();

	// 壁情報の更新
	this->updateWallInfo();

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
	
	// センサを1回作動
	this->getlumidataOnce(lumidata);

	// 各方向の距離を求める
	this->distdata[WallDetection::Direction::Left] = this->calcOneDistance(LightSensor::Sensor::Left, lumidata[LightSensor::Sensor::Left]);
	this->distdata[WallDetection::Direction::Right] = this->calcOneDistance(LightSensor::Sensor::Right, lumidata[LightSensor::Sensor::Right]);
	this->distdata[WallDetection::Direction::Forward] = this->calcOneDistance(LightSensor::Sensor::Forward1, lumidata[LightSensor::Sensor::Forward1]);
	this->distdata[WallDetection::Direction::Forward] += this->calcOneDistance(LightSensor::Sensor::Forward2, lumidata[LightSensor::Sensor::Forward2]);

	// 前方は2回あるので、平均を求める
	this->distdata[WallDetection::Direction::Forward] /= 2;

	if (this->isDebug)
	{
		this->writeDistancefromEachSensor();
	}
}

// センサの取得値を返す(1回計測を返す)
void WallDetector::getlumidataOnce(int lumidata[4]) {
	
	// センサ作動
	get_sensor_sts(lumidata);

	// 取得データをそのまま返す
	lumidata[LightSensor::Sensor::Left] = this->lumihistory[0].lumidata[LightSensor::Sensor::Left];
	lumidata[LightSensor::Sensor::Right] = this->lumihistory[0].lumidata[LightSensor::Sensor::Right];
	lumidata[LightSensor::Sensor::Forward1] = this->lumihistory[0].lumidata[LightSensor::Sensor::Forward1];
	lumidata[LightSensor::Sensor::Forward2] = this->lumihistory[0].lumidata[LightSensor::Sensor::Forward2];
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
double WallDetector::calcOneDistance(LightSensor::Sensor direction, int lumi)
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
	for (int i = 0; i < WallDetection::Direction::Max; i++)
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
	ofs << this->distdata[LightSensor::Sensor::Left] << " ";
	ofs << this->distdata[LightSensor::Sensor::Forward1] << " ";
	ofs << this->distdata[LightSensor::Sensor::Forward2] << " ";
	ofs << this->distdata[LightSensor::Sensor::Right] << endl;
	ofs.close();
}

// 光センサの取得値をファイル出力(light_history.txt)
void WallDetector::writeLightfromEachSensor()
{
	ofstream ofs("dist_history.txt", ios::app);
	ofs << this->distdata[LightSensor::Sensor::Left] << " ";
	ofs << this->distdata[LightSensor::Sensor::Forward1] << " ";
	ofs << this->distdata[LightSensor::Sensor::Forward2] << " ";
	ofs << this->distdata[LightSensor::Sensor::Right] << endl;
	ofs.close();
}

