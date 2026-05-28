#ifndef LR3D_RENDER_HPP
#define LR3D_RENDER_HPP
#include "LR3D_Math.hpp"
#include <cstdio>
#include <cstdint>
#include <vector>
#include <thread>
#include <chrono>

namespace LR3D {

    inline const char* INIT_STR  = "\033[2J\033[H\033[?25l\033[?7l\033[?12l";
    inline const char* RESET_STR = "\033[?25h\033[?7h\033[0m";

    extern int WIDTH, HEIGHT, FOV;
    extern float SCALE;
    extern std::vector<char> buffer;

    struct Clock {
        std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
        float deltaTime = 0;
        Clock();
        void tick(int fps);
    };

    struct Pixel {
        uint8_t r, g, b;
    };

    namespace Colors {
        inline const Pixel White   = {255, 255, 255};
        inline const Pixel Black   = {0, 0, 0};
        inline const Pixel Red     = {255, 0, 0};
        inline const Pixel Green   = {0, 255, 0};
        inline const Pixel Blue    = {0, 0, 255};
        inline const Pixel Yellow  = {255, 255, 0};
        inline const Pixel Magenta = {255, 0, 255};
        inline const Pixel Cyan    = {0, 255, 255};
        inline const Pixel Gray    = {128, 128, 128};
    }

    struct Screen {
        int WIDTH, HEIGHT;
        std::vector <Pixel> pixels;
        Screen(int width, int height);
        void setPixel(int x, int y, Pixel c);
        void Clear(Pixel color);
    };

    void render(Screen &screen);

    struct Mesh2D {
        std::vector<LR3D::Vec2f> vertices;
        std::vector<int> indices;
    };

    struct Mesh3D {
        std::vector<Vec3f> vertices;
        std::vector<int> indices;
    };

    struct Rasterizer {
        void initProjection();
        Vec2f fixed_camera_project(Vec3f A);
        void draw_line_dda(Screen &screen, Vec2f A, Vec2f B, Pixel color);
        void draw_line_bresenham(Screen &screen, Vec2f A, Vec2f B, Pixel color);
        void draw_triangle_vec2f(Screen &screen, Vec2f A, Vec2f B, Vec2f C, Pixel color);
        void draw_quad_vec2f(Screen &screen, Vec2f A, Vec2f B, Vec2f C, Vec2f D, Pixel color);
        void draw_line_vec3f(Screen &screen, Vec3f A, Vec3f B, Pixel color);
        void draw_triangle_vec3f(Screen &screen, Vec3f A, Vec3f B, Vec3f C, Pixel color);
        void draw_quad_vec3f(Screen &screen, Vec3f A, Vec3f B, Vec3f C, Vec3f D, Pixel color);
        void draw_mesh_2D(Screen &screen, Mesh2D &mesh, Vec2f pos, float rot, float scale, Pixel color);
        void draw_mesh_3D(Screen &screen, Mesh3D &mesh, Vec3f pos, Vec3f rot, float scale, Pixel color);
        void draw_mesh_bounds(Screen &screen, Mesh3D &mesh, Vec3f pos, Vec3f rot, Pixel color);
    };
    void merge_mesh(Mesh3D &main_mesh, const Mesh3D &addon_mesh, Vec3f offset);
}
#endif
