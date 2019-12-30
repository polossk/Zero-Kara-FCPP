lambda 函数
========================================

C++11 中正式引入了 lambda 函数这一简单的工具。lambda 函数使得很多在之前不被允许的事情变得可能，也正式的宣告函数成功升级为 C++ 的一等公民。我们先看一个简单的 lambda 函数的例子。

```cpp
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    auto addition = [](int _1, int _2)->int{ return _1 + _2; };
    vector<int> a = { 1, 2, 3, 4, 5 };
    vector<int> b = { 6, 7, 8, 9, 10 };
    for (int i = 0, sz = a.size(); i < sz; i++)
    {
        a[i] = addition(a[i], b[i]);
    }
    for (auto e : a) cout << " " << e; cout << endl;
    // => 7 9 11 13 15
    return 0;
}

// filename: ch2-lambda-example-1.cpp
// compile this> g++ ch2-lambda-example-1.cpp -o ch2-lambda-example-1.exe
```

第 8 行我们定义了一个 lambda 函数，这个函数用来计算两个数的和。注意到他的调用方式和正常的函数完全一致，而不同点在于书写方法的不同。首先，这个函数**没有名字**。`addition` 并不是 lambda 的名字，他只是被一个 lambda 函数附身了而已，lambda 函数自身是没有名字的，所以自然这个函数就不能简单的实现递归，即调用自己。其次，在参数列表前出现了一组中括号，叫做**捕捉列表**，用来捕捉当前 lambda 所在环境里的变量。而参数列表后面有一个箭头，箭头后面跟返回值的类型。最后，与函数声明完之后直接用大括号收尾不同，在这里可以暂时把 lambda 函数看做一种特殊的变量，所以最后需要用分号 `;` 对当前语句进行收尾。

```cpp
auto a_alias = [capture](parameters)->return_type{ ... };
```

那么捕捉列表能做什么呢？看一下这个例子：

```cpp
#include <iostream>

using namespace std;

int main()
{
    int j = 1;
    auto by_val_lambda = [=]{ return j + 1; };
    auto by_ref_lambda = [&]{ return j + 1; };
    auto print = [=]{
        cout << "print by val lambda: " << by_val_lambda() << ", ";
        cout << "by ref lambda: " << by_ref_lambda() << endl;
    };
    print();
    j += 10; print();
    return 0;
}

// filename: ch2-lambda-example-2.cpp
// compile this> g++ ch2-lambda-example-2.cpp -o ch2-lambda-example-2.exe
```

这个例子里面我们使用了两个最常用的捕捉方法，`[=]` 表示按照值传递的方法捕捉父作用域的所有变量，`[&]` 表示按照引用传递的方法捕捉父作用域的所有变量。如果想要捕捉特定的变量，可以直接写 `[var]` 或 `[&var]`，前者是值传递，后者是引用传递。注意，你甚至可以捕捉 `this` 指针，如果父作用域中存在的话。在知道这些内容之后，我们再来观察运行结果

```plain
print by val lambda: 2, by ref lambda: 2
print by val lambda: 2, by ref lambda: 12
```

在这里，由于 `by_val_lambda` 是通过值传递捕捉的变量 `j`，所以这个变量在其内部的值永远是 1，所以不论何时调用这个函数都会返回 2。而 `by_ref_lambda` 是通过引用传递捕捉的变量 `j`，所以内部的值会随着外部的使用而发生变化，所以在 `j += 10` 之后，显然会返回 12。

那么为什么会有 lambda 函数呢？一种最简单的理解是，lambda 的出现减少了对命名空间的污染，或者你也可以理解为增加了一种简单的局部函数的实现。举个例子，你现在在实现一个简单的五子棋小应用。每一次落子时我们都需要对这一手棋的合法性进行测试：落点是否在棋盘内，落点是否已经有棋子，落点是否六子连珠违规，落点是否先手双活三违规等等。传统的写法是每一个判断单独为一个函数（或方法），然后逐一判断。现在你可以把这些判断直接写在主要逻辑里面，你甚至还可以在保证每一个判断函数的类型完全一致的情况下使用一个函数指针列表来更清晰的管理这些逻辑判断函数。可以这么说，lambda 函数提供了一种更为简单更为方便的，随时随地写函数的方式，怎么方便怎么来就完事了。

现在我们已经了解了足够多的基础知识了，现在来思考一些奇怪的东西。

## 如何递归调用一个 lambda

准确的讲，这个说法是有“问题”的。一个没有名字的函数，怎么去调用自己呢？不信？可以试着编译这份代码：

```cpp
auto fact = [](int _) { if (_ <= 1) return _; else return _ * fact(_ - 1); };
```

这份代码在编译时会报错，`fact` 在 lambda 中是一个未声明的量，因为这个称呼 `fact` 的声明并不在 lambda 的作用域中。

那么如果我们提前声明这个称呼 `fact` 然后再用 `[&fact]` 捕捉他，不就大功告成了么？

```cpp
#include <iostream>
#include <functional>

using namespace std;

int main()
{
    std::function<int(int)> fact;
    fact = [&fact](int x)->int{ return x <= 1 ? x : x * fact(x - 1); };
    cout << fact(5) << endl; // => 120

    std::function<int(int, int)> gcd = [&](int a, int b)->int{
        return b == 0 ? a : gcd(b, a % b);
    };
    cout << gcd(24, 36) << endl; // => 12
    return 0;
}

// filename: ch2-lambda-example-3.cpp
// compile this> g++ ch2-lambda-example-3.cpp -o ch2-lambda-example-3.exe
```

这里我们用一个简单的工具 `std::function<int(int)>` 预先声明了一个函数名 `fact`，然后再用引用传递捕捉他，从而实现了递归调用。

（虽然 emmm 感觉好像哪里不太对，不过也实现递归了我也懒得管了 emmm

### 以下内容初学者可跳过

准确的讲，上面的**根本不是** lambda 函数的递归调用，他只是**一般函数**的递归调用而已。更准确的讲，想要实现 lambda 函数的递归调用，必须首先对 Y-组合子有一定的了解。简单的讲，虽然 lambda 是没有名字的，但是我们可以把他作为一个参数传递给一个更上层的函数，然后让他再调用。这时候，熟悉 Haskell 或者其他函数式语言的大家就会想起不动点组合子。也就是我们构造一个函数，返回他的不动点。

```haskell
fix :: (a -> a) -> a
fix f = let {x = f x} in x
```

在使用这个不动点组合子之前，我们先看一下正常的函数递归计算是什么样的

```haskell
fact :: Int -> Int
fact 0 = 1
fact n = n * fact (n - 1)
main = putStrLn $ show $ fact 5 -- => 120
```

而当我们使用 lambda 时，则需要借助不动点组合子，写成这样

```haskell
fix :: (a -> a) -> a
fix f = let {x = f x} in x
main = putStrLn $ show $ fix (\f n -> if n == 0 then 1 else n * f (n-1)) 5 -- => 120
```

简单的讲，我们需要把自己的 lambda 函数增加一个参数 `f` 表示这个函数自己，然后利用 Y-组合子将其自动展开到不动点，从而完成计算。

道理我都懂，C++11 能不能实现 Y-组合子呢？当然是可以的，而且非常的简单。

```cpp
#include <iostream>
#include <functional>

using namespace std;

template <typename F>
struct Y
{
    Y(F f) : _f(f) {}
    template <typename... Args>
    auto operator()(Args&&... t) const
    {
        return _f(*this, std::forward<Args>(t)...);
    }
    F _f;
};

template <typename F>
Y<F> fix(F&& f)
{
    return Y<F>(forward<F>(f));
}

int main()
{
    auto gcd = fix(
        [](auto g, int a, int b)->int{ return b == 0 ? a : g(b, a % b); }
    );
    cout << gcd(63, 105) << endl; // => 21
    return 0;
}

// filename: ch2-Y-combinator.cpp
// compile this> g++ ch2-Y-combinator.cpp -o ch2-Y-combinator.exe
```

首先我们定义了一个组合子 Y 用来“拼接”函数，然后外部公开了方法 `fix` 来转发 lambda 函数，而内部则用变长模板来转发 lambda 函数的参数。这里注意第 27 行里，需要显式的声明 lambda 函数的返回类型，否则会导致编译报错，原因是函数参数 g 并没有一个明确的返回值。

如果对不动点组合子或者 lambda 演算有兴趣，可以自行阅读下 SICP 这本书，对培养编程思维（函数式和命令式）都有很大帮助。

## lambda 在编译器内部是如何实现的

我们常说学习一个东西不仅要知其然，还要知其所以然。编程学习中也是一样，不仅要会用，而且还要知道这些特性是怎么来的。因为源码之下，了无秘密。

lambda 函数在诞生之前，很多人都利用仿函数（functor）来把函数作为一等公民来使用。仿函数简单地说就是重定义了成员函数 `operator()` 的一种自定义类型对象，虽然在代码层面上和普通的函数使用别无二致，但是本质上仿函数是一个对象（object）而函数是代码段（statements），两者区别很大。

不过在有了 lambda 之后，仿函数的风光就不在了。因为在之前要初始化变量时需要额外的声明，而 lambda 则可以通过捕捉列表来抓取变量。不止如此，lambda 不会造成额外的碎片函数的污染，而仿函数几乎是一个声明只能用一次的碎片垃圾制造大户。但是本质上，lambda 和仿函数是同一类的，两者在代码层面上可以实现相同的功能。而事实上，早期的编译器确实是利用仿函数来实现 lambda 这一功能的。

## lambda 能否返回一个 lambda

既然诚心诚意的问了，那我就光明正大的告诉你，当然是可以的。

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

恭喜你，如果你看懂了这个代码，你已经成功了和柯里化打了个照面了。第一个 `addition` 是我们最常见的函数定义，而第二个 `addition2` 则显得不那么常见。如果只从代码层面分析，他是一个依次接受两个参数的函数。如果只接受一个参数，他会返回一个接受一个参数的函数。但是实际上是因为 `addition2` 首先返回一个 lambda 函数，然后如果还有第二个参数他才会返回具体的计算结果。如果对这个不是很理解的话，可以对照 python 的装饰器思考一下，lambda 函数提供了一个更为简单的打包或者修饰函数的方法。当然具体怎么用当然还是看怎么写怎么方便来。
