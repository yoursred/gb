#ifndef __INSSET_H
#define __INSSET_H
#include "gb.h"

byte get_length(byte opcode);
byte get_cycles(byte opcode);
byte get_length_prefixed(byte opcode);
byte get_cycles_prefixed(byte opcode);
#endif