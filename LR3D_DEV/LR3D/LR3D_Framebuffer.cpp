#include "LR3D_Framebuffer.hpp"

namespace LR3D {
    Framebuffer::Framebuffer(int width, int height) : WIDTH(width), HEIGHT(height) {
        pixels.resize(width * height, {0, 0, 0});
    }

    void Framebuffer::setPixel(int x, int y, Color c) {
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
            pixels[x + y * WIDTH] = c;
        }
    }

    void Framebuffer::Clear(Color color) {
        std::fill(pixels.begin(), pixels.end(), color);
    }
}
