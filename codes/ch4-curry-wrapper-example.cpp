#include <iostream>
#include <tuple>
#include <functional>
#include <cassert>

using std::cout;
using std::endl;

#include "function_traits.hpp"

template <class Func, class Tuple, std::size_t... I>
constexpr auto tuple_apply_impl(const Func &f, Tuple && t, std::index_sequence<I...>)
{
    return f(std::get<I>(std::forward<Tuple>(t))...);
}

template <class Func, class Tuple>
constexpr auto tuple_apply(Func && f, Tuple && t)
{
    return tuple_apply_impl( std::forward<Func>(f), std::forward<Tuple>(t),
        std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

template <class Func, class Tuple = std::tuple<> >
class curried_functor
{
private:
    Func f_;
    Tuple t_;
    std::size_t argc_;
    using final_ret_type = typename __utils::function_traits<Func>::return_type;
public:
    curried_functor(Func && f)
        : f_(std::forward<Func>(f)), argc_(__utils::arity(f_)) {}
    curried_functor(const Func &f, const Tuple &t)
        : f_(f), argc_(__utils::arity(f_)), t_(t) {}
    template <class... Args>
    auto operator()(Args&&... args) const -> final_ret_type
    {
        std::size_t tuple_len = std::tuple_size<Tuple>::value;
        std::size_t args_len = sizeof...(args);
        assert(tuple_len + args_len == argc_); // test if parameters don't match!
        auto argv = std::tuple_cat(t_, std::make_tuple(std::forward<Args>(args)...));
        return tuple_apply(f_, argv);
    }
    template <class Arg> auto curry(Arg && arg) const
    {
        std::size_t tuple_len = std::tuple_size<Tuple>::value;
        std::size_t arg_len = 1;
        assert(tuple_len + arg_len <= argc_); // test if too much curry!
        auto argv = std::tuple_cat(t_, std::make_tuple(std::forward<Arg>(arg)));
        return curried_functor<Func, decltype(argv)>(f_, argv);
    }
};

template <class Func> auto curry(Func && f)
{
    auto _f = __utils::make_func(f);
    return curried_functor<decltype(_f)>(std::forward<decltype(_f)>(_f));
}

#define DISPLAY(expr) do { cout << #expr"= " << expr << endl; } while (0);

int main()
{
    cout << "-------- 4 - 5= ? --------\n";
    auto sub = [](int a, int b)->int{ return a - b; };
    auto curried_sub = curry(sub);
    auto curried_sub_4 = curried_sub.curry(4);
    DISPLAY(curried_sub(4, 5)); // => -1
    DISPLAY(curried_sub_4(5)); // => -1
    DISPLAY(curried_sub.curry(4).curry(5)()); // => -1

    cout << "-------- 2 * 3 * 4= ? --------\n";
    auto mul3 = curry([](int a, int b, int c)->int{ return a * b * c; });
    DISPLAY(mul3(2, 3, 4)); // => 24
    DISPLAY(mul3.curry(2)(3, 4)); // => 24
    DISPLAY(mul3.curry(2).curry(3)(4)); // => 24
    DISPLAY(mul3.curry(2).curry(3).curry(4)()); // => 24
    return 0;
}

// filename: ch4-curry-wrapper-example.cpp
// compile this> g++ ch4-curry-wrapper-example.cpp -o ch4-curry-wrapper-example.exe
