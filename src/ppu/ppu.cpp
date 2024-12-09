#include "ppu/ppu.h"
#include "cpu/cpu.h"
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
    WX  (mem.IO_R[0x4B]),
    IF  (mem.IO_R[0x0F])

{
    VRAM = mem.VRAM;
    OAM = mem.OAM_T;
    ticks = 0;
    
    LCDC = 0x91;
    STAT = 0x87;
    SCX  = 0;
    SCY  = 0;
    WX   = 0;
    WY   = 0;
    LY   = 0;
    LYC  = 0;

    BGP  = 0xFC; 

    scx_wait = SCX % 8;

    // STAT &= 0;
    // STAT |= 2;

    x = 0;

    // background = new byte[256 * 256];
    buffer = new byte[160 * 144 * 4]; // RGBA for convenience
    for (size_t i = 0; i < 160 * 144 * 4; i++) {
        if ((i + 1) % 4) {
            
        } else {

        }
    }
}

/*
PPU runs at 2MHz, 1 PPU ticks = 2 CPU cycles.
*/

void PPU::tick() {
    // TODO: clean up
    switch (STAT & 0x3) {
        case PIXEL_TRANSFER: // 144 * 160 = 23040
            
            // Push out 8 pixels = (2bpp * 8) bits or 2 bytes to buffer
            // Switch to HBLANK if x=160
            if (scx_wait)
                scx_wait--;
            else {
                if (fifo.size()) {
                    push_pixel(); // Only push pixels if we have more than 8 in fifo
                    x++;
                }
                if (ticks > (90 + scx_wait)) { 
                    // This is to simulate the 12 tick delay at the start of every scanline
                    // It takes 2 ticks to fill FIFO from 0
                    // TODO: explain what the fuck is happening (still don't know what i'm supposed to explain)
                    fetch(); // FIFO fetch is always executed in mode 3
                }
                
                if (x == 160) {
                    STAT = (STAT & ~0x3) | HBLANK;
                }
            }
        break;
        case OAM_SEARCH:
            // TODO: Implement objects ("sprites")
            if (ticks == 79) {
                x = 0;
                scx_wait = SCX % 8;
                // fifo_size = 0;
                // fifo = 0;
                fifo.clear();
                fifo.shrink_to_fit();
                STAT = (STAT & ~0x3) | PIXEL_TRANSFER;
            }
        break;
        case HBLANK:
        // Switch to VBLANK if LY=144 and scanline is done
        if (ticks == 455){ // At end of scanline
            // ticks = 0;
            if ((LCDC & 32) && (LY >= WY) && (WX < 167))
                wly++;
            LY++;
            if (LY == 144) { // Visible `lines` done, switch to vblank
                STAT = (STAT & ~0x3) | VBLANK;
            }
            else { // Start next normal scanline
                STAT = (STAT & ~0x3) | OAM_SEARCH;
            }
        }
        break;
        case VBLANK:
        // Switch to OAM_SEARCH if LY=153, end of VBLANK
        if (ticks == 455) {
            // ticks = 0;
            LY++;
            wly = 0;
            // if ((LCDC & 32) && (WX < 167) && (WY < 144))
            //     wly++;
            if (LY == 153) { // Jump back to top
                LY = 0;
                STAT = (STAT & ~0x3) | OAM_SEARCH;
            }
        }
        break;
    }
    ticks++;
    ticks %= 456;
    if (LY == LYC) {
        STAT |= 4;
    } else {
        STAT &= ~4;
    }
    // wly %= 144;
    curr_stat_irq  = (STAT & 64) && (STAT & 4);
    curr_stat_irq |= (STAT & 32) && ((STAT & 3) == OAM_SEARCH);
    curr_stat_irq |= (STAT & 16) && ((STAT & 3) == VBLANK);
    curr_stat_irq |= (STAT &  8) && ((STAT & 3) == HBLANK);
    curr_vblank_irq = (STAT & 3) == HBLANK;

    if (!last_stat_irq && curr_stat_irq) { // Rising edge detection
        IF |= IRQ_LCD_STAT;
    }
    if (!last_vblank_irq && curr_vblank_irq)
        // IF |= IRQ_VBLANK;
    last_stat_irq = curr_stat_irq;
    last_vblank_irq = curr_vblank_irq;

}

void PPU::fetch() {
    // Fetch 8 pixels from VRAM and put them into fifo if empty
    if (fifo.empty()) {
        // Get address of the tile data in current tile map
        if (LCDC & 1){
            byte sy = LY + SCY;
            byte sx = x + SCX;
            byte wsy = LY - WY;
            byte wsx = x - (WX - 7);
            if ((LY >= WY) && (x >= (WX - 7)) && (LCDC & 32)) {
                // std::cout << "USING WINDOW" << std::endl;
                tile_id = VRAM[WINDOW_TILE_MAP + (wly / 8) * 32 + (wsx / 8)];
                if ((LCDC & 16)) {
                    tile_row  = VRAM[(tile_id << 4) + (wly % 8) * 2 + 1];
                    tile_row |= VRAM[(tile_id << 4) + (wly % 8) * 2] << 8;
                } else {
                    tile_row  = VRAM[0x1000 + ((sbyte)tile_id << 4) + (wly % 8) * 2 + 1];
                    tile_row |= VRAM[0x1000 + ((sbyte)tile_id << 4) + (wly % 8) * 2] << 8;
                }
            }
            else {
                tile_id = VRAM[BG_TILE_MAP + (sy / 8) * 32 + (sx / 8)];
                if (LCDC & 16) {
                    tile_row  = VRAM[(tile_id << 4) + (sy % 8) * 2 + 1];
                    tile_row |= VRAM[(tile_id << 4) + (sy % 8) * 2] << 8;
                } else {
                    tile_row  = VRAM[0x1000 + ((sbyte)tile_id << 4) + (sy % 8) * 2 + 1];
                    tile_row |= VRAM[0x1000 + ((sbyte)tile_id << 4) + (sy % 8) * 2] << 8;
                }
            }
            // std::cout << COUT_HEX_WORD_DS(BG_TILE_MAP + (LY / 8) * 32 + (x / 8)) << std::endl;

            unfuck_tile_data(tile_row);
            for (int i = 0; i < 8; i++) {
                // TODO: this works but god no
                fifo.push_back((BGP >> (2 * ((tile_row >> (2 * (7 - i))) & 0x3))) & 0x3);
                fifo.shrink_to_fit();
            }
            // fifo.push_back(tile);

            // tile_id = VRAM[0x1800 + (LY * 32) + x];
            // word cluster = ((VRAM[T(tile_id, LY) + 1]) | (VRAM[T(tile_id, LY)] << 8));
            // fifo &= (0xFFFF0000 << (2 * (8 - fifo_size))); // Zero out the bits to be updated
            // fifo |= cluster << (2 * (8 - fifo_size));
            // fifo_size += 8;
        } else {
            for (int i = 0; i < 8; i++) {
                // TODO: this works but god no
                fifo.push_back(BGP & 0x3);
                fifo.shrink_to_fit();
            }
        }
    }
    // fetch_ticks++;
    // fetch_ticks %= 8;
}

void inline PPU::push_pixel() {
    // buffer[LY * 160 + x] = fifo >> 30;
    for (int i = 0; i < 4; i++) {
        if (i != 3)
            buffer[(LY * 160 + x) * 4 + i] = (3 - fifo.front()) * 85; // 255 - 3
        else
            buffer[(LY * 160 + x) * 4 + i] = 255; // alpha value
    }
    fifo.pop_front();
    fifo.shrink_to_fit();
    // fifo <<= 2;
    // fifo_size--;
}

void PPU::print_ppu_registers(std::ostream& output) {
    PRINT_PPU_REG(output, LCDC)
    PRINT_PPU_REG(output, STAT)
    PRINT_PPU_REG(output, SCY)
    PRINT_PPU_REG(output, SCX)
    PRINT_PPU_REG(output, LY)
    PRINT_PPU_REG(output, LYC)
    PRINT_PPU_REG(output, x)
    PRINT_PPU_REG(output, DMA)
    PRINT_PPU_REG(output, BGP)
    PRINT_PPU_REG(output, OBP0)
    PRINT_PPU_REG(output, OBP1)
    PRINT_PPU_REG(output, WY)
    PRINT_PPU_REG(output, WX)
}

std::string PPU::str() {
    std::stringstream out;
    out << std::dec << ticks << "," << ppu_state[STAT & 0x3] << "," << std::dec <<(int) LY << "," << std::dec <<(int) x;
    out << "," << std::dec << (int) fifo_size << "\n";
    return out.str();
}


void PPU::operator delete(void* obj) {
    // delete ((PPU*) obj)->background;
    delete ((PPU*) obj)->buffer;
}

void unfuck_tile_data(word &x) {
    // this turns 0b_xxxxxxxxyyyyyyyy into 0b_yxyxyxyxyxyxyxyx -0b_xyxyxyxyxyxyxyxy-
    word ret = 0;
    x = (x << 8) | (x >> 8);
    for (int i = 0; i < 8; i++) {
        ret |= ((x & (1 << i)) << i);
        ret |= ((x & (1 << (i + 8))) >> (7 - i));
    }
    // return ret;
    x = ret;
}