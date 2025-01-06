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

Memory::Memory(byte BOOTROM[], byte ROM[], unsigned int size) {
    // prox = MemoryProxy();

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
    if (!(mode & MODE_MBC5)) {
        rom_bank = 1;
    }

    if (size > (0x4000 * rom_banks)) {
        std::cerr << "ROM size error" << std::endl;
        exit(-1);
    }

    BANKS = new byte[size];

    // It's populatin' time!
    // memcpy(BANKS, ROM, size);
    if (BOOTROM != NULL) {
        std::memcpy(Memory::BOOTROM, BOOTROM, 0x100);
        boot_rom = true;
    }
    std::memcpy(BANKS, ROM, size);

    JOYP = {
        1, 1, 1, 1,
        3
    };
}

Memory::Memory(byte BOOTROM[]) {
    mode = MODE_ROM;
    rom_banks = 2;
    ram_banks = 0;
    std::memcpy(Memory::BOOTROM, BOOTROM, 0x100);
    BANKS = new byte[0x8000];
    boot_rom = true;
    
}

byte Memory::read(word address) {
    if (!last_read_flag) {
        last_read = address;
        last_read_flag = true;
    }

    // if (dma && (address < 0xFF80)) {
    //     return 0xFF;
    // }

    // if ((0x7fff < address) && (address < 0xa000) && ((IO_R[41] & 3) == 3))
    //     std::cout << "ILLEGAL WRITE" << std::endl;
    // address &= 0xFFFF;
    // return Memory::MBC1_read(address);
    // if (address == 0xFF44) {
    //     return 0x90; // More clown behavior
    // }
    /* TODO: WIP
    switch (address & 0xE000) {
        case (ROM_BANK_00_START):
            if (address < BOOT_ROM_END && boot_rom) {
                return BOOTROM[address];
            }
            return BOOTROM[address];
        case (ROM_BANK_NN_START):
            // TODO: This looks sketchy
            return BANKS[address + (rom_bank - 1) * ROM_BANK_NN_START]; 
        case (VRAM_START):
            // TODO: CGB Banks
            return VRAM[address & 0x1FFF];
        case (ERAM_START):
            if (ram_enable) {
                // TODO: Implement properly
                return ERAM[(address & 0x1FFF) + ram_bank * 0x2000];
            }
            return 0xFF;
        case (WRAM_START):
            // TODO: CGB Banks
            return WRAM[address & 0x1FFF];
        case (ECHO_RAM_START):
            switch (address) {
                case
            }

    }
    */

    if (address < BOOT_ROM_END && boot_rom) {
        return BOOTROM[address];
    }

    if (address < 0x4000) {
        return BANKS[address];
    }
    if (address < 0x8000) { // Banks 01-XX, 00-XX for MBC5
        return BANKS[address + rom_bank * 0x4000 - 0x4000];
    }
    if (address < 0xA000) { // VRAM
        return VRAM[address - 0x8000];
        // VRAM/OAM blocking is not recommended
        // if (((IO_R[0x41] & 3) == 3) && (IO_R[0x40] & 0x80)) {
        //     return 0xFF;
        // } else {
        //     return VRAM[address - 0x8000];
        // }
    }
    if (address < 0xC000) {
        return ram_enable ? ERAM[address + ram_bank * 0x2000 - 0xA000] : 0xFF;
    }
    if (address < 0xE000)
        return WRAM[address - 0xC000];
    if (address < 0xFE00)
        return WRAM[address - 0xE000];
    if (address < 0xFEA0) {
        return OAM_T[address - 0xFE00];
        // VRAM/OAM blocking is not recommended
        // if ((IO_R[0x41] & 2)) {
        //     return 0xFF;
        // }
        // else {
        //     return OAM_T[address - 0xFE00];
        // }
    }
    // if (address < 0xFF00)
    //     return 0xFF;
    if (address == 0xFF00) { // Absolute fucking buffoon
        // JOYP.update(sf::Keyboard::isKeyPressed);
        // joyp_wait = true;
        // while (joyp_wait);
        JOYP.update(btns);
        return *(byte*) &JOYP;
    }
        // return 0xFF;
    if (address < 0xFF80)
        return IO_R[address - 0xFF00];
    if (address < 0xFFFF)
        return HRAM[address - 0xFF80];
    return IE;
}

void Memory::write(word address, byte value) {
    if (!last_wrote_flag) {
        last_wrote = address;
        last_wrote_flag = true;
    }
    if (dma && (address < 0xFF80)) {
        
    } else if (address < 0x8000) {
        write_regs(address, value);
    }
    else if (address < 0xA000) { // VRAM/OAM blocking is not recommended
        VRAM[address - 0x8000] = value;
    //     if (((IO_R[0x41] & 3) != 3) || !(IO_R[0x40] & 0x80))
    //         VRAM[address - 0x8000] = value;
    }
    else if (address < 0xC000) {
        if (ram_enable)
            ERAM[address + ram_bank * 0x2000 - 0xA000] = value;
    }
    else if (address < 0xE000)
        WRAM[address - 0xC000] = value;
    else if (address < 0xFE00);
        // return; // clown behaviour
    else if (address < 0xFEA0) { // VRAM/OAM blocking is not recommended
        OAM_T[address - 0xFE00] = value;
    //     if (!(IO_R[0x41] & 2))
    }
    else if (address < 0xFF00) {
        // IO_R[0] = (value & ~0xF) | (IO_R[0] & 0xF);
        // IO_R[0] &= 0xF;
        // IO_R[0] |= value;
        // std::cout << COUT_HEX_BYTE_DS(value) << " : " << COUT_HEX_BYTE_DS(IO_R[0]) << std::endl;
    } else if (address == 0xFF00) {
        // IO_R[0] = (value & ~0xF) | (IO_R[0] & 0xF);
        // JOYP.update(sf::Keyboard::isKeyPressed);
        JOYP.mode = (value >> 4) & 0x3;
        btns.polled = ~JOYP.mode;
        btns.delivered = false;
        // IO_R[0] &= 0xF;
        // IO_R[0] |= value;
        // std::cout << COUT_HEX_BYTE_DS(value) << " : " << COUT_HEX_BYTE_DS(IO_R[0]) << std::endl;
    }
        // return; // clown behaviour
    else if (address == 0xFF41)
        IO_R[0x41] = (value & ~7) | IO_R[0x41];
    else if (address == 0xFF44);
    else if (address == 0xFF45) {
        IO_R[0x45] = value;
        if (value == IO_R[0x44]) {
            IO_R[0x41] |= 4;
        }
    }
    else if (address == 0xFF46) {
        dma = true;
        IO_R[0x46] = value & 0xDF;
        // std::cout << "Requesting DMA transfer" << std::endl;
    }
    else if (address == 0xFF50) 
        boot_rom = !value;
    else if (address < 0xFF80) // I'm not a clown, I'm the whole circus
        IO_R[address - 0xFF00] = value;
    else if (address < 0xFFFF)
        HRAM[address - 0xFF80] = value;
    else
        IE = value;
    
    // mem_writes.push_back(mem_write(cpu_steps, address, value));

    if (address == 0xFF02 && value == 0x81) { // Basic serial output
        std::cout << (char) IO_R[01];
        // fflush(stdout);
    }
    if (address == 0xFF04) {
        cpu->div_timer = 0;
        IO_R[04] = 0;
    }
    if ((0x7fff < address) && (address < 0xa000) && ((IO_R[41] & 3) == 3))
        std::cout << "ILLEGAL WRITE" << std::endl;
}

void Memory::write_regs(word address, byte value) {
    // TODO: Fix banking
    switch (mode & 0xF) {
        case (MODE_MBC1):
            if (address < 0x2000) {
                ram_enable = ((value & 0xF) == 0xA);
            } else if (address < 0x4000) {
                value &= 0x1F;
                if (!value) value = 1;
                rom_bank &= 0xFFC0; // This zeroes out the bottom 5 bits
                rom_bank |= value; // This updates them with only the supported bits
                // rom_bank %= rom_banks;
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
                if (value == 0) {
                    mbc3_latch_register = 0;
                } else if (value == 1 && mbc3_latch_register == 0) {
                    mbc3_latch = !mbc3_latch;
                }
            }
            
            break;
        case (MODE_MBC5):
            if (address < 0x2000) {
                ram_enable = ((value & 0xF) == 0xA);
            } else if (address < 0x3000) {
                rom_bank &= 0xFF00;
                rom_bank |= value;
                // rom_bank %= rom_banks;
            } else if (address < 0x4000) {
                rom_bank &= ~0xFEFF;
                rom_bank |= ((!!value) << 8);
                // rom_bank %= rom_banks;
            } else if (address < 0x6000) {
                ram_bank = value & 0xF;
                // ram_bank %= ram_banks;
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

Memory::MemoryProxy Memory::operator[](const word value) {
    return Memory::MemoryProxy(this, value);
}

Memory::MemoryProxy Memory::operator[](const word* value) {
    return Memory::MemoryProxy(this, *value);
}