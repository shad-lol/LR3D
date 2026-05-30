#include "LR3D_Rasterizer.hpp"
#include "LR3D_Render.hpp"

namespace LR3D {
    Vec2f Rasterizer::fixed_camera_project(Vec3f A) {
        Vec2f B = {0, 0};
        B.x = (A.x / A.z) * SCALE + WIDTH / 2;
        B.y = (A.y / A.z) * SCALE + HEIGHT / 2;
        return B;
    }

    void Rasterizer::draw_line_dda(Framebuffer &framebuffer, Vec2f A, Vec2f B, Color color) {
        Vec2f dir = B - A;
        float dx = dir.x, dy = dir.y;
        int steps = std::max(abs((int)dx), abs((int)dy));
        if (steps < 1) steps = 1;
        Vec2f step = dir * (1.0f / steps), current = A;
        for (int i = 0; i <= steps; i++) {
            framebuffer.setPixel(roundf(current.x), roundf(current.y), color);
            current += step;
        }
    }

    void Rasterizer::draw_line_bresenham(Framebuffer &framebuffer, Vec2f A, Vec2f B, Color color) {
        int x1 = (int)roundf(A.x), y1 = (int)roundf(A.y), x2 = (int)roundf(B.x), y2 = (int)roundf(B.y);
        int dx = abs((int)x2 - x1), dy = -abs((int)y2 - y1);
        int sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1, err = dx + dy;
        while (x1 != x2 || y1 != y2) {
            framebuffer.setPixel(x1, y1, color);
            int e2 = 2 * err;
            if (e2 >= dy) {err += dy; x1 += sx;}
            if (e2 <= dx) {err += dx; y1 += sy;}
        }
    }

    void Rasterizer::draw_triangle_vec2f(Framebuffer &framebuffer, Vec2f A, Vec2f B, Vec2f C, Color color) {
        draw_line_bresenham(framebuffer, A, B, color);
        draw_line_bresenham(framebuffer, B, C, color);
        draw_line_bresenham(framebuffer, C, A, color);
    }

    void Rasterizer::draw_quad_vec2f(Framebuffer &framebuffer, Vec2f A, Vec2f B, Vec2f C, Vec2f D, Color color) {
        draw_line_bresenham(framebuffer, A, B, color);
        draw_line_bresenham(framebuffer, B, C, color);
        draw_line_bresenham(framebuffer, C, D, color);
        draw_line_bresenham(framebuffer, D, A, color);
    }

    void Rasterizer::draw_line_vec3f(Framebuffer &framebuffer, Vec3f A, Vec3f B, Color color) {
        if (A.z < 0.1f || B.z < 0.1f) return;
        Vec2f A_vec2f = fixed_camera_project(A);
        Vec2f B_vec2f = fixed_camera_project(B);
        draw_line_bresenham(framebuffer, A_vec2f, B_vec2f, color);
    }

    void Rasterizer::draw_triangle_vec3f(Framebuffer &framebuffer, Vec3f A, Vec3f B, Vec3f C, Color color) {
        if (A.z < 0.1f || B.z < 0.1f || C.z < 0.1f) return;
        Vec2f A_vec2f = fixed_camera_project(A), B_vec2f = fixed_camera_project(B), C_vec2f = fixed_camera_project(C);
        draw_triangle_vec2f(framebuffer, A_vec2f, B_vec2f, C_vec2f, color);
    }

    void Rasterizer::draw_quad_vec3f(Framebuffer &framebuffer, Vec3f A, Vec3f B, Vec3f C, Vec3f D, Color color) {
        if (A.z < 0.1f || B.z < 0.1f || C.z < 0.1f || D.z < 0.1f) return;
        Vec2f A_vec2f = fixed_camera_project(A), B_vec2f = fixed_camera_project(B), C_vec2f = fixed_camera_project(C), D_vec2f = fixed_camera_project(D);
        draw_quad_vec2f(framebuffer, A_vec2f, B_vec2f, C_vec2f, D_vec2f, color);
    }

    void Rasterizer::draw_mesh_2D(Framebuffer &framebuffer, Mesh2D &mesh, Vec2f pos, float rot, float scale, Color color) {
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
            draw_triangle_vec2f(framebuffer, tri[0], tri[1], tri[2], color);
        }
    }

    void Rasterizer::draw_mesh_3D(Framebuffer &framebuffer, Mesh3D &mesh, Vec3f pos, Vec3f rot, float scale, Color color) {
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
            draw_triangle_vec3f(framebuffer, tri[0], tri[1], tri[2], color);
        }
    }
}
