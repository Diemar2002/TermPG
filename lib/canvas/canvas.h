#ifndef CANVAS_H
#define CANVAS_H
#include "../render/framebuffer.h"
#include "../2D/2D.h"

namespace tpg {
    template<typename color_mode = tpg::ColorModes::RGB24>
    class Canvas : public FrameBuffer<color_mode> {
        public:
        Canvas(void(*loop)(Canvas<color_mode>&), const unsigned int width, const unsigned int height);
        Canvas(void(*loop)(Canvas<color_mode>&));

        /**
         * @brief starts the draw loop that will run n times per second (being n redefineable)
         * 
         */
        void run();
        /**
         * @brief stops the execution of the draw loop
         * 
         */
        void stop() { running = false; }

        /**
         * @brief Set the desired frame rate object
         * 
         * @param frame_rate 
         */
        void set_desired_frame_rate(const size_t frame_rate) { desired_frame_rate_ = frame_rate; }
        const float& frame_rate = frame_rate_;
        const float& t = current_time_;
        const float& dt = delta_time_;

        void draw(const graphics2D::Shape& shape, tpg::Color color);

        private:

        void(*func)(Canvas<color_mode>&);
        bool running = true;

        size_t desired_frame_rate_ = 60;
        float frame_rate_, delta_time_, current_time_;
    };
    #include "canvas.hpp"
}

#endif