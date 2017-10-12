#include <iostream>
#include <fstream>
#include "TactSW.h"

using namespace std;

void TactSW::getTactSwStsAll(int *tswSts)
{
	ifstream ifs0("/dev/rtswitch0");
	ifs0 >> tsw_sts[0];
	ifstream ifs1("/dev/rtswitch1");
	ifs1 >> tsw_sts[1];
	ifstream ifs2("/dev/rtswitch2");
	ifs2 >> tsw_sts[2];

	/*
	// Debug
	cout 	<< tsw_sts[0] << " " 
		<< tsw_sts[1] << " "  
		<< tsw_sts[2] << endl;
	//*/
}


int TactSW::getTactSwSts1()
{
	int tswSts = 0;
	ifstream ifs("/dev/rtswitch0");
	ifs >> tswSts;

	return tswSts;
}

int TactSW::getTactSwSts2()
{
	int tswSts = 0;
	ifstream ifs("/dev/rtswitch1");
	ifs >> tswSts;

	return tswSts;
}

int Mouse::getTactSwSts3()
{
	int tswSts = 0;
	ifstream ifs("/dev/rtswitch2");
	ifs >> tswSts;

	return tswSts;
}