#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "colors.h"

#include <cstring>

typedef unsigned char uint8_t;
// typedef unsigned int size_t; // Definición temporal
namespace tpg
{

    // Setup
    void setup();
    // Miscelaneous functions
    unsigned int get_screen_width();
    unsigned int get_screen_height();

    // Color declarations
    struct ColorRGB {
        uint8_t r = 0, g = 0, b = 0;
    };
    // ------------------

    namespace ColorModes {        
        typedef tpg::Color24 RGB24;
        typedef tpg::Color256 RGB256;
    }
    
    // FrameBuffer ################################################################
    class FB {
        public:
        const unsigned int& width = width_, &height = height_;
        FB(unsigned int width, unsigned int height); // Autodetects full size
        FB();

        // Main methods


        protected:
        /**
         * Renders the FrameBuffer into the terminal
         */
        void render();
        inline virtual void render_pixel(const size_t index) {};
        inline virtual void update_prev_internal() {};
        static unsigned int screen_width, screen_height;
        static bool to_run;
        unsigned int width_, height_;

        friend unsigned int get_screen_width();
        friend unsigned int get_screen_height();

        bool first_render = true;

        private:
        class SetupRunner {
            public:
            SetupRunner();
        };
        static SetupRunner sr_;
    };
    // #############################################################################

    // Implementación específica del FrameBuffer ###################################
    template<typename color_mode = tpg::ColorModes::RGB24>
    class FrameBuffer : public FB {
        public:
        /**
         * @brief Construct a new Frame Buffer object
         * 
         * @param width width of the new FrameBuffer
         * @param height height of the new FrameBuffer
         * 
         * @details If you want the size of the rendering frame buffer to be the current size of the terminal then use the empty constructor
         */
        FrameBuffer(const unsigned int width, const unsigned int height);
        /**
         * @brief Construct a new Frame Buffer object
         * 
         * @details The size of the render will be the current size of the terminal
         */
        FrameBuffer();
        ~FrameBuffer();

        /**
         * @brief Draws the stored FrameBuffer into the terminal
         * 
         * @details Before the rendering the cursor will go to the start of the terminal, henceforth the terminal won't scroll down
         */
        void render() { FB::render(); }

        // Operators
        /**
         * @param index Index of the wanted pixel
         * @return color_mode& of the pixel color at the specified index on the internal frame buffer
         */
        color_mode& operator[](const size_t index) {return internal_[index];}
        
        /**
         * @brief Gets the pixel at the desired position indicated by two coordinates
         * 
         * @param x x position of the pixel
         * @param y y position of the pixel
         * @return color_mode& of the pixel color at the specified position
         */
        color_mode& get_at_position(const size_t x, const size_t y) {return internal_[y * width + x];}

        protected:
        inline void render_pixel(const size_t index);
        inline void update_prev_internal();

        private:
        color_mode* internal_, *prev_internal_;
        color_mode black_;
    };

    #include "framebuffer.hpp"
    // #############################################################################
    
} // namespace tpg


#endif