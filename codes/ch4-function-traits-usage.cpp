#include <iostream>
#include <tuple>
#include <utility>
#include <functional>
#include <typeinfo>
#include <string>
#include <cxxabi.h>

using std::cout;
using std::endl;
using std::is_same;

#include "function_traits.hpp"

// realname(), demangling. Transforming the c++ abi identifiers into original
// C++ source identifiers.
std::string realname(const std::type_info &info)
{
    int status = 0;
    return std::string(abi::__cxa_demangle(info.name(), 0, 0, &status));
}

auto profile = [](auto f)
{
    using traits = __utils::function_traits<decltype(f)>;
    cout << ":\tarity= " << __utils::arity(f);
    const std::type_info &ret = typeid(typename traits::return_type);
    const std::type_info &fun = typeid(typename traits::func_type);
    cout << ", return_type= " << realname(ret);
    cout << ",\tfunc_type= " << realname(fun) << endl;
};

#define DISPLAY_PROFILE(f) do { cout << "  "#f; profile(f); } while (0);

double fun(int, int) { return 0; }

struct Foo {
    int operator()(int a, int b) const { return a - b; }
    bool odd(int a) { return a % 2; }
    double f = 5.0;
} sub;

std::function<int(int, int)> add = [](int a, int b)->int{ return a + b; };

auto bar = [](int a, double b, int c)->double{ return a + b + c; };

int main()
{
    cout << "-------- struct function_traits --------\n";
    using traits = __utils::function_traits<decltype(bar)>;
    cout << "  bar:\tarity= " << traits::arity << " ";
    cout << std::boolalpha;
    cout << ", double? " << is_same<double, traits::return_type>::value << " ";
    cout << ", int? " << is_same<int, traits::argument<0>::type>::value << " ";
    cout << ", int? " << is_same<int, traits::argument<1>::type>::value << " ";
    cout << ", int? " << is_same<int, traits::argument<2>::type>::value << endl;
    
    cout << "-------- arity(), make_func() --------\n";
    cout << "normal function:\n"; auto fun_ = __utils::make_func(fun);
    DISPLAY_PROFILE(fun); DISPLAY_PROFILE(fun_);
    cout << "std::function object:\n"; auto add_ = __utils::make_func(add);
    DISPLAY_PROFILE(add); DISPLAY_PROFILE(add_);
    cout << "lambda object:\n"; auto bar_ = __utils::make_func(bar);
    DISPLAY_PROFILE(bar); DISPLAY_PROFILE(bar_);
    cout << "functor object:\n"; auto sub_ = __utils::make_func(sub);
    DISPLAY_PROFILE(sub); DISPLAY_PROFILE(sub_);
    
    cout << "-------- member object, member function --------\n";
    cout << "member object pointer:\n"; DISPLAY_PROFILE(&Foo::f);
    cout << "member function instance:\n"; DISPLAY_PROFILE(sub.odd);
    cout << "member function pointer:\n"; DISPLAY_PROFILE(&Foo::odd);

    cout << "-------- make_func() with lambda expression --------\n";
    auto mul = __utils::make_func([](int a, int b)->int{ return a * b; });
    DISPLAY_PROFILE(mul);

    cout << "-------- struct argument_type --------\n";
    using traits = __utils::function_traits<decltype(bar)>;
    const std::type_info &ret = typeid(typename traits::return_type);
    const std::type_info &fun = typeid(typename traits::func_type);
    const std::type_info &fst = typeid(
        typename __utils::argument_type<decltype(bar), 0>::type);
    const std::type_info &snd = typeid(
        typename __utils::argument_type<decltype(bar), 1>::type);
    const std::type_info &trd = typeid(
        typename __utils::argument_type<decltype(bar), 2>::type);
    cout << "  bar:\tarity= " << __utils::arity(bar);
    cout << ", return_type= " << realname(ret) << endl;
    cout << "\tfunc_type= " << realname(fun) << endl;
    cout << "\tfunc_type= std::function<" << realname(ret) << " (";
    cout << realname(fst) << ", ";
    cout << realname(snd) << ", ";
    cout << realname(trd) << ")>" << endl;
    return 0;
}

// filename: ch4-function-traits-usage.cpp
// compile this> g++ ch4-function-traits-usage.cpp -o ch4-function-traits-usage.exe
