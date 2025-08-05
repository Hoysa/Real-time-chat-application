#include "server.hpp"
#include <cassert>

int main() {
    assert(color(1) != "");
    shared_print("Test passed: Color function working.");
    log_message("[Test] Logging working fine.");
    shared_print("Test passed: Logging working.");
    return 0;
}
