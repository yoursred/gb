#include <ppu/ppu.h>
#include <iomanip>

// std::string lcdc::str() {

// }

std::string obj::str() {
    std::stringstream ret;
    ret << "OBJ [" << COUT_HEX_BYTE_DS(id) << "] @ (";
    ret << std::dec << (int) x << ", " << (int) y << ")";

    return ret.str();
}

bool obj::visible() {
    return (8 < y) && (y < 160) && (0 < x) && (x < 168);
}

bool obj::renderable(bool obj16, byte ly) {
    return ((ly + 16) >= y) && ((ly + (obj16 ? 0 : 8)) < y);
}

byte dmg_plt::color(byte id) {
    switch (id)
    {
    case 0:
        return color0;
    case 1:
        return color1;
    case 2:
        return color2;
    case 3:
        return color3;
    default:
        return 0;
    }
}

byte dmg_plt::color_obj(byte id) {
    switch (id)
    {
    case 0:
        return color0;
    case 1:
        return color1;
    case 2:
        return color2;
    case 3:
        return color3;
    default:
        return 0;
    }
}