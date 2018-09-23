#include <iostream>
#include <tuple>
#include <utility>

#include "tuple_utils.hpp"

template<typename ...Ts>
auto last(std::tuple<Ts...> t)
{
    return std::get<sizeof...(Ts) - 1u>(t);
}

template <class T>
constexpr std::size_t size(const T &t)
{
    return std::tuple_size<T>::value;
}

template <class T>
auto last2(const T &t)
{
    return std::get<size(t) - 1u>(t);
}

int main()
{
    auto t = std::make_tuple(2, 3.14, 'c');
    __utils::print_tuple((t)); std::cout << std::endl; // => (2, 3.14, 'c')
    std::cout << size(t) << std::endl; // => 3
    std::cout << last(t) << ", " << last2(t) << std::endl; // => c, c
    return 0;
}

// filename: ch1-tuple-constexpr-last.cpp
// compile this> g++ ch1-tuple-constexpr-last.cpp -o ch1-tuple-constexpr-last.exe -std=c++14
