// #include <iostream>
#include <stdexcept>

#include "include.h"
#include "debugger/debug.h"

void Debugger::cmd_break(std::map<std::string, std::string> args) {
    // poc
    try {
        args.at("type");
        args.at("x");
        args.at("y");
        breakpoints.push_back(Breakpoint(args["type"], x, y));
    }
    catch (std::out_of_range) {
        output << "Syntax error" << std::endl;
    }
}