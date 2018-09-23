基础序列式容器 `vector`
========================================

先回答上一篇的问题，显然是不可以的。由于模板要求在编译时必须提供给他常量，而 `size()` 函数并不是一个常量，所以在编译时会报错告诉你编译器需要一个常量。当然你也可以用 `#define` 把 `size()` 变成一个宏定义函数，这样确实简单粗暴。不过如果我们知道 C++11 已经提供了常量表达式这一特性之后，这个问题就迎刃而解了。之前我们所接触的 `const` 更多的是指**运行时常量性**，其实更准确的理解方式是**构造运行时的只读变量**。而很多时候我们真正需要的是**编译时**的常量性，比如我们都知道 `size()` 会返回一个**编译时**就**确定的**同时也**不会改变的**值，但是在 C++11 标准出现之前，编译器却不买账。不过现在，我们只需要对 `size()` 函数进行常量表达式修饰就可以解决这个难题。简单地说，这里需要用到 C++11 新给出的关键字 `constexpr` 去修饰函数，同时也需要保证这个函数必须有一个返回值，而且必须是一个常量表达式。具体参考下面的代码：

```cpp
#include <iostream>
#include <tuple>
#include <utility>

#include "tuple_utils.hpp"

template<typename ...Ts>
auto last(std::tuple<Ts...> t)
{
    return std::get<sizeof...(Ts) - 1u>(t);
}

template <class T>
constexpr std::size_t size(const T &t)
{
    return std::tuple_size<T>::value;
}

template <class T>
auto last2(const T &t)
{
    return std::get<size(t) - 1u>(t);
}

int main()
{
    auto t = std::make_tuple(2, 3.14, 'c');
    __utils::print_tuple((t)); std::cout << std::endl; // => (2, 3.14, 'c')
    std::cout << size(t) << std::endl; // => 3
    std::cout << last(t) << ", " << last2(t) << std::endl; // => c, c
    return 0;
}

// filename: ch1-tuple-constexpr-last.cpp
// compile this> g++ ch1-tuple-constexpr-last.cpp -o ch1-tuple-constexpr-last.exe -std=c++14
```

在这里我们先是对 `size()` 前面添加了 `constexpr` 关键字，表示这个是一个**常量函数**，一个在编译时就能返回确定值的函数。同时注意到形式参数也需要用 `const` 加以约束，否则就不能保证这个函数的常量性。说起来很绕口，但是简而言之就是两句话：

* 确定变量 `var` 在函数体内部不会发生变化的时候用 `const Type &var` 一定没错；
* 确定整个函数在**编译时**就有正确结果的时候可以使用**常量表达式**对他进行约束，尤其是在使用模板参数的时候。

## 基础序列式容器 `vector`

一般而言我们把一组可以排序的（但是未必是有序的）数据称之为序列式的数据，自然“盛放”这些数据的容器，也被称为序列式容器。标准模板库（Standard Template Library，缩写：STL）提供了很多简单轻巧的容器，包含典型的序列式容器如向量 `vector`，链表 `list`，双端队列 `deque`，以及其他的关联式容器如集合 `set`，映射表 `map`，散列表 `hash_set` 等等。这里我们只介绍最基础的序列式容器 `vector`。

在 STL 中， `vector` 被设计为一个不定长的，顺序存储的，存储元素类型统一的容器，定位是更强的数组。然而之所以不叫 `array` 是因为 `array` 这个关键字已经被 STL 使用了，被设计为一个定长的，顺序存储的，存储元素类型统一的容器。我们用一个简单的表格来对比一下原始的数组和这几种容器的异同。

| 特性 | `int []` | `std::array<int>` | `std::vector<int>` | `std::tuple<...>` |
|:----:|:--------:|:-----------------:|:------------------:|:-----------------:|
| 是否固定长度 | Y | Y | N | Y |
| 是否顺序存储 | Y | Y | Y | Y |
| 是否统一元素类型 | Y | Y | Y | N |

但是实际上，固定长度有的时候反而会很不方便，所以如果有一个能想什么时候加一个元素就加一个元素的更简便一些的数组出现的话，自然会受到大家的喜欢。更简单地讲就是，把内存管理这一问题交给一个更负责任的工具，而非我们自己。试想，如果我们一开始知道一个数组的最大长度还好，否则摸着石头过河的时候一但发生数组越界就糟糕了。但是 `vector` 也不是傻乎乎的每次新增一个元素，就只新增一个元素的最大容量，稍后会简单提到他的扩充原理。我们先观察一份代码，简单的看看 `vector` 的能耐如何。

```cpp
#include <iostream>
#include <string>
#include <vector>

template<class T>
std::ostream& operator<<(std::ostream &os, const std::vector<T> &xs)
{
    os << '[';
    for (auto x : xs) os << " " << x;
    return os << ']';
}

int main()
{
    std::vector<std::string> echo(3, "Hello");
    std::cout << echo << std::endl; // => [ Hello Hello Hello]
    std::vector<int> fib = {1, 1, 2, 3, 5, 8, 13};
    std::cout << fib << std::endl; // => [ 1 1 2 3 5 8 13]
    fib.push_back(fib.back());
    std::cout << fib << std::endl; // => [ 1 1 2 3 5 8 13 13]
    fib.back() = fib.front();
    std::cout << fib << std::endl; // => [ 1 1 2 3 5 8 13 1]
    fib.pop_back();
    
    for (int i = 0, len = fib.size(); i < len; i++)
    {
        std::cout << " " << fib[i];
    }
    std::cout << std::endl; // => 1 1 2 3 5 8 13

    for (std::vector<int>::iterator it = fib.begin(); it != fib.end(); it++)
    {
        (*it) = (*it) + 1;
    }

    for (std::vector<int>::reverse_iterator ri = fib.rbegin(); ri != fib.rend(); ri++)
    {
        std::cout << " " << (*ri);
    }
    std::cout << std::endl; // =>  14 9 6 4 3 2 2
    return 0;
}

// filename: ch1-vector-example-1.cpp
// compile this> g++ ch1-vector-example-1.cpp -o ch1-vector-example-1.exe
```

这份代码简单的介绍了 `vector` 最常用的几种用法，包含构造函数，元素操作，迭代器三大类，我们接着例子逐一说明。

* 第 5- 11 行定义了一个输出一个 `vector` 的函数，注意到第 9 行 `for` 循环的写法，这个是 C++11 所提供的范围枚举方法，`for (auto x : xs)` 表示 `for` 循环时依次在 `xs` 中从前往后取元素，并命名为 `x`；
* 第 15 行是 `vector` 最常用的构造函数，构造函数 `vector(size_type count, const T& value)` 要求提供两个参数，`vector` 内部有多少个元素 `count` 和内部元素的初始值 `value`；
* 第 17 行是 C++11 新提供的列表初始化方法，类似于数组初始化一样，`vector` 现在可以直接把元素的值扔到大括号里，编译时就会自动创建一个初始化好的 `vector`;
* 第 19-23 行使用了 4 个 `vector` 最常见的方法，`front()` 和 `back()` 分别会返回当前 `vector` 的第一个元素和最后一个元素的一个引用（所以第 21 行可以直接对其进行修改），`push_back()` 和 `pop_back()` 分别表示在当前 `vector` 的最后添加或删除一个元素；
* 第 25-29 行展示了类似于数组的遍历 `vector` 的方法，`size()` 表示获取当前 `vector` 内元素的个数，`operator[]` 的使用方法和数组完全一样，但是需要注意的是和数组一样，下标引用的时候内部并不会进行越界检查，所以可能会产生越界的错误，如果想要规避则可以使用 `at()` 方法，该方法默认进行越界检查，一但越界则会抛出 `std::out_of_range` 异常从而方便后续处理；
* 第 31-40 行分别展示了 `vector` 内部所提供的类似于指针的工具，迭代器。`begin()` 和 `end()` 分别表示正向遍历时的第一个元素的迭代器和遍历结束的迭代器。遍历结束的意思并不等同于最后一个元素，事实上，`end()` 所指向的位置是 `vector` 最后一个元素（末端）的下一个储存元素的位置。所以如果直接写 `cout << (*foo.end())` 就会报越界错误。同样，`rbegin()` 和 `rend()` 分别表示反向遍历时的第一个元素的迭代器和遍历结束的迭代器。这里 `rbegin()` 指向的是 `vector` 末端的元素，而 `rend()` 指向的则是 `vector` 头部的前一个元素。同样的，直接 `*rend()` 也会报越界错误。具体也可以参考这个图片![range-rbegin-rend](http://upload.cppreference.com/mwiki/images/3/39/range-rbegin-rend.svg)（图片来源：<https://en.cppreference.com/w/cpp/container/vector/rend>）

需要额外说明一点的是，由于事实上很多编译器已经支持了 C++11，所以针对迭代器，完全可以不必把他的类型准确无误地写出来。在这里，我们可以用 `auto` 关键字代劳，让编译器帮助我们完成类型推导工作。所以更简单地写法是：

```cpp
for (auto it = foo.begin(); it != foo.end(); it++)
{
    // do something
}
```

在了解了这些简单的方法之后，我们再来看看 `vector` 是如何管理自己的内存占用的。我们先站在设计者的角度来思考，假设我们想要对一个顺序存储的数据结构实现一个功能，告诉用户当前容器的最大容量和当前容器内安排了多少元素，最简单的方法就是维护三个迭代器 `start`，`finish`，`end_of_storage`，分别表示使用空间的头，使用空间的尾，可用空间的尾，然后公开两个方法 `size()` 和 `capacity()`，表示使用空间的大小和可用空间的大小。实际上，`vector` 就是沿着这条思路实现的自己的内存管理。在了解了这一点之后我们现在开始思考一开始提出的问题，如果我们对 `vector` 反复添加一个元素，`vector` 是怎么处理自己的可用空间的。这里我们先看一份代码。

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<class T>
ostream& operator<<(ostream &os, const vector<T> &xs)
{
    os << "size: " << xs.size() << ", ";
    os << "capacity: " << xs.capacity() << ", [";
    for (auto x : xs) os << " " << x;
    return os << ']';
}

int main()
{
    vector<int> a = { 1, 2 };
    cout << a << endl;
    for (int i = 0; i < 10; i++)
    {
        a.push_back(i); cout << a << endl;
    }
    cout << "----------------" << endl;
    vector<int> b = { 1, 2, 3 };
    cout << b << endl;
    for (int i = 0; i < 10; i++)
    {
        b.push_back(i); cout << b << endl;
    }
    return 0;
}

// filename: ch1-vector-example-2.cpp
// compile this> g++ ch1-vector-example-2.cpp -o ch1-vector-example-2.exe
```

这份代码在不同的编译器上的结果可能是不同的，因为不同的编译器对 `vector` 的内存管理中的一些参数的实现可能不同。在我的编译器上，这份代码的结果如下：

```plain
size: 2, capacity: 2, begin(): 0x7016f0, by now: 0x7016f8, end(): 0x7016f8
size: 3, capacity: 4, begin(): 0x701710, by now: 0x70171c, end(): 0x701720
size: 4, capacity: 4, begin(): 0x701710, by now: 0x701720, end(): 0x701720
size: 5, capacity: 8, begin(): 0x701950, by now: 0x701964, end(): 0x701970
size: 6, capacity: 8, begin(): 0x701950, by now: 0x701968, end(): 0x701970
size: 7, capacity: 8, begin(): 0x701950, by now: 0x70196c, end(): 0x701970
size: 8, capacity: 8, begin(): 0x701950, by now: 0x701970, end(): 0x701970
size: 9, capacity: 16, begin(): 0x7016f0, by now: 0x701714, end(): 0x701730
size: 10, capacity: 16, begin(): 0x7016f0, by now: 0x701718, end(): 0x701730
size: 11, capacity: 16, begin(): 0x7016f0, by now: 0x70171c, end(): 0x701730
size: 12, capacity: 16, begin(): 0x7016f0, by now: 0x701720, end(): 0x701730
----------------
size: 3, capacity: 3, begin(): 0x701950, by now: 0x70195c, end(): 0x70195c
size: 4, capacity: 6, begin(): 0x701970, by now: 0x701980, end(): 0x701988
size: 5, capacity: 6, begin(): 0x701970, by now: 0x701984, end(): 0x701988
size: 6, capacity: 6, begin(): 0x701970, by now: 0x701988, end(): 0x701988
size: 7, capacity: 12, begin(): 0x701990, by now: 0x7019ac, end(): 0x7019c0
size: 8, capacity: 12, begin(): 0x701990, by now: 0x7019b0, end(): 0x7019c0
size: 9, capacity: 12, begin(): 0x701990, by now: 0x7019b4, end(): 0x7019c0
size: 10, capacity: 12, begin(): 0x701990, by now: 0x7019b8, end(): 0x7019c0
size: 11, capacity: 12, begin(): 0x701990, by now: 0x7019bc, end(): 0x7019c0
size: 12, capacity: 12, begin(): 0x701990, by now: 0x7019c0, end(): 0x7019c0
size: 13, capacity: 24, begin(): 0x706ae0, by now: 0x706b14, end(): 0x706b40
```

可以注意到，一但达到了上限，`vector` 会把可用空间的大小扩充为之前的 k 倍，在这里 k = 2。有的编译器的 k 值为 1.5，有兴趣的可以思考下为什么。

最后还有三个方法 `clear()`，`insert()` 和 `erase()` 还没介绍，我们照例还是先上一份代码，通过运行结果来加深印象。

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<class T>
ostream& operator<<(ostream &os, const vector<T> &xs)
{
    os << '[';
    for (auto x : xs) os << " " << x;
    return os << ']';
}

int main()
{
    vector<int> a = { 1, 2, 3, 4 };
    a.clear(); cout << a << endl; // => []
    a.insert(a.begin(), 5);
    cout << a << endl; // => [ 5]
    for (int i = 0; i < 3; i++) a.push_back(7);
    cout << a << endl; // => [ 5 7 7 7]
    a.insert(a.begin(), 2, 9);
    cout << a << endl; // => [ 9 9 5 7 7 7]
    
    vector<int> b = {10, 11, 12};
    a.insert(a.begin() + 1, b.begin(), b.end());
    cout << a << endl; // => [ 9 10 11 12 9 5 7 7 7]

    int c[5] = {20, 21, 22, 999, 999};
    a.insert(a.begin(), c, c + 3);
    cout << a << endl; // => [ 20 21 22 9 10 11 12 9 5 7 7 7]

    a.erase(a.begin());
    cout << a << endl; // => [ 21 22 9 10 11 12 9 5 7 7 7]
    a.erase(a.begin() + 2, a.begin() + 5);
    cout << a << endl; // => [ 21 22 12 9 5 7 7 7]

    // erase all number gather than 10
    for (auto it = a.begin(); it != a.end(); )
    {
        it = (*it > 10) ? a.erase(it) : it + 1;
    }
    cout << a << endl; // => [ 9 5 7 7 7]
    return 0;
}

// filename: ch1-vector-example-3.cpp
// compile this> g++ ch1-vector-example-3.cpp -o ch1-vector-example-3.exe
```

* 第 18 行的 `clear()` 方法表示清空当前的 `vector`，清空后 `size() == 0`；
* 第 19 行是 `insert(pos, value)` 方法的最基础版本，第一个参数迭代器 `pos` 表示位置，第二个参数 `value` 表示插入元素的值，效果是在迭代器 `pos` 之前插入元素 `value`；
* 第 23 行是 `insert(pos, count, value)` 方法的另一个版本，多出来的参数 `count` 表示插入 `count` 个元素 `value`；
* 第 26-28 行和第 30-32 行是 `insert(pos, first, last)` 版本，`first` 和 `last` 分别表示插入数据所在数组或向量的范围，这里的区间是左闭右开的 `[first, last)`；
* `insert()` 也会返回值，如果成功插入元素，则返回第一个所插入的元素所在的迭代器，如果没有插入元素，比如 `count == 0` 或 `first == last` 会返回 `pos` 本身，你也可以理解为插入了 0 个元素；
* 第 34 行和第 36 行展示了 `erase()` 方法，两者分别是删除一个元素和删除一个区间的元素，后者的区间依旧是左闭右开的；
* `insert()` 也会返回值，返回删除元素前，所删除元素后面的第一个元素的迭代器，如果后面没有元素则会返回 `end()`；

值得一提的是，`insert()` 与 `erase()` 的效率有时会非常低，有兴趣的人可以借助 `data()` 方法获取到内存地址观察下变化分析下原因。

如果能够熟练使用 `vector`，那么自然而然就可以借助一本参考书（比如 <https://en.cppreference.com/w/>）快速上手使用其他的序列容器。因此就不再过多描述其他的序列容器了。
