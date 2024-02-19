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

int Debugger::split_command(std::string cmd, std::map<std::string, std::string> &tokenized) {
    std::vector<std::string> split = {};
    std::stringstream cmd_ss{cmd};
    std::string token;
    cmd_ss >> token;

    if (token == "break" || token == "b") {

    } else
    if (token == "clear") {

    } else
    if (token == "continue" || token == "c") {

    } else
    if (token == "cycles") {

    } else
    if (token == "delete" || token == "d") {

    } else
    if (token == "dump") {

    } else
    if (token == "exit") {

    } else
    if (token == "flags") {

    } else
    if (token == "g") {

    } else
    if (token == "ic") {

    } else
    if (token == "list" || token == "l") {

    } else
    if (token == "opc") {

    } else
    if (token == "pause") {

    } else
    if (token == "ppu") {

    } else
    if (token == "registers") {

    } else
    if (token == "state") {

    } else
    if (token == "step" || token == "s"){

    } else {

    }





    return 0;
}