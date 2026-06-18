#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <map>
class Process; 

class PrintManager {
private:
    PrintManager() = default; 
    ~PrintManager() = default;

    PrintManager(const PrintManager&) = delete;
    PrintManager& operator=(const PrintManager&) = delete;

    bool isEnabled = true;
    std::mutex printMutex;
    std::map<std::string, std::vector<std::string>> memoryLogs;

public:
    static PrintManager& getInstance();
    void setEnabled(bool status);

    void logInstruction(const Process& process, int coreId);
    void flushToFiles();
};