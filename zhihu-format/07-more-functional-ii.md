柯里化
========================================

子曰：“工欲善其事，必先利其器。”在[上一篇文章](https://zhuanlan.zhihu.com/p/46737243)中介绍了三个工具： `std::bind()`，`std::apply` 以及我们自己写的用来解构函数的工具 `function_traits<>`。那么借助这些工具，我们终于可以开始进行柯里化操作了。上一篇文章里提到了我们有两条简单的思路来处理柯里化：

> 方案一，需要额外手动维护一个记录参数的容器；方案二，利用变长模板进行函数包装。本来，调用函数时我们提供的是完整的函数参数列表，现在相当于我们需要自己处理所有的参数调用问题。

那么我们就顺着之前的思路继续往下走，实现两个不同的柯里化的包装器。当然，我们的要求依旧和之前一样：既要保持函数不必每次重写，也要保持形式上的简洁性。

## 带有一定 lazy 程度的 currying

由于你已经知道怎么来把元组应用到函数上了，那么只需要手动维护这个元组就行了！废话不多说，我们先看代码：

```cpp
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
```

核心代码从第 37 行开始，到第 52 行结束，刨除函数声明和大括号，我们只用了 10 行代码就完成了柯里化操作。下面简单的解释下这份代码的核心内容：

* 我们把必要的信息（函数本体，已经接收到的参数）打包到一个数据结构 `curried_functor` 当中，同时在构造这个数据结构时记录函数所需的参数数量。
* 第 36-52 行定义了柯里化行为，并且由于我们还要把 `operator()` 留给函数调用，所以这里额外用 `curry()` 代替。第 50 行负责检查参数是否已经超过了函数的要求。而第 51 行则是把参数进行了打包，同时在第 52 行新构造一个 `curried_functor` 并且返回他。
* 第 37-45 行定义了函数的执行行为。同样是预检查参数数量是否符合要求，达标后我们再通过 `tuple_apply` 把数据应用到函数上。
* 第 56-60 行外部定义了柯里化函数，相当于一个装饰器，把一个函数修饰为一个可以柯里化的函数。在这里同样为了方便处理，我们把函数统一修改类型为 `std::function`。当然如果确保这个代码只是用来对付函数的话，这一步操作其实是可有可无的。
* 第 66-72 行测试了对一个保存好的 lambda 函数的柯里化处理，可以看到我们依旧保持了函数本身的写法不变，只是对他应用了一次 `curry()` 让他变得可以进行柯里化而已。第 69 行和第 72 行展示了两种不同的写法，既可以把柯里化的函数保存下来，也可以随用随写，托管给编译器来解决。最后，第 72 行之所以有一个空的括号，是因为我们需要调用函数 `operator()`（函数已经不再需要额外给任何参数了），否则会类似于第 69 行，返回的结果是一个函数，而不是计算结果。
* 第 74-79 行测试了一个需要三个参数的函数的柯里化。第 75 行里我们直接把一个 lambda 表达式作为参数传给了 `curry()`，随后的四行测试了柯里化的计算结果。

注意到在这个代码中，实际上我们这样的柯里化是具备了一定的 lazy 功能的。只有外部显调用 `operator()` 的时候才会进行计算。但是这样写并没有完全 lazy 计算，因为在传递参数的时候，我们传递的值，而不是参数的计算定义或计算路径。

## 把函数包装托管给编译器

除了手动包装参数之外，我们也可以利用模板特化功能，把函数包装托管给编译器。在这里我们还是以处理 `std::function` 对象为主，预先利用刚才提到的 `function_traits` 的 `func_type` 把所有的函数转换为功能相同的 `std::function` 对象，然后通过模板特化把 `std::function` 转换为柯里化函数。这里我们的柯里化就不再是延时求值了，而是每一次柯里化都返回一个真实的函数对象。既然是模板特化，我们就要先把基础的模板写出来：

```cpp
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
```

这两个函数规定了一元函数和零元函数的柯里化规则，在有了这一个终止递归的条件之后，我们就可以用变长模板来定义更一般地柯里化规则：

```cpp
template <class Return, class Arg, class... Args>
auto curry_impl(std::function<Return(Arg, Args...)> && f)
{
    return [f = std::forward<decltype(f)>(f)](Arg arg)
    auto _f = [f = std::forward<decltype(f)>(f)](Arg arg)
    {
        std::function<Return(Args...)> rest
            = [f = std::forward<decltype(f)>(f), arg=std::forward<Arg>(arg)]
            (Args... args)->Return { return f(arg, args...); };
        return curry_impl(std::forward<decltype(rest)>(rest));
    };
    return __utils::make_func(_f);
}
```

在这里，所有的参数和函数对象本身都是用右值引用来进行转发，保证了效率和值的不变性。然后只需要把第一个参数写到调用列表之中就行了。最后，我们再把用 `function_traits::func_type` 把函数规整到 `std::function` 对象这一功能封装到 `curry()` 函数当中：

```cpp
template <class Func> auto curry(Func && f)
{
    auto _f = __utils::make_func(f);
    return curry_impl(std::forward<decltype(_f)>(_f));
}
```

在使用的时候，我们就可以保持原函数的写法不做改变，只需要外部对其进行一次 `curry()` 修饰即可。与上一种写法类似，我们可以写出这样的测试代码：

```cpp
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
```

由于这次的使用了模板特化来实现柯里化，为了保证编译器处理时不出错（变长模板展开需要有一个终止条件），所以在特化时我们只处理了没有参数和只有一个参数的情形。这样也严格的执行了柯里化的定义，即函数永远只接受一个参数。同时也确保了返回值是一个函数对象而不是之前的那种包装了参数数据的特殊的数据结构。

## 写在最后的思考

到此为止我们已经接触了足够多的工具来实现 C++ 下的柯里化操作。但是注意到，并不是所有时候都需要柯里化，不恰当的柯里化反而会把代码变得复杂，同时也会变得晦涩难懂。事实上，柯里化的提出正是因为函数成为了一等公民后的很自然的结果。很大程度上，我们需要柯里化，或者更普遍地讲，我们需要部分应用的主要原因其实就是减少不必要的代码量，让代码更精简，让逻辑更清晰。举一个简单的例子，一个 MVC 三层架构的系统，我们完全可以把一个在 M 层部分应用的函数传到 C 层，然后再部分应用后传到 V 层做调用，这样就大大简化了跨逻辑层的参数传递问题。但是，比如你想做的只是一个二维向量的加法运算，最常见的写法是这样

```haskell
vec_add :: (Int, Int) -> (Int, Int) -> (Int, Int)
vec_add (x1, y1) (x2, y2) = (x1 + x2, y1 + y2)

main = do
    putStrLn $ show $ vec_add (1, 2) (3, 4) -- => (4, 6)
```

但是也可以写成这样

```haskell
vec_add' :: Int -> Int -> Int -> Int -> (Int, Int)
vec_add' x1 y1 x2 y2 = (x1 + x2, y1 + y2)

main = do
    putStrLn $ show $ vec_add' 1 2 3 4      -- => (4, 6)
```

那么后者这种柯里化的写法就真的好么？未必，因为二维向量几乎都是成对使用的，很少有情况要做到对两个二维向量的某一个的某一维去做部分应用。所以，如果你的参数是可以分割的相互独立的，那么柯里化在帮助函数抽象，在处理代码的冗余、不良耦合时，自然能起到不错的作用。但是，如果你的参数本身就是相互存在的缺一不可的，那自然也没必要费劲柯里化，反而会造成不必要的麻烦。
