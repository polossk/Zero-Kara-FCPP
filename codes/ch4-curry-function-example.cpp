#include <iostream>
#include <tuple>
#include <functional>
#include <cassert>

using std::cout;
using std::endl;

#include "function_traits.hpp"

template <class Return>
auto curry_impl(std::function<Return()> && f)
{
    return std::forward<decltype(f)>(f);
}

template <class Return, class Arg>
auto curry_impl(std::function<Return(Arg)> && f)
{
    return std::forward<decltype(f)>(f);
}

template <class Return, class Arg, class... Args>
auto curry_impl(std::function<Return(Arg, Args...)> && f)
{
    auto _f = [f = std::forward<decltype(f)>(f)](Arg arg)
    {
        std::function<Return(Args...)> rest
            = [f = std::forward<decltype(f)>(f), arg=std::forward<Arg>(arg)]
            (Args... args)->Return { return f(arg, args...); };
        return curry_impl(std::forward<decltype(rest)>(rest));
    };
    return __utils::make_func(_f);
}

template <class Func> auto curry(Func && f)
{
    auto _f = __utils::make_func(f);
    return curry_impl(std::forward<decltype(_f)>(_f));
}

#define DISPLAY(expr) do { cout << #expr"= " << expr << endl; } while (0);

int main()
{
    cout << "-------- subtraction --------\n";
    auto sub = [](int a, int b)->int{ return a - b; };
    auto curried_sub = curry(sub);
    auto curried_sub_4 = curried_sub(4);
    DISPLAY(curried_sub_4(5)); // => -1
    DISPLAY(curried_sub(4)(5)); // => -1

    cout << "-------- multiplication --------\n";
    auto mul3 = curry([](int a, int b, int c)->int{ return a * b * c; });
    DISPLAY(mul3(2)(3)(4)); // => 24
    DISPLAY(mul3(3)(4)(5)); // => 60
    DISPLAY(mul3(4)(5)(6)); // => 120
    DISPLAY(mul3(5)(6)(7)); // => 210
    return 0;
}

// filename: ch4-curry-function-example.cpp
// compile this> g++ ch4-curry-function-example.cpp -o ch4-curry-function-example.exe
