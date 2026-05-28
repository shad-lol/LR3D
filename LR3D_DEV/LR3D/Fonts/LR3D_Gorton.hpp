#ifndef LR3D_FONT_HPP
#define LR3D_FONT_HPP

#include "C:\Users\user\Desktop\some_stuff\3D training\LR3D\LR3D_Graphics.hpp"
#include <unordered_map>

namespace LR3D {
    inline std::unordered_map<char, Mesh2D> font = {
        {'A', {
            {
                {-0.50f, -0.50f},
                { 0.50f, -0.50f},
                { 0.00f,  0.50f},
                {-0.375f, -0.25f},
                { 0.375f, -0.25f}
            },
            {
                0, 2, 2,
                1, 2, 2,
                3, 4, 4
            }
        }}
    };
}

#endif
