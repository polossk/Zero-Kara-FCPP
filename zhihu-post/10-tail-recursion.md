尾递归
========================================

上一节提到了尾递归优化，那么什么是尾递归呢。我们用阶乘计算来举例说明。注意，以下的内容均不考虑数值溢出！注意，以下的内容均不考虑数值溢出！注意，以下的内容均不考虑数值溢出！

众所周知，阶乘的递归定义用 Haskell 表示如下：

```haskell
fact :: Int -> Int
fact 0 = 1
fact 1 = 1
fact n = n * fact (n-1)
```

我们也可以根据这份定义写出 C++ 版本的用递归的方式计算阶乘的方法：

```cpp
int frac_re(int n)
{
    if (n == 0) return 1;
    else if (n == 1) return 1;
    else return n * frac_re(n - 1);
}
```

很简单是吧。同时我们也可以写一份不用递归方式，用迭代方法计算阶乘的函数：

```cpp
int frac_it(int n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    int ret = 1;
    for (int i = 2; i <= n; ret *= i++);
    return ret;
}
```

在这里，我之所以不用 `while (n--)` 这种简洁但是令人混淆的语句，是因为我要把 `for` 循环中的循环变量 `i` 单独拿来说明。注意到，两份代码的最大区别就在于，递归把进入下一步的事情托管给了编译器（或解释器）。而对于迭代法，我们可以理解成，迭代法需要自行维护如何进行下一步的计算。在这里，所谓的下一步的计算，对应迭代法就是 `ret *= i++`，而对应递归则是 `n * frac_re(n - 1)`。也就是说，递归和迭代本身应当是同一类事务，只不过前者额外依托了运行环境中的函数堆栈，而后者则相当于由程序员来维护一个结构来进行迭代而已。那么，既然是同一类事务，为什么我们常说递归的效率并没有迭代好呢？问题就出在这个函数堆栈这里。比如我们计算 5 的阶乘，迭代法只需要把循环体重复 5 次即可，而执行 `frac_re(5)` 的时候，我们会遇到不止 5 次的操作：

```cpp
frac_re(5)
=> 5 * frac_re(4)
=> 5 * 4 * frac_re(3)
=> 5 * 4 * 3 * frac_re(2)
=> 5 * 4 * 3 * 2 * frac_re(1)
=> 5 * 4 * 3 * 2 * 1
```

很多人会问，没毛病啊，确实是 5 次调用。没错，但是问题是，调用后，你还要返回值呀，所以好端端的 5 次操作，硬生生的因为调用和返回值，变成了 10 次操作（准确的说是，5 次调用操作和 5 次返回操作，这里假设了函数堆栈操作所消耗的时间均为同一量级）。

```cpp
=> 5 * 4 * 3 * 2 * 1 // return 1 for frac_re(1)
=> 5 * 4 * 3 * 2 // return 2 for frac_re(2)
=> 5 * 4 * 6 // return 6 for frac_re(3)
=> 5 * 24 // return 24 for frac_re(4)
=> 120 // return 120 for frac_re(5)
```

这时候有人会讲了，有一种写法是可以把迭代法和递归结合起来，比如这样：

```cpp
int frac_tr(int n, int ret = 1)
{
    if (n == 0) return ret;
    else if (n == 1) return ret;
    else return frac_tr(n - 1, n * ret);
}
```

如果是这个函数的话，计算 5 的阶乘的时候就变成了这样

```cpp
frac_tr(5) = frac_tr(5, 1)
=> frac_tr(4, 5)
=> frac_tr(3, 20)
=> frac_tr(2, 60)
=> frac_tr(1, 120)
=> 120 // return 120 for frac_tr(1, 120)
=> 120 // return 120 for frac_tr(2, 60)
=> 120 // return 120 for frac_tr(3, 20)
=> 120 // return 120 for frac_tr(4, 5)
=> 120 // return 120 for frac_tr(5, 1)
```

注意到这种写法的话，后面的这些 `return` 操作实际上是一根绷紧的线：一但这头执行了返回值操作，就像线的一头被弹了一下一样，中间不必进行任何的修饰，自动的就传递到了线的另一头。而对比普通的递归操作，虽然线的另一头已经发出了信号，但是这个信号每到一个地方，就要停下来进行加工，然后再传递给下一个位置。那么一种显然易见的优化方案就浮出水面了：把线的两端直接绑定到一起，像虫洞一样忽略掉中间的这段链接，这就是尾递归优化了。如果编译器支持尾递归优化，那么像 `frac_tr()` 的这种写法，最后就不必一层一层返回，更不必一层一层在函数堆栈上压栈压栈，执行 `frac_tr(5, 1)` 的时候，就会变换成执行 `frac_tr(4, 5)`，以此类推，最后的返回值，就是整族函数的返回值。

说的很高大上，但是实际上就是我们手工维护了记录答案的中间变量呗？同时返回值那里不要写计算方法，而是直接写函数自己。

那么尾递归优化除了效率上的优势以外，还有什么特殊的功能么？当然有，最大的优点就是，规避了函数堆栈溢出的情况。同样是阶乘函数，我们都知道计算比较小的值，哪种写法都无所谓，但是如果是计算 10000 的阶乘，100000 的阶乘，想象一下函数堆栈里挤满了这些函数的场景。但是如果具有尾递归优化的话，递归调用会变化成类似于循环迭代的方式进行执行，自然也就不会有函数堆栈溢出的情况了。（你已经不是函数调用了，还怎么让函数堆栈溢出呢？）

那么 C++ 是否支持尾递归优化呢？必然是支持的，至少我的环境（如下所示）是支持的，而且比我想象的还要好很多。

```shell
>>>g++ --version
g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

我们随手写一个 benchmark 来看看效果

```cpp
void benchmark(int cnt)
{
    #define TEST_CASE(id, foo) int sum##id = 0;\
    for (int i = 0; i < cnt; i += 1000)\
    {\
        sum##id += (i) * foo(i);\
    }\
    auto t##id = std::chrono::high_resolution_clock::now();

    auto t0 = std::chrono::high_resolution_clock::now();
    TEST_CASE(1, frac_re)
    TEST_CASE(2, frac_tr)
    TEST_CASE(3, frac_it)
    cout << std::fixed << std::setprecision(2);
    cout << "======== " << cnt << " ========\n";
    cout << "frac_re(): " << std::chrono::duration<double, std::milli>(t1 - t0).count() << "ms\n";
    cout << "frac_tr(): " << std::chrono::duration<double, std::milli>(t2 - t1).count() << "ms\n";
    cout << "frac_it(): " << std::chrono::duration<double, std::milli>(t3 - t2).count() << "ms\n";
    cout << "(sum1, sum2, sum3) % 11 = (";
    cout << (sum1 % 11) << ", " << (sum2 % 11) << ", " << (sum3 % 11) << ")\n";
    cout << endl;

    #undef TEST_CASE
}

int main()
{
    for (int i = 1; i < 10; i += 2)
        benchmark(i * 100000);
    return 0;
}

// filename: ch6-frac-benchmark.cpp
// compile this> g++ ch6-frac-benchmark.cpp -o ch6-frac-benchmark.exe -O2
```

注意，这份代码必须用 `-O2` 选项进行编译并执行。因为如果不开优化的话，会直接因为堆栈溢出而终止。这里也送上我本地的运行结果加以参考：

```plain
======== 100000 ========
frac_re(): 4.99ms
frac_tr(): 3.99ms
frac_it(): 3.99ms
(sum1, sum2, sum3) % 11 = (0, 0, 0)

======== 300000 ========
frac_re(): 37.90ms
frac_tr(): 36.90ms
frac_it(): 37.41ms
(sum1, sum2, sum3) % 11 = (0, 0, 0)

======== 500000 ========
frac_re(): 104.72ms
frac_tr(): 105.72ms
frac_it(): 103.72ms
(sum1, sum2, sum3) % 11 = (0, 0, 0)

======== 700000 ========
frac_re(): 206.99ms
frac_tr(): 204.42ms
frac_it(): 205.45ms
(sum1, sum2, sum3) % 11 = (0, 0, 0)

======== 900000 ========
frac_re(): 339.09ms
frac_tr(): 348.08ms
frac_it(): 339.64ms
(sum1, sum2, sum3) % 11 = (0, 0, 0)
```

其中，这三个分别是普通的递归，尾递归，和普通的迭代法的实现。从结果可以看出，三者的时间消耗基本相同，均在同一数量级别。如果把普通的迭代法作为标杆的话，我们可以认为，开启 `-O2` 优化后的，编译器对尾递归和普通递归都进行了一定程度的优化。那么到底这个优化程度有多大呢？我们换一个样例试试。注意到，阶乘运算是一个线性递归，我们换一个简单的树形递归试试。这里我们选择斐波那契数列作为我们的测试对象。同样，斐波那契数列（0, 1, 1, 2, 3, 5, 8, 13, 21, ...）的递归定义用 Haskell 表示如下：

```haskell
fib :: Int -> Int
fib 0 = 0
fib 1 = 1
fib n = fib (n - 1) + fib (n - 2)
```

同样我们写出三种计算斐波那契数列的程序如下：

```cpp
int fib_it(int n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    int ret = 1, a = 1, b = 0;
    for (int i = 2; i <= n; i++)
    {
        ret = a + b; b = a; a = ret;
    }
    return ret;
}

int fib_re(int n)
{
    if (n == 0) return 0;
    else if (n == 1) return 1;
    else return fib_re(n - 1) + fib_re(n - 2);
}

int fib_tr(int n, int ret = 0, int tmp = 1)
{
    if (n == 0) return ret;
    else return fib_tr(n - 1, ret + tmp, ret);
}
```

值得一提的是，由于斐波那契数列的特殊性，使得他可以被改造成线性递归。这里我们还是用类似的方法测试效率，但是不再测试 `fib_re()` 的效率。众所周知，如果 `fib_re()` 不使用记忆化技术去缓存以求过的值的话，其效率是非常低的。

```cpp
void benchmark(int cnt)
{
    #define TEST_CASE(id, foo) int sum##id = 0;\
    for (int i = 0; i < cnt; i += 1000)\
    {\
        sum##id += (i) * foo(i);\
    }\
    auto t##id = std::chrono::high_resolution_clock::now();

    auto t0 = std::chrono::high_resolution_clock::now();
    TEST_CASE(1, fib_tr)
    TEST_CASE(2, fib_it)
    cout << std::fixed << std::setprecision(2);
    cout << "======== " << cnt << " ========\n";
    cout << "fib_tr(): " << std::chrono::duration<double, std::milli>(t1 - t0).count() << "ms\n";
    cout << "fib_it(): " << std::chrono::duration<double, std::milli>(t2 - t1).count() << "ms\n";
    cout << "(sum1, sum2) % 11 = (";
    cout << (sum1 % 11) << ", " << (sum2 % 11) << ")\n";
    cout << endl;

    #undef TEST_CASE
}

int main()
{
    for (int i = 0; i < 10; i++)
    {
        cout << "\t" << fib_it(i);
        cout << "\t" << fib_re(i);
        cout << "\t" << fib_tr(i) << "\n";
    }
    for (int i = 1; i < 10; i += 2)
        benchmark(i * 100000);
    return 0;
}
```

同样附上我这里的运行结果：

```plain
======== 100000 ========
fib_tr(): 2.99ms
fib_it(): 4.99ms
(sum1, sum2) % 11 = (3, 3)

======== 300000 ========
fib_tr(): 21.95ms
fib_it(): 25.93ms
(sum1, sum2) % 11 = (3, 3)

======== 500000 ========
fib_tr(): 78.79ms
fib_it(): 91.77ms
(sum1, sum2) % 11 = (3, 3)

======== 700000 ========
fib_tr(): 131.66ms
fib_it(): 174.53ms
(sum1, sum2) % 11 = (6, 6)

======== 900000 ========
fib_tr(): 217.43ms
fib_it(): 297.92ms
(sum1, sum2) % 11 = (8, 8)
```

可以看到，两者几乎是同一水平。可见，尾递归的确可以被优化到和普通循环几乎相同的程度。