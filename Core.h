#pragma once
#include <memory>
#include <mutex>
#include "Process.h"

class Core {
private:
    int coreId;
    std::shared_ptr<Process> currentProcess;
    bool busy;
    std::mutex coreMtx;

public:
    Core(int id);
    
    int getId() const;
    bool isBusy();
    std::shared_ptr<Process> getCurrentProcess();
    
    void assignProcess(std::shared_ptr<Process> process);
    void stepExecution(); // Runs a single execution instruction step
    void releaseProcess();
};