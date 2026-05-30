#include "LR3D_Render.hpp"
#include <cstdio>
#include <cstring>

namespace LR3D {

    int WIDTH = 0;
    int HEIGHT = 0;
    int FOV = 0;
    float SCALE = 0.0f;
    std::vector<char> buffer;
    char color_text_cache[256][3];

    void initProjection() {
        SCALE = (LR3D::WIDTH / 2.0f) / (std::tan(LR3D::FOV * M_PI / 360.0f));
    }

    void initColorCache() {
        for (int i = 0; i < 256; i++) {
            color_text_cache[i][0] = '0' + (i / 100);
            color_text_cache[i][1] = '0' + ((i / 10) % 10);
            color_text_cache[i][2] = '0' + (i % 10);
        }
    }

    void render(Framebuffer &framebuffer) {
        size_t ssize = framebuffer.WIDTH * framebuffer.HEIGHT * 25 + framebuffer.HEIGHT * 10 + 100;
        if (buffer.size() < ssize) buffer.resize(ssize);

        char* ptr = buffer.data();
        std::memcpy(ptr, "\033[H", 3); ptr += 3;

        Color last_color;
        bool color_dirty = true;

        for (int y = framebuffer.HEIGHT - 1; y >= 0; y--) {
            const Color* row_ptr = &framebuffer.pixels[y * framebuffer.WIDTH];
            for (int x = 0; x < framebuffer.WIDTH; x++) {
                Color c = row_ptr[x];
                if (color_dirty || c.r != last_color.r || c.g != last_color.g || c.b != last_color.b) {
                    std::memcpy(ptr, "\033[48;2;", 7); ptr += 7;
                    std::memcpy(ptr, color_text_cache[c.r], 3); ptr += 3; *ptr++ = ';';
                    std::memcpy(ptr, color_text_cache[c.g], 3); ptr += 3; *ptr++ = ';';
                    std::memcpy(ptr, color_text_cache[c.b], 3); ptr += 3;
                    *ptr++ = 'm';

                    last_color = c;
                    color_dirty = false;
                }

                *ptr++ = ' ';
                *ptr++ = ' ';
            }

            std::memcpy(ptr, "\033[0m\033[K\n", 8);
            ptr += 8;
            color_dirty = true;
        }

        std::memcpy(ptr, "\033[0m", 4);
        ptr += 4;

        std::fwrite(buffer.data(), 1, ptr - buffer.data(), stdout);
        std::fflush(stdout);
    }
}
