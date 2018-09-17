#include <iostream>
#include <string>
#include <tuple>

template<class Func, class Tuple, int N>
struct TupleHelper
{
    static void func(Func f, const Tuple &_)
    {
        TupleHelper<Func, Tuple, N - 1>::func(f, _);
        f(std::get<N - 1>(_));
    }
};

template<class Func, class Tuple>
struct TupleHelper<Func, Tuple, 1>
{
    static void func(Func f, const Tuple &_)
    {
        f(std::get<0>(_));
    }
};

template<class Func, class ...Args>
void manipulate_tuple(Func f, const std::tuple<Args...> &_)
{
    TupleHelper<Func, decltype(_), sizeof...(Args)>::func(f, _);
}

int main()
{
    manipulate_tuple([](auto _){ std::cout << " " << _; },
                     std::make_tuple(10, 1.5, 'A'));
    // => 10 1.5 A
    return 0;
}

// filename: ch1-tuple-helper.cpp
// compile this> g++ ch1-tuple-helper.cpp -o ch1-tuple-helper.exe -std=c++14
