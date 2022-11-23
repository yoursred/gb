#include "memory/memory.h"
#include <ctime>



byte Memory::MBC3_read(word address) {
    if (address < 0x4000) {
        return address;
    }
    if (address < 0x8000) {  // Banks 01-XX, 00-XX for MBC5
        return BANKS[address + rom_bank * 0x4000 - 0x4000];
    }
    if (address < 0xC000) {
        if (ram_bank < 8) {
            return ram_enable ? ERAM[address + ram_bank * 0x2000 - 0xA000] : 0xFF;
        } else if (ram_bank < 0xD) {
            return 0; // TODO: Implement RTC
            // switch (ram_bank) {
            //     case ()
            // }
        }
    }
    if (address < 0xA000) { // WRAM
        return VRAM[address - 0x8000];
    }
    if (address < 0xE000) {
        return WRAM[address - 0xC000];
    }
    if (address < 0xFE00) {
        return WRAM[address - 0xE000];
    }
    if (address < 0xFEA0) {
        return OAM_T[address - 0xFE00];
    }
    if (address < 0xFF00) {
        return 0xFF;
    }
    if (address < 0xFF80) {
        return IO_R[address - 0xFF00];
    }
    if (address < 0xFFFF) {
        return HRAM[address - 0xFF80];
    }
    return IE;
}