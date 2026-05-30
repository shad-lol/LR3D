#pragma once
#ifndef LR3D_FRAMEBUFFER_HPP
#define LR3D_FRAMEBUFFER_HPP
#include <vector>
#include "LR3D_Color.hpp"

namespace LR3D {
    struct Framebuffer {
        int WIDTH, HEIGHT;
        std::vector <Color> pixels;
        Framebuffer(int width, int height);
        void setPixel(int x, int y, Color c);
        void Clear(Color color);
    };
}
#endif
