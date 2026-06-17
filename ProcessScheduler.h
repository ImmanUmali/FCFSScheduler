#pragma once

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

#include "Process.h"
#include "Core.h" 

class ProcessScheduler {
public:
    int numCores;
    std::queue<std::shared_ptr<Process>> readyQueue;
    std::vector<std::shared_ptr<Process>> finishedProcesses;
    
    std::vector<std::shared_ptr<Core>> cpuCores; 

    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;
    std::vector<std::thread> workers;

    ProcessScheduler(int numCores);

    void addProcess(std::shared_ptr<Process> process);
    void start();
    void shutdown();
    void printScreenLS();
};