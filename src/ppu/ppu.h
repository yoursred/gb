#include "include.h"
#include "memory/memory.h"
#include <cstdint>

typedef uint32_t FIFO;

// vvvv TILE_ROW_ADDRESS vvvv
#define T(ID, ROW) ID * 16 + (ROW % 8) * 2
// #define SHIFT_OUT(x, n) 
// #define GET_WORD(SRC, ADDRESS) ((SRC[ADDRESS] << 8) | SRC[ADDRESS + 1])


#define HBLANK 0
#define VBLANK 1
#define OAM_SEARCH 2
#define PIXEL_TRANSFER 3

#define FETCH_TILEID  0
#define FETCH_DATA0   1
#define FETCH_DATA1   2
#define FETCH_PUSH    3


class PPU {
    byte &LCDC, &STAT, &SCY, &SCX, &LY, &LYC, 
         &DMA, &WY, &WX, &BGP, &OBP0, &OBP1;
    byte* VRAM;
    byte* OAM;
    
    // byte* background;
    byte* buffer; 
    // [160][144]; // convenience my boy
    
    
    byte fifo_size;
    // byte fetch_state;
    // bool fetch_tick;

    FIFO fifo; // this pushes pixels to the left
    
    word ticks;
    byte x;
    byte tile_line;
    byte tile_id;


    PPU(Memory& mem);

    void tick();
    void fetch();
    
    
    void inline push_pixel();

    void operator delete(void* obj);
};


word unfuck_tile_data(word x);