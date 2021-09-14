#ifndef UTILS_H
#define UTILS_H

namespace tpg // Vectors
{
    typedef unsigned int size_t; // Definición temporal

    class Vector2 {
        public:
        Vector2();
        Vector2(float x, float y);
        Vector2(float value);

        float x, y;
        private:
    };

    class Vector3 {
        public:
        Vector3();
        Vector3(float x, float y, float z);
        Vector3(float value);

        float x, y, z;
        private:
    };
} // namespace tpg

namespace tpg // Matrices
{
    template<int size>
    class Matrix {
        public:
        template<typename ...Args>
        Matrix(Args...);

        // Matrix functions
        float* operator[](const size_t index) const;
        Matrix operator*(const Matrix& other) const;

        private:
        // Attributes
        float internal_[size*size];

        // Methods
        template<typename ...Args>
        void set_matrix_arguments(float a, Args ...args);
        void set_matrix_arguments(float a);
    };
    
} // namespace tpg
#include "matrix.h"


#endif