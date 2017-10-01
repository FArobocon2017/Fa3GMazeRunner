#include <thread>
#include "WallDetector.h"
#include "MicroMouseDriver.h"
#include "Motor.h"

MicroMouseDriver::MicroMouseDriver()
    : m_pWallDetector(new WallDetector())
    , m_pMotor(new Motor())
{}

MicroMouseDriver::~MicroMouseDriver()
{}

MotorOutputParam MicroMouseDriver::drive(const MotorInputParam& motorParam)
{
    // モーターに指令
    auto output = m_pMotor->setMotorparam(motorParam.lVal, motorParam.rVal, motorParam.time);
    // 壁との距離を計測
    auto wall = m_pWallDetector->chkWall();
    output.wallDistance[0] = wall.data[0];
    output.wallDistance[1] = wall.data[1];
    output.wallDistance[2] = wall.data[2];
    return output;
}