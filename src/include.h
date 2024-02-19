#ifndef __GLOBAL_INCLUDE_H
#define __GLOBAL_INCLUDE_H

#ifndef VERSION
#define VERSION "UNKNOWN"
#endif

#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10
#define CC_Z 0x1
#define CC_NZ 0x2
#define CC_C 0x4
#define CC_NC 0x8


// TODO: specify size
typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short word;
typedef signed short sword;

#define COUT_HEX_WORD_DS(x) "$" << std::setfill('0') << std::setw(4) << std::hex << x
#define COUT_HEX_WORD(x) std::uppercase << std::setfill('0') << std::setw(4) << std::hex << x
#define COUT_HEX_BYTE_DS(x) "$" << std::setfill('0') << std::setw(2) << std::hex << (word) x
#define COUT_HEX_BYTE(x) std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (word) x





#endif