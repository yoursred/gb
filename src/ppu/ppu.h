#ifndef __PPU_H
#define __PPU_H

#include "include.h"
#include "memory/memory.h"
#include <cstdint>
#include <string>
#include <deque>

typedef uint32_t FIFO;

// vvvv TILE_ROW_ADDRESS vvvv
#define T(ID, ROW) ID * 16 + (ROW % 8) * 2
// #define SHIFT_OUT(x, n) 
// #define GET_WORD(SRC, ADDRESS) ((SRC[ADDRESS] << 8) | SRC[ADDRESS + 1])
#define PRINT_PPU_REG(out, reg) out << #reg << "=" << COUT_HEX_BYTE_DS(reg) << std::endl;


#define HBLANK 0
#define VBLANK 1
#define OAM_SEARCH 2
#define PIXEL_TRANSFER 3

#define FETCH_TILEID  0
#define FETCH_DATA0   1
#define FETCH_DATA1   2
#define FETCH_PUSH    3

#define TILE_BLOCK  (0x1000 - ((LCDC & 16) >> 4) * 0x1000)
#define BG_TILE_MAP  (0x1800 + ((LCDC & 8) >> 3) * 0x400)
#define WINDOW_TILE_MAP  (0x1800 + ((LCDC & 64) >> 6) * 0x400)


class PPU {
    public:
    // TODO: initialize these values.
    byte &LCDC, &STAT, &SCY, &SCX, &LY, &LYC, 
         &DMA, &BGP, &OBP0, &OBP1, &WY, &WX;
    byte &IF;
    byte* VRAM;
    byte* OAM;
    
    // byte* background;
    byte* buffer;
    
    byte scx_wait;
    byte wly;
    byte fifo_size;
    byte fetch_ticks;
    std::deque<byte> fifo;
    bool last_stat_irq = false;
    bool curr_stat_irq = false;
    bool last_vblank_irq = false;
    bool curr_vblank_irq = false;

    // FIFO fifo; // this pushes pixels to the left
    
    word ticks;
    byte x;
    byte tile_line;
    byte tile_id;
    word tile_row;


    PPU(Memory& mem);

    void tick();
    void fetch();

    void print_ppu_registers(std::ostream& output);
    std::string str();
    
    
    void inline push_pixel();

    void operator delete(void* obj);
};


void unfuck_tile_data(word &x);
#endif