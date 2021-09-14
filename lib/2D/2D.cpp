#include "2D.h"

#include <string.h>

graphics2D::Shape::Shape(const Tri* triangles, const size_t len) {
    tri = new Tri[len];
    memcpy(tri, triangles, len * sizeof(Tri));
    this->len = len;
}