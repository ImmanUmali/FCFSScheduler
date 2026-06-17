#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
class Instruction {
private:
	std::time_t date_created;
	std::string print_string;
	int assigned_core;
};