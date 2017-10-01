#include "MazeSolver.h"
#include "Localizer.h"

MazeSolver::MazeSolver()
    : m_pLocalizer(new Localizer())
{}

MazeSolver::~MazeSolver()
{}

// 学習プロセス
MotorInputParam MazeSolver::calcNextPosForLearning(const Position& myPos)
{
    // 1. update wall

    // 2. get next direction

    // 3, set Motor input

    // 現状は仮実装
    MotorInputParam param;
    param.time=1;
    return param;
}
// 最短経路作成プロセス
MotorInputParam MazeSolver::calcNextPosForSolving(const Position& myPos)
{
    // 1. get next direction

    // 2, set Motor input

    // 現状は仮実装
    MotorInputParam param;
    param.time=1;
    return param;
}

MazeProgress MazeSolver::updatePosition(const MotorOutputParam& nowParam)
{
    MazeProgress progress;
    progress.currentPos = m_pLocalizer->calcSelfPosition(nowParam.xAdvanced, nowParam.yAdvanced);
    // ゴール判定は何をもって行うか...
//  progress.isGoal = ...
    return progress;
}

MotorInputParam MazeSolver::calcNextPosition(const Position& myPos, RunMode mode)
{
   // 最適経路プロセス
   if(mode==Solving) return calcNextPosForSolving(myPos);
   // 学習プロセス
   return calcNextPosForLearning(myPos);
}
// 最適経路の計算
void MazeSolver::calcOptimalRoute()
{
    // 仮実装
}