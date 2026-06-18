#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

enum State {
    NEW,
    RUNNING,
    WAITING,
    READY,
    FINISHED
};

class Process {
private:
    int pid;
    State state;
    std::string name;
    std::string dateTimeCreated; 
    int assignedCore;
    int currentInstructionIndex;
    std::vector<std::string> instruction_list; // Kept as string for simplicity of print statements

public:
    Process(int pid, std::string name, std::string dateTimeCreated, std::vector<std::string> instructions)
        : pid(pid), name(name), dateTimeCreated(dateTimeCreated), assignedCore(-1),
          instruction_list(instructions), state(NEW), currentInstructionIndex(0) {}

    int getPid() const { return pid; }
    std::string getName() const { return name; }
    std::string getDateTimeCreated() const { return dateTimeCreated; }
    int getAssignedCore() const { return assignedCore; }
    State getState() const { return state; }
    
    int getCurrentLine() const { return currentInstructionIndex; }
    int getTotalLines() const { return instruction_list.size(); }
    
    void setCore(int coreNo) { assignedCore = coreNo; }
    void setState(State s) { state = s; }
    
    int getCurrentInstructionIndex() const { return currentInstructionIndex; }

    const std::vector<std::string>& getInstructions() const { return instruction_list; }

    // Simulates executing one print command line
    void incrementInstruction() {
        if (currentInstructionIndex < instruction_list.size()) {
            currentInstructionIndex++;
        }
    }

    bool isFinished() const {
        return currentInstructionIndex >= instruction_list.size();
    }
};