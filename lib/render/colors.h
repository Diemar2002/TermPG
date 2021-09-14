#ifndef COLORS_H
#define COLORS_H

namespace tpg
{
    typedef unsigned int size_t; // Temporal definition
    typedef	unsigned char uint8_t;

    class Color24;
    class Color256;

    // ColorRGBA
    /**
     * 32bit color mode
     * Stores the value of the color in three different variables: r, g, b and an alpha value.
     */
    class ColorRGBA {
        public:
        // Red, green and blue components of the color
        uint8_t r = 0, g = 0, b = 0;
        // Alpha value of the color
        uint8_t a;

        ColorRGBA() = default;
        /**
         * Sets each component of the color individually. Defaults alpha value to 255 (max.)
         * @param r Red component of the color
         * @param g Green component of the color
         * @param b Blue component of the color
         * @param a (optional) Alpha value of the color
         */
        ColorRGBA(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255);
        /**
         * Sets all the components of the color to the gray shade. Default alpha value to 255 (max.)
         * @param gray value for all components
         * @param alpha (optional) Alpha value of the color
         */
        ColorRGBA(const uint8_t gray, const uint8_t alpha);

        ColorRGBA& operator=(const Color24& other);

        bool operator!=(const ColorRGBA& other);
        bool operator==(const ColorRGBA& other);
    };


    // Color24
    /**
     * 24bit color mode
     * Stores the value of the color in three different variables: r, g, b
     */
    class Color24 {
        public:
        // Red, green and blue components
        uint8_t r = 0, g = 0, b = 0;

        Color24() = default;
        /** 
         * @brief Sets each component of the color individually
         * @param r Red component of the color
         * @param g Green component of the color
         * @param b Blue component of the color
         */
        Color24(const uint8_t r, const uint8_t g, const uint8_t b);
        /**
         * @brief Sets all the components of the color to the same value
         * @param gray Value to set the r, g & b componenets
         */
        Color24(const uint8_t gray);
        Color24(const ColorRGBA& other);

        Color24& operator=(const ColorRGBA& other);
        Color24& operator=(const Color24& other) = default;

        operator Color256();

        bool operator!=(const Color24& other) const;
        bool operator==(const Color24& other) const;
    };
    // Color256
    class Color256 {
        public:
        uint8_t color;
        Color256() = default;
        Color256(const unsigned int other);

        Color256& operator=(const ColorRGBA& other);
        Color256& operator=(const Color256& other) = default;
        Color256& operator=(const unsigned int other);

        operator Color24();

        bool operator!=(const Color256& other) const;
        bool operator==(const Color256& other) const;
    };

    // ########################
    // Main color transformator
    class Color {
        public:
        Color();
        Color(Color24);
        Color(Color256);

        Color& operator=(const Color256& other);
        Color& operator=(const Color24& other);
        Color& operator=(const ColorRGBA& other);
        
        private:
        uint8_t r = 0, g = 0, b = 0, a = 0; // RGB24
        uint8_t color;
    };

} // namespace tpg

#endif