#include <iostream>
#include <fstream>
#include "Buzzer.h"

// �p�ӂ����p�^�[���Ŗ炷
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

// ���g���Ŗ炷
void Buzzer::putBuzzerHz(int hz)
{
	ofstream buzzer("/dev/rtbuzzer0");
	buzzer << hz;
}