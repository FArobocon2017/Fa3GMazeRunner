#ifndef _MAZE_RUNNER_
#define _MAZE_RUNNER_

#include <memory>
#include "ThreadPool.h"
#include "MazeData.h"

class MicroMouseDriver;
class MazeSolver;

class MazeRunner
{
    std::unique_ptr<MicroMouseDriver> m_pMicroMouseDriver;
    std::unique_ptr<MazeSolver>  m_pMazeSolver;

    enum ThreadType
    {
        ThreadSize = 2
    };
    enum Status
    {
        Stop,
        Run,
        Exit
    };
    ThreadPool m_Pool;
    Status m_Status;
    RunMode m_Mode;

    void runMicroMouseDriver();   
public:
    MazeRunner();
    ~MazeRunner();
    void runAll();

};

#endif