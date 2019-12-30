C++ 的元编程
========================================

## 元编程是什么

元编程（metaprogramming）是一个典型的复合词，他由前缀 meta- 和词根 programming 复合而成，有“对一个程序进行编程”的意思。换句话说，编译器先编译你的代码，得到一份新的代码，然后再编译新的代码来解决问题。那么问什么要有这种编写技巧呢？无外乎两点：减少重复的代码以方便后期的维护，减少重复性的工作以方便后期的优化。其本质还是在于对编码逻辑的抽象，把重复性最高的工作，用最少的代码量来完成，而且在编译时让编译器来展开执行，这就是元编程的目的和核心思路。

在 C++ 中，关于元编程有两个门派，一个是模板元编程，主要借助的是模板特化来实现自动化编程。另一个是常量表达式门派，主要是借助 `constexpr` 这一关键字来实现。两者就目前而言区别不算很大，而且基本上提到 C++ 的元编程，大家都会首先想到模板元编程，所以后面的讨论主要讲解的工具也是模板元编程，特此说明一下。

多说无益，我们先从最简单的例子说起。

## 简单的递归

回顾下数学里关于自然数的加法乘法乘方的递归定义。我们都知道自然数一开始只有两种运算：前趋（predcesser）与后继（successer）。两个自然数相加，a + b，等于 a 执行 b 次后继运算。比如我们规定函数 `pred` 表示自然数的前趋，函数 `succ` 表示自然数的后继，那么我们的加法可以这样计算：

```cpp
int a = succ(1); // a = 2
int b = pred(5); // b = 4
int c = a + b;
/*
c = a + b
  = 2 + 4 = succ(2) + (4 - 1)
  = 3 + 3 = succ(3) + (3 - 1)
  = 4 + 2 = succ(4) + (2 - 1)
  = 5 + 1 = succ(5)
  = 6
*/
```

这种定义自然数计算的方式也被叫做递归定义，因为我们把加法看做递归调用后继函数。那么同样，两个自然数相乘，a * b，就等价于 a 执行 b 次 (+a) 函数，(+a) 表示用参数 a 柯里化后的加法。那么我们的乘法可以这样展开：

```cpp
int a = 2, b = 4;
int c = a * b;
/*
c = a * b
  = 0 + 2 * 4 = (+2)(0) + (2 * 3)
  = 2 + 2 * 3 = (+2)(2) + (2 * 2)
  = 4 + 2 * 2 = (+2)(4) + (2 * 1)
  = 6 + 2 * 1 = (+2)(6)
  = 8
*/
```

同样，两个自然数的乘方，a ^ b，就等价于 a 执行 b 次 (\*a) 函数，(\*a) 表示用参数 a 柯里化后的乘法。那么我们的乘方可以这样展开：

```cpp
int a = 2, b = 4;
int c = pow(a, b);
/*
c = pow(a, b)
  = 1 * pow(2, 4) = (*2)(1) * pow(2, 3)
  = 2 * pow(2, 3) = (*2)(2) * pow(2, 2)
  = 4 * pow(2, 2) = (*2)(4) * pow(2, 1)
  = 8 * pow(2, 1) = (*2)(8)
  = 16
*/
```

熟悉了递归定义的加法乘法乘方之后，我们先写一个简单的普通版本的递归函数

```cpp
int pow(int a, int b)
{
    if (b == 0) return 1;
    if (b == 1) return a;
    return a * pow(a, b - 1);
}
```

这个代码特化了两个终止条件，相对之特化一个终止条件的减少了一层递归调用，能提速不少。但是我们都知道，每递归调用一次，就要压一次函数堆栈，尤其是一但递归次数变多了，不仅仅是效率大打折扣，还有很大概率因为堆栈溢出而出错。那么元编程是怎么处理这个任务的呢？

```cpp
template <int A, int B> struct Pow
{
    enum { result = A * Pow<A, B - 1>::result };
};

template <int A> struct Pow<A, 1> { enum { result = A }; };

template <int A> struct Pow<A, 0> { enum { result = 1 }; };
```

和递归写法类似，我们同样特化了两个终止条件，然后用一个枚举量 `result` 来记录结果。之所以选择枚举量，是因为枚举量是一个常量表达式的最原始的实现方法，但是现在我们有 `constexpr` 关键字了，所以也可以这样写

```cpp
template <int A, int B> struct ConstPow
{
    constexpr static int result = A * ConstPow<A, B - 1>::result;
};

template <int A> struct ConstPow<A, 1> { constexpr static int result = A; };

template <int A> struct ConstPow<A, 0> { constexpr static int result = 1; };
```

这两种写法本质上是相同的。当然有人会问为什么用 `struct` 而不是用 `class`，事实上，C++ 的 `class` 和 `struct` 最主要的区别就是前者可以显式修饰成员的属性，而后者只有一个 `public` 属性。那么对于这份代码，结果 `result` 显然是我们要公开出去的一个成员，所以为了节省代码量，我就偷懒写成 `struct` 了。

那么这两种写法在具体调用时，有什么区别呢？

```cpp
cout << pow(2, 4) << endl; // => 16
cout << Pow<2, 4>::result << endl; // => 16
```

区别就在于执行计算的时期不同。前者是典型的运行时展开运行时计算，也就是只有在代码真正运行的时候，才会进入函数，开始递归计算。而后者是典型的编译时展开编译时计算。实际上 `Pow<2, 4>::result` 在编译时就会被展开写作如下形式：

```cpp
    Pow<2, 4>::result
<=> 2 * Pow<2, 3>::result
<=> 2 * 2 * Pow<2, 2>::result
<=> 2 * 2 * 2 * Pow<2, 1>::result
<=> 2 * 2 * 2 * 2
<=> 16
```

所以代码中的这句 `cout << Pow<2, 4>::result` 实际上和直接写 `cout << 16` 是一样的，也就是说，真正的计算任务给了编译器，而运行时只是把编译时算好的答案打印出来而已。

在这里，`Pow<>` 这份模板就被称为模板元编程技术，即把一些可以在编译时完成的求值计算，通过模板特化的技术绑定到具体的实例化代码上，从而减少不必要的代码量，提升代码的运行效率。

这里首先要泼一盆冷水给大家，那就是天底下没有免费的好事：模板元编程在提升了代码的运行效率的同时，必然会导致编译时的效率降低，因为你把很多本来运行时的计算前置到了编译时计算，总的计算量并没有改变。所以我们反复强调的是，提升了运行效率，而非提升了整体的性能。

其次还有一个问题，就是在常量的选择上。刚才我们提到了两种定义方法，枚举变量，或者常量表达式。众所周知，枚举变量，是没有地址的！是没有地址的！是没有地址的！所以是不能当做左值使用的！所以是不能当做左值使用的！所以是不能当做左值使用的！为什么要强调这个呢？比如我们有一个函数是这样的接口 `void foo(const int&);`，然后你调用了一个 `foo(Pow<2, 5>::result);` ，这样是不可以的。因为引用传递枚举值的时候，并不会使用任何静态内存，类似于字面量一样，所以一旦你把枚举值当左值使用了，编译器自然就会报错提醒。但是如果是常量表达式，就不会有这种问题，因为常量表达式的第一个前提是，它是变量，所以他有物理地址，所以可以 `foo(ConstPow<2, 5>::result);`。唯一需要注意的是，很多古老的编译器对常量表达式的支持不够，并没有**完全**提到编译时来计算，当然你也可以将错就错：“我们这叫运行时计算的元编程。（这是 feature，不是 bug。）”

## 带分支条件的递归

现在我们实现一个更复杂的递归：利用二分法计算整数的平方根，结果向下取整。那么很简单，我们可以写出来一份简洁的递归版二分代码：

```cpp
int __sqrt(int n, int l, int r)
{
    if (l == r) return l;
    int mid = (l + r + 1) / 2;
    if (n < mid * mid)
        return __sqrt(n, l, mid - 1);
    else return __sqrt(n, mid, r);
}

inline int sqrt(int n) { return __sqrt(n, 0, n); }
```

那么我们对着这个代码照猫画虎把元编程版的写出来。

```cpp
template <int N, int L, int R> struct __Sqrt
{
    enum { mid = (L + R + 1) / 2 };
    enum { result = (N < mid * mid) ? __Sqrt<N, L, mid - 1>::result
                                    : __Sqrt<N, mid, R>::result };
};

template <int N, int L> struct __Sqrt<N, L, L> { enum { result = L }; };

template <int N> struct Sqrt { enum { result = __Sqrt<N, 1, N>::result }; };
```

那么这个代码能用吗？当然是不行的。因为这个三目运算符会自动展开两路的结果，然后就emmm。所以我们得手工实现一个自动屏蔽掉错误道路的 `if-else` 结构：

```cpp
template <bool Flag, class MaybeA, class MaybeB> class IfElse;

template <class MaybeA, class MaybeB>
class IfElse<true, MaybeA, MaybeB>
{
public:
    using ResultType = MaybeA;
};

template <class MaybeA, class MaybeB>
class IfElse<false, MaybeA, MaybeB>
{
public:
    using ResultType = MaybeB;
};
```

然后我们再用这个手工版本的 if-else 去替代之前的三目运算符：

```cpp
template <int N, int L, int R> struct __Sqrt
{
    enum { mid = (L + R + 1) / 2 };

    using ResultType = typename IfElse<(N < mid * mid),
        __Sqrt<N, L, mid - 1>, __Sqrt<N, mid, R> >::ResultType;

    enum { result = ResultType::result };
};

template <int N, int L> struct __Sqrt<N, L, L> { enum { result = L }; };

template <int N> struct Sqrt { enum { result = __Sqrt<N, 1, N>::result }; };
```

现在这个代码就可以正常编译并且得到正确的结果了，同时依旧保持 O(log N) 的时间复杂度不变，因为只有道路正确的模板被实例化了，错误的道路上的模板全部被屏蔽掉了，所以我们只特化了 O(log N) 数量级的模板，自然就保持了原时间复杂度不变了。

## 试试稍复杂的模板元编程

最后我们实现一个稍微复杂的例子：计算两个向量的一般数学意义上的内积，即比如我们有两个三维向量 `a[]` 和 `b[]`，其内积的定义为 `a[0] * b[0] + a[1] * b[1] + a[2] * b[2]`。

我们先考虑最简单的模板函数的实现方式，不妨假设我们的向量是通过容器 `T[]` 储存的，那么我们可以这样写：

```cpp
template <class T> T dot(int dim, T *a, T *b)
{
    int result = T();
    for (int i = 0; i < dim; i++)
        result += a[i] * b[i];
    return result;
}
```

这是最简单的循环写法，那么我们还是先把他改成递归：

```
template <class T> T dot_re(int dim, T *a, T *b)
{
    if (dim == 1) return (*a) * (*b);
    return (*a) * (*b) + dot_re(dim - 1, a + 1, b + 1);
}
```

有了递归版本之后，我们再把它改为元编程版本

```cpp
template <int DIM, class T> class __Dot
{
public:
    static T result(T *a, T *b)
    {
        return (*a) * (*b) + __Dot<DIM - 1, T>::result(a + 1, b + 1);
    }
};

template <class T> class __Dot<1, T>
{
public:
    static T result(T *a, T *b) { return (*a) * (*b); }
};

template <int DIM, class T>
T dot_meta(T *a, T *b)
{
    return __Dot<DIM, T>::result(a, b);
}
```

这时候可能会问，为什么命名第一种写法最简单最快，但是我们还要刻意写一份元编程版本的呢？

这是由于，现代编译器普遍都具有了针对循环和递归的优化，这种优化对于比较大规模的循环是非常值得的，但是针对于比较小的向量内积计算，可能就没有一句直接展开 `a[0] * b[0] + a[1] * b[1] + a[2] * b[2]` 更为迅速了。可能一次两次的这种计算体现不出来，但是如果是大规模的计算可能就差别很大了。为了方便，我们折腾一个 benchmark 测试一下。

```cpp
vector<int> a = {1, 2, 3, 4}, b = {5, 6, 7, 8};

void benchmark(int cnt)
{
    #define DOT_META_(dim, a, b) dot_meta<dim>(a, b)

    #define TEST_CASE(id, foo) for (int i = 0; i < cnt; i++)\
        for (int j = 0; j < cnt; j++)\
    {\
        int hoge = foo(4, a.data(), b.data());\
        sum##id += hoge * foo(4, a.data(), a.data());\
    }\
    auto t##id = std::chrono::high_resolution_clock::now();

    int sum1 = 0, sum2 = 0, sum3 = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    TEST_CASE(1, dot)
    TEST_CASE(2, dot_re)
    TEST_CASE(3, DOT_META_)
    cout << std::fixed << std::setprecision(2);
    cout << "======== " << cnt << " ========\n";
    cout << "dot():      " << std::chrono::duration<double, std::milli>(t1 - t0).count() << "ms\n";
    cout << "dot_re():   " << std::chrono::duration<double, std::milli>(t2 - t1).count() << "ms\n";
    cout << "dot_meta(): " << std::chrono::duration<double, std::milli>(t3 - t2).count() << "ms\n";
    cout << "(sum1 % 100, sum2 % 100, sum3 % 100)= (";
    cout << (sum1 % 100) << ", " << (sum2 % 100) << ", "<< (sum3 % 100) << ")\n";
    cout << endl;
    #undef TEST_CASE
    #undef DOT_META_
}

int main()
{
    for (int i = 1; i < 10; i += 2)
        benchmark(i * 1000);
    return 0;
}
```

然后我们在编译时加上 `-O2` 选项，直接看结果：

```plain
======== 1000 ========
dot():      4.02ms
dot_re():   1.99ms
dot_meta(): 0.00ms
(sum1 % 100, sum2 % 100, sum3 % 100)= (0, 0, 0)

======== 3000 ========
dot():      37.87ms
dot_re():   15.98ms
dot_meta(): 1.99ms
(sum1 % 100, sum2 % 100, sum3 % 100)= (16, 16, 16)

======== 5000 ========
dot():      104.75ms
dot_re():   43.89ms
dot_meta(): 6.99ms
(sum1 % 100, sum2 % 100, sum3 % 100)= (48, 48, 48)

======== 7000 ========
dot():      203.42ms
dot_re():   87.76ms
dot_meta(): 13.96ms
(sum1 % 100, sum2 % 100, sum3 % 100)= (-4, -4, -4)

======== 9000 ========
dot():      336.08ms
dot_re():   144.64ms
dot_meta(): 23.93ms
(sum1 % 100, sum2 % 100, sum3 % 100)= (-40, -40, -40)
```

可见，元编程版本的速度远远超过了前面的两种写法，而为什么递归版本的速度比循环要快呢？这是因为编译器把我们的递归版本进行了尾递归改写，然后再尾递归优化。有兴趣的可以利用 `g++ -S` 把汇编代码编译出来对比下区别。在我的电脑上，如果没有开 `-O2` 优化，汇编的代码忠实的记录了执行时会进行递归调用。但是如果开了 `-O2` 优化之后，就会发现整个递归已经被改写成非常简洁的迭代代码。这就是最简单的尾递归优化。但是即便如此，也比不上直接展开计算的效率。当然，并不是所有时候这种展开都会很快，而这更多的细节就等到真正用到的时候再去测试吧。