#pragma once
#ifndef LR3D_RASTERIZER_HPP
#define LR3D_RASTERIZER_HPP
#include "LR3D_Math.hpp"
#include "LR3D_Framebuffer.hpp"
#include "LR3D_Color.hpp"
#include "LR3D_Mesh.hpp"

namespace LR3D {
    struct Rasterizer {
        Vec2f fixed_camera_project(Vec3f A);
        void draw_line_dda(Framebuffer &framebuffer, Vec2f A, Vec2f B, Color color);
        void draw_line_bresenham(Framebuffer &framebuffer, Vec2f A, Vec2f B, Color color);
        void draw_triangle_vec2f(Framebuffer &framebuffer, Vec2f A, Vec2f B, Vec2f C, Color color);
        void draw_quad_vec2f(Framebuffer &framebuffer, Vec2f A, Vec2f B, Vec2f C, Vec2f D, Color color);
        void draw_line_vec3f(Framebuffer &framebuffer, Vec3f A, Vec3f B, Color color);
        void draw_triangle_vec3f(Framebuffer &framebuffer, Vec3f A, Vec3f B, Vec3f C, Color color);
        void draw_quad_vec3f(Framebuffer &framebuffer, Vec3f A, Vec3f B, Vec3f C, Vec3f D, Color color);
        void draw_mesh_2D(Framebuffer &framebuffer, Mesh2D &mesh, Vec2f pos, float rot, float scale, Color color);
        void draw_mesh_3D(Framebuffer &framebuffer, Mesh3D &mesh, Vec3f pos, Vec3f rot, float scale, Color color);
        void draw_mesh_bounds(Framebuffer &framebuffer, Mesh3D &mesh, Vec3f pos, Vec3f rot, Color color);
    };
}
#endif
