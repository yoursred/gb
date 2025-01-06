#include <ppu/ppu.h>

void FIFO::push_bg(word row) {
    // This is supposed to insert 8 pixels at a time
    bg_queue &= ~(0xFFFFFFFF >> position * 2);
    bg_queue |= row << (16 - position * 2);
    position += 8;
}

void FIFO::push_obj(word row, obj attributes) {
    byte a = 0b10 | attributes.prio;
    word flip;
    int j, k, l;
    k = 8;
    bool partial = 0 < attributes.x && attributes.x < 8;

    if (attributes.xflip) {
        flip = 0;
        for (int i = 0; i < 7; i++) {
            flip |= (row >> 2 * i) & 0b11;
            flip <<= 2;
        }
        flip |= (row >> 14) & 0b11;
        row = flip;
    }

    if (partial) { // If off the left side of the screen
        // TODO: properly render partially offscreen objs
        row <<= (16 - 2 * attributes.x);  // Discard the appropriate amount of pixels
        k = attributes.x; // And only render what hasn't been discarded
    }
    for (int i = 0; i < k; i++) {
        j = 14 - i * 2;
        l = ~(attr_queue >> (30 - i * 2)) & 0b11;
        if ((l & 0b10) || !(l & 0b10) && !((obj_queue >> (30 - i * 2)) & 0b11)) { // Only overwrite transparent pixels
        // if (~(attr_queue >> (30 - i * 2)) & 0b10) { // Only overwrite transparent pixels
            obj_queue &= ~(0b11 << (30 - i * 2));
            obj_queue |= ((row >> j) & 0b11) << (30 - i * 2);

            attr_queue &= ~(0b11 << (30 - i * 2));
            attr_queue |= a << (30 - i * 2);

            plt_queue &= ~(0b11 << (30 - i * 2));
            plt_queue |= attributes.dmgp << (30 - i * 2);
        }
    }
}

byte FIFO::pop(dmg_plt bgp, dmg_plt obp0, dmg_plt obp1) {
    // Mixes and applies color palettes, pops out a pixel
    byte b = bgp.color((bg_queue >> 30) & 0b11);
    byte s = (obj_queue >> 30) & 0b11;
    byte p = (plt_queue >> 30) & 0b11;
    byte a = (attr_queue >> 30) & 0b11;

    if ((a & 2) && s) {
        if ((a & 1) && b) {
        } else {
            b = p ? obp1.color_obj(s) : obp0.color_obj(s);
        }
    }
    bg_queue <<= 2;
    obj_queue <<= 2;
    plt_queue <<= 2;
    attr_queue <<= 2;
    position--;

    return b;
}

void FIFO::clear() {
    bg_queue = 0;
    obj_queue = 0;
    plt_queue = 0;
    attr_queue = 0;
    position = 0;
}