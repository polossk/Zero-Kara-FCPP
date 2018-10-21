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

template <class... Fs>
class composited_functor
{
private:
    const std::tuple<Fs...> fs_;
    const static std::size_t _size_ = sizeof...(Fs);

    template <class Arg, std::size_t I>
    auto call_impl(Arg && arg, const std::index_sequence<I>&) const
        -> decltype(std::get<I>(fs_)(std::forward<Arg>(arg)))
    {
        return std::get<I>(fs_)(std::forward<Arg>(arg));
    }

    template <class Arg, std::size_t I, std::size_t... Is>
    auto call_impl(Arg && arg, const std::index_sequence<I, Is...>&) const
        -> decltype(call_impl(std::get<I>(fs_)(std::forward<Arg>(arg)), std::index_sequence<Is...>{}))
    {
        return call_impl(std::get<I>(fs_)(std::forward<Arg>(arg)), std::index_sequence<Is...>{});
    }

    template <class Arg> auto call(Arg && arg) const
        -> decltype(call_impl(std::forward<Arg>(arg), std::make_index_sequence<_size_>{}))
    {
        return call_impl(std::forward<Arg>(arg), std::make_index_sequence<_size_>{});
    }
public:
    composited_functor() : fs_(std::tuple<>()) {}
    composited_functor(std::tuple<Fs...> && fs) : fs_(std::forward<decltype(fs)>(fs)) {}
    
    template <class F>
    inline auto composite(F && f) const
    {
        return composited_functor<Fs..., F>(std::tuple_cat(fs_, std::make_tuple(std::forward<F>(f))));
    }

    template <class Arg>
    inline auto operator()(Arg && arg) const
    {
        return call(std::forward<Arg>(arg));
    }
};

template <class Arg, class F>
auto operator|(Arg && arg, F && f) -> decltype(f(std::forward<Arg>(arg)))
{
    return f(std::forward<Arg>(arg));
}

template <class... Fs, class F>
auto operator,(composited_functor<Fs...> && composited, F && f)
{
    return composited.composite(std::forward<F>(f));
}

// ($) :: (a -> b) -> a -> b
// f $ x = f x
template <class Return, class Arg>
auto operator<<(std::function<Return(Arg)> && f, Arg && arg)
{
    return f(std::forward<Arg>(arg));
}

// (.) :: (b -> c) -> (a -> b) -> a -> c
// f . g = \x -> f $ g x
template <class MaybeA, class MaybeB, class MaybeC>
auto operator<<(std::function<MaybeC(MaybeB)> && f, std::function<MaybeB(MaybeA)> && g)
    -> std::function<MaybeC(MaybeA)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
        (MaybeA arg)->MaybeC { return f(g(arg)); };
    return __utils::make_func(composited);
}

int main()
{
    cout << (2 | [](int _){ return _ + 5; }
               | [](int _){ return _ * 2; }
               | [](int _){ return "result= " + std::to_string(_); }) << endl;


    cout << (2 | (composited_functor<>()
                , [](int _){ return _ + 5; }
                , [](int _){ return _ * 2; }
                , [](int _){ return "result= " + std::to_string(_); } )) << endl;

    cout << (curry([](int _){ return _ + 5; }) << 4) << endl;
    cout << (curry([](int _){ return "result= " + std::to_string(_); })
             << curry([](int _){ return _ * 2; })
             << curry([](int _){ return _ + 5; }) << 4) << endl;
    return 0;
}

// filename: ch4-pipeline-example.cpp
// compile this> g++ ch4-pipeline-example.cpp -o ch4-pipeline-example.exe
