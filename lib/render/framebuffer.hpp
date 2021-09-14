#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "framebuffer.h"

template<typename color_mode>
tpg::FrameBuffer<color_mode>::FrameBuffer(const unsigned int width, const unsigned int height)
    : FB{width , height} {
        internal_ = new color_mode[width * height];
        prev_internal_ = new color_mode[width * height];
}

template<typename color_mode>
tpg::FrameBuffer<color_mode>::FrameBuffer()
    : FB{} {
        internal_ = new color_mode[FB::screen_width * FB::screen_height];
        prev_internal_ = new color_mode[FB::screen_width * FB::screen_height];
}

template<typename color_mode>
tpg::FrameBuffer<color_mode>::~FrameBuffer() {
    delete[] internal_;
    delete[] prev_internal_;
}

template<typename color_mode>
void tpg::FrameBuffer<color_mode>::update_prev_internal() {
    memcpy(prev_internal_, internal_, sizeof(color_mode) * width * height);
}

// Render specialization implementation
// RGB24 color pixel render implementation
template<> void tpg::FrameBuffer<tpg::ColorModes::RGB24>::render_pixel(const size_t index);
// RGB256 color pixel implementation
template<> void tpg::FrameBuffer<tpg::ColorModes::RGB256>::render_pixel(const size_t index);

#endif