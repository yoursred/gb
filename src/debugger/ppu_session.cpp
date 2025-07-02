#include"debugger/debug.h"
// #include"png++/png.hpp"

void Debugger::ppu_debug_main() {
    for (int i = 0; i < (456 * 144); i++) {
        ppu.tick();
    }

}