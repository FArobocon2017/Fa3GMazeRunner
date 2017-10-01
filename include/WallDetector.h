#pragma once

class WallDetector
{
public:
    struct Wall
    {
        int data[3];
        Wall():data(){}
    };
    WallDetector(){}
// TODO 壁検知実装
    Wall chkWall() const{return Wall();}
};