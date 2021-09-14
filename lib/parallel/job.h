#ifndef JOB_H
#define JOB_H

#include <thread>
#include <utility>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>

/**
 * Utilities for compute shaders
 */
namespace parallel {
    /**
     * Index to pass to each function execution
     * @param dimensions number of dimensions of the work (for example 2 dimensions for a graphics shader)
     */
    template<int dimensions>
    struct Index {
        unsigned int values[dimensions];
    };

    template<>
    struct Index<1> {
        unsigned int x;
    };

    template<>
    struct Index<2> {
        unsigned int x;
        unsigned int y;
    };

    template<>
    struct Index<3> {
        unsigned int x;
        unsigned int y;
        unsigned int z;
    };

    template<>
    struct Index<4> {
        unsigned int x;
        unsigned int y;
        unsigned int z;
        unsigned int w;
    };

    template<int dimensions>
    __inline Index<dimensions> getIndex(size_t index);

    /**
     * Number of threads to run the parallel functions
     */
    size_t threads = 1;
    /**
     * Helper class to initizalize the number of threads
     */
    namespace impl {
        class Initializer {
            private:
            static uint8_t func() {
                threads = std::thread::hardware_concurrency();
                return 0;
            }
            static uint8_t boi;
        };
        uint8_t Initializer::boi = Initializer::func();
    }

    /**
     * @brief Worker used to execute n times (proportionally to the number of threads running the parallel work).
     * @param func function to execute
     * @param dimensions size of the work (dimensions)
     * @param start start index
     * @param len number of times to run (n)
     * @param args arguments of the function to run
     */
    template<int dimensions, typename ...Args, typename ...F_args>
    void worker(void (*func)(Index<dimensions>, F_args...), size_t (&&size)[dimensions], size_t start, size_t len, Args&&... args) {
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
     * @brief Runs the parallel work on all the cores available on the system
     * @param func function to execute on parallel
     * @param dimensions size of the work (dimensions)
     * @param args arguments of the function to run   
     */
    template<int dimensions, typename ...Args, typename ...F_args>
    void run(void (*func)(Index<dimensions>, F_args...), size_t (&&size)[dimensions], Args&&... args) {
        long total_size = 1;
        for (size_t i = 0; i < dimensions; i++)
            total_size *= size[i];
        size_t divisor = total_size / threads;

        size_t created_threads = (total_size < threads) ? total_size:threads;
        std::future<void> buff_threads[created_threads];
        
        if (total_size < threads) {
            for (size_t i = 0; i < total_size; i++) {
                
                buff_threads[i] = std::async(std::launch::async, [&func, &size, i, &args...]() {
                worker(func, std::forward<decltype(size)>(size), i, 1, std::forward<Args>(args)...);
                });
            }
            return; 
        }

        for (size_t i = 0; i < threads; i++) {
            size_t len = ((total_size -= divisor) < 0) ? (divisor + total_size):divisor;
            buff_threads[i] = std::async(std::launch::async, [&func, &size, divisor, i, len, &args...]() {
                worker(func, std::forward<decltype(size)>(size), divisor * i, len, std::forward<Args>(args)...);
            });
        }

        for (size_t i = 0; i < created_threads; i++)
            buff_threads[i].wait();
    }

    /**
     * Allows loading a shader into memory and more importantly creates a thread only on the construction of the compute shader
     */
    template<int dimensions, typename F, typename ...Args>
    class LoadedComputeShader {
        public:
        /**
         * Runs the loaded shader one time
         */
        void run();
        /**
         * Waits until all the threads have been closed
         */ 
        ~LoadedComputeShader();

        private:
        /**
         * Loads the shader into memory and waits to run it
         */
        LoadedComputeShader(F func, int (&&size)[dimensions], Args&&... args);

        void loaded_worker(F func, int (&&size)[dimensions], size_t start, size_t len, Args&&... args);

        size_t created_threads = 0;
        std::future<void>* running_threads;
        bool exiting = false;
        bool runing = false;
        std::mutex run_mutex;
        std::mutex finished_mutex;
        std::condition_variable cv_run;
        std::condition_variable cv_finished_running;

        template<int d, typename ...A, typename ...F_args>
        friend decltype(auto) create_loaded_shader(void(*func)(Index<d>, F_args...), int (&&size)[d], A&&... args);
    };

    template<int dimensions, typename ...Args, typename ...F_args>
    decltype(auto) create_loaded_shader(void(*func)(Index<dimensions>, F_args...), int (&&size)[dimensions], Args&&... args);
}

#include "job.hpp"

#endif