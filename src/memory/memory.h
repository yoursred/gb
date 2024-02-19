#ifndef __MEMORY_H
#define __MEMORY_H

#include "include.h"
// #include "cpu/cpu.h"
#include <string>
#include <sstream>
#include <vector>

class CPU;

struct mem_write {
    size_t cpu_steps;
    word address;
    byte value;

    mem_write(size_t _, word __, byte ___) {
        cpu_steps = _;
        address = __;
        value = ___;
    }
};

struct mem_read {
    size_t cpu_steps;
    word address;
    byte value;

    mem_read(size_t _, word __, byte ___) {
        cpu_steps = _;
        address = __;
        value = ___;
    }
};

#define MODE_ROM 0
#define MODE_MBC1 1
#define MODE_MBC2 2
#define MODE_MBC3 4
#define MODE_MBC5 8
#define MODE_RAM 16
#define MODE_BATTERY 32
#define MODE_TIMER 64

#define ASSIGNMENT_DECLARE(op) void operator op## =(byte const& value);



// TODO: Figure out a better name
#define MEMORY_MAPPED 0
#define DIRECT_POINTER 1
#define CPU_REGISTER 2

#define R_LO 0
#define R_HI 1


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
    byte BANKS[0x10000]; // MBC5 supports up to 512 RAM banks
    byte VRAM  [0x2000]; // more clown behaviour
    byte ERAM  [0x2000]; // MBC5 supports up to 16 RAM banks
    byte WRAM  [0x2000];
    byte OAM_T  [0x100];
    byte IO_R    [0x80];
    byte HRAM    [0x7F];
    byte IE;

    byte RTC_S, RTC_M, RTC_H, RTC_DL, RTC_DH;

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

    CPU *cpu;

    std::stringstream serial_o;
    std::vector<mem_write> mem_writes;
    std::vector<mem_read> mem_reads;
    size_t cpu_steps = 0;

    // Debugger stuff
    word last_read;
    bool last_read_flag = false;
    word last_wrote;
    bool last_wrote_flag = false;


    Memory(byte ROM[], unsigned int size);
    Memory(byte BOOTROM[]);

    byte read(word address);
    void write(word address, byte value);
    void write_regs(word address, byte value);

    byte raw_read(word address);
    void raw_write(word address, byte value);

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
        // TODO: Explain
        public:
        Memory* parent;
        word address;
        byte* ptr;
        byte mode;

        word* ptr16;
        byte hilo;

        MemoryProxy(Memory* parent, word address);
        MemoryProxy(byte* ptr);
        MemoryProxy(word& ptr16, byte hilo);

        byte read() const;
        void write(byte value);

        operator byte() const;
        
        MemoryProxy& operator ++(); // prefix
        MemoryProxy& operator --(); // prefix
        byte operator ++(int); // prefix
        byte operator --(int); // prefix
        

        MemoryProxy& operator =(const byte& value);
        MemoryProxy& operator =(const MemoryProxy& value);

        ASSIGNMENT_DECLARE(+)
        ASSIGNMENT_DECLARE(-)
        ASSIGNMENT_DECLARE(&)
        ASSIGNMENT_DECLARE(|)
        ASSIGNMENT_DECLARE(^)

        // void operator --(void) {
        // I am not doing this, what is wrong with x = x - 1?
        // Future me: I'm doing it, aren't I?
        // }
    };

    MemoryProxy operator[](const word value);
    MemoryProxy operator[](const word* value);

    // byte raw(word address);
};

typedef Memory::MemoryProxy MP;

#endif