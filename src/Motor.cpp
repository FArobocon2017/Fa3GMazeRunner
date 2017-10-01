#include <thread>
#include "Motor.h"

Motor::Motor()
{}

MotorOutputParam Motor::setMotorparam(int lVal, int rVal, int time)
{
    // 暫定
    std::this_thread::sleep_for(std::chrono::seconds(time));

    return MotorOutputParam();
}