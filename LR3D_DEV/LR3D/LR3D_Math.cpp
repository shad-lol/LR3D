#include "LR3D_Math.hpp"

namespace LR3D {

    float Vec3f::length() const {return std::sqrt(x * x + y * y + z * z);}

    Vec3f Vec3f::normalize() const {
        float len = length();
        if (len < 0.00001f) return {0, 0, 0};
        return {x / len, y / len, z / len};
    }

    float Vec3f::dot(const Vec3f &a, const Vec3f &b) {return a.x * b.x + a.y * b.y + a.z * b.z;}

    Vec3f Vec3f::rotate(Vec3f v, Vec3f rad) {
        float sx = std::sin(rad.x), cx = std::cos(rad.x);
        float sy = std::sin(rad.y), cy = std::cos(rad.y);
        float sz = std::sin(rad.z), cz = std::cos(rad.z);
        float y1 = v.y * cx - v.z * sx;
        float z1 = v.y * sx + v.z * cx;
        v.y = y1; v.z = z1;
        float x2 = v.x * cy + v.z * sy;
        float z2 = -v.x * sy + v.z * cy;
        v.x = x2; v.z = z2;
        float x3 = v.x * cz - v.y * sz;
        float y3 = v.x * sz + v.y * cz;
        v.x = x3; v.y = y3;
        return v;
    }
}
