#pragma once

class WallDetector
{
public:

	// 方向ごとの壁情報の参照方法
	struct tag1{
	enum WallDirectionenum
	{
		Left = 0,
		Forward,
		Right,
		Max = 3,
	};
	};

	////// 以下、属性 //////
	// 壁検知結果を返す構造体
	// 0：未検知、1:検知
    struct Wall
    {
        int data[3]; 
        Wall():data(){}
    };
	Wall wall;

	////// 以下、関数 //////
	// コンストラクタ
	WallDetector(){};
	// デストラクタ
	~WallDetector(){};
	
	// 壁情報を返す
	Wall chkWall();

	// 光センサの計測を延々と続ける関数
	void startLightSensor();

	// 光センサの計測を中止する関数（中身はフラグの変更）
	int stopLightSensor();

	// センサのキャリブレーション
	int calcCoefficient();

private:

	// センサの参照方法
	struct tag2{
	enum SensorAccessenum
	{
		Left = 0,
		Forward1,
		Forward2,
		Right,
		Max = 4,
	};
	};

	////// 以下、属性 //////
	// センサ実行の時間間隔(INIファイルに移行する？)
	int chkInterval = 100;

	// 検知判定の距離(cm)(INIファイルに移行する？)
	const double wallDetectThreshold = 4.0;

	// 履歴の最大保持数(INIファイルに移行する？)
	static const int historyMax = 10;

	// 履歴の保持数(MAX=historyMax)
	int historyHolderNum = 0;

	// 最新の結果が入っている番地
	int historyLatestLocation;

	// 過去の検知結果の履歴(保持数は検知間隔によって要検討)
	struct LumiData
	{
		int lumidata[4];
		LumiData() :lumidata() {}
	};
	LumiData lumihistory[historyMax];

	// 距離データ
	double distdata[3];

	// 逆2乗の法則で、光量のルートを取る前にを割る値
	double coeffDistanceDivision[4] = {200, 200, 200, 200};

	// 逆2乗の法則で、光量を割る前に引く値
	double coeffDistanceSubtraction[4] = { 200, 200, 200, 200 };

	// センサ実行を続けるためのフラグ
	bool isContinueSense = false;

	// デバッグモードかどうか
	bool isDebug = false;


	////// 以下、関数 //////
	// 壁情報の更新
	void updateWallInfo();

	// 距離計算（4センサまとめて）
	void calcDistances();

	// 距離計算
	double calcOneDistance(tag2::SensorAccessenum direction, int lumi);

	// 光センサのキャリブレーション→距離計算の係数を求める
	int getCoefficient();

	// センサの取得値を返す
	void getlumidata(int lumidata[4]);

	// 壁センサクラスの初期化
	void initWallDetector();

	// センサからの距離をファイル出力(dist_history.txt)
	void writeDistancefromEachSensor();

	// 光センサの取得値をファイル出力(light_history.txt)
	void writeLightfromEachSensor();
void get_sensor_sts(int* lumidata);
};