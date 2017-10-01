#pragma once

#include <memory>
#include "MazeData.h"

class Localizer;

struct MazeProgress
{
    Position currentPos;
    bool isGoal;
    MazeProgress()
    : currentPos()
    , isGoal(false)
    {}
};

class MazeSolver
{
    std::unique_ptr<Localizer> m_pLocalizer;
    MotorInputParam calcNextPosForLearning(const Position& myPos);
    MotorInputParam calcNextPosForSolving(const Position& myPos);
public:
    MazeSolver();
    ~MazeSolver();
    MazeProgress updatePosition(const MotorOutputParam& nowParam);
    void calcOptimalRoute();
    MotorInputParam calcNextPosition(const Position& nowPos, RunMode mode);
};