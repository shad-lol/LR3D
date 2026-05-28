#ifndef LR3D_MATH_HPP
#define LR3D_MATH_HPP

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#define DEG_TO_RAD 0.0174532925f
#endif

namespace LR3D {

    struct Vec2f {
        float x, y;
        Vec2f() : x(0), y(0) {}
        Vec2f(float _x, float _y) : x(_x), y(_y) {}
        Vec2f operator+(const Vec2f &other) const { return {x + other.x, y + other.y}; }
        Vec2f operator-(const Vec2f &other) const { return {x - other.x, y - other.y}; }
        Vec2f operator*(float s) const { return {x * s, y * s}; }
        Vec2f& operator+=(const Vec2f &other) { x += other.x; y += other.y; return *this; }
        Vec2f& operator-=(const Vec2f &other) { x -= other.x; y -= other.y; return *this; }
        Vec2f& operator*=(float s) { x *= s; y *= s; return *this; }
    };

    struct Vec3f {
        float x, y, z;
        Vec3f() : x(0), y(0), z(0) {}
        Vec3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
        Vec3f operator+(const Vec3f &other) const {return {x + other.x, y + other.y, z + other.z};}
        Vec3f operator-(const Vec3f &other) const {return {x - other.x, y - other.y, z - other.z};}
        Vec3f operator*(float s) const {return {x * s, y * s, z * s};}
        Vec3f& operator+=(const Vec3f &other) {x += other.x; y += other.y; z += other.z; return *this;}
        Vec3f& operator-=(const Vec3f &other) {x -= other.x; y -= other.y; z -= other.z; return *this;}
        Vec3f& operator*=(float s) {x *= s; y *= s; z *= s; return *this;}
        float length() const;
        Vec3f normalize() const;
        static float dot(const Vec3f &a, const Vec3f &b);
        static Vec3f rotate(Vec3f v, Vec3f rad);
    };
}
#endif
