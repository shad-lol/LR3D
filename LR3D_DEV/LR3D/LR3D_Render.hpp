#pragma once
#ifndef LR3D_RENDER_HPP
#define LR3D_RENDER_HPP
#include <vector>
#include "LR3D_Math.hpp"
#include "LR3D_Framebuffer.hpp"

namespace LR3D {

    inline const char* INIT_STR  = "\033[2J\033[H\033[?25l\033[?7l\033[?12l";
    inline const char* RESET_STR = "\033[?25h\033[?7h\033[0m";

    extern int WIDTH, HEIGHT, FOV;
    extern float SCALE;
    extern std::vector<char> buffer;
    extern char color_text_cache[256][3];

    void initProjection();
    void initColorCache();

    void render(Framebuffer &framebuffer);
}
#endif
