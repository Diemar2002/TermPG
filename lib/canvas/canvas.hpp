#include "canvas.h"

#include <chrono>
#include <stdio.h>

template<typename color_mode>
tpg::Canvas<color_mode>::Canvas(void(*func)(Canvas<color_mode>&))
    : FrameBuffer<color_mode>{}, func(func) {
}

template<typename color_mode>
tpg::Canvas<color_mode>::Canvas(void(*func)(Canvas<color_mode>&), const unsigned int width, const unsigned int height)
    : FrameBuffer<color_mode>{width, height}, func(func) {
}

template<typename color_mode>
void tpg::Canvas<color_mode>::run() {

    while (running) {
        auto start = std::chrono::high_resolution_clock::now();

        func(*this); // Ejecución de la función personalizada
        FrameBuffer<color_mode>::render();

        // Cálculo del tiempo 
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;
        if (desired_frame_rate_ != -1) {
            float waitTime = (1.0f / desired_frame_rate_) - duration.count(); // Tiempo a esperar para cumplir el frameRate

            if (waitTime > 0) {
                #ifdef __unix__
                    std::this_thread::sleep_for(std::chrono::duration<float>(waitTime));
                #elif defined(WIN32)
                    Sleep(waitTime * 1000);
                #endif
            }
        }
        // Cálculo final del tiempo para calcular el frameRate
        end = std::chrono::high_resolution_clock::now();
        
        duration = end - start;

        delta_time_ = duration.count();
        frame_rate_ = 1.0f / delta_time_;
        current_time_ += delta_time_;
    }
    
}

// #######################################################################
// Rasterization and shape drawing
// #######################################################################

// FACTORIO.run();
template<typename color_mode>
void tpg::Canvas<color_mode>::draw(const graphics2D::Shape& shape, tpg::Color color) {
    // for (int i = (shape.len - 1); i >= 0; i--) {
    //     // Raster
    // }
}

