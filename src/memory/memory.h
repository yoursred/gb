#ifndef __MEMORY_H
#define __MEMORY_H

#include "include.h"
// #include "cpu/cpu.h"
#include <string>
#include <sstream>
#include <vector>
#include <SFML/Window/Keyboard.hpp>

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


/*  Start	End 	Size	Description                 	Notes
       0	3FFF	4000	16 KiB ROM bank 00          	From cartridge, usually a fixed bank
    4000	7FFF	4000	16 KiB ROM Bank 01~NN       	From cartridge, switchable bank via mapper (if any)
    8000	9FFF	2000	8 KiB Video RAM (VRAM)      	In CGB mode, switchable bank 0/1
    A000	BFFF	2000	8 KiB External RAM          	From cartridge, switchable bank if any
    C000	CFFF	1000	4 KiB Work RAM (WRAM)	
    D000	DFFF	1000	4 KiB Work RAM (WRAM)       	In CGB mode, switchable bank 1~7
    E000	FDFF	1E00	Mirror of C000~DDFF (ECHO RAM)	Nintendo says use of this area is prohibited.
    FE00	FE9F	  A0	Sprite attribute table (OAM)	
    FEA0	FEFF	  60	Not Usable                  	Nintendo says use of this area is prohibited
    FF00	FF7F	  80	I/O Registers	
    FF80	FFFE	  7F	High RAM (HRAM)	
    FFFF	FFFF	  1	Interrupt Enable register (IE)	
*/

// Memory offsets for address translation
#define BOOT_ROM_START       0x0
#define ROM_BANK_00_START    0x0
#define ROM_BANK_NN_START 0x4000
#define VRAM_START        0x8000                       
#define ERAM_START        0xA000
#define WRAM_START        0xC000
#define ECHO_RAM_START    0xE000
#define OAM_T_START       0xFE00
#define ILLEGAL_START     0xFEA0
#define IO_R_START        0xFF00
#define HRAM_START        0xFF80

#define BOOT_ROM_END    0x100
#define ROM_BANK_00_END 0x4000
#define ROM_BANK_NN_END 0x8000
#define VRAM_END        0xA000
#define VRAM_CART_END   0xC000
#define ERAM_END        0xD000
#define WRAM_BANK_0_END 0xE000
#define ECHO_RAM_END    0xFE00
#define OAM_T_END       0xFEA0
#define ILLEGAL_END     0xFF00
#define IO_R_END        0xFF80
#define HRAM_END        0xFFFF

#define DPAD 2
#define BTNS 1

typedef struct buttons {
    bool a, b, start, select;
    bool up, down, left, right;
    bool polled, delivered;

    void update(bool (&isKeyPressed)(sf::Keyboard::Key));
    std::string str();
} buttons;

typedef struct joyp {
    byte a_right: 1;
    byte b_left: 1;
    byte select_up: 1;
    byte start_down: 1;
    // bool dpad: 1;
    // bool buttons: 1;
    byte mode: 2;
    byte: 2;

    void update(buttons src);
} joyp;

class Memory {
    public:
    // byte BANK_0[0x4000];
    byte BOOTROM [0x100] = {0};
    byte* BANKS; // [0x10000] = {0xFF}; // MBC5 supports up to 512 ROM banks
    byte VRAM   [0x2000] = {0xFF}; // more clown behaviour
    byte ERAM   [0x2000] = {0xFF}; // MBC5 supports up to 16 RAM banks
    byte WRAM   [0x2000] = {0xFF};
    byte OAM_T   [0xA0] = {0xFF};
    byte IO_R     [0x80] = {0xFF};
    byte HRAM     [0x7F] = {0};
    byte IE;

    buttons btns = {
        false, false, false, false,
        false, false, false, false,
        false, false
    };
    joyp JOYP;
    // bool joyp_wait = false;

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

    bool dma = false;

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


    Memory(byte BOOTROM[], byte ROM[], unsigned int size);
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

    // MemoryProxy operator[](const word value);
    // MemoryProxy operator[](const word* value);
    // byte& operator[](const word value);
    // byte operator[](const word* value);

    // byte raw(word address);
};

typedef Memory::MemoryProxy MP;

#endif