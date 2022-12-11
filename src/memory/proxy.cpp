#include "memory/memory.h"
#include <iostream>
#include <iomanip>

#define ASSIGNMENT(op) void MP::operator op## =(byte const& value) {MP::write(MP::read() op value);}
// TODO: explicitely overload all operators
// #define OP(op) void MP::operator op##(byte)

MP::MemoryProxy(Memory* parent, word address) : parent(parent), address(address), mode(MEMORY_MAPPED) {}
MP::MemoryProxy(byte* ptr) : ptr(ptr), mode(DIRECT_POINTER) {}
MP::MemoryProxy(word& ptr16, byte hilo) : mode(CPU_REGISTER), ptr16(&ptr16), hilo(hilo) {}

byte MP::read() const {
    switch (mode) {
        case MEMORY_MAPPED:
            return parent->read(address);
            break;
        case DIRECT_POINTER:
            return *ptr;
            break;
        case CPU_REGISTER:
            if (hilo == R_LO) {
                return *ptr16;
            }
            else { // R_HI
                return (*ptr16) >> 8;
            }
        default:
            return 0xFF; // Something has gone to shits ig
    }
}

void MP::write(byte value) {
    switch (mode) {
        case MEMORY_MAPPED:
            parent->write(address, value);
            break;
        case DIRECT_POINTER:
            *ptr = value;
            break;
        case CPU_REGISTER:
            if (hilo == R_LO) {
                *ptr16 &= 0xFF00;
                *ptr16 |= value;
            }
            else { // R_HI
                *ptr16 &= 0xFF;
                *ptr16 |= (value << 8);
            }
            break;
        default:
            break;
    }
}

MP::operator byte() const {
    return MP::read();
}

MP& MP::operator ++() {
    MP::write(MP::read() + 1);
    return *this;
}

byte MP::operator ++(int) {
    byte temp = *this;
    MP::write(MP::read() + 1);
    return temp;
}

MP& MP::operator --() {
    MP::write(MP::read() + 1);
    return *this;
}

byte MP::operator --(int) {
    byte temp = *this;
    MP::write(MP::read() + 1);
    return temp;
}


// Assignment operators
// TODO: define a destructor

MP& MP::operator= (const byte& value) {
    MP::write(value);
    return *this;
}

MP& MP::operator= (const MP& value) {
    MP::write(value);
    return *this;
}

ASSIGNMENT(+)
ASSIGNMENT(-)
ASSIGNMENT(&)
ASSIGNMENT(|)
ASSIGNMENT(^)