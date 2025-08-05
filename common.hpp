#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>

#define MAX_LEN 200
#define NUM_COLORS 6

extern std::string def_col;
extern std::string colors[NUM_COLORS];

std::string color(int code);
void log_message(const std::string& message);
