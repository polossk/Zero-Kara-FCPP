更多的函数
========================================

## 函数的部分应用

在处理柯里化之前，我们先思考这样一个问题，如何对一个函数进行部分应用。举个例子，比如我们最常见的除法 `a / b`，我能否只传递其中的某一个参数给这个除法运算，从而得到一个接收另一个参数的函数。话很绕口，但是代码不绕口：

```cpp
#include <iostream>

using namespace std;

int main()
{
    auto div = [](int a, int b)->int{ return a / b; };
    auto div3 = [div](int a)->int{ return div(a, 3); };
    auto ten_div = [div](int b)->int{ return div(10, b); };
    cout << div(10, 3) << " " << div3(10) << " " << ten_div(3) << endl; // => 3 3 3
    return 0;
}

// filename: ch4-partial-apply-example.cpp
// compile this> g++ ch4-partial-apply-example.cpp -o ch4-partial-apply-example.exe
```

这就是一个简单的部分应用的例子，但是他实在是太简陋了，以至于当你写完这段代码你都不禁想问：

为什么放着好好的 `div` 不用，非得定义个 `div3` 和 `ten_div` 呢？那如果我有很多个函数需要这样部分应用，岂不是要额外多写很多份几乎相同的声明？有没有更简单的方法来实现这个功能。

方法当然是有的，C++11 标准就提供了一个函数 `std::bind()` 来处理这种奇奇怪怪的部分应用，而且写法也比我们的这种相对而言优雅（繁琐）一点。百闻不如一见，我们先看一个简单的例子：

```cpp
#include <iostream>
#include <functional>

using std::cout;
using std::endl;

void f(int x, int y, const int &z)
{
    cout << "=> f(" << x << ", " << y << ", " << z << ")" << endl;
}

int g(int x, int y, int z) { return x + y * z; }

struct Foo
{
    int data = -1;
    int addition(int a, int b) { return data + a + b; }
};

int main()
{
    using namespace std::placeholders;
    int n = 5;
    auto fyx5 = std::bind(f, _2, _1, n);
    fyx5(1, 2); // => f(2, 1, 5)
    auto f5yy = std::bind(f, n, std::ref(n), std::cref(n));
    f5yy(); // => f(5, 5, 5)
    n += 2; f5yy(); // => f(5, 7, 7)

    auto gxxx = std::bind(g, _1, _1, _1);
    cout << "5 + 5 * 5= " << gxxx(5) << endl; // => 5 + 5 * 5= 30
    auto gx45 = std::bind(std::bind(g, _1, _2, 5), _1, 4);
    cout << "3 + 4 * 5= " << gx45(3) << endl; // => 3 + 4 * 5= 23

    Foo foo_object;
    auto foo_data = std::bind(&Foo::data, _1);
    cout << foo_data(foo_object) << endl; // => -1

    auto foo_member = std::bind(&Foo::addition, &foo_object, 1, _1);
    cout << foo_member(10) << endl; // => 10

    return 0;
}

// filename: ch4-bind-example.cpp
// compile this> g++ ch4-bind-example.cpp -o ch4-bind-example.exe
```

在这个例子中，我们先行定义了两个函数 `f(x, y, z)` 和 `g(x, y, z)`，同时为了方便观察效果，这里的所有新产生的函数都是用类似 `fxyz` 这种方式进行的命名。在这个例子中，其实并没有做到部分应用，我们只是使用了占位符 `std::placeholders::_1, _2, ...` 来达成我们的部分应用的效果。这里简单的总结一些关于 `std::bind()` 的事项：

* `std::bind()` 的函数原型为 `template <class F, class ...Args> /*unspecified*/ bind(F && f, Args && ...args )`，即第一个参数必须是一个可调用的对象，当然现在这么说一般就认为是一个函数了，后面跟着的是调用这个函数的所有参数，参数可以显式声明（部分应用），也可以用占位符表示应用次序；
* 参数声明部分，所有的引用传递必须使用 `std::ref()` 或 `std::cref()` 来表示，否则默认认为是值传递；
* 可调用的对象，既可以是普通的函数或 lambda，也可以是函数指针，甚至可以是对象的非静态成员函数或非静态数据成员，如果是后者，则第一个参数必须是引用或者指针，用于指向访问成员的对象。

看起来我们的这个部分应用的问题得到了解决，但是解决的不是很优雅。如果有印象的话应该还记得之前的这种写法：

```cpp
#include <iostream>
#include <functional>

using namespace std;

int main()
{
    auto addition = [](int _1, int _2)->int { return _1 + _2; };
    auto add3 = [=](int _1)->int { return addition(_1, 3); };
    auto addition2 = [](int _1)->auto {
        return [_1](int _2)->int { return _1 + _2; };
    };
    auto add5 = addition2(5);
    cout << addition(10, 20) << endl; // => 30
    cout << addition2(10)(20) << endl; // => 30
    cout << add3(10) << endl; // => 13
    cout << add5(10) << endl; // => 15
    return 0;
}

// filename: ch2-lambda-example-4.cpp
// compile this> g++ ch2-lambda-example-4.cpp -o ch2-lambda-example-4.exe
```

注意这份代码中的 `addition2` 的定义，虽然定义繁琐，但是他可以逐一应用参数，只不过一次只能应用一个参数；而 `std::bind()` 则可以保持原函数的定义，但是部分应用的时候需要把其他项也给写进来占位。所以有没有什么方法可以，既保持原函数的写法不变，又能逐一应用参数，而且还能想应用几次就应用几次呢？很好，这就很有“柯里化”的味道了。

## 柯里化

柯里化一般是指，把接受多个参数的函数变换成接受一个单一参数（最初函数的第一个参数）的函数，并且返回接受余下的参数而且返回结果的新函数的技术。比如我们现在有一个函数 `y = f(a, b, c)`，从数学上讲，这是一个从 `[A x B x C]` 映射到 `[Y]` 的一个函数，其中 `A`，`B`，`C`，`Y` 分别是 `a`，`b`，`c`，`y` 的取值范围，记号 `[A x B]` 表示集合 `A`，`B` 的笛卡尔积。用数学语言描述的话，柯里化就是一种建立函数到函数的映射。就刚才的例子具体而言，`y=f(a, b, c)` 可以理解为 `f: [a, b, c] -> y`，其中记号 `->` 表示映射符号，而其柯里化就是 `curry(f): a -> ([b, c] -> y)`。如果完全柯里化刚才的这个函数，就可以进一步的写成 `fully_curried(f): a -> (b -> (c -> y))`。如果我们再额外规定，映射运算符是右结合的（优先计算右边），那么括号也可以省略掉 `fully_curried(f): a -> b -> c -> y`。

那么如果我们能篡改 C++ 标准，让那群制定标准的人认同这种很 Haskell 的写法，是不是就能实现这个柯里化的目标了呢？当然是不现实的了，就算标准委员会通过了这个提案，等到编译器支持这个特性估计也到猴年马月了。所以我们还是靠现有的 C++ 技术来完成这个任务。之前我们提到了，我们不想为了柯里化而柯里化，把每一个函数的声明都写的十分冗长（参见之前 `lambda-example-2.cpp` 中 `addition2` 的定义），也不想用 `std::bind()` 每次都把所有的参数都写出来（形式不够简洁）。我们的目标就是既保持函数本身的写法不做改变，同时不必每次都提供完整的参数表。

这个问题也不难解决，凭借之前所提到的内容我们至少有两条道路可走：方案一，需要额外手动维护一个记录参数的容器；方案二，利用变长模板进行函数包装。本来，调用函数时我们提供的是完整的函数参数列表，现在相当于我们需要自己处理所有的参数调用问题。那么下面我们开始准备一些必要的手段。

## 把元组应用到函数上

这个方案应当是最容易想到的，由于本来函数调用时是由编译器来托管了所有的参数进栈出栈操作，那么既然现在一次只能来一个参数，我们自然会想到用一个容器把所有的参数安排上，然后一但调用，则把这些参数一股脑儿全送给原始函数，大功告成。

那么选择什么样的容器来安排这些参数呢？考虑到函数的参数不定长且不定类型，那么就选择是你了，`tuple`！我们之前练习过对 `tuple` 的解包打包，所以这些都不是问题。当然如果能直接把 `tuple` 内的参数直接喂给函数就更好了。这里就需要额外介绍一个小工具 `tuple_apply`。在最近的 C++17 标准中，提供了 `apply` 函数来完成这一操作，但是实际上我们也可以自己写一份：

```cpp
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
    cout << tuple_apply(add_3_nums, std::make_tuple(1, 2, 3)) << endl;
    // => 3 6 3 6
    return 0;
}

// filename: ch4-tuple-apply-example.cpp
// compile this> g++ ch4-tuple-apply-example.cpp -o ch4-tuple-apply-example.exe -std=c++17
```

为了方便理解 `std::apply()` 的功能，在这里我额外写了一个 `tuple_apply()` 函数，两者的功能、实现方法以及使用方式基本一致。`std::apply()` 以及 `std::invoke()` 都是在 C++17 引入的两个工具，用于以指定参数调用指定函数。后者的函数原型是 `Result std::invoke(F && f, Args&&... args)`，和之前的 `std::bind()` 非常类似，只不过 `std::bind()` 返回一个函数，这个返回函数的运行结果。`std::apply()` 的函数原型是 `Result std::apply(F && f, Tuple && t)`，即把元组 `t` 中的数据作为参数按顺序送到函数 `f` 当中。在这里有人可能会问为什么不直接写 `f(args ...)` 呢？由于 `std:invoke()` 和 `std::bind()` 对于 `f` 的定义都是**可调用对象**，所以这个对象不仅包含函数，也可能是某一个实例的数据或者某一个对象的成员函数。但是如果 `f` 的范围限定为函数对象，那么 `f(args ...)` 和 `std::invoke(f, args...)` 这两个写法是完全等价的。顺便一提，如果编译环境没有 C++17 那么高，在保证 `f` 为函数对象的前提下，完全可以直接写 `f(args ...)`。

那么 `std::apply()` 以及 `tuple_apply()` 又是怎么工作的呢？注意观察第 11 行关于下标 `I` 的声明以及第 13 行的后半段关于下标 `I` 的使用 `std::get<I>` 还有第 21 行对下标 `I` 的构造 `std::make_index_sequence<>`，我们不难发现，其实是借助 `std::make_index_sequence<>` 在编译时直接把下标序列构造出来，然后在应用到 `std::get<I>` 上。比如第 30 行的 `tuple_apply()`，表面上是写 `tuple_apply(add, std::make_pair(1, 2))`，实际上编译之后就会执行 `std::invoke(add, std::get<0>(t), std::get<1>(t))`。借助这个工具，我们只需要完成函数参数的包装任务就行了。

## function traits

那么怎么样才能判断这个函数需要多少个参数呢？在这里我们需要写一个类型判断器，把函数解构，获取到一个函数的必要信息：返回类型（return type）是什么，参数（arity）需要多少个，每一个参数的类型（argument type）是什么。我们把这些东西包装到一个 `function_traits` 类型当中。并且把函数的类型规范到形如 `std::function<Return(Args...)>` 的格式当中。当然了，为了确保我们的类型是真实可靠的，我们需要借助 C++ 非常严格的类型检验功能 `std::is_same<U, V>::value`，同时需要借助 `typeid().name()` 把这个类型具体是什么输出出来。这里需要注意，如果你和我一样使用的是 GCC 直接使用 `typeid().name()` 会得到编译器装饰过的类型别名（[decorated name](https://en.wikipedia.org/wiki/Name_mangling)），难以阅读，浪费时间。这里需要用 GCC 自带的工具 [`c++filt`](http://sourceware.org/binutils/docs-2.16/binutils/c_002b_002bfilt.html) 或者 [`__cxa_demangle`](http://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html) 来帮助把类型名替换为我们代码中所写的原始名称。在这里我是用的是后者，并且用了一个函数 `realname()` 帮助我们把类型直接转换成正常形式。

```cpp
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
```

在这个文件中，我们借助 traits 技术把函数的基本信息进行了解构和处理，同时公开了两个辅助函数。`make_func()` 会把函数、lambda 表达式、仿函数等可调用对象转换成一个 std::function 对象，`arity()` 会返回函数的所需的参数数量。同时为了方便获取函数的参数类型，额外把这个功能包装到了 `argument_type` 当中。下面让我们看一下他的使用说明：

```cpp
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
```

我们把输出和源代码放一起对比着阅读：

```plain
-------- struct function_traits --------
  bar:  arity= 3 , double? true , int? true , int? false , int? true
-------- arity(), make_func() --------
normal function:
  fun:  arity= 2, return_type= double,  func_type= std::function<double (int, int)>
  fun_: arity= 2, return_type= double,  func_type= std::function<double (int, int)>
std::function object:
  add:  arity= 2, return_type= int, func_type= std::function<int (int, int)>
  add_: arity= 2, return_type= int, func_type= std::function<int (int, int)>
lambda object:
  bar:  arity= 3, return_type= double,  func_type= std::function<double (int, double, int)>
  bar_: arity= 3, return_type= double,  func_type= std::function<double (int, double, int)>
functor object:
  sub:  arity= 2, return_type= int, func_type= std::function<int (int, int)>
  sub_: arity= 2, return_type= int, func_type= std::function<int (int, int)>
-------- member object, member function --------
member object pointer:
  &Foo::f:  arity= 0, return_type= double,  func_type= std::function<double ()>
member function instance:
  sub.odd:  arity= 1, return_type= bool,    func_type= std::function<bool (int)>
member function pointer:
  &Foo::odd:    arity= 1, return_type= bool,    func_type= std::function<bool (int)>
-------- make_func() with lambda expression --------
  mul:  arity= 2, return_type= int, func_type= std::function<int (int, int)>
-------- struct argument_type --------
  bar:  arity= 3, return_type= double
    func_type= std::function<double (int, double, int)>
    func_type= std::function<double (int, double, int)>
```

* 第一大段，代码的第 49-56 行，对应输出的前两行，是 `function_traits` 的基本用法，这个类主要负责记录 4 个信息：函数的类型，返回值类型，函数所需参数个数，函数的参数类型。在这里为了统一表示，函数的类型统一用 `std::function` 表示。函数的参数下标从 0 开始，与数组保持一致。
* 第二大段，代码的第 58-66 行，对应输出的第 3-15 行，依次测试了 `function_traits` 以及 `artiy()` 和 `make_func()` 两个函数对四种经典情形的表现。我们分别用普通的函数（函数指针），`std::function` 对象，存储好的 lambda 对象，仿函数进行了测试。对象名后面加下划线表示是利用 `make_func()` 所获得的 `std::function` 对象。在这里主要关注的是 `make_func()` 对于右值的处理。在 `function_traits.hpp` 的第 60-61 行，这里是为了减少运行过程中对参数的复制行为，所以我们用了 `std::forward` 来进行完美转发。
* 第三大段，代码的第 68-71 行，对应输出的第 16-22 行，我们测试了比较少见的两个可调用对象：成员数据指针和成员函数。在这里，我们把成员对象指针解构为一个只有返回值的函数，而成员函数则不需要额外提供对象的指针。此处与很多的 `function_traits` 写法稍有出入。原则上，一个成员函数，除非他是静态函数，否则他是需要额外的具体对象（指针或实例）才能调用的。这里我的考虑是我们只考虑**函数**而不关心他是哪里的成员。所以在 `function_traits.hpp` 的第 44-51 行，我在特化处理是并没有写做 `function_traits<Return(Class&, Args...)>`。如果这样写，会导致在解构 lambda 表达式也会额外多出一个参数 `Class&`，从而让后面的处理变得非常麻烦。本着**简单省事**的原则，我们的 `function_traits` 就不对这两个可调用对象进行过多的支持了。
* 第四大段，代码的第 73-75 行，对应输出的第 23-24 行，我们测试了一个简单的 lambda 表达式的转发。当然这行代码的作用和代码的第 43 行是一样的。单独把他写出来只是为了强化一个概念：函数就是第一公民。除此之外并没有什么太多需要额外说明的。
* 最后一段，代码的第 77-93 行，对应输出的最后 4 行，我们测试了 `argument_type` 功能，这个功能的初衷是为了减少不必要的代码量。比如，代码的第 82 行中 `argument_type<decltype(bar), 0>::type` 等价于 `function_traits<decltype(bar)>::argument<0>::type`。
