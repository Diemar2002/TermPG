#include "job.h"
#include "./vendor/applyTuple.h"

#include <tuple>

template<typename ...Args>
Job<Args...>::Job(void (*func)(Args...), Args&&... args)
    : func_(func), args_(std::tuple<Args...>(std::forward<Args&&...>(args...))) {}

template<typename ...Args>
void Job<Args...>::run() {
    apply_from_tuple(func_, args_);
}

template<typename ...Args>
Job<Args...> create_job(void(*func)(Args...), Args ...args) {
    return Job<Args...>(func, std::forward<Args>(args)...);
}

template<int dimensions, typename ...Args>
Job<Index<dimensions>, Args...> create_job(void(*func)(Index<dimensions>, Args...), Args&&... args) {
    return Job<Index<dimensions>, Args...>(func, Index<dimensions>(), std::forward<Args&&>(args)...);
}

// Indexer
// template<int dimensions>
// Index<dimensions> Indexer<dimensions>::next() {
//     // I know this is unsecure but I don't care enough is two in the morning.
//     Index<dimensions> buff;
//     unsigned int* arr = (unsigned int*)((void*)(&buff));
//     unsigned int start_buff = start_;
//     unsigned int buffindex = index;

//     for (unsigned int i = (dimensions - 1); i > 0; i--) {
//         arr[i] = buffindex/ start_buff;
//         buffindex = buffindex % start_buff;
//         start_buff /= dimensions_[i - 1];
//     }
//     arr[0] = buffindex;

//     index++;
//     return buff;
// }

/**
 * TODO: hacer una implementación para sacar los índices (x, y, z, w...) dependiendo del número de dimensiones (con un bucle for).
 * Además, tienes que hacerlo modificando diréctamente la dirección de memoria porque si no tengo que hacerlo para el resto de dimensiones UwU. 
 */