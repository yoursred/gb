#ifndef __PPU_H
#define __PPU_H

#include "include.h"
#include "memory/memory.h"
#include <cstdint>
#include <string>
#include <deque>
#include <set>

typedef struct obj {
    byte y, x, id;
    byte cgbp: 3;
    bool bank: 1;
    byte dmgp: 1;
    bool xflip: 1;
    bool yflip: 1;
    bool prio: 1;

    bool visible();
    bool renderable(bool obj16, byte y);
    bool visible_scanline(byte x);
    std::string str();
} obj;

typedef struct lcdc {
    bool bg_win_enable: 1;
    bool obj_enable: 1;
    bool obj16_enable: 1;
    bool bg_tilemap: 1;
    bool bg_win_tileset: 1;
    bool win_enable: 1;
    bool win_tilemap: 1;
    bool lcd_enable: 1;

    // std::string str(); // TODO: Implement
} lcdc;

typedef struct lcd_stat {
    byte mode: 2;
    bool ly_eq_lyc: 1;
    bool mode0_irq_enable: 1;    
    bool mode1_irq_enable: 1;
    bool mode2_irq_enable: 1;
    bool ly_eq_lyc_irq_enable: 1;
    bool: 1; // Padding
    // std::string str(); // TODO: Implement
} lcd_stat;

typedef struct dmg_plt {
    byte color0: 2;    
    byte color1: 2;
    byte color2: 2;
    byte color3: 2;

    byte color(byte id);
    byte color_obj(byte id);
    std::string str();
} dmg_plt;

typedef struct FIFO {
    std::uint32_t bg_queue;
    // std::uint32_t win;
    std::uint32_t obj_queue;
    std::uint32_t plt_queue;
    std::uint32_t attr_queue;
    byte position: 4;
    byte: 4;


    void push_bg(word row); // 8 pixels
    void push_obj(word row, obj attributes); // 8 pixels
    byte pop(dmg_plt bgp, dmg_plt obp0, dmg_plt obp1); // 1 pixel
    void clear();
    // TODO: Implement to allow separation of screen layers
    // byte pop_bg();
    // byte pop_win();
    // byte pop_obj();
} FIFO;

#define PRINT_PPU_REG(out, reg) out << #reg << "=" << COUT_HEX_BYTE_DS(reg) << std::endl;


#define HBLANK 0
#define VBLANK 1
#define OAM_SEARCH 2
#define PIXEL_TRANSFER 3

#define FETCH_TILEID  0
#define FETCH_DATA0   1
#define FETCH_DATA1   2
#define FETCH_PUSH    3

#define BG_TILE_MAP  (0x1800 + LCDC.bg_tilemap * 0x400)
#define WINDOW_TILE_MAP  (0x1800 + LCDC.win_tilemap * 0x400)


class PPU {
    public:
    // TODO: initialize these values.
    lcdc& LCDC;
    lcd_stat& STAT;
    byte &SCY, &SCX, &LY, &LYC, 
         &DMA, &WY, &WX;
    dmg_plt &BGP, &OBP0, &OBP1;
    byte &IF;
    byte* VRAM;
    byte* OAM;
    obj* OAM_T; // 40 objs
    
    // byte* background;
    byte* working_buffer;
    byte* render_buffer;
    byte* buffer;
    
    byte scx_wait;
    byte wly;
    byte fifo_size;
    byte fetch_ticks;
    byte fetch_ticks_obj;
    word fetch_tile = 0;
    byte& fetch_tile_high = *((byte*) &fetch_tile + 1);
    byte& fetch_tile_low = *((byte*) &fetch_tile);
    byte fetch_x, fetch_wx;
    byte w_fifo_ticks = 0;
    bool wy_in_range = false;
    bool window_trigger = false;

    // std::deque<byte> fifo;
    FIFO fifo;
    // std::deque<byte> fifo_obj;
    std::deque<const obj*> visible_objs;
    // const obj* vobj_arr[10];
    // size_t vobj_arr_index = 0;
    // byte visible_obj_count = 0;
    const obj* current_obj;
    bool fetch_halt = false;
    bool partial_fetch = false;
    // std::set<obj*> visible_objs_frame; // Debug
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
    void fetch(bool window);
    void fetch_obj();

    void print_ppu_registers(std::ostream& output);
    std::string str();
    
    
    void inline push_pixel();

    void operator delete(void* obj);
};


word decode_tile_data(word x);
#endif