#include "LR3D_Render.hpp"
#include <algorithm>
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

    Clock::Clock() {
        last_time = std::chrono::high_resolution_clock::now();
    }

    void Clock::tick(int fps) {
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = current_time - last_time;
        deltaTime = elapsed.count();
        float target_frame_time = 1.0f / fps;
        if (deltaTime < target_frame_time) {
            float sleep_time = (target_frame_time - deltaTime) * 1000.0f;
            std::this_thread::sleep_for(std::chrono::milliseconds((int)sleep_time));
            current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> final_elapsed = current_time - last_time;
            deltaTime = final_elapsed.count();
        }
        last_time = current_time;
    }

    Screen::Screen(int width, int height) : WIDTH(width), HEIGHT(height) {
        pixels.resize(width * height, {0, 0, 0});
    }

    void Screen::setPixel(int x, int y, Pixel c) {
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
            pixels[x + y * WIDTH] = c;
        }
    }

    void Screen::Clear(Pixel color) {
        std::fill(pixels.begin(), pixels.end(), color);
    }

    void render(Screen &screen) {
        size_t ssize = screen.WIDTH * screen.HEIGHT * 25 + screen.HEIGHT * 10 + 100;
        if (buffer.size() < ssize) buffer.resize(ssize);

        char* ptr = buffer.data();
        std::memcpy(ptr, "\033[H", 3); ptr += 3;

        Pixel last_color;
        bool color_dirty = true;

        for (int y = screen.HEIGHT - 1; y >= 0; y--) {
            const Pixel* row_ptr = &screen.pixels[y * screen.WIDTH];
            for (int x = 0; x < screen.WIDTH; x++) {
                Pixel c = row_ptr[x];
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

    Vec2f Rasterizer::fixed_camera_project(Vec3f A) {
        Vec2f B = {0, 0};
        B.x = (A.x / A.z) * SCALE + WIDTH / 2;
        B.y = (A.y / A.z) * SCALE + HEIGHT / 2;
        return B;
    }

    void Rasterizer::draw_line_dda(Screen &screen, Vec2f A, Vec2f B, Pixel color) {
        Vec2f dir = B - A;
        float dx = dir.x, dy = dir.y;
        int steps = std::max(abs((int)dx), abs((int)dy));
        if (steps < 1) steps = 1;
        Vec2f step = dir * (1.0f / steps), current = A;
        for (int i = 0; i <= steps; i++) {
            screen.setPixel(roundf(current.x), roundf(current.y), color);
            current += step;
        }
    }

    void Rasterizer::draw_line_bresenham(Screen &screen, Vec2f A, Vec2f B, Pixel color) {
        int x1 = (int)roundf(A.x), y1 = (int)roundf(A.y), x2 = (int)roundf(B.x), y2 = (int)roundf(B.y);
        int dx = abs((int)x2 - x1), dy = -abs((int)y2 - y1);
        int sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1, err = dx + dy;
        while (x1 != x2 || y1 != y2) {
            screen.setPixel(x1, y1, color);
            int e2 = 2 * err;
            if (e2 >= dy) {err += dy; x1 += sx;}
            if (e2 <= dx) {err += dx; y1 += sy;}
        }
    }

    void Rasterizer::draw_triangle_vec2f(Screen &screen, Vec2f A, Vec2f B, Vec2f C, Pixel color) {
        draw_line_bresenham(screen, A, B, color);
        draw_line_bresenham(screen, B, C, color);
        draw_line_bresenham(screen, C, A, color);
    }

    void Rasterizer::draw_quad_vec2f(Screen &screen, Vec2f A, Vec2f B, Vec2f C, Vec2f D, Pixel color) {
        draw_line_bresenham(screen, A, B, color);
        draw_line_bresenham(screen, B, C, color);
        draw_line_bresenham(screen, C, D, color);
        draw_line_bresenham(screen, D, A, color);
    }

    void Rasterizer::draw_line_vec3f(Screen &screen, Vec3f A, Vec3f B, Pixel color) {
        if (A.z < 0.1f || B.z < 0.1f) return;
        Vec2f A_vec2f = fixed_camera_project(A);
        Vec2f B_vec2f = fixed_camera_project(B);
        draw_line_bresenham(screen, A_vec2f, B_vec2f, color);
    }

    void Rasterizer::draw_triangle_vec3f(Screen &screen, Vec3f A, Vec3f B, Vec3f C, Pixel color) {
        if (A.z < 0.1f || B.z < 0.1f || C.z < 0.1f) return;
        Vec2f A_vec2f = fixed_camera_project(A), B_vec2f = fixed_camera_project(B), C_vec2f = fixed_camera_project(C);
        draw_triangle_vec2f(screen, A_vec2f, B_vec2f, C_vec2f, color);
    }

    void Rasterizer::draw_quad_vec3f(Screen &screen, Vec3f A, Vec3f B, Vec3f C, Vec3f D, Pixel color) {
        if (A.z < 0.1f || B.z < 0.1f || C.z < 0.1f || D.z < 0.1f) return;
        Vec2f A_vec2f = fixed_camera_project(A), B_vec2f = fixed_camera_project(B), C_vec2f = fixed_camera_project(C), D_vec2f = fixed_camera_project(D);
        draw_quad_vec2f(screen, A_vec2f, B_vec2f, C_vec2f, D_vec2f, color);
    }

    void Rasterizer::draw_mesh_2D(Screen &screen, Mesh2D &mesh, Vec2f pos, float rot, float scale, Pixel color) {
        float rad = rot * 0.0174532925f;
        float s = sin(rad), c = cos(rad);
        for (size_t i = 0; i < mesh.indices.size(); i += 3) {
            Vec2f tri[3];
            for (int j = 0; j < 3; j++) {
                Vec2f v = mesh.vertices[mesh.indices[i + j]];
                float x1 = v.x * scale;
                float y1 = v.y * scale;
                float x2 = x1 * c - y1 * s;
                float y2 = x1 * s + y1 * c;
                tri[j].x = x2 + pos.x;
                tri[j].y = y2 + pos.y;
            }
            draw_triangle_vec2f(screen, tri[0], tri[1], tri[2], color);
        }
    }

    void Rasterizer::draw_mesh_3D(Screen &screen, Mesh3D &mesh, Vec3f pos, Vec3f rot, float scale, Pixel color) {
        Vec3f rad = rot * 0.0174532925f;
        float sx = sin(rad.x), cx = cos(rad.x);
        float sy = sin(rad.y), cy = cos(rad.y);
        float sz = sin(rad.z), cz = cos(rad.z);

        for (size_t i = 0; i < mesh.indices.size(); i += 3) {
            Vec3f tri[3];
            for (int j = 0; j < 3; j++) {
                Vec3f v = mesh.vertices[mesh.indices[i + j]];
                float y1 = v.y * cx - v.z * sx;
                float z1 = v.y * sx + v.z * cx;
                v.y = y1; v.z = z1;
                float x2 = v.x * cy + v.z * sy;
                float z2 = -v.x * sy + v.z * cy;
                v.x = x2; v.z = z2;
                float x3 = v.x * cz - v.y * sz;
                float y3 = v.x * sz + v.y * cz;
                v.x = x3; v.y = y3;
                tri[j] = v * scale + pos;
            }
            draw_triangle_vec3f(screen, tri[0], tri[1], tri[2], color);
        }
    }
}
