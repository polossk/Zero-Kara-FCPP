#include <iostream>
#include <tuple>
#include <utility>

#include "tuple_utils.hpp"

int main()
{
    auto t1 = std::make_tuple(10, "abc", 'B', 32);
    int n = 11;
    auto t2 = std::make_tuple("bcd", 'C', 64);
    auto t3 = std::tuple_cat(t1, std::tie(n), t2, std::make_pair("abc", "def"));
    n = 16;
    __utils::print_tuple(t3); std::cout << std::endl;
    // => (10, abc, B, 32, 16, bcd, C, 64, abc, def)

    std::cout << std::tuple_size<decltype(t3)>::value << std::endl; // => 10

    std::tuple_element<1, decltype(t3)>::type hoge = "test";
    std::cout << hoge << std::endl; // => "test"

    std::tuple<> t_empty;
    __utils::print_tuple(t_empty);
    // => (Warning! Manipulating an EMPTY tuple.)
    return 0;
}

// filename: ch1-tuple-tools.cpp
// compile this> g++ ch1-tuple-tools.cpp -o ch1-tuple-tools.exe -std=c++14
