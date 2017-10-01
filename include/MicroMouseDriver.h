#pragma once
#include <memory>
#include "MazeData.h"

class WallDetector;
class Motor;


class MicroMouseDriver
{   
     std::unique_ptr<WallDetector> m_pWallDetector;
     std::unique_ptr<Motor> m_pMotor;
public:
    MicroMouseDriver();
    ~MicroMouseDriver();
    MotorOutputParam drive(const MotorInputParam& motorParam);
};