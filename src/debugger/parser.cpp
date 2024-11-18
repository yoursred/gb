#include<map>
#include<string>
#include<sstream>
#include<vector>
#include"include.h"
#include"debugger/debug.h"

/*
break|b <value> [<type> [<address: type==m>]]
clear
continue|c
cycles
delete|d <index>
dump <start> [<len: $10>]
exit
flags
g
ic
list|lb|l
opc
pause
ppu
registers
state
step|s
*/

bool hex_byte(std::string lit, byte &x) {
    size_t digits, pos;
    unsigned int y;

    pos = lit.find_last_of('$');
    if (pos == 0) {
        lit.erase(0, 1);
    } else if (pos != std::string::npos) {
        return false;
    }

    try {
        y = std::stoul(lit, &digits, 16);
    } catch (std::invalid_argument const&) {
        return false;
    }
    x = y;

    return digits == lit.size() && digits > 0 && (y < 0x100);
}

bool dec_byte(std::string lit, byte &x) {
    size_t digits;
    unsigned int y;

    try {
        y = std::stoul(lit, &digits, 16);
    } catch (std::invalid_argument const&) {
        return false;
    }
    x = y;

    return digits == lit.size() && digits > 0 && (y < 0x100);
}

bool hex_word(std::string lit, word &x) {
    size_t digits, pos;
    unsigned int y;

    pos = lit.find_last_of('$');
    if (pos == 0) {
        lit.erase(0, 1);
    } else if (pos != std::string::npos) {
        return false;
    }
    
    try {
        y = std::stoul(lit, &digits, 16);
    } catch (std::invalid_argument const&) {
        return false;
    }
    x = y;

    return digits == lit.size() && digits > 0 && (y < 0x10000);
}

bool dec_word(std::string lit, word &x) {
    size_t digits;
    unsigned int y;

    try {
        y = std::stoul(lit, &digits, 16);
    } catch (std::invalid_argument const&) {
        return false;
    }
    x = y;

    return digits == lit.size() && digits > 0 && (y < 0x10000);
}

bool dec_uint(std::string lit, uint &x) {
    size_t digits;
    unsigned int y;

    try {
        y = std::stoul(lit, &digits, 16);
    } catch (std::invalid_argument const&) {
        return false;
    }
    x = y;

    return digits == lit.size() && digits > 0;
}

bool str(std::string lit, std::vector<std::string> const& valid) {
    for (auto &&compare: valid) {
        if (lit == compare) {
            return true;
        }
    }
    return false;
}

bool str(std::string lit, size_t min, size_t max) {
    return min < lit.size() && (lit.size() < max || max == std::string::npos);
}


std::vector<std::string> Debugger::split_command(std::string cmd) {
    std::vector<std::string> split = {};
    size_t i;
    // std::string token = cmd.substr(0, i);
    // std::cout << cmd.substr(0, ) << std::endl;
    while (cmd.size()) {
        i = cmd.find(' ');
        if (i != std::string::npos && i < cmd.size()) {
            split.push_back(cmd.substr(0, i));
            cmd.erase(0, i + 1);
        } else {
            split.push_back(cmd);
            break;
        }
    }

    return split;
}