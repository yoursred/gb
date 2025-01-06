#include "ppu/ppu.h"
#include "cpu/cpu.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

const char* ppu_state[] = {
    "HBLANK",
    "VBLANK",
    "OAM_SEARCH",
    "PIXEL_TRANSFER"
};


PPU::PPU(Memory& mem):
    LCDC(*((lcdc*) &mem.IO_R[0x40])), 
    STAT(*((lcd_stat*) &mem.IO_R[0x41])),
    SCY (mem.IO_R[0x42]),
    SCX (mem.IO_R[0x43]),
    LY  (mem.IO_R[0x44]),
    LYC (mem.IO_R[0x45]),
    DMA (mem.IO_R[0x46]),
    BGP (*((dmg_plt*) &mem.IO_R[0x47])),
    OBP0(*((dmg_plt*) &mem.IO_R[0x48])),
    OBP1(*((dmg_plt*) &mem.IO_R[0x49])),
    WY  (mem.IO_R[0x4A]),
    WX  (mem.IO_R[0x4B]),
    IF  (mem.IO_R[0x0F])

{
    VRAM = mem.VRAM;
    OAM = mem.OAM_T;
    OAM_T = (obj*) OAM;
    ticks = 0;
    fetch_ticks = 0;

    mem.IO_R[0x40] = 0; // LCDC
    mem.IO_R[0x41] = 0x82; // STAT
    SCX  = 0;
    SCY  = 0;
    WX   = 0;
    WY   = 0;
    LY   = 0;
    LYC  = 0;

    mem.IO_R[0x47] = 0xFC; // BGP

    scx_wait = SCX % 8;

    x = 0;

    buffer = new byte[160 * 144 * 4 * 2]{}; // 160 * 144 pixels * 4 channels/pixel * 2 buffers
    working_buffer = buffer;
    render_buffer = &buffer[160 * 144 * 4]; // Halfway through the main buffer

    for (size_t i = 0; i < 160 * 144 * 4 * 2; i++) {
        buffer[i] = 255;
    }
}

// PPU runs at 2MHz, 1 PPU ticks = 2 CPU cycles.

void PPU::tick() {
    // TODO: clean up
    if (!LCDC.lcd_enable) {
        return;
    }
    switch (STAT.mode) {
        case PIXEL_TRANSFER: // 144 * 160 = 23040
            
            if (ticks > 87) { 
                // This is to simulate the -12- 14 tick delay at the start of every scanline
                // TODO: Implement the ppu's "double tile fetch"
                if (visible_objs.size() && current_obj == nullptr) {
                    for (auto it = visible_objs.begin(); it != visible_objs.end();) {
                        if ((*it)->x == (x + 8)) {
                            current_obj = (*it);
                            visible_objs.erase(it);
                            fetch_halt = true;
                            break;
                        } if (0 < (*it)->x && (*it)->x < (x + 8)) {
                            current_obj = (*it);
                            visible_objs.erase(it);
                            fetch_halt = true;
                            partial_fetch = true;
                            break;
                        } else {
                            it++;
                        }
                    }

                }

                if (!fetch_halt || fetch_ticks) { // bg fetches always have priority
                    // Push out 8 pixels = (2bpp * 8) bits or 2 bytes to queue
                    if (!window_trigger) {
                        window_trigger = (((WX - 7) == x) && wy_in_range && LCDC.win_enable);
                        if (window_trigger) {   
                            fetch_ticks = 0;
                            fifo.clear();
                            fetch(true);
                        } else
                            fetch(false);
                    } else {
                        fetch(true);
                    }
                } else {
                    fetch_obj();
                }
                if (fifo.position > 7 && !fetch_halt) { // Stop rendering pixels if an obj fetch is underway
                    if (scx_wait) { // Horizontal scrolling
                        scx_wait--;
                        fifo.pop(BGP, OBP0, OBP1);
                    }
                    else {
                        push_pixel(); // Only push pixels if we have more than 8 in fifo
                        x++;
                    }
                }
                
                if (x == 160) {
                    STAT.mode = HBLANK;
                    if (window_trigger) {
                        wly++;
                    }
                    window_trigger = false;
                }
            }
        break;
        case OAM_SEARCH:
            if (!(ticks % 2)) {
                obj& curr = OAM_T[ticks / 2];
                // if (curr.renderable(LCDC.obj16_enable, LY) && visible_objs.size() < 10) {
                if (((LY + 16) >= curr.y) && ((LY + (LCDC.obj16_enable ? 0 : 8)) < curr.y) && visible_objs.size() < 10) {
                    visible_objs.push_back(&curr);
                }
            }
            if (ticks == 79) {
                x = 0;
                fetch_ticks = 0;
                scx_wait = SCX % 8;
                fetch_x = (SCX / 8) % 32;
                fetch_wx = 0;

                std::sort(visible_objs.begin(), visible_objs.end(), 
                [](const obj* a, const obj* b){
                    return a->x < b->x;
                });

                fifo.clear();
                wy_in_range = (WY <= LY);

                STAT.mode = PIXEL_TRANSFER;
            }
        break;
        case HBLANK:
        // Switch to VBLANK if LY=144 and scanline is done
        if (ticks == 455){ // At end of scanline
            LY++;
            if (LY == 144) { // Visible `lines` done, switch to vblank
                STAT.mode = VBLANK;
            }
            else { // Start next normal scanline
                visible_objs.clear();
                STAT.mode = OAM_SEARCH;
            }
        }
        break;
        case VBLANK:
        // Switch to OAM_SEARCH if LY=153, end of VBLANK
        if (ticks == 455) {
            // ticks = 0;
            LY++;
            wly = 0;
            if (LY == 153) { // Jump back to top
                std::swap(working_buffer, render_buffer);
                LY = 0;
                fifo.clear();
                STAT.mode = OAM_SEARCH;
            }
        }
        break;
    }
    ticks++;
    ticks %= 456;


    STAT.ly_eq_lyc = (LY == LYC);

    // Interrupt signaling
    curr_stat_irq  = STAT.ly_eq_lyc_irq_enable && STAT.ly_eq_lyc;
    curr_stat_irq |= STAT.mode2_irq_enable && (STAT.mode == OAM_SEARCH);
    curr_stat_irq |= STAT.mode1_irq_enable && (STAT.mode == VBLANK);
    curr_stat_irq |= STAT.mode0_irq_enable && (STAT.mode == HBLANK);
    curr_vblank_irq = (STAT.mode == VBLANK); // Why was this set to HBLANK???

    if (!last_stat_irq && curr_stat_irq) { // Rising edge detection
        IF |= IRQ_LCD_STAT;
    }
    if (!last_vblank_irq && curr_vblank_irq)
        // IF |= IRQ_VBLANK;
    last_stat_irq = curr_stat_irq;
    last_vblank_irq = curr_vblank_irq;
}

void PPU::fetch(bool window) {
    byte sy = LY + SCY;


    switch (fetch_ticks) {
        case 0: // Get tile id
            if (window && LCDC.win_enable) {
                tile_id = VRAM[WINDOW_TILE_MAP + (wly / 8) * 32 + fetch_wx];
            } else {
                tile_id = VRAM[BG_TILE_MAP + (sy / 8) * 32 + fetch_x];
            }
        break;
        case 2: // Get tile data low
            if (window && LCDC.win_enable) {
                if (LCDC.bg_win_tileset) {
                    fetch_tile_low  = VRAM[(tile_id << 4) + (wly % 8) * 2];
                } else {
                    fetch_tile_low  = VRAM[0x1000 + ((sbyte)tile_id << 4) + (wly % 8) * 2];
                }
            } else {
                if (LCDC.bg_win_tileset) {
                    fetch_tile_low  = VRAM[(tile_id << 4) + (sy % 8) * 2];
                } else {
                    fetch_tile_low  = VRAM[0x1000 + ((sbyte)tile_id << 4) + (sy % 8) * 2];
                }
            }
        break;
        case 4: // Get tile data high
            if (window && LCDC.win_enable) {
                if (LCDC.bg_win_tileset) {
                    fetch_tile_high = VRAM[(tile_id << 4) + (wly % 8) * 2 + 1];
                } else {
                    fetch_tile_high = VRAM[0x1000 + ((sbyte)tile_id << 4) + (wly % 8) * 2 + 1];
                }
            } else {
                if (LCDC.bg_win_tileset) {
                    fetch_tile_high = VRAM[(tile_id << 4) + (sy % 8) * 2 + 1];
                } else {
                    fetch_tile_high = VRAM[0x1000 + ((sbyte)tile_id << 4) + (sy % 8) * 2 + 1];
                }
            }
        break;
        case 5:
        case 6:
        case 7: // Attempt to push pixels, only push if there's enough space
            if (fifo.position < 8) { // Only insert the 8 pixels if there's enough space
                word row = decode_tile_data(fetch_tile);
                if (LCDC.bg_win_enable) {
                    fifo.push_bg(row);
                } else {
                    fifo.push_bg(0);
                }

                fetch_ticks = 0;
                fetch_x++;
                fetch_x %= 32;
                if (window && LCDC.win_enable) {
                    fetch_wx++;
                    fetch_wx %= 32;
                }
                return; // Avoids incrementing the fetch counter unnecessarily (:
            }
        break;
    }

    fetch_ticks++;
    fetch_ticks %= 8;
}


void PPU::fetch_obj() {
    switch (fetch_ticks_obj) {
        case 0: // Get tile
            if (current_obj) {
                tile_id = current_obj->id;
                if (LCDC.obj16_enable) {
                    tile_id &= 0xFE; // Bit 0 of tile id should be ignored on 8x16 objects
                }
            }
        break;
        case 2: // Get tile data low
            if (current_obj) {
                byte y = (LY - current_obj->y + 16);
                if (current_obj->yflip) {
                    y = LCDC.obj16_enable ? 15 - y : 7 - y;
                }
                fetch_tile_low = VRAM[(tile_id << 4) + y * 2];
            }
        break;
        case 4: // Get tile data high
            if (current_obj) {
                byte y = (LY - current_obj->y + 16);
                if (current_obj->yflip) {
                    y = LCDC.obj16_enable ? 15 - y : 7 - y;
                }
                fetch_tile_high = VRAM[(tile_id << 4) + y * 2 + 1];
            }
        break;
        case 5:
            if (LCDC.obj_enable && current_obj) {
                word row = decode_tile_data(fetch_tile);
                fifo.push_obj(row, *current_obj);
            }
            fetch_ticks_obj = 0;
            current_obj = nullptr;
            fetch_halt = false;
            return;
        break;
    }

    fetch_ticks_obj++;
    fetch_ticks_obj %= 8;
}

void inline PPU::push_pixel() {
    byte pixel = fifo.pop(BGP, OBP0, OBP1);
    for (int i = 0; i < 4; i++) {
        if (i != 3) {
            working_buffer[(LY * 160 + x) * 4 + i] = (3 - pixel) * 85;
        }
    }
}

void PPU::print_ppu_registers(std::ostream& output) {
    // PRINT_PPU_REG(output, LCDC)
    // PRINT_PPU_REG(output, STAT)
    PRINT_PPU_REG(output, SCY)
    PRINT_PPU_REG(output, SCX)
    PRINT_PPU_REG(output, LY)
    PRINT_PPU_REG(output, LYC)
    PRINT_PPU_REG(output, x)
    PRINT_PPU_REG(output, DMA)
    // PRINT_PPU_REG(output, BGP)
    // PRINT_PPU_REG(output, OBP0)
    // PRINT_PPU_REG(output, OBP1)
    PRINT_PPU_REG(output, WY)
    PRINT_PPU_REG(output, WX)
}

std::string PPU::str() {
    std::stringstream out;
    out << std::dec << ticks << "," << ppu_state[STAT.mode] << "," << std::dec <<(int) LY << "," << std::dec <<(int) x;
    out << "," << std::dec << (int) fifo_size << "\n";
    return out.str();
}


void PPU::operator delete(void* obj) {
    delete ((PPU*) obj)->buffer;
}

word decode_tile_data(word x) {
    // this turns 0b_xxxxxxxxyyyyyyyy into 0b_yxyxyxyxyxyxyxyx -0b_xyxyxyxyxyxyxyxy-
    word ret = 0;
    for (int i = 0; i < 8; i++) {
        ret |= ((x & (1 << i)) << i);
        ret |= ((x & (1 << (i + 8))) >> (7 - i));
    }
    return ret;
}