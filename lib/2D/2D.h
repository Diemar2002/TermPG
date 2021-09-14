#ifndef _2D_H
#define _2D_H

namespace graphics2D {
    typedef unsigned int size_t;
    
    class Vertex {
        public:
        /**
         * @brief Construct a new Vertex object
         */
        Vertex() : x(0), y(0) {}
        /**
         * @brief Construct a new Vertex object
         * 
         * @param x x position of the vertex
         * @param y y position of the vertex
         */
        Vertex(const float x, const float y);
        

        private:
        float x, y;
    };

    class Tri {
        public:
        /**
         * @brief Construct a new Tri object
         * 
         * @param v1 first vertex
         * @param v2 second vertex
         * @param v3 third vertex
         */
        Tri(const Vertex& v1, const Vertex& v2, const Vertex& v3);
        Tri() = default;
        
        private:
        Vertex vertices[3];
    };

    class Shape {
        public:
        Shape(const Tri* triangles, const size_t len);

        private:
        Tri* tri;
        size_t len;
    };
}

#endif