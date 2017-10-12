#include <iostream>
#include <fstream>
#include "Buzzer.h"

// 用意したパターンで鳴らす
void Buzzer::putBuzzerPattern(int pattern)
{
	ofstream buzzer("/dev/rtbuzzer0");
	switch(pattern)
	{
		case 1:
			buzzer << OFF;
			break;
		case 2:
			buzzer << DO;
			break;
		case 3:
			break;
		default:
			break;
	}
}

// 周波数で鳴らす
void Buzzer::putBuzzerHz(int hz)
{
	ofstream buzzer("/dev/rtbuzzer0");
	buzzer << hz;
}