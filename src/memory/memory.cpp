#include <iostream>
#include <iomanip>
#include <cstring>
#include "memory/memory.h"
#include "cpu/cpu.h"

void buttons::update(bool (&isKeyPressed)(sf::Keyboard::Key)) {
    a = isKeyPressed(sf::Keyboard::Key::X);
    b = isKeyPressed(sf::Keyboard::Key::Z);
    select = isKeyPressed(sf::Keyboard::Key::Backspace);
    start = isKeyPressed(sf::Keyboard::Key::Enter);
    right = isKeyPressed(sf::Keyboard::Key::Right);
    left = isKeyPressed(sf::Keyboard::Key::Left);
    up = isKeyPressed(sf::Keyboard::Key::Up);
    down = isKeyPressed(sf::Keyboard::Key::Down);
    delivered = true; // With love
}

void joyp::update(buttons src) {
    a_right = true;
    b_left = true;
    select_up = true;
    start_down = true;
    switch (mode) {
        case BTNS:
            a_right = !src.a;
            b_left = !src.b;
            select_up = !src.select;
            start_down = !src.start;
        break;
        case DPAD:
            a_right = !(!src.left && src.right);
            b_left = !(!src.right && src.left);
            select_up = !(!src.down && src.up);
            start_down = !(!src.up && src.down);
        break;
    }
}

std::string buttons::str() {
    /*
       ^        A  B
    <  O  >      
       v    ST SL
    */
    std::stringstream s;

    s << "   " << (up ? '^' : ' ') << "        " << (a ? 'A' : ' ') << "  " << (b ? 'B' : ' ') << std::endl;
    s << (left ? '<' : ' ') << "  O  " << (right ? '>' : ' ') << std::endl;
    s << "   " << (down ? 'v' : ' ') << "    " << (start ? "ST" : "  ") << ' ' << (select ? "SL" : "  ") << std::endl;

   return s.str();
}

rom_header::rom_header(byte ROM[]) {
    static_assert(sizeof(rom_header) == ROM_HEADER_SIZE);
    std::memcpy(this, ROM + 0x100, sizeof(rom_header));
}

// Memory::Memory() {}

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
            std::cerr << "ROM type error" << std::endl;
            exit(-1);
    }

    if (ROM[0x148] > 8) { // ROM size
        std::cerr << "ROM bank count error" << std::endl;
        std::cerr << "Got " << (int) ROM[0x147] << ", expected < 8" << std::endl;
        exit(-1);
    }
    rom_banks = 2 * (1 << ROM[0x148]);

    std::cout << "ROM banks: " << rom_banks << std::endl;
    
    switch (ROM[0x149]) { // RAM size
        case (0): ram_banks =  0; break;
        case (2): ram_banks =  1; break;
        case (3): ram_banks =  4; break;
        case (4): ram_banks = 16; break; // Nintendo!
        case (5): ram_banks =  8; break; // Why?
        default:
            std::cerr << "RAM bank count error" << std::endl;
            exit(-1);
    }

    if (mode & MODE_MBC2) {
        ram_banks = 1;
    }

    rom_bank = 1;

    if (size > (0x4000 * rom_banks)) {
        std::cerr << "ROM size error" << std::endl;
        exit(-1);
    }

    if ((mode & MODE_MBC1) && (rom_banks > 16)) {
        word checksum = 0;
        for (auto i = (0x104 + 32 * ROM_BANK_SIZE); i < (0x134 + 32 * ROM_BANK_SIZE); i++) {
            checksum += ROM[i];
        }
        if (checksum == 0x1546) {
            std::cout << "Got multicart!" << std::endl;
            mode |= MODE_MULTICART;
        }
    }

    BANKS = new byte[size];
    ERAM = new byte[ram_banks * RAM_BANK_SIZE];

    // It's populatin' time!
    std::memcpy(BANKS, ROM, size);

    JOYP = {
        1, 1, 1, 1,
        3
    };
}

Memory::Memory(byte BOOTROM[], byte ROM[], unsigned int size)
    : Memory(ROM, size) {
    if (BOOTROM != nullptr) {
        std::memcpy(Memory::BOOTROM, BOOTROM, BOOT_ROM_END);
        boot_rom = true;
    }
}

Memory::Memory(byte BOOTROM[]) {
    if (!rom_banks) {
        rom_banks = 2;
        ram_banks = 0;
    }
    std::memcpy(Memory::BOOTROM, BOOTROM, BOOT_ROM_END);
    BANKS = new byte[0x8000];
    boot_rom = true;
}

byte Memory::read(word address) {
    if (!last_read_flag) {
        last_read = address;
        last_read_flag = true;
    }
    
    if (boot_rom && address < BOOT_ROM_END) { // Short circuit sooner
        return BOOTROM[address];
    }

    if (address < ROM_BANK_00_END) {
        if ((mode & MODE_MULTICART) && mbc1_adv_banking) {
            return BANKS[
                (address + (rom_bank & 0b110000) * ROM_BANK_SIZE) &
                (rom_banks * ROM_BANK_SIZE - 1)
            ];
        } else if ((mode & MODE_MBC1) && (rom_banks > 32) && mbc1_adv_banking) {
            return BANKS[
                (address + (rom_bank & 0b1100000) * ROM_BANK_SIZE) &
                (rom_banks * ROM_BANK_SIZE - 1)
            ];
        }
        return BANKS[address];
    }
    if (address < ROM_BANK_NN_END) { // Banks 01-XX, 00-XX for MBC5
        return BANKS[
            (address + rom_bank * ROM_BANK_SIZE - ROM_BANK_NN_START) & 
            (rom_banks * ROM_BANK_SIZE - 1)
        ];
    }
    if (address < VRAM_END) { // VRAM
        return VRAM[address - VRAM_START];
        // VRAM/OAM blocking is not recommended
        // if (((IO_R[0x41] & 3) == 3) && (IO_R[0x40] & 0x80)) {
        //     return 0xFF;
        // } else {
        //     return VRAM[address - 0x8000];
        // }
    }
    if (address < ERAM_END) {
        if (ram_enable && ram_banks) {
            if ((mode & MODE_MBC1) && !mbc1_adv_banking) {
                return ERAM[address - ERAM_START];
            } else if (mode & MODE_MBC2) {
                // MBC2 only has 512 half-bytes of ram
                // Apparently the upper nybble returned should be 0xF
                // https://github.com/LIJI32/SameBoy/blob/aff7f1706c380861eecadae2a3e9d54ebb66db27/Core/memory.c#L456
                return ERAM[(address - ERAM_START) & 0x1FF] | 0xF0;
            } else {
                return ERAM[address + ram_bank * RAM_BANK_SIZE - ERAM_START];
            }
        }
        return 0xFF;
    }
    if (address < WRAM_END)
        return WRAM[address - WRAM_START]; // TODO: CGB
    if (address < ECHO_RAM_END)
        return WRAM[address - ECHO_RAM_START];
    if (address < OAM_T_END)
        return OAM_T[address - OAM_T_START]; // VRAM/OAM blocking is not recommended
        
        // if ((IO_R[0x41] & 2)) {
        //     return 0xFF;
        // }
        // else {
        //     return OAM_T[address - 0xFE00];
        // }
    if (address == JOYP_ADDRESS) {
        JOYP.update(btns);
        return *(byte*) &JOYP;
    }
    if (address == DIV_ADDRESS)
        return cpu->div_timer >> 6;
    if (address < IO_R_END)
        return IO_R[address - IO_R_START];
    if (address < HRAM_END)
        return HRAM[address - HRAM_START];
    return IE;
}

void Memory::write(word address, byte value) {
    if (!last_wrote_flag) {
        last_wrote = address;
        last_wrote_flag = true;
    }
    if (dma && (address < HRAM_START)) {
        
    } else if (address < ROM_BANK_NN_END) {
        write_regs(address, value);
    }
    else if (address < VRAM_END) { // VRAM/OAM blocking is not recommended
        VRAM[address - VRAM_START] = value;
    //     if (((IO_R[0x41] & 3) != 3) || !(IO_R[0x40] & 0x80))
    //         VRAM[address - 0x8000] = value;
    }
    else if (address < ERAM_END) {
        if (ram_enable && ram_banks) {
            if ((mode & MODE_MBC1) && !mbc1_adv_banking) {
                ERAM[address - ERAM_START] = value;
            } else if (mode & MODE_MBC2) {
                // MBC2 only has 512 half-bytes of ram
                ERAM[(address - ERAM_START) & 0x1FF] = value & 0xF;
            } else {
                ERAM[address + ram_bank * RAM_BANK_SIZE - ERAM_START] = value;
            }
        }
    }
    else if (address < WRAM_END)
        WRAM[address - WRAM_START] = value;
    else if (address < ECHO_RAM_END);
        // return; // clown behaviour
    else if (address < OAM_T_END) { // VRAM/OAM blocking is not recommended
        OAM_T[address - OAM_T_START] = value;
    }
    else if (address < ILLEGAL_END) {
        // TODO: OAM corruption bug
    } else if (address == JOYP_ADDRESS) {
        JOYP.mode = (value >> 4) & 0x3;
        btns.polled = ~JOYP.mode;
        btns.delivered = false;
    }
    else if (address == STAT_ADDRESS)
        IO_R[0x41] = (value & 0b1111'1000) | IO_R[0x41];
    else if (address == LY_ADDRESS);
    else if (address == LYC_ADDRESS) {
        IO_R[0x45] = value;
        if (value == IO_R[0x44]) {
            IO_R[0x41] |= 4;
        }
    }
    else if (address == DMA_ADDRESS) {
        dma = true;
        IO_R[0x46] = value & 0xDF;
        // std::cout << "Requesting DMA transfer" << std::endl;
    }
    else if (address == BOOTROM_UNMAP) 
        boot_rom = false;
    else if (address < IO_R_END) // I'm not a clown, I'm the whole circus
        IO_R[address - IO_R_START] = value;
    else if (address < HRAM_END)
        HRAM[address - HRAM_START] = value;
    else
        IE = value;
    

    if (address == 0xFF02 && value == 0x81) { // Basic serial output
        std::cerr << (char) IO_R[1];
        if (IO_R[1] == '\n')
            std::cout << "> ";
    }
    if (address == DIV_ADDRESS) {
        cpu->div_timer = 0;
    }

    // if (address == BOOTROM_UNMAP && !value) {
    //     std::cout << "Scheiße!" << std::endl;
    // }
    // if ((0x7fff < address) && (address < 0xa000) && ((IO_R[41] & 3) == 3))
    //     std::cout << "ILLEGAL WRITE" << std::endl;
}

void Memory::write_regs(word address, byte value) {
    // TODO: Fix banking
    switch (mode & 0xF) {
        case (MODE_MBC1):
            if (address < MBC1_RAM_ENABLE) {
                ram_enable = ((value & 0xF) == 0xA);
            } else if (address < MBC1_ROM_BANK) {
                if (mode & MODE_MULTICART) {
                    if (!(value & 0x1F)) value |= 1;
                    value &= 0xF;
                    rom_bank &= ~0xF;
                } else {
                    value &= 0x1F;
                    if (!(value & 0x1F)) value |= 1;
                    value &= (rom_banks - 1);
                    rom_bank &= ~(rom_banks - 1);
                }

                rom_bank |= value; // This updates them with only the supported bits
                // rom_bank %= rom_banks;
            } else if (address < MBC1_RAM_BANK) {
                value &= 0b11;
                
                if (mode & MODE_MULTICART) {
                    rom_bank &= ~(0b110000);
                    rom_bank |= (value << 4);
                } else {
                    ram_bank = value & (ram_banks - 1);
                    if (rom_banks == 64) {
                        value &= 1;
                    } else if (rom_banks < 128) {
                        value = 0;
                    }

                    rom_bank &= ~(0b1100000);
                    rom_bank |= (value << 5);
                }
            } else if (address < MBC1_MODE) {
                mbc1_adv_banking = value & 1;
            }
            break;
        case (MODE_MBC2):
            if (address < MBC2_SELECT) {
                value &= 0xF;
                if (address & 0x100) { // Bit 8 is set, lower 4 bits are rom_bank
                    rom_bank = value;
                    if (!value) rom_bank = 1;
                    rom_bank &= (rom_banks - 1);
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
                if (value == 0) {
                    mbc3_latch_register = 0;
                } else if (value == 1 && mbc3_latch_register == 0) {
                    mbc3_latch = !mbc3_latch;
                }
            }
            
            break;
        case (MODE_MBC5):
            if (address < MBC5_RAM_ENABLE) {
                ram_enable = (value == 0xA);
            } else if (address < MBC5_ROM_BANK_LOW) {
                value &= (rom_banks - 1);
                rom_bank &= ~((rom_banks - 1) & 0xFF);
                rom_bank |= value;
            } else if (address < MBC5_ROM_BANK_HIGH) {
                if (rom_banks > 256) {
                    rom_bank &= 0xFEFF;
                    rom_bank |= ((value & 1) << 8);
                }
            } else if (address < MBC5_RAM_BANK) {
                value &= (ram_banks - 1);
                ram_bank = value & 0xF;
            }
            break;
    }
}

byte Memory::raw_read(word address) {
    if (address < 0x4000) {
        return BANKS[address];
    }
    if (address < 0x8000) { // Banks 01-XX, 00-XX for MBC5
        return BANKS[address + rom_bank * 0x4000 - 0x4000];
    }
    if (address < 0xA000) // WRAM
        return VRAM[address - 0x8000];
    if (address < 0xC000) {
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

void Memory::raw_write(word address, byte value) {
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
        ;
    else if (address < 0xFEA0)
        OAM_T[address - 0xFE00] = value;
    else if (address < 0xFF00)
        ;
    else if (address < 0xFF80)
        IO_R[address - 0xFF00] = value;
    else if (address < 0xFFFF)
        HRAM[address - 0xFF80] = value;
    else
        IE = value;
}