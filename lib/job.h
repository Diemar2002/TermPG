#ifndef JOB_H
#define JOB_H

#include <tuple>

template<int dimensions>
struct Index;

class BaseJob {
    public:
    virtual void run() {};
};


// Remove first element of tuple
namespace details
{
    template <typename Tuple, size_t... Indices>
    auto unshift_tuple_with_indices(
        const Tuple& tuple,
        std::index_sequence<Indices...> index_type)
    {
        return std::make_tuple(std::get<Indices + 1>(tuple)...);
    }
}

template <typename T, typename... Ts>
std::tuple<Ts...> unshift_tuple(const std::tuple<T, Ts...>& tuple)
{
    return details::unshift_tuple_with_indices(tuple, std::index_sequence_for<Ts...>());
}
// -----------------------------

template <typename ...Args>
class Job : public BaseJob {
    public:

    Job(void (*func)(Args...), Args&&... args);
    
    //Job(const Job& other) = default;
    void run() override;

    template<int dimensions>
    void run(Index<dimensions>&& i) {
        apply_from_tuple_indexed(std::forward<decltype(i)>(i), func_, unshift_tuple(args_));
    };

    private:
    void (*func_)(Args...);

    std::tuple<Args&&...> args_;
    template<int index>
    typename std::tuple_element<index, decltype(args_)>::type& get_argument_at() {return std::get<index>(args_);};
};

template<typename ...Args>
Job<Args...> create_job(void(*func)(Args...), Args ...args);

// Indexer
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

// template<int dimensions>
// class Indexer {
//     public:

//     template<typename ...Args>
//     Indexer(Args... args) {
//         static_assert(sizeof...(args) == dimensions);
//         unsigned int buff[] = {(unsigned int)args...};
//         for (unsigned int i = 0; i < dimensions; i++) {
//             dimensions_[i] = (unsigned int)buff[i];
//             start_ *= dimensions_[i];
//         }
//         start_ /= dimensions_[dimensions - 1];
//     }

//     Index<dimensions> next();
//     void run(BaseJob j);
//     template<typename J>
//     void run(J j) {
//         unsigned int n = 1;
//         for (unsigned int i = 0; i < dimensions; i++)
//             n *= dimensions_[i];
    
//         for (unsigned int i = 0; i < n; i++)
//             j.run(next());
//     }
    
//     Index<dimensions> start() {return Index<dimensions>();}

//     private:
//     unsigned int dimensions_[dimensions];
//     unsigned int index = 0;
//     unsigned int start_ = 1;

//     public:
//     unsigned int disected_position_[dimensions] = {0,};
// };


// template<int dimensions, typename ...Args>
// Job<Index<dimensions>, Args...> create_job_indexed(void(*func)(Index<dimensions>, Args...), Args ...args);

// template<typename ...Args>
// Job<Args...> create_job(void(*func)(Args...), Args&& ...args);

#include "job.hpp"
#endif