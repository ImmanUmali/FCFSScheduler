#pragma once
#pragma once

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

#include "Process.h"

class ProcessScheduler {
public:
    int numCores;
    std::queue<std::shared_ptr<Process>> readyQueue;
    std::vector<std::shared_ptr<Process>> finishedProcesses;

    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;

    ProcessScheduler(int numCores);

    void addProcess(std::shared_ptr<Process> process);
    void run();
    void shutdown();
};