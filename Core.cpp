#include "Core.h"

Core::Core(int id) : coreId(id), busy(false), currentProcess(nullptr) {}

int Core::getId() const {
    return coreId;
}

bool Core::isBusy() {
    std::lock_guard<std::mutex> lock(coreMtx);
    return busy;
}

std::shared_ptr<Process> Core::getCurrentProcess() {
    std::lock_guard<std::mutex> lock(coreMtx);
    return currentProcess;
}

void Core::assignProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(coreMtx);
    currentProcess = process;
    busy = true;
}

void Core::stepExecution() {
    std::lock_guard<std::mutex> lock(coreMtx);
    if (currentProcess != nullptr) {
        currentProcess->incrementInstruction();
    }
}

void Core::releaseProcess() {
    std::lock_guard<std::mutex> lock(coreMtx);
    currentProcess = nullptr;
    busy = false;
}