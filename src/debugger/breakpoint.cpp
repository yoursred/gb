#include "debugger/debug.h"
#include <string>
#include <sstream>
#include <iomanip>

Breakpoint::Breakpoint(std::string rs, word value16, byte value8 = 0) {
    // TODO: Expand breakpoint types
    RWATCH_STR_IF(pc)
    RWATCH_STR_IF(af) RWATCH_STR_IF(bc) RWATCH_STR_IF(de) RWATCH_STR_IF(hl)
    RWATCH_STR_IF(a) RWATCH_STR_IF(b) RWATCH_STR_IF(c) RWATCH_STR_IF(d)
    RWATCH_STR_IF(e) RWATCH_STR_IF(h) RWATCH_STR_IF(l)
    RWATCH_STR_IF(wread)  RWATCH_STR_IF(wwrite) RWATCH_STR_IF(wmem)
    if (watch >> 8) {
        address16 = value16;
        value8 = value8;
    } else
    if (watch >> 4) {
        Breakpoint::value16 = value16;
    } else {
        Breakpoint::value8 = value8;
    }
    
}

Breakpoint::Breakpoint(rwatch watch, word value) {
    Breakpoint::value16 = value;
    Breakpoint::watch = watch;
}

Breakpoint::Breakpoint(rwatch watch, byte value) {
    Breakpoint::value8 = value;
    Breakpoint::watch = watch;
    std::stringstream ss; // Hello?
}

Breakpoint::Breakpoint(rwatch watch, word address, byte value) {
    Breakpoint::value8 = value;
    Breakpoint::address16 = address;
    Breakpoint::watch = watch;
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
            break;
        case wread:
            x = ((address16 == cpu.memory.last_read) && cpu.memory.last_read_flag);
            break;
        case wwrite:
            x = ((address16 == cpu.memory.last_wrote) && cpu.memory.last_wrote_flag);
            break;
        case wmem:
            cpu.memory.last_read_flag = true;
            x = value8 == cpu.memory.read(address16);
            break;
    }

    cpu.memory.last_read_flag = false;
    cpu.memory.last_wrote_flag = false;

    return x;
}

std::string Breakpoint::str() {
    // TODO: Change enum to make code more comprehensible
    std::stringstream ss;
    switch (watch) {
        RWATCH_STR_CASE(pc)
        RWATCH_STR_CASE(af)
        RWATCH_STR_CASE(bc)
        RWATCH_STR_CASE(de)
        RWATCH_STR_CASE(hl)
        RWATCH_STR_CASE(a)
        RWATCH_STR_CASE(b)
        RWATCH_STR_CASE(c)
        RWATCH_STR_CASE(d)
        RWATCH_STR_CASE(e)
        RWATCH_STR_CASE(h)
        RWATCH_STR_CASE(l)
        case wread:
            ss << "[" << COUT_HEX_WORD_DS(address16) << "] -->";
            break;
        case wmem:
            ss << "[" << COUT_HEX_WORD_DS(address16) << "] == " << COUT_HEX_BYTE_DS(value8);
            break;
        case wwrite:
            ss << "[" << COUT_HEX_WORD_DS(address16) << "] <--";
            break;
    }
    

    if (watch >> 4 == 1) {
        ss << " == " << COUT_HEX_WORD_DS(value16);
    } else 
    if (watch >> 8 != 1) {
        ss << " == " << COUT_HEX_BYTE_DS(value8);
    }

    return ss.str();
}
