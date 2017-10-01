#pragma once

#include "MazeData.h"

class Motor
{
public:
    Motor();
    MotorOutputParam setMotorparam(int lVal, int rVal, int time);
};