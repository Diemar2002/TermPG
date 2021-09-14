#include "framebuffer.h"

#define WINDOWS defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#if WINDOWS
    // FOR WINDOWS ONLY INCLUDES
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

#include <stdio.h>

// Runs the setup
tpg::FB::SetupRunner tpg::FB::sr_ = tpg::FB::SetupRunner();
tpg::FB::SetupRunner::SetupRunner() {
    // Setup
    // Get terminalk size
    #if WINDOWS
    #else
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        FB::screen_width = size.ws_col;
        FB::screen_height = size.ws_row * 2; // Two pixels in one terminal character
    #endif
}
// Static variables
unsigned int tpg::FB::screen_width = 0, tpg::FB::screen_height = 0;
// Miscelaneous functions
unsigned int tpg::get_screen_width() {return tpg::FB::screen_width;}
unsigned int tpg::get_screen_height() {return tpg::FB::screen_height;}

tpg::FB::FB(const unsigned int width, const unsigned int height)
    : width_(width), height_(height) {
}

tpg::FB::FB()
    : width_(screen_width), height_(screen_height) {
}

size_t cnt = 0; // DEBUG
static bool prev_changed; // For pixel render use only
// Render implementation
void tpg::FB::render() {
    // Sets the cursor at the begginig
    fputs("\033[0;0H", stdout);
    prev_changed = true;
    cnt = 0; // DEBUG
    for (size_t y = 0; y < height; y += 2) {
        for (size_t x = 0; x < width; x++) {
            render_pixel(x + y * width);
        }
        if (y < height - 2) printf("\n");
        fflush(stdout);
    }
    // Reset the style
    fputs("\033[0m", stdout);
    update_prev_internal();
    first_render = false;
}
/**
 * Inyects the number into the desired string starting at {@code index}. The length of the number will be 3 characters.
 * @param number number to be inyected into the desired string
 * @param buffer the string to which insert the number
 * @param index starting position to insert the number
 */
void set_3digit_integer(uint8_t number, char* buffer, size_t index);

#define RENDER_PIXEL_BOILERPLATE \
    bool hide_last_line = (index + width) > (width * height);\
    auto& upper = internal_[index]; \
    auto& prev_upper = prev_internal_[index]; \
    auto& lower = hide_last_line ? black_:internal_[index + width]; \
    auto& prev_lower = hide_last_line ? black_:prev_internal_[index + width]; \
    bool has_changed = (upper != prev_upper) || (hide_last_line ? false:(lower != prev_lower)) || first_render;

#define RENDER_PIXEL_PRINT(buffer) \
    if (!prev_changed) { \
        set_3digit_integer(index % width, RGB24_ansi, 6); \
        set_3digit_integer((index / width) / 2, RGB24_ansi, 2); \
        fputs(buffer, stdout);} else fputs(buffer + 10, stdout);
// Render pixel specializations ############################################################
// RGB24 color pixel render implementation
char RGB24_ansi[] = "\033[000;000H\033[38;2;000;000;000m\033[48;2;000;000;000m▄";
//                         ^   ^  ^    ^     ^   ^   ^       ^     ^   ^   ^
//                         x   y  |  lower   r   g   b     upper   r   g   b 
// index   :               2   6  |          17   21  25           36  40  44
//                                without position: 10  
template<> void tpg::FrameBuffer<tpg::ColorModes::RGB24>::render_pixel(const size_t index) {
    // Check if is the same
    RENDER_PIXEL_BOILERPLATE;
    if (!has_changed) cnt++;
    // TODO: si los dos pixeles tienen el mismo color que los dos anteriores no se tiene que actualizar, simplemente imprimir el medio bloque.
    if (has_changed) {
        set_3digit_integer(upper.r, RGB24_ansi, 36); // )
        set_3digit_integer(upper.g, RGB24_ansi, 40); // | Upper pixel
        set_3digit_integer(upper.b, RGB24_ansi, 44); // )

        if (!hide_last_line) {
            set_3digit_integer(lower.r, RGB24_ansi, 17); // )
            set_3digit_integer(lower.g, RGB24_ansi, 21); // | Lower pixel
            set_3digit_integer(lower.b, RGB24_ansi, 25); // )        
        } else {
            memset(RGB24_ansi + 17, '0', 3);
            memset(RGB24_ansi + 21, '0', 3);
            memset(RGB24_ansi + 25, '0', 3);
        }
        RENDER_PIXEL_PRINT(RGB24_ansi);
    }
    
    prev_changed = has_changed;
}
// RGB256 color pixel implementation
char RGB256_ansi[] = "\033[000;000H\033[38;5;000m\033[48;5;000m▄";
//                          ^   ^  ^    ^     ^       ^     ^
//                          x   y  |   lower color   upper color
//                          2   6  |          17            28
//                               without position: 10
template<> void tpg::FrameBuffer<tpg::ColorModes::RGB256>::render_pixel(const size_t index) {
    RENDER_PIXEL_BOILERPLATE;

    if (has_changed) {
        set_3digit_integer(upper.color, RGB256_ansi, 28); // Upper pixel

        if (!hide_last_line)
            set_3digit_integer(lower.color, RGB256_ansi, 17); // Lower pixel
        else
            memset(RGB256_ansi + 17, '0', 3);

        RENDER_PIXEL_PRINT(RGB256_ansi);
    }

    prev_changed = has_changed;
}
// ########################################################################################
// Set print parameters####################################################################
__inline void set_3digit_integer(uint8_t number, char* buffer, size_t index) {
    buffer[index + 2] = (number % 10) + '0';
    number /= 10; 
    buffer[index + 1] = (number % 10) + '0';
    number /= 10;
    buffer[index] = (number % 10) + '0';
}