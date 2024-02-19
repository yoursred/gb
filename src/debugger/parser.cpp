#include<map>
#include<string>
#include<sstream>
#include<vector>
#include"include.h"

/*
break|b <value> [<type> [<address: type==m>]]
clear
continue|c
cycles
delete|d <index>
dump <start> [<len: $10>]
exit
flags
ic
list|lb|l
opc
pause
ppu
registers
state
step|s
*/

int split_command(std::string cmd, std::map<std::string, std::string> &tokenized, byte x, byte y, word z) {
    std::vector<std::string> split = {};
    
    return 0;
}