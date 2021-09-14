#include "job.h"

#include <string.h>

// template<int dimensions, typename F_args, typename ...Args>
// void parallel::LoadedComputeShader::

template<int dimensions, typename F, typename ...Args>
parallel::LoadedComputeShader<dimensions, F, Args...>::LoadedComputeShader(F func, int (&&size)[dimensions], Args&&... args) {
    std::unique_lock<std::mutex> lck(run_mutex);

    long total_size = 1;
    for (size_t i = 0; i < dimensions; i++)
        total_size *= size[i];
    size_t divisor = total_size / threads;

    created_threads = (total_size < threads) ? total_size:threads;
    running_threads = new std::future<void>[created_threads];

    if (total_size < threads) {
        for (size_t i = 0; i < total_size; i++) {
            running_threads[i] = std::async(std::launch::async, [&func, &size, i, &args...]() {
            worker(func, std::forward<decltype(size)>(size), i, 1, std::forward<Args>(args)...);
            });
            // Copy tmp to heap for when closing the threads
        }
        return;
    }

    for (size_t i = 0; i < threads; i++) {
        size_t len = ((total_size -= divisor) < 0) ? (divisor + total_size):divisor;
        running_threads[i] = std::async(std::launch::async, [&func, &size, divisor, i, len, &args..., this]() {
            // Run worker function
            this->loaded_worker(func, std::forward<decltype(size)>(size), divisor * i, len, std::forward<Args>(args)...);
        });
    }

}

template<int dimensions, typename F, typename ...Args>
parallel::LoadedComputeShader<dimensions, F, Args...>::~LoadedComputeShader() {
    fflush(stdout);
    exiting = true;
    for (size_t i = 0; i < created_threads; i++)
        running_threads[i].wait();
    
    delete[] running_threads;
}

template<int dimensions, typename ...Args, typename ...F_args>
decltype(auto) parallel::create_loaded_shader(void(*func)(Index<dimensions>, F_args...), int (&&size)[dimensions], Args&&... args) {
    return LoadedComputeShader<dimensions, void(*)(Index<dimensions>, F_args...), Args...>(
        std::forward<decltype(func)>(func),
        std::forward<decltype(size)>(size),
        std::forward<Args>(args)...
    );
}

// Execution of the shader
template<int dimensions, typename F, typename ...Args>
void parallel::LoadedComputeShader<dimensions, F, Args...>::loaded_worker(F func, int (&&size)[dimensions], size_t start, size_t len, Args&&... args) {
    if (exiting)
        return;
    
    std::unique_lock<std::mutex> lck{run_mutex};
    cv_run.wait(lck, [&]() {
        return exiting;
    });

    size_t start__ = 1;
        for (int i = 0; i < (dimensions - 1); i++)
            start__ *= size[i];

        for (size_t index = 0; index < len; index++) {
            Index<dimensions> buff;
            unsigned int* arr = (unsigned int*)((void*)(&buff));
            unsigned int start_buff = start__;
            unsigned int buffindex = index + start;

            for (unsigned int j = (dimensions - 1); j > 0; j--) {
                arr[j] = buffindex / start_buff;
                buffindex = buffindex % start_buff;
                start_buff /= size[j - 1];
            }
            arr[0] = buffindex;
            func(buff, std::forward<Args>(args)...);
        }
}

/**
 * TODO: Hacer que funcione con std::thread (ver como pasar la función al thread además de los argumentos). Para ello igual hay que tener en cuenta el siguiente TODO.
 * 
 * TODO: Para la poder cambiar las variables mediante la funcion run se podría hacer un cambio pasando un lambda que se almacene en la clase antes de
 * avisar a los threads, después se puede utilizar ese lambda para que los threads lo ejecuten.
 * 
 * Para ello recomiendo mirar el tipo de dato de los lambda para que funcione bien con los templates
 */

template<int dimensions, typename F, typename ...Args>
void parallel::LoadedComputeShader<dimensions, F, Args...>::run() {
    {
        std::unique_lock<std::mutex> lck(run_mutex);
        cv_run.notify_all();
    }
    std::unique_lock<std::mutex> lck(finished_mutex);
    cv_finished_running.wait(lck);
}