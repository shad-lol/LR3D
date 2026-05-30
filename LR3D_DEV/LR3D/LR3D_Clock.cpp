#include "LR3D_Clock.hpp"
#include <thread>

namespace LR3D {

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
}
