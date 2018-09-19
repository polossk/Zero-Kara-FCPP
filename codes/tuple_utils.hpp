// filename: tuple_utils.hpp
#ifndef __TUPLE_UTILS_HPP__
#define __TUPLE_UTILS_HPP__

namespace __utils{

template<class Func, class Tuple, int N>
struct TupleHelper
{
    static void func(Func f, const Tuple &_)
    {
        TupleHelper<Func, Tuple, N - 1>::func(f, _);
        f(std::get<N - 1>(_), N - 1);
    }
};

template<class Func, class Tuple>
struct TupleHelper<Func, Tuple, 1>
{
    static void func(Func f, const Tuple &_)
    {
        f(std::get<0>(_), 0);
    }
};

template<class Func>
void manipulate_tuple(Func f, const std::tuple<> &_)
{
    std::cerr << "Warning! Manipulating an EMPTY tuple.";
};

template<class Func, class ...Args>
void manipulate_tuple(Func f, const std::tuple<Args...> &_)
{
    TupleHelper<Func, decltype(_), sizeof...(Args)>::func(f, _);
}

template<class ...Args>
void print_tuple(const std::tuple<Args...> &_)
{
    std::cout << "(";
    manipulate_tuple(
        [](auto _, std::size_t idx)
        {
            if (idx != 0) std::cout << ", ";
            std::cout << _;
        }, _);
    std::cout << ")";
}

} // namespace __utils
#endif // __TUPLE_UTILS_HPP__
