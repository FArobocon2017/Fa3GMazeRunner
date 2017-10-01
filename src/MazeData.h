#pragma once

struct Position
{
    int x;
    int y;
    Position()
    : x(0)
    , y(0)
    {}
};

struct MotorInputParam
{
    int lVal;
    int rVal;
    int time;
    MotorInputParam()
    : lVal(0)
    , rVal(0)
    , time(0)
    {}
};

struct MotorOutputParam
{
    int xAdvanced;
    int yAdvanced;
    int wallDistance[3]; // どういう型にすれば良いか
    MotorOutputParam()
    : xAdvanced(0)
    , yAdvanced(0)
    , wallDistance()
    {}
};

enum RunMode 
{
    Learning,
    Solving
};