#ifndef MATRIX_H
#define MATRIX_H

#include "utils.h"

template<int size>
template<typename ...Args>
void tpg::Matrix<size>::set_matrix_arguments(float a, Args ...args) {
    internal_[(size * size) - sizeof...(args) - 1] = a;
    if (sizeof...(args) != 0)
        set_matrix_arguments(args...);
}

template<int size>
void tpg::Matrix<size>::set_matrix_arguments(float a) {
    internal_[size * size - 1] = a;
}

template<int size>
template<typename ...Args>
tpg::Matrix<size>::Matrix(Args ...args) {
    static_assert(sizeof...(args) == (size * size), "Invalid number of arguments for matrix inizialization");
    set_matrix_arguments(args...);
}

#endif