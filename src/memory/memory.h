#ifndef __MEMORY_H
#define __MEMORY_H

#include "include.h"

#define MODE_ROM 0
#define MODE_MBC1 1
#define MODE_MBC2 2
#define MODE_MBC3 4
#define MODE_MBC5 8
#define MODE_RAM 16
#define MODE_BATTERY 32
#define MODE_TIMER 64

// TODO: Figure out a better name
#define MEMORY_MAPPED 0
#define DIRECT_POINTER 1


/*  Start	End     Description         	        Notes
    0000	3FFF	16 KiB ROM bank 00	            From cartridge, usually a fixed bank
    4000	7FFF	16 KiB ROM Bank 01~NN	        From cartridge, switchable bank via mapper (if any)
    8000	9FFF	8 KiB Video RAM (VRAM)	        In CGB mode, switchable bank 0/1
    A000	BFFF	8 KiB External RAM	            From cartridge, switchable bank if any
    C000	CFFF	4 KiB Work RAM (WRAM)	
    D000	DFFF	4 KiB Work RAM (WRAM)	        In CGB mode, switchable bank 1~7
    E000	FDFF	Mirror of C000~DDFF (ECHO RAM)	Nintendo says use of this area is prohibited.
    FE00	FE9F	Sprite attribute table (OAM)	
    FEA0	FEFF	Not Usable                      Nintendo says use of this area is prohibited
    FF00	FF7F	I/O Registers	
    FF80	FFFE	High RAM (HRAM)	
    FFFF	FFFF	Interrupt Enable register (IE)	
*/

class Memory {
    public:
    // byte BANK_0[0x4000];
    byte BANKS[0x4000]; // MBC5 supports up to 512 RAM banks
    byte VRAM [0x2000];
    byte ERAM [0x2000]; // MBC5 supports up to 16 RAM banks
    byte WRAM [0x2000];
    byte OAM_T [0x100];
    byte IO_R   [0x80];
    byte HRAM   [0x7F];
    byte IE;

    byte RTC_S, RTC_M, RTC_H, RTC_DL, RTC_DH;
    // TODO: clean up above code to look nicer

    word rom_bank = 0;
    byte ram_bank = 0;
    bool ram_enable = false;
    bool mbc1_adv_banking = false;
    byte mbc3_latch_register = 0xFF;
    bool mbc3_latch = false;
    word rom_banks = 0;
    byte ram_banks = 0;
    bool boot_rom = false;

    byte mode;


    Memory(byte ROM[], unsigned int size);

    byte read(word address);
    void write(word address, byte value);
    void write_regs(word address, byte value);

    void tick(byte cycles);

    // private:
    // byte MBCnone_read(word address);
    byte MBC1_read(word address);
    byte MBC2_read(word address);
    byte MBC3_read(word address); 
    byte MBC5_read(word address);

    // void MBCnone(word address, byte value);
    void MBC1(word address, byte value);
    // void MBC2(word address, byte value); 
    // void MBC3(word address, byte value); 
    // void MBC5(word address, byte value); 

    struct MemoryProxy {
        public:
        Memory* parent;
        word address;
        byte mode;
        byte* ptr;

        MemoryProxy(Memory* parent, word address) : parent(parent), address(address), mode(MEMORY_MAPPED) {};
        MemoryProxy(byte* ptr) : ptr(ptr), mode(DIRECT_POINTER) {};

        operator byte() const {
            if (mode == MEMORY_MAPPED) {
                return parent->read(address);
            }
            else {
                return *ptr;
            }
        };
        operator sbyte() const {
            if (mode == MEMORY_MAPPED) {
                return parent->read(address);
            }
            else {
                return *ptr;
            }
        };
        operator word() const {
            if (mode == MEMORY_MAPPED) {
                return parent->read(address);
            }
            else {
                return *ptr;
            }
        };
        // operator int() const;

        void operator =(byte const& value) {
            if (mode == MEMORY_MAPPED) {
                parent->write(address, value);
            }
            else {
                *ptr = value;
            }
        };
    };

    MemoryProxy operator[](const word value);
};

typedef Memory::MemoryProxy MP;

#endif