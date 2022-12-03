#include "debugger/debug.h"
#include <string>
#include <sstream>
#include <iomanip>

Breakpoint::Breakpoint(std::string rs, word value) {
    STR_RWATCH(pc)
    STR_RWATCH(af) STR_RWATCH(bc) STR_RWATCH(de) STR_RWATCH(hl)
    STR_RWATCH(a) STR_RWATCH(b) STR_RWATCH(c) STR_RWATCH(d)
    STR_RWATCH(e) STR_RWATCH(h) STR_RWATCH(l)
    if (value >> 4) {
        value16 = value;
    } else {
        value8 = value;
    }
    
}

Breakpoint::Breakpoint(rwatch watch, word value) {
    Breakpoint::value16 = value;
    Breakpoint::watch = watch;
}

Breakpoint::Breakpoint(rwatch watch, byte value) {
    Breakpoint::value8 = value;
    Breakpoint::watch = watch;
    std::stringstream ss;
}

bool Breakpoint::match(CPU& cpu) {
    bool x;
    switch (watch) {
        case pc:
            x = value16 == cpu.R.pc;
            break;
        case af:
            x = value16 == cpu.R.af;
            break;
        case bc:
            x = value16 == cpu.R.bc;
            break;
        case de:
            x = value16 == cpu.R.de;
            break;
        case hl:
            x = value16 == cpu.R.hl;
            break;
        case a:
            x = value8 == (cpu.R.af >> 8);
            break;
        case b:
            x = value8 == (cpu.R.bc >> 8);
            break;
        case c:
            x = value8 == (cpu.R.bc & 0xFF);
            break;
        case d:
            x = value8 == (cpu.R.de >> 8);
            break;
        case e:
            x = value8 == (cpu.R.de & 0xFF);
            break;
        case h:
            x = value8 == (cpu.R.hl >> 8);
            break;
        case l:
            x = value8 == (cpu.R.hl & 0xFF);
    }

    return x;
}

std::string Breakpoint::str() {
    std::stringstream ss;
    switch (watch) {
        RWATCH_STR(pc)
        RWATCH_STR(af)
        RWATCH_STR(bc)
        RWATCH_STR(de)
        RWATCH_STR(hl)
        RWATCH_STR(a)
        RWATCH_STR(b)
        RWATCH_STR(c)
        RWATCH_STR(d)
        RWATCH_STR(e)
        RWATCH_STR(h)
        RWATCH_STR(l)
    }

    if (watch >> 4) {
        ss << "==" << COUT_HEX_WORD_DS(value16);
    } else {
        ss << "==" << COUT_HEX_BYTE_DS(value8);
    }

    return ss.str();
}
