#include <iostream>
#include <string.h>
#include "memory/memory.h"

Memory::Memory(byte ROM[], unsigned int size) {
    switch (ROM[0x147]) { // Cart type
        case (0x00): mode = MODE_ROM; break;
        case (0x01): mode = MODE_MBC1; break;
        case (0x02): mode = MODE_MBC1 | MODE_RAM; break;
        case (0x03): mode = MODE_MBC1 | MODE_RAM | MODE_BATTERY; break;
        case (0x05): mode = MODE_MBC2; break;
        case (0x06): mode = MODE_MBC2 | MODE_BATTERY; break;
        case (0x08): mode = MODE_RAM; break;
        case (0x09): mode = MODE_RAM | MODE_BATTERY; break;
        case (0x0F): mode = MODE_MBC3 | MODE_TIMER | MODE_BATTERY; break;
        case (0x10): mode = MODE_MBC3 | MODE_TIMER | MODE_RAM | MODE_BATTERY; break;
        case (0x11): mode = MODE_MBC3; break;
        case (0x12): mode = MODE_MBC3 | MODE_RAM; break;
        case (0x13): mode = MODE_MBC3 | MODE_RAM | MODE_BATTERY; break;
        case (0x19):
        case (0x1C): mode = MODE_MBC5; break;
        case (0x1A):
        case (0x1D): mode = MODE_MBC5 | MODE_RAM; break;
        case (0x1B):
        case (0x1E): mode = MODE_MBC5 | MODE_RAM | MODE_BATTERY; break;
        default: 
            std::cerr << "Scheisse! I don't know what that is!" << std::endl;
            exit(-1);
    }

    if (ROM[0x148] > 8) { // ROM size
        std::cerr << "Scheisse! I don't know what that is!" << std::endl;
        exit(-1);
    }
    rom_banks = 2 * (1 << ROM[0x148]);
    
    switch (ROM[0x149]) { // RAM size
        case (2): ram_banks =  1; break;
        case (3): ram_banks =  4; break;
        case (4): ram_banks = 16; break; // Nintendo!
        case (5): ram_banks =  8; break; // Why?
        default:
            std::cerr << "Scheisse! I don't know what that is!" << std::endl;
            exit(-1);
    }
    if (!(mode & MODE_MBC5)) {
        rom_bank = 1;
    }

    if (size > (0x4000 * rom_banks)) {
        std::cerr << "Scheisse! I don't know what that is!" << std::endl;
        exit(-1);
    }

    // It's populatin' time!
    memcpy(BANKS, ROM, size);
}

byte Memory::read(word address) {
    if (address < 0x4000) { // Bank 00/X0
        if (mode & MODE_MBC1) {
            return BANKS[address + (rom_bank & 0x60) * 0x4000 * mbc1_adv_banking];
        }
        return BANKS[address];
    }
    if (address < 0x8000) { // Banks 01-XX, 00-XX for MBC5
        return BANKS[address + rom_bank * 0x4000 - 0x4000];
    }
    if (address < 0xA000) // WRAM
        return VRAM[address - 0x8000];
    if (address < 0xC000) {
        if (mode & MODE_MBC2) {
            // The only edge case when reading ERAM is with MBC2
            // It has 0x1FF half-bytes repeated 16 times
            return ram_enable ? ERAM[(address & 0x1FF) - 0xA000] & 0xF : 0xF;
        }
        if (mode & MODE_MBC1) {
            // Lol "only edge case" my ass
            return 
                ram_enable ? ERAM[address + ((rom_bank >> 5) * mbc1_adv_banking) * 0x2000 - 0xA000] : 0xFF;
        }
        
        return ram_enable ? ERAM[address + ram_bank * 0x2000 - 0xA000] : 0xFF;
    }
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

void Memory::write(word address, byte value) {
    if (address < 0x8000) {
        write_regs(address, value);
    }
    else if (address < 0xA000)
        VRAM[address - 0x8000] = value;
    else if (address < 0xC000) {
        if (ram_enable)
            ERAM[address + ram_bank * 0x2000 - 0xA000] = value;
    }
    else if (address < 0xE000)
        WRAM[address - 0xC000] = value;
    else if (address < 0xFE00)
        return;
    else if (address < 0xFEA0)
        OAM_T[address - 0xFE00] = value;
    else if (address < 0xFF00)
        return;
    else if (address < 0xFF80)
        IO_R[address - 0xFF00] = value;
    else if (address < 0xFFFF)
        HRAM[address - 0xFF80] = value;
    else
        IE = value;
}

void Memory::write_regs(word address, byte value) {
    switch (mode & 0xF) {
        case (MODE_MBC1):
            if (address < 0x2000) {
                ram_enable = (value == 0xA);
            } else if (address < 0x4000) {
                value &= 0x1F;
                if (value == 0) value = 1;
                rom_bank &= 0xFFC0; // This zeroes out the bottom 5 bits
                rom_bank |= (value & (rom_banks - 1)); // This updates them with only the supported bits
            } else if (address < 0x6000) {
                rom_bank &= 0xFF9F;
                rom_bank |= (value & 2) << 5;
            } else {
                mbc1_adv_banking = value;
            }
            break;
        case (MODE_MBC2):
            if (address < 0x4000) {
                if (address & 0x100) { // Bit 8 is set, lower 4 bits are rom_bank
                    rom_bank = value & 0xF;
                }
                else {
                    ram_enable = (value == 0xA);
                }
            }
        break;
        case (MODE_MBC3):
            if (address < 0x2000) {
                ram_enable = (value == 0xA);
            } else if (address < 0x4000) {
              if (value == 0) value = 1;
              rom_bank = value;  
            } else if (address < 0x6000) {
                ram_bank = value;
            } else {
                
            }
            
            break;
        case (MODE_MBC5):
            
            break;
    }
}