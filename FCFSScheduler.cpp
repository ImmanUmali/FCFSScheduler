#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include "ProcessScheduler.h"
#include "PrintManager.h"

// Helper function to get current timestamp 
std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[30];
    struct tm timeinfo;
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&timeinfo, &now); // Runs if on Windows
    #else
        localtime_r(&now, &timeinfo); // Runs if on Mac/Linux
    #endif
    std::strftime(buf, sizeof(buf), "%m/%d/%Y %I:%M:%S%p", &timeinfo);
    return std::string(buf);
}

int main() {
    std::cout << "OS Emulator with 4 Cores...\n";
    
    // Declare 4 cores for the CPU scheduler
    ProcessScheduler scheduler(4); 
    scheduler.start(); 

    // Create 10 processes, each with 100 print commands, upon the start of your OS emulator
    std::vector<std::string> testInstructions(100, "print command placeholder");
    
    for (int i = 1; i <= 10; i++) {
        std::string procName = "process" + (i < 10 ? "0" + std::to_string(i) : std::to_string(i));
        
        auto newProc = std::make_shared<Process>(i, procName, getCurrentTimestamp(), testInstructions);
        scheduler.addProcess(newProc);
    }

    // screen -ls and exit
    std::string command;
    while (true) {
        std::cout << "\nOS_Emulator> ";
        std::getline(std::cin, command);

        if (command == "screen -ls") {
            scheduler.printScreenLS();
        } 
        else if (command == "print") { 
            PrintManager::getInstance().flushToFiles();
        }
        else if (command == "exit") {
            scheduler.shutdown(); 
            std::cout << "OS Emulator with 4 Cores closed.\n";
            break;
        } 
        else if (!command.empty()) {
            std::cout << "Unknown command: " << command << "\n";
        }
    }

    return 0;
}