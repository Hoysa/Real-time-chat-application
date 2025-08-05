#include "common.hpp"
#include <fstream>

std::string def_col = "\033[0m";
std::string colors[NUM_COLORS] = {
    "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"
};

std::string color(int code) {
    return colors[code % NUM_COLORS];
}

void log_message(const std::string& message) {
    std::ofstream log_file("chat_log.txt", std::ios::app);
    log_file << message << std::endl;
}
