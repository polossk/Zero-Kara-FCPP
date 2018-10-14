// filename: function_traits.hpp
#ifndef __FUNCTION_TRAITS_HPP__
#define __FUNCTION_TRAITS_HPP__
namespace __utils{

template <class F> struct function_traits;

// normal function, static member function
template <class Return, class...Args>
struct function_traits<Return(Args...)>
{
    using func_type = std::function<Return(Args...)>;
    using return_type = Return;
    static constexpr std::size_t arity = sizeof...(Args);
    template <std::size_t I> struct argument
    {
        static_assert(I < arity,
            "error: invalid index of this function's parameter.");
        using type = typename 
            std::tuple_element<I, std::tuple<Args...>>::type;
    };
};

// function pointer, &f
template <class Return, class...Args>
struct function_traits<Return(*)(Args...)>
    : public function_traits<Return(Args...)> {};

// std::function
template <class Return, class...Args>
struct function_traits<std::function<Return(Args...)>>
    : public function_traits<Return(Args...)> {};

// functor, callable object, struct with overloaded operator()
template <typename Functor>
struct function_traits
    : public function_traits<decltype(&Functor::operator())> {};

// lambda expression, const member function pointer
template <class Class, class Return, class...Args>
struct function_traits<Return(Class::*)(Args...) const>
    : public function_traits<Return(Args...)> {};

// member function pointer
template <class Class, class Return, class...Args>
struct function_traits<Return(Class::*)(Args...)>
    : public function_traits<Return(Args...)> {};

// member object pointer
template <class Class, class Return>
struct function_traits<Return(Class::*)> : public function_traits<Return()> {};

// clear reference F&, F&&
template <class F> struct function_traits<F&> : public function_traits<F> {};
template <class F> struct function_traits<F&&> : public function_traits<F> {};

// make_func(), return a std::function<> object from function-like object
template <class Func> auto make_func(Func && f)
{
    typename function_traits<Func>::func_type _f = std::forward<decltype(f)>(f);
    return std::forward<decltype(_f)>(_f);
}

// arity(), return numbers of parameters
template <class Func> inline constexpr std::size_t arity(Func &&)
{
    return function_traits<std::remove_reference_t<Func>>::arity;
}

// argument_type<Func, I>::type, return I-th parameter's type, I start from 0
template <class Func, std::size_t I> struct argument_type
{
    using type = typename function_traits<Func>::template argument<I>::type;
};

} // namespace __utils
#endif // __FUNCTION_TRAITS_HPP__
