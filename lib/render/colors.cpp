#include "colors.h"

#define APPLY_TRANSPARENCY(base, mask) \
base = ((unsigned int)base * (255 - (unsigned int)other.a)) / 255; \
mask = ((unsigned int)mask * (unsigned int)other.a) / 255; \
base = base + mask;

/**
 * Remember that Color256 has less color space than Color24 so the converted color won't necessarily be the same
 * @param other Color24 to convert to Color256
 * @return nearest Color256 from Color24
 */
tpg::Color256 color24_to_color256(const tpg::Color24 other) {
    const tpg::size_t offset = 21;
    tpg::Color256 buff;

    tpg::size_t mean = ((tpg::size_t)other.r + (tpg::size_t)other.g + (tpg::size_t)other.b) / 3;
    if (((mean - other.r) < offset) && ((mean - other.g) < offset) && ((mean - other.b) < offset))
        buff.color = ((mean * 24) / 255) + 232;
    else
        buff.color = 16 + 36 * ((unsigned int)other.r / (255 / 6)) + 6 * ((unsigned int)other.g / (255 / 6)) + ((unsigned int)other.b / (255 / 6));

    return buff;
} 

tpg::Color256 color24_to_color256(const tpg::ColorRGBA other) {
    color24_to_color256(tpg::Color24(other));
}

/**
 * @param other Color256 to convert to Color24
 * @return Color256 with the same color as the Color24 passed
 */
tpg::Color24 color256_to_color24(tpg::Color256 other) {
    tpg::Color24 buff;

    other.color -= 16;
    buff.r = (other.color % 36) * (255 / 5); \
    other.color /= 36; \
    buff.g = (other.color % 6) * (255 / 5); \
    other.color /= 6 * (255 / 5); \
    buff.b = other.color;

    return buff;
}

// ##############################################################
// ColorRGBA ####################################################

tpg::ColorRGBA::ColorRGBA(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
    : r(r), g(g), b(b), a(a) {}

tpg::ColorRGBA::ColorRGBA(const uint8_t gray, const uint8_t a)
    : r(gray), g(gray), b(gray), a(a) {}

tpg::ColorRGBA& tpg::ColorRGBA::operator=(const Color24& other) {
    r = other.r;
    g = other.g;
    b = other.b;
    a = 255;
    return *this;
}

bool tpg::ColorRGBA::operator!=(const ColorRGBA& other) {
    return (r != other.r) || (g != other.g) || (b != other.b) || (a != other.a);
}

bool tpg::ColorRGBA::operator==(const ColorRGBA& other) {
    return (r == other.r) && (g == other.g) && (b == other.b) && (a == other.a);
}

// ##############################################################
// Color24 ######################################################

tpg::Color24::Color24(const ColorRGBA& other)
    : r(other.r), g(other.g), b(other.b) {}

tpg::Color24::Color24(const uint8_t r, const uint8_t g, const uint8_t b)
    : r(r), g(g), b(b) {}

tpg::Color24::Color24(const uint8_t gray)
    : r(gray), g(gray), b(gray) {}

bool tpg::Color24::operator!=(const tpg::Color24& other) const {
    return (r != other.r) || (g != other.g) || (b != other.b);
}

bool tpg::Color24::operator==(const tpg::Color24& other) const {
    return (r == other.r) && (g == other.g) && (b == other.b);
}

tpg::Color24& tpg::Color24::operator=(const tpg::ColorRGBA& other) {
    tpg::ColorRGBA o(other);

    APPLY_TRANSPARENCY(r, o.r);
    APPLY_TRANSPARENCY(g, o.g);
    APPLY_TRANSPARENCY(b, o.b);
    
    return *this;
}

tpg::Color24::operator tpg::Color256() {
    Color24& buff = *this;
    return color24_to_color256(buff);
}

// ##############################################################
// Color256 #####################################################

tpg::Color256::Color256(const unsigned int other)
    : color((uint8_t)other) {}

bool tpg::Color256::operator!=(const tpg::Color256& other) const {
    return color != other.color;
}

bool tpg::Color256::operator==(const tpg::Color256& other) const {
    return color == other.color;
}

tpg::Color256& tpg::Color256::operator=(const ColorRGBA& other) {
    Color24 buffer = *this;
    tpg::ColorRGBA o(other);

    APPLY_TRANSPARENCY(buffer.r, o.r);
    APPLY_TRANSPARENCY(buffer.g, o.g);
    APPLY_TRANSPARENCY(buffer.b, o.b);

    color = color24_to_color256(buffer).color;

    return *this;
}

tpg::Color256& tpg::Color256::operator=(const unsigned int other) {
    color = other;
    return *this;
}

tpg::Color256::operator tpg::Color24() {
    return color256_to_color24(*this);
}

// Default Color ###################################################
tpg::Color& tpg::Color::operator=(const tpg::Color256& other) {
    this->color = other.color;
    tpg::Color24 buffer = color256_to_color24(other.color);
    r = buffer.r;
    g = buffer.g;
    b = buffer.b;
    a = 255;
}
tpg::Color& tpg::Color::operator=(const tpg::Color24& other) {
    r = other.r;
    g = other.g;
    b = other.b;
    a = 255;
    this->color = color24_to_color256(other).color;
}
tpg::Color& tpg::Color::operator=(const tpg::ColorRGBA& other) {
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
    this->color = color24_to_color256(other).color;
}