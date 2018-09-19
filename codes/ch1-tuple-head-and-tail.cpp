#include <iostream>
#include <tuple>
#include <utility>

#include "tuple_utils.hpp"

template<typename T, typename ...Ts>
auto head(std::tuple<T, Ts...> t)
{
    return std::get<0>(t);
}

template<std::size_t ...Ns, typename ...Ts>
auto tail_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t)
{
    return std::make_tuple(std::get<Ns + 1u>(t)...);
}

template <typename ...Ts>
auto tail(std::tuple<Ts...> t)
{
    return tail_impl(std::make_index_sequence<sizeof...(Ts) - 1u>() , t);
}

int main()
{
    auto t = std::make_tuple(2, 3.14, 'c');
    std::cout << head(t) << std::endl; // => 2
    __utils::print_tuple(tail(t)); std::cout << std::endl; // => (3.14, 'c')
    return 0;
}

// filename: ch1-tuple-head-and-tail.cpp
// compile this> g++ ch1-tuple-head-and-tail.cpp -o ch1-tuple-head-and-tail.exe -std=c++14
