#include <iostream>
#include <algorithm>
#include "MazeRunner.h"
#include "MicroMouseDriver.h"
#include "MazeSolver.h"

MazeRunner::MazeRunner()
    : m_pMicroMouseDriver(new MicroMouseDriver()) 
    , m_pMazeSolver(new MazeSolver())
    , m_Pool(ThreadSize)
    , m_Status(Stop)
    , m_Mode(Learning)
{
}

MazeRunner::~MazeRunner()
{

}

// 走行タスク
// 自己位置推定 - 迷路解析 - ロボット制御のフローを実施する
void MazeRunner::runMicroMouseDriver()
{
    std::cout << "start running, input [STOP] for stopping task," << std::endl;
    std::cout << "[RESTART] for restart task, [EXIT] for exit program." << std::endl;
    
    m_Status = Run;
    MotorOutputParam motorOutput;
    MotorInputParam motorInput;

    while(m_Status==Run) // 停止指令またはゴール到着まで動き続ける
    {
        auto progress = m_pMazeSolver->updatePosition(motorOutput);
        
        if(progress.isGoal)
        {
            // 最短経路組み立て
            m_pMazeSolver->calcOptimalRoute();
            m_Mode = Solving;
            break;
        }
        
        motorInput = m_pMazeSolver->calcNextPosition(progress.currentPos, m_Mode);
        
        motorOutput = m_pMicroMouseDriver->drive(motorInput);
    }
    std::cout << "end tasks." << std::endl;
}


void MazeRunner::runAll()
{
    // マイクロマウスドライバのタスク起動
    auto futureForMicroMouseDriver = m_Pool.enqueue([this](){
        runMicroMouseDriver();
    });

    while(1)
    {
        std::string val,commandTxt;
        // コマンド入力待ち
        std::cin >> val;
        commandTxt.resize(val.size());
        // 大文字に変換
        std::transform(val.begin(),val.end(),commandTxt.begin(),toupper);

        if(commandTxt=="STOP") 
        {
            m_Status = Stop;
            futureForMicroMouseDriver.wait();
        }
        else if(commandTxt=="EXIT")
        {
            m_Status = Exit;
            futureForMicroMouseDriver.wait();
            break;
        }
        else if(commandTxt=="RESTART" && m_Status!=Run)
        {   // 再開指令の場合、スレッドを再起動する
            futureForMicroMouseDriver = m_Pool.enqueue([this](){
                runMicroMouseDriver();
            });
        }
    }
}
