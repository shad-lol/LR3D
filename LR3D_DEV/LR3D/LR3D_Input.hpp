#ifndef LR3D_INPUT_HPP
#define LR3D_INPUT_HPP

#include <windows.h>

namespace LR3D {
    struct Input {
        enum Keys {
            W = 0x57, A = 0x41, S = 0x53, D = 0x44,
            Space = 0x20, Esc = 0x1B, Left = 0x25, Right = 0x27, Up = 0x26, Down = 0x28
        };
        static bool isPressed(int key) {
            return (GetAsyncKeyState(key) & 0x8000);
        }
    };
}
#endif
