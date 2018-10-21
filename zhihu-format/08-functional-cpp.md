写更函数式的 C++
========================================

## 从管道说起

我们都知道，在 linux 里有一种特殊的文件，叫做管道。程序可以读管道的数据，也可以把数据写到管道里供应给其他程序。管道是一个重要的交互手段，比如这个命令

```shell
ps -aux --sort -pcpu | less
```

这个命令是经典的检查你的 CPU 占用的指令，中间的这个 `| less` 就是借助了管道，把原始数据送给 `less` 来进行二次处理。那么通俗点讲，我们可以粗略地认为，运算符 `|` 接受两个参数 `lhs` 和 `rhs`，左端 `lhs` 提供数据，右端 `rhs` 则提供处理数据的函数。现在我们把这个运算符重载下。

```cpp
template <class Arg, class F>
auto operator|(Arg && arg, F && f) -> decltype(f(std::forward<Arg>(arg)))
{
    return f(std::forward<Arg>(arg));
}
```

这样的话我们就能写这种样子的代码了：

```cpp
int result = 5 | [](int _){ return _ + 2; }; // => 7
```

现在这种写法就有点函数式的意思了，但是仅仅是这样还远远不够。回到一开始的那条命令，我们还可以继续给他加一个工具处理

```shell
ps -aux --sort -pcpu | less | head -n 10
```

那么我们的这个重载能做到类似的功能吗？当然是可以的，因为运算符 `|` 是从左到右计算的，保持了运算顺序，所以只需要不停的往数据后面堆积木就行了，想堆多少就堆多少。

```cpp
cout << (2 | [](int _){ return _ + 5; }
           | [](int _){ return _ * 2; }
           | [](int _){ return "result= " + std::to_string(_); }) << endl;
// => result= 14
```

这时候你就会发现 lambda 是多么的平易近人了。千言万语，不及一句“随用随写”。如果没有 lambda 支持，同样的功能可能光函数头声明，就够麻烦的了。

## 如果只要管道不要数据呢？

在充分理解这个管道运算符 `|` 之后，我们开始思考标题所提到的这个问题。假设我们现在有这样一个管道链 `a | f | g`，我们不再关心数据 `a`，而关心函数的组合 `| f | g`，那么能不能用一种数据结构把这个组合记录下来呢？

这里就涉及到一个简单的概念：函数组合。假设我们手上有函数 `f`，其定义为 `f :: b -> c`，表示是一个从 `b` 映射到 `c` 的函数。同时有函数 `g`，其定义为 `g :: a -> b`，表示是一个从 `a` 映射到 `b` 的函数，那么函数的组合 `f . g` 表示一个从 `a` 经过 `b` 得到 `c` 的函数。如果了解一些 Haskell 的知识，想必对这个表示，运算符 `(.)` 会非常熟悉：

```haskell
(.) :: (b -> c) -> (a -> b) -> a -> c
f . g = \x -> f (g x)
```

其实扯这么多，就是说函数的组合就是 `f(g(x))`。

既然了解了函数组合的定义，那么很自然的我们会想到让程序自动生成这种形如 `f(g(h(x)))` 的代码就可以了。更直接一点，我们首先记录好所有的函数，然后需要执行时，把函数依次应用就行了。在记录函数这里，注意到每一个函数的类型都可能是不同的，而且这是一个线性的结构，所以我们选用 `tuple` 作为核心容器来记录所有的函数。同时，对外公开两个方法 `composite()` 和 `operator()` 用来处理组合新函数和执行函数调用，内部则用递归的方法把函数一次调用。具体的代码如下：

```cpp
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
```

注意观察 `composited_functor::call()`，利用 `std::make_index_sequence<>{}` 直接在编译时获得已有的函数下标数列，然后利用变长模板和终止条件来实现函数组合的调用。这份代码也是具有一定 lazy 能力的，也就是只有在调用的时候才会真正的计算，否则只会忠实的记录函数数据而已。当然为了添加彩蛋，我重载了逗号运算符 `operator,` 用来更方便的使用函数组合。至于为什么不重载 Haskell 里面的 `(.)` 运算符，因为 C++ 不允许重载呀！（这不是 bug，这是 feature。）

```cpp
template <class... Fs, class F>
auto operator,(composited_functor<Fs...> && composited, F && f)
{
    return composited.composite(std::forward<F>(f));
}
```

有了这个逗号之后，我们配合之前的 `|` 这样写：

```cpp
cout << (2 | (composited_functor<>()
            , [](int _){ return _ + 5; }
            , [](int _){ return _ * 2; }
            , [](int _){ return "result= " + std::to_string(_); } )) << endl;
// => result= 14
```

如果说这种写法有什么不妥，emmm，当然是这个 `composited_functor<>()` 太碍眼了，完全可以不要这个东西的。所以我们换个思路重新考虑一下这个问题。

## 特殊的应用符

Haskell 中我们有一个特殊的运算符 `($)` 来消除括号，同样我们也可以把这个函数引入 C++ 中。在 Haskell 中，运算符 `($)` 是这样定义的

```haskell
($) :: (a -> b) -> a -> b
f $ x = f x
```

换句话说，就是可以不写括号，直接把数据应用到函数上而已。由于 C++ 不能创建新的运算符，我们只好曲线救国，换一个运算符 `operator<<` 来实现这个功能了。

```cpp
// ($) :: (a -> b) -> a -> b
// f $ x = f x
template <class Return, class Arg>
auto operator<<(std::function<Return(Arg)> && f, Arg && arg)
{
    return f(std::forward<Arg>(arg));
}
```

这里还是只处理 `std::function` 类型的函数，至于其他的类型，只需要转换为 `std::function` 就好了，那么来都来了，不如索性直接柯里化吧

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
```

虽然我们只需要 `__utils::make_func` 功能，不过柯里化也不是啥坏事。有了这两个组件，我们就可以这样写了

```cpp
cout << (curry([](int _){ return _ + 5; }) << 4) << endl;
// => 9
```

稍微有点意思了，那么函数组合应该怎么写呢？很简单，直接用 `operator<<` 把两个函数直接组合掉就行了。

```cpp
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
```

这里的 `MaybeA` 可不是什么 monad，他只是用来做一个类型的标记。现在就可以写一些很像 Haskell 的代码了。

```cpp
cout << (curry([](int _){ return "result= " + std::to_string(_); })
         << curry([](int _){ return _ * 2; })
         << curry([](int _){ return _ + 5; }) << 4) << endl;
// => result= 18
```

很像 Haskell 的 `f $ g $ h x` ，不过得益于 lambda 的存在代码不至于过于冗长。

现在我们手上已经有了柯里化和函数组合这两个工具了。借助这两个工具，我们可以非常自然地把逻辑用函数来进行抽象，把参数的传递用柯里化来代替，从而更简单的组合我们的代码，更大限度的减少不良代码。这里需要再次强调一次，那就是 DRY 原则，不要反复重复没用的代码。编程的最大乐趣在于一层一层的把逻辑抽象，从而用简洁的代码，通过一层一层的组合、堆叠，完成一个又一个复杂的任务。可能之前我们手上只有一种实现的方法，而现在道路多了之后，就会大大地解放我们的思路，方便我们思考出最适合自己的一条实现方案。