#pragma once
#ifndef LR3D_CLOCK_HPP
#define LR3D_CLOCK_HPP
#include <chrono>

namespace LR3D {
    struct Clock {
        std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
        float deltaTime = 0;
        Clock();
        void tick(int fps);
    };
}
#endif
