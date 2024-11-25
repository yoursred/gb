#ifndef __GLOBAL_INCLUDE_H
#define __GLOBAL_INCLUDE_H

#include <cstdint>

#ifndef VERSION
#define VERSION "UNKNOWN"
#endif

/* You are not expected to understand this. */
#define IS_BIG_ENDIAN (*(std::uint16_t *)”\0\xff” < 0x100)

#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10
#define CC_Z 0x1
#define CC_NZ 0x2
#define CC_C 0x4
#define CC_NC 0x8



typedef std::uint8_t byte;
typedef std::int8_t sbyte;
typedef std::uint16_t word;
typedef std::int16_t sword;

#define COUT_HEX_WORD_DS(x) "$" << std::setfill('0') << std::setw(4) << std::hex << x
#define COUT_HEX_WORD(x) std::uppercase << std::setfill('0') << std::setw(4) << std::hex << x
#define COUT_HEX_BYTE_DS(x) "$" << std::setfill('0') << std::setw(2) << std::hex << (word) x
#define COUT_HEX_BYTE(x) std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (word) x





#endif