#include "memory/memory.h"

byte Memory::MBC1_read(word address) {
    if (address < 0x4000)
        return BANKS[address + (rom_bank & 0x60) * 0x4000 * mbc1_adv_banking];
    if (address < 0x8000) // Banks 01-XX, 00-XX for MBC5
        return BANKS[address + rom_bank * 0x4000 - 0x4000];
    if (address < 0xC000)
        return ram_enable ? 
        ERAM[address + ((rom_bank >> 5) * mbc1_adv_banking) * 0x2000 - 0xA000] : 0xFF;
    if (address < 0xE000)
        return WRAM[address - 0xC000];
    if (address < 0xFE00)
        return WRAM[address - 0xE000];
    if (address < 0xFEA0)
        return OAM_T[address - 0xFE00];
    if (address < 0xFF00)
        return 0xFF;
    if (address < 0xFF80)
        return IO_R[address - 0xFF00];
    if (address < 0xFFFF)
        return HRAM[address - 0xFF80];
    return IE;
}