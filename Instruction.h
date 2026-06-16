#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
class Instruction {
private:
	std::time_t date_created;
	std::string print_string;
	std::int assigned_core;
};