#include "ppu/ppu.h"
#include <iostream>
#include <sstream>
#include <iomanip>

const char* ppu_state[] = {
    "HBLANK",
    "VBLANK",
    "OAM_SEARCH",
    "PIXEL_TRANSFER"
};

PPU::PPU(Memory& mem):
    LCDC(mem.IO_R[0x40]), 
    STAT(mem.IO_R[0x41]),
    SCY (mem.IO_R[0x42]),
    SCX (mem.IO_R[0x43]),
    LY  (mem.IO_R[0x44]),
    LYC (mem.IO_R[0x45]),
    DMA (mem.IO_R[0x46]),
    BGP (mem.IO_R[0x47]),
    OBP0(mem.IO_R[0x48]),
    OBP1(mem.IO_R[0x49]),
    WY  (mem.IO_R[0x4A]),
    WX  (mem.IO_R[0x4B])

{
    VRAM = mem.VRAM;
    OAM = mem.OAM_T;
    ticks = 0;
    
    STAT &= 0;
    STAT |= 2;

    x = 0;

    // background = new byte[256 * 256];
    buffer = new byte[160 * 144];
}

/*
PPU runs at 2MHz, 1 PPU ticks = 2 CPU cycles.
*/

void PPU::tick() {
    switch (STAT & 0b11) {
        case PIXEL_TRANSFER: // 144 * 160 = 23040
            // Push out 8 pixels = (2bpp * 8) bits or 2 bytes to buffer
            // Switch to HBLANK if x=160
            if (ticks > 90) { 
                // This is to simulate the 12 tick delay at the start of every scanline
                // It takes 2 ticks to fill FIFO from 0
                // TODO: explain what the fuck is happening
                fetch(); // FIFO fetch is always executed in mode 3
            }
            if (fifo_size > 8) {
                push_pixel(); // Only push pixels if we have more than 8 in fifo
                x++;
            }
            if (x == 160) {
                STAT = (STAT & ~0b11) | HBLANK;
            }
        break;
        case OAM_SEARCH:
            // TODO: Implement objects ("sprites")
            if (ticks == 79) {
                x = 0;
                fifo_size = 0;
                fifo = 0;
                STAT = (STAT & ~0b11) | PIXEL_TRANSFER;
            }
        break;
        case HBLANK:
        // Switch to VBLANK if LY=144 and scanline is done
        if (ticks == 455){ // At end of scanline
            // ticks = 0;
            LY++;
            if (LY == 144) { // Visible `lines` done, switch to vblank
                STAT = (STAT & ~0b11) | VBLANK;
            }
            else { // Start next normal scanline
                STAT = (STAT & ~0b11) | OAM_SEARCH;
            }
        }
        break;
        case VBLANK:
        // Switch to OAM_SEARCH if LY=153, end of VBLANK
        if (ticks == 455) {
            // ticks = 0;
            LY++;
            if (LY == 153) { // Jump back to top
                LY = 0;
                STAT = (STAT & ~0b11) | OAM_SEARCH;
            }
        }
        break;
    }
    ticks++;
    ticks %= 456;
}

void PPU::fetch() {
    // Fetch 8 pixels from VRAM and put them into fifo if fifo_size <= 8
    fetch_ticks++;
    // if (fetch_ticks == 6 && fifo_size <= 8) {
    if (fifo_size <= 8) {
        // Get address of the tile data in current tile map
        word cluster = ((VRAM[T(tile_id, LY) + 1]) | (VRAM[T(tile_id, LY)] << 8));
        tile_id = VRAM[0x1800 + (LY * 32) + x];
        fifo &= (0xFFFF0000 << (2 * (8 - fifo_size))); // Zero out the bits to be updated
        fifo |= cluster << (2 * (8 - fifo_size));
        fifo_size += 8;
    }
    // fetch_ticks %= 6;
}

void inline PPU::push_pixel() {
    buffer[LY * 160 + x] = fifo >> 30;
    fifo <<= 2;
    fifo_size--;
}

void PPU::print_ppu_registers() {
    PRINT_PPU_REG(LCDC)
    PRINT_PPU_REG(STAT)
    PRINT_PPU_REG(SCY)
    PRINT_PPU_REG(SCX)
    PRINT_PPU_REG(LY)
    PRINT_PPU_REG(LYC)
    PRINT_PPU_REG(x)
    PRINT_PPU_REG(DMA)
    PRINT_PPU_REG(BGP)
    PRINT_PPU_REG(OBP0)
    PRINT_PPU_REG(OBP1)
    PRINT_PPU_REG(WY)
    PRINT_PPU_REG(WX)
}

std::string PPU::str() {
    std::stringstream out;
    out << std::dec << ticks << "," << ppu_state[STAT & 0b11] << "," << std::dec <<(int) LY << "," << std::dec <<(int) x;
    out << "," << std::dec << (int) fifo_size << "\n";
    return out.str();
}


void PPU::operator delete(void* obj) {
    // delete ((PPU*) obj)->background;
    delete ((PPU*) obj)->buffer;
}

word unfuck_tile_data(word x) {
    // this turns 0b_xxxxxxxxyyyyyyyy into  0b_xyxyxyxyxyxyxyxy
    // TODO: make it apply color palettes
    word ret = 0;
    for (int i = 0; i < 8; i++) {
        ret |= ((x & (1 << i)) << i);
        ret |= ((x & (1 << (i + 8))) >> (7 - i));
    }
    return ret;
}