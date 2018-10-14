#include <iostream>
#include <tuple>
#include <utility>
#include <functional>

using std::cout;
using std::endl;

template <class Func, class Tuple, std::size_t... I>
constexpr auto tuple_apply_impl(Func && f, Tuple && t,
    std::index_sequence<I...>)
{
    return std::invoke(std::forward<Func>(f), std::get<I>(std::forward<Tuple>(t))...);
}

template <class Func, class Tuple>
constexpr auto tuple_apply(Func && f, Tuple && t)
{
    return tuple_apply_impl(
        std::forward<Func>(f), std::forward<Tuple>(t),
        std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

int main()
{
    auto add = [](int a, int b)->int{ return a + b; };
    auto add_3_nums = [](int a, int b, int c)->int{ return a + b + c; };
    cout << std::apply(add, std::make_pair(1, 2)) << " ";
    cout << std::apply(add_3_nums, std::make_tuple(1, 2, 3)) << " ";
    cout << tuple_apply(add, std::make_pair(1, 2)) << " ";
    cout << tuple_apply(add_3_nums, std::make_tuple(1, 2))(4) << endl;
    // => 3 6 3 6
    return 0;
}

// filename: ch4-tuple-apply-example.cpp
// compile this> g++ ch4-tuple-apply-example.cpp -o ch4-tuple-apply-example.exe -std=c++17
