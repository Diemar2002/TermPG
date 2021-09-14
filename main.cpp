// #include "./lib/utils/math/utils.h"
#include <stdio.h>
#include "./lib/render/framebuffer.h"
#include "./lib/parallel/job.h"
#include "./lib/canvas/canvas.h"
// #include "./lib/utils/vector.h"
#include <iostream>
#include <typeinfo>

#include <unistd.h>

#include <cmath>

// // void colorize(parallel::Index<2> index, tpg::FrameBuffer<tpg::ColorModes::RGB24>& fb) {
// //     tpg::Color24& buff = fb.get_at_position(index.x, index.y);
// //     buff = tpg::Color24(buff.r * index.x / fb.width, buff.g * index.y / fb.height, buff.g * 255 / iterations);
// // }

void loop(tpg::Canvas<>& canvas) {
    float x = rand() % canvas.width, y = rand() % canvas.height;
    tpg::Color24& color = canvas.get_at_position(x, y);
    color.r = 255; color.g = 255; color.b = 255;

    tpg::ColorRGBA black = {0, 0, 0, 1};
    for (size_t y = 0;  y < canvas.height; y++) {
        for (size_t x = 0; x < canvas.width; x++) {
            canvas.get_at_position(x, y) = black;
        } 
    }
}

int main() {
    tpg::Canvas c(loop);
    c.set_desired_frame_rate(99999999999);

    c.run();
}