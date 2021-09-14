#include "utils.h"

template<int size>
float* tpg::Matrix<size>::operator[](const size_t index) const {
    return internal_ + index * size;
}

// TODO: matrix multiplication implementation

template<int size>
tpg::Matrix<size> tpg::Matrix<size>::operator*(const Matrix& other) const {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            float sum = 0;
            for (size_t k = 0; k < size; k++)
                break;
                // sum += internal_[]
        }
    }
}
