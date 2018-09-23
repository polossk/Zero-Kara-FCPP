#include <iostream>
#include <tuple>
#include <utility>

#include "tuple_utils.hpp"

template<std::size_t ...Ns, typename ...Ts>
auto init_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t)
{
    return std::make_tuple(std::get<Ns>(t)...);
}

template<typename ...Ts>
auto init(std::tuple<Ts...> t)
{
    return init_impl(std::make_index_sequence<sizeof...(Ts) - 1u>() , t);
}

template<typename ...Ts>
auto last(std::tuple<Ts...> t)
{
    return std::get<sizeof...(Ts) - 1u>(t);
}

template <class T>
auto size(T t)
{
    return std::tuple_size<T>::value;
}

template <class T>
auto last2(T t)
{
    return std::get<std::tuple_size<T>::value - 1u>(t);
}

int main()
{
    auto t = std::make_tuple(2, 3.14, 'c');
    std::cout << size(t) << std::endl; // => 3
    std::cout << last(t) << std::endl; // => 'c'
    std::cout << last2(t) << std::endl; // => 'c'
    __utils::print_tuple(init(t)); std::cout << std::endl; // => (2, 3.14)
    return 0;
}

// filename: ch1-tuple-init-and-last.cpp
// compile this> g++ ch1-tuple-init-and-last.cpp -o ch1-tuple-init-and-last.exe -std=c++14
