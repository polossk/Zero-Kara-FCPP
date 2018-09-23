STL 中的主要工具
========================================

STL 中不仅提供了很多容器，同时也提供了两个基本的数据单位供大家使用。之所以叫数据单位，是因为本质上 `pair` 和 `tuple` 是两个通用的数据类型模板。更简单的讲，关键在于两点，其一是通用，也就是可以随用随写，其二是模板，也就是他的具体实现方法。

## 包装两个数据的元组 `pair`

每个人都会遇到如此简单的问题，数据的绑定。比如你有一个学习小组，每个人的姓名和他的学习能力这两个数据构成了这个小组里面所有成员的实体。假设姓名用 `string` 类型，学习能力用 `int` 类型，最传统的方法就是构造一个只有数据的结构体。而现在有了 `pair` 之后你只需要直接用 `pair<string, int>` 就可以代替前面冗长的结构体声明，而且使用起来也十分简单。具体而言请参考如以下代码所示。

```cpp
#include <iostream>
#include <string>
#include <utility>
#include <functional>

int main()
{
    std::string names[3] = { "alice", "bob", "carl" };
    int score[3] = { 5, 6, 7 };

    // build a pair from string and int
    std::pair<std::string, int> p0 = std::make_pair(names[0], score[0]);
    std::cout << "The value of p0 is "
              << "(" << p0.first << ", " << p0.second << ")\n";
    // => The value of p0 is (alice, 5)

    // build a pair from string and std::ref(int)
    auto p1 = std::make_pair(names[1], std::ref(score[1]));
    std::cout << "The value of p1 is "
              << "(" << p1.first << ", " << p1.second << ")\n";
    // => The value of p1 is (bob, 6)
    score[1] += 10;
    std::cout << "After score[1] += 10, the value of p1 is "
              << "(" << p1.first << ", " << p1.second << ")\n";
    // => After score[1] += 10, the value of p1 is (bob, 16)
    p1.second += 100;
    std::cout << "After p1.second += 100, the value of p1 is "
              << "(" << p1.first << ", " << p1.second << "), "
              << "while score[1] is " << score[1] << "\n";
    // => After p1.second += 100, the value of p1 is (bob, 116), while score[1] is 116
    names[1] += "bi";
    std::cout << "After names[1] += \"bi\", the value of p1 is "
              << "(" << p1.first << ", " << p1.second << "), "
              << "while names[1] is " << names[1] << "\n";
    // => After names[1] += "bi", the value of p1 is (bob, 116), while names[1] is bobbi
    return 0;
}

// filename: ch1-pair-example-1.cpp
// compile this> g++ ch1-pair-example-1.cpp -o ch1-pair-example-1.exe
```

在这份代码当中，有以下几点需要额外注明：

* 第 3-4 行添加了必要的头文件，其中 `std::pair<>` 以及 `std::make_pair()` 在头文件 `<utility>` 当中，`std::ref()` 在头文件 `<functional>` 当中；
* 第 8-9 行预定义了一些数据，第 12 行显式的定义了一个元组 `p0`，并在第 13-14 行分别使用 `p0.first` 和 `p0.second` 选择其数据并输出；
* 第 18 行的前半段利用编译时的类型推导功能同样定义了一个元组 `p1`，关键字 `auto` 表示使用类型推导；
* 第 18 行的后半段用 `std::ref()` 制造了一个 `score[1]` 的引用类型，可以注意到在第 22 行对其进行修改后，元组 `p1` 的值也发生了改变，而在第 26 行直接对 `p1.second` 进行修改之后，原始数据 `score[1]` 也随之发生了改变；
* 第 31 行虽然我们对 `names[1]` 的值进行了修改，但是并没有影响到 `p1` 的内部取值。

从上面这个代码中，我们不难推测出一些关于 `pair` 简单的特性。

* `pair` 是一个仅包含**两个元素**的一个模板类，使用时需要通过**显式**的声明或者**隐式**的推导对其进行模板特化，从而制造出我们需要的特定类型，换言之，任何的类型推导错误都会导致其出现编译时错误；
* `pair` 只包含两个数据成员，分别是 `first` 与 `second`，其类型和数据值按照声明时的**代码书写顺序**依次对应；
* `auto` 在进行类型推导时会默认使用**基础类型**，而非指针与引用，只有在初始化时使用了显式的引用（如 `std::ref()`）时才会将类型变更为引用类型。

在有以上思考的基础上，我们就不难阅读 `pair` 的定义声明了。

```cpp
namespace std{
    template<class T1, class T2>
    struct pair
    {
        using first_type = T1;
        using second_type = T2;
        T1 first;
        T2 second;
        pair(const pair&) = default;
        pair(pair&&) = default;
        constexpr pair();
        constexpr pair(const T1& x, const T2& y);
        template<class U, class V> constexpr pair(U&& x, V&& y);
        template<class U, class V> constexpr pair(const pair<U, V>& p);
        template<class U, class V> constexpr pair(pair<U, V>&& p);
        pair& operator=(const pair& p);
        template<class U, class V> pair& operator=(const pair<U, V>& p);
        pair& operator=(pair&& p) noexcept();
        template<class U, class V> pair& operator=(pair<U, V>&& p);
        void swap(pair& p) noexcept();
    };
}
// filename: ch1-pair-defination-simplified.cpp
```

借助这份定义我们进一步的了解一下 `pair` 内建的其他方法。

```cpp
#include <iostream>
#include <string>
#include <utility>
#include <functional>

using namespace std;

template<class T1, class T2>
ostream& operator<<(ostream &out, const pair<T1, T2> &_)
{
    return out << "(" << _.first << ", " << _.second << ")";
}

void display_separator() { cout << "--------" << endl; }

int main()
{
    pair<int, double> p1; // => (0, 0)
    cout << "Default constructor: " << p1 << endl;
    pair<int, double> p2(65, 1.1); // => (65, 1.1)
    cout << "Initialized with two values: " << p2 << endl;
    pair<int, double> p3(p2); // => (65, 1.1)
    cout << "Copy constructor: " << p3 << endl;
    pair<char, int> p4(p2); // => (A, 1)
    cout << "Copy then convert: " << p4 << endl;

    display_separator();

    pair<char, int> p5 = make_pair('B', 2);
    cout << "Before swap: p4 is " << p4 << " , p5 is " << p5 << endl;
    // => Before swap: p4 is (A, 1) , p5 is (B, 2)
    p5.swap(p4);
    cout << "After swap: p4 is " << p4 << " , p5 is " << p5 << endl;
    // => After swap: p4 is (B, 2) , p5 is (A, 1)

    display_separator();

    pair<char, int> p6 = make_pair('B', 1);
    pair<char, int> p7 = make_pair('A', 2);
    cout << "p4 is " << p4 << " , p5 is " << p5 << ", "
         << "p6 is " << p6 << " , p7 is " << p7 << endl;
    // => p4 is (B, 2) , p5 is (A, 1), p6 is (B, 1) , p7 is (A, 2)
    cout << std::boolalpha;
    cout << (p4 == p5) << endl; // => false
    cout << (p4 != p5) << endl; // => true
    cout << (p5 < p7) << endl; // => true
    cout << (p4 < p6) << endl; // => false
    cout << (p5 < p6) << endl; // => true
    cout << (p6 < p7) << endl; // => false
    return 0;
}

// filename: ch1-pair-example-2.cpp
// compile this> g++ ch1-pair-example-2.cpp -o ch1-pair-example-2.exe
```

在这个例子中，有以下几点需要注明。

* 第 18-25 行简单的介绍了 `pair` 的初始化方法，当然也可以用更简单的 `std::make_pair()` 通过赋值来进行初始化；
* 第 32 行使用了内建的 `swap()` 方法，注意该方法只有在两个 `pair` 的类型**完全一致**的时候才能使用，否则会报编译错误；
* 第 44-49 行测试了内置的比较运算符 `operator==()` 和 `operator<()`，从结果可以推测出，比较运算符的工作原理是依次比较 `first` 和 `second` 的大小，具体而言则是去调用所比较数据类型的内建的比较运算符。

总结一下，`pair` 提供了一种操作二元数据组的更为简便的方式，同时由于自带了一些常用的内建方法从而相对于自行组织结构体有了天然的优势。另外 `pair` 也是标准库中所有**键值数据（key-value data）**的默认返回类型，而毫不夸张地说 `pair` 是快速使用标准库的第一把钥匙，熟练地使用它是熟练使用标准库的第一步。

## 包装多个数据的元组 `tuple`

既然两个元素可以用 `pair` 来包装到一起，那么更多的元素能不能用一种类似的数据结构包装在一起呢？这便是更一般的元组 `tuple` 的设计初衷，可以把多个不同数据类型的数据绑定到一个实例上。比如现在我们手上有这三种数据，学生的名字、平时表现评级以及期末考试成绩，那么我们就可以选择使用 `tuple` 来操作我们的数据。具体而言请参考如以下代码所示。

```cpp
#include <iostream>
#include <string>
#include <tuple>
#include <functional>

int main()
{
    std::string names[3] = { "alice", "bob", "carl" };
    char ranks[3] = { 'A', 'B', 'C' };
    int score[3] = { 5, 6, 7 };

    auto student0 = std::make_tuple(names[0], ranks[0], score[0]);
    std::cout << "student0> name: " << std::get<0>(student0)
              << ", rank: " << std::get<1>(student0)
              << ", score: " << std::get<2>(student0) << std::endl;

    std::string hoge;
    char piyo;
    int fuga;
    auto student1 = std::make_tuple(names[1], ranks[1], score[1]);
    std::tie(hoge, piyo, fuga) = student1;
    std::cout << "student1> name: " << hoge << ", rank: " << piyo
              << ", score: " << fuga << std::endl;

    auto student2 = std::tie(names[2], ranks[2], score[2]);
    auto [a, b, c] = student2; // C++17 structured binding
    std::cout << "student2> name: " << a << ", rank: " << b
              << ", score: " << c << std::endl;
    return 0;
}

// filename: ch1-tuple-example-1.cpp
// compile this> g++ ch1-tuple-example-1.cpp -o ch1-tuple-example-1.exe -std=c++17
```

从这份代码我们不难看出，和 `pair` 类似，`tuple` 的组装与拆包任务分别由两个函数来担当，`std::make_tuple()` 创建一个元组，`std::get()` 则获取确定位置的数据。同时注意到还有一个函数 `std::tie()` 既可以组装元组，也可以对一个元组进行拆包。关于 `std::tie()` 与 `std::make_tuple()` 的异同，我们通过下面的例子来一探究竟。

```cpp
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <functional>

using namespace std;

template<class T1, class T2>
ostream& operator<<(ostream &out, const pair<T1, T2> &_)
{
    return out << "(" << _.first << ", " << _.second << ")";
}

void display_separator() { cout << "--------" << endl; }

int main()
{
    string name = "alice";
    char rank = 'A';
    int score = 5;

    auto student = make_tuple(name, rank, score);
    // => name: alice, rank: A, score: 5
    cout << "student> name: " << get<0>(student)
              << ", rank: " << get<1>(student)
              << ", score: " << get<2>(student) << endl;
    cout << "Here, (score, get<2>(student)) is "
         << make_pair(score, get<2>(student)) << endl;
    score += 10;
    cout << "        score   +=  10 => "
         << make_pair(score, get<2>(student)) << endl;
    get<2>(student) += 100;
    cout << "get<2>(student) += 100 => "
         << make_pair(score, get<2>(student)) << endl;
    return 0;
}

// filename: ch1-tuple-example-2.cpp
// compile this> g++ ch1-tuple-example-2.cpp -o ch1-tuple-example-2.exe -std=c++17
```

这份代码的输出如下所示：

```plain
student> name: alice, rank: A, score: 5
Here, (score, get<2>(student)) is (5, 5)
        score   +=  10 => (15, 5)
get<2>(student) += 100 => (15, 105)
```

在这个例子中，我们组装元组时使用的是 `std::make_tuple()` 方法，而获取元素则使用了 `std::get()` 方法。注意到我们在代码的第 30 行和第 33 行分别对外部的数值 `score` 和元组内部的数值 `std::get<2>(student)` 依次进行了变动，并且把这两个值用 `std::make_pair()` 方法将其值打包输出。注意到我们的输出结果里，只有变动的那一项发生了改变。这说明 `std::make_tuple()` 和 `std::make_pair()` 类似，默认新创建一个变量而且直接构建参数的一个引用。和之前类似，如果需要包装一个变量的引用，则额外使用 `std::ref()` 即可。

在充分了解到 `std::make_tuple()` 的这一性质之后，我们通过下一个例子来观察 `std::tie()` 在组装与拆包的表现，同时观察在 C++17 引入的 structured binding 方法与 `std::tie()` 在拆包时候的异同。

```cpp
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <functional>

using namespace std;

template<class T1, class T2>
ostream& operator<<(ostream &out, const pair<T1, T2> &_)
{
    return out << "(" << _.first << ", " << _.second << ")";
}

template<class T1, class T2, class T3>
ostream& operator<<(ostream &out, const tuple<T1, T2, T3> &_)
{
    auto [_1, _2, _3] = _;
    return out << "[" << _1 << ", " << _2 << ", " << _3 << "]";
}

void display_separator() { cout << "--------" << endl; }

int main()
{
    string names[3] = { "alice", "bob", "carl" };
    char ranks[3] = { 'A', 'B', 'C' };
    int score[3] = { 5, 6, 7 };

    int hoge;
    auto student0 = tie(names[0], ranks[0], score[0]);
    // => name: bob, rank: B, score: 6
    tie(std::ignore, std::ignore, hoge) = student0;
    cout << "student0> [hoge, score[0], get<2>(student0)] is "
         << make_tuple(hoge, score[0], get<2>(student0)) << endl;
    hoge += 10;
    cout << "            hoge +=   10 => "
         << make_tuple(hoge, score[0], get<2>(student0)) << endl;
    score[0] += 100;
    cout << "        score[0] +=  100 => "
         << make_tuple(hoge, score[0], get<2>(student0)) << endl;
    get<2>(student0) += 1000;
    cout << "get<2>(student0) += 1000 => "
         << make_tuple(hoge, score[0], get<2>(student0)) << endl;

    display_separator();

    auto student1 = make_tuple(names[1], ranks[1], score[1]);
    // => name: carl, rank: C, score: 7
    auto [_1, _2, piyo] = student1;
    cout << "student1> [piyo, score[1], get<2>(student1)] is "
         << make_tuple(piyo, score[1], get<2>(student1)) << endl;
    piyo += 10;
    cout << "            piyo +=   10 => "
         << make_tuple(piyo, score[1], get<2>(student1)) << endl;
    score[1] += 100;
    cout << "        score[1] +=  100 => "
         << make_tuple(piyo, score[1], get<2>(student1)) << endl;
    get<2>(student1) += 1000;
    cout << "get<2>(student1) += 1000 => "
         << make_tuple(piyo, score[1], get<2>(student1)) << endl;

    display_separator();

    auto student2 = tie(names[2], ranks[2], score[2]);
    // => name: carl, rank: C, score: 7
    auto [_3, _4, fuga] = student2;
    cout << "student2> [fuga, score[2], get<2>(student2)] is "
         << make_tuple(fuga, score[2], get<2>(student2)) << endl;
    fuga += 10;
    cout << "            fuga +=   10 => "
         << make_tuple(fuga, score[2], get<2>(student2)) << endl;
    score[2] += 100;
    cout << "        score[2] +=  100 => "
         << make_tuple(fuga, score[2], get<2>(student2)) << endl;
    get<2>(student2) += 1000;
    cout << "get<2>(student2) += 1000 => "
         << make_tuple(fuga, score[2], get<2>(student2)) << endl;
    return 0;
}

// filename: ch1-tuple-example-3.cpp
// compile this> g++ ch1-tuple-example-3.cpp -o ch1-tuple-example-3.exe -std=c++17
```

这份代码的运行结果如下：

```plain
student0> [hoge, score[0], get<2>(student0)] is [5, 5, 5]
            hoge +=   10 => [15, 5, 5]
        score[0] +=  100 => [15, 105, 105]
get<2>(student0) += 1000 => [15, 1105, 1105]
--------
student1> [piyo, score[1], get<2>(student1)] is [6, 6, 6]
            piyo +=   10 => [16, 6, 6]
        score[1] +=  100 => [16, 106, 6]
get<2>(student1) += 1000 => [16, 106, 1006]
--------
student2> [fuga, score[2], get<2>(student2)] is [7, 7, 7]
            fuga +=   10 => [17, 17, 17]
        score[2] +=  100 => [117, 117, 117]
get<2>(student2) += 1000 => [1117, 1117, 1117]
```

下面我们逐段解析这份代码：

* 第 33 行使用了一个全新的关键字 `std::ignore`，这个关键字的意思是解包时忽略它所在位置的数据，也可以理解为占位符；
* 第 30-44 行展示了第一个例子，注意到在 `student0` 中利用 `std::tie()` 解包得到的变量 `hoge` 并没有与另外两个变量同步，这是因为 `hoge` 并不是引用类型；而另外的两个变量均同步发生了变化，这说明这两个变量的其中一个是另一个的引用类型；从而可以推测出，`std::tie()` 会默认创建原始变量的引用，然后在包装到 `tuple` 里面；
* 第 48-61 行展示了第二个例子，注意到三个变量是完全独立变化的，因此可以推测出利用 `auto [_1, _2, piyo]` 解包出的数据可能是原原本本的复制；
* 第 65-78 行展示了第三个例子，注意到三个变量是完全同步的，原因是 `std::tie()` 构造了原始数据的引用，而且 `auto [_3, _4, fuga]` 解包时同样复制了一份引用出来。

为了验证我们的这一结论，我们接着看下一个例子

```cpp
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <functional>

using namespace std;

void display_separator() { cout << "--------" << endl; }

int main()
{
    string names[4] = { "alice", "bob", "carl", "dell" };
    char ranks[4] = { 'A', 'B', 'C', 'D' };
    int score[4] = { 5, 6, 7, 8 };

    cout << std::boolalpha;

    int hoge;
    auto student0 = tie(names[0], ranks[0], score[0]);
    // => name: bob, rank: B, score: 6
    tie(std::ignore, std::ignore, hoge) = student0;
    cout << "0> " << std::is_same<int, decltype(hoge)>::value;
    cout << ", " << std::is_same<int&, decltype(hoge)>::value << endl;
    
    display_separator();

    auto student1 = make_tuple(names[1], ranks[1], score[1]);
    // => name: carl, rank: C, score: 7
    auto [_1, _2, piyo] = student1;
    cout << "1> " << std::is_same<int, decltype(piyo)>::value;
    cout << ", " << std::is_same<int&, decltype(piyo)>::value << endl;
    
    display_separator();

    auto student2 = tie(names[2], ranks[2], score[2]);
    // => name: carl, rank: C, score: 7
    auto [_3, _4, fuga] = student2;
    cout << "2> " << std::is_same<int, decltype(fuga)>::value;
    cout << ", " << std::is_same<int&, decltype(fuga)>::value << endl;
    
    display_separator();
    
    auto student3 = make_tuple(names[2], ranks[2], std::ref(score[2]));
    // => name: carl, rank: C, score: 7
    auto [_5, _6, pika] = student3;
    cout << "3> " << std::is_same<int, decltype(pika)>::value;
    cout << ", " << std::is_same<int&, decltype(pika)>::value << endl;
    
    display_separator();

    cout << "bonus> "
         << std::is_same<int&, decltype(std::get<2>(student0))>::value << ", "
         << std::is_same<int&, decltype(std::get<2>(student1))>::value << endl;
    return 0;
}

// filename: ch1-tuple-example-4.cpp
// compile this> g++ ch1-tuple-example-4.cpp -o ch1-tuple-example-4.exe -std=c++17
```

这份代码的运行结果如下所示：

```plain
0> true, false
--------
1> true, false
--------
2> false, true
--------
3> false, true
--------
bonus> true, true
```

和之前类似，我们继续来逐行解析这份代码：

* 这个例子我们使用了 C++11 所提供的类型判别机制，这个机制作用在编译期，`std::is_same<U, V>` 判断类型 `U` 和 `V` 是否为同一类型，`decltype()` 则是在编译时解析类型，借助这一套工具我们就能很简单的判断目标变量的具体类型是什么了；
* 第 52-54 行测试了 `std::get()` 的返回类型，注意到不管元组是如何构造的，`std::get()` 的返回类型永远是引用类型，原因是他的返回值不仅可以作为右值进行计算，也可以作为左值进行赋值并且改变其在 `tuple` 中的值，那么在设计上自然需要将其的返回类型设置为一个引用类型；
* 第 19-24 行测试了 `std::tie()` 解包时所解包数据 `hoge` 的返回类型，由于一开始已经声明 `hoge` 为 `int`，所以自然会在判断是否为 `int` 时返回 `ture`;
* 第 28-32 行测试了通过 `std::make_tuple()` 创建的元组在 `auto [_1, _2, piyo]` 解包是的表现，注意到 `piyo` 和上面的例子中的表现相同，这是由于 `std::make_tuple()` 并不会创建引用，所以导致了后续操作里不会出现引用类型了；
* 第 36-40 行测试了通过 `std::tie()` 创建的元组的解包表现，注意到由于 `std::tie()` 创建的是引用，所以后续解包时产生的变量也是引用；
* 第 44-48 行测试了显式构造引用类型的数据的解包表现，根据前面的分析我们不难理解其结果。

最后我们总结一下关于元组的目前已经解析的信息：

* `std::make_tuple()` 以及 `std::tie()` 均可以构造一个 `tuple`，两者的区别在于前者构造复制，后者创建引用，更准确的说法是后者创建一个**左值引用**；
* `std::get()` 会解包出来固定位置的值，其返回值是元组内部值的引用；
* `std::tie()` 会解包出来该元组的所有值，不需要的位置可以用占位符 `std::ignore` 代替，解包数据的数据类型是基本类型而非引用类型；
* C++17 引入的 structured binding 方法 `auto [_1, _2, _3, ...]` 在解包时的表现与 `std::tie()` 类似。

与 `pair` 类似，`tuple` 提供了一种更为简便的包装数据的方法，和 `pair` 一样不限制所包装的数据的类型，同时不限制所包装的数据的个数。我们甚至可以把它看做一个特殊的列表。基于这样的认识，我们再来认识这三个工具 `std::tuple_cat()`，`std::tuple_size()` 以及 `std::tuple_element`。注意，以下内容均在不同程度上涉及了模板编程的知识，初学者可以先行放弃待积累了一定的模板编程经验之后再重新学习本小节的知识。

### 以下内容初学者可跳过

首先转变观念，把 `tuple` 看做一种特殊的要求每一个元素的数据类型的不定长列表。基于这样的考虑，我们先给自己设置一个目标，实现两个元组的拼接操作。拼接其实并不难，因为如果已知两个元组内部的元素之后只需要提取出来然后 `std::make_tuple()` 就行了。这个问题的难点就在于，由于 C++ 要求所有的变量类型必须在编译时声明，所以如果要实现两个元组的拼接就需要对它们分别进行解包操作，同时还需要声明数据类型。所以现在我们关注这样一个问题，给定一个元组，能否实现以下的三种操作：自动解包，得到元组内部元素的个数，得到确定某一元素的数据类型。这时需要注意，刚刚所提到的三种操作实际上只是一种操作。得到元组内部元素的个数，等价于自动解包时把元组内部的每一个元素都看作自然数 1 然后把解包操作修改为自然数的加法。而得到确定某一元素的数据类型则更为方便，我们只需要在某一个位置上应用一次 `decltype` 就大功告成了。所以基于以上的思路，我们的任务只有一个，就是如何自动分解元组。

这里一种很简单的方法就是递归处理，每一次把当前元组分解为两部分，当前元祖的头部（第一个元素）和剩余部分，之后再对剩余部分进行处理，直到剩余部分是一个只包含一个元素的元组为止。基于这一思路，我们可以写出下面所示的简单代码。

```cpp
#include <iostream>
#include <string>
#include <tuple>

template<class Func, class Tuple, int N>
struct TupleHelper
{
    static void func(Func f, const Tuple &_)
    {
        TupleHelper<Func, Tuple, N - 1>::func(f, _);
        f(std::get<N - 1>(_), N - 1);
    }
};

template<class Func, class Tuple>
struct TupleHelper<Func, Tuple, 1>
{
    static void func(Func f, const Tuple &_)
    {
        f(std::get<0>(_), 0);
    }
};

template<class Func, class ...Args>
void manipulate_tuple(Func f, const std::tuple<Args...> &_)
{
    TupleHelper<Func, decltype(_), sizeof...(Args)>::func(f, _);
}

template<class ...Args>
void print_tuple(const std::tuple<Args...> &_)
{
    std::cout << "(";
    manipulate_tuple(
        [](auto _, std::size_t idx)
        {
            if (idx != 0) std::cout << ", ";
            std::cout << _;
        }, _);
    std::cout << ")";
}

int main()
{
    print_tuple(std::make_tuple(10, 1.5, 'A'));
    // => (10, 1.5, A)
    return 0;
}
// filename: ch1-tuple-helper.cpp
// compile this> g++ ch1-tuple-helper.cpp -o ch1-tuple-helper.exe -std=c++14
```

在这份代码中，我们实现了一个对 `tuple` 内部的所有元素都进行一次 `Func` 操作的 `TupleHelper` 工具。在第 5 行的模板声明中， `class Func` 被假设为一个接受元组内元素和所在位置 `idx` 这两个参数的函数类型，`int N` 则表示这个元组的长度。在这里我们使用的是递归处理的方法，所以在第 15-22 行我们对模板进行了特化处理。另外我们还将 `TupleHelper` 封装到了函数 `manipulate_tuple` 当中，利用 `decltype` 获取元组的具体类型，用变长模板参数获取 `tuple` 的内部所有元素的个数。一切准备就绪之后就交给编译器处理就行了，因为模板的所有操作都是在编译时完成的。注意这个 `TupleHelper` 并不是 bug free 的，因为没有处理空元组 `tuple<>` 的情况，具体怎么修改就仁者见仁智者见智了。

既然了解了如何自动解包，那么我们完全可以独立的借助刚才的组件自行开发一个完成 `tuple` 合并操作的工具。不过，如果你已经充分理解了借助模板来递归分解元组的这个操作，你完全可以开始使用标准库所提供的三个组件 `std::tuple_cat()`，`std::tuple_size()` 以及 `std::tuple_element` 来构建自己的程序，因为事到如今你自己就会感慨，这三者的实现并不见得会有多么高深莫测。

让我们回到上一份例子中，为了简便起见我们把上一份代码的第 5-41 行封装进 `tuple_utils.hpp` 当中，同时包装到命名空间 `__utils` 以防止与之后的内容混淆。封装后的代码如下：

```cpp
// filename: tuple_utils.hpp
#ifndef __TUPLE_UTILS_HPP__
#define __TUPLE_UTILS_HPP__

namespace __utils{

/// same as ch1-tuple-helper.cpp

template<class Func>
void manipulate_tuple(Func f, const std::tuple<> &_)
{
    std::cerr << "Warning! Manipulating an EMPTY tuple.";
};

} // namespace __utils
#endif // __TUPLE_UTILS_HPP__
```

在拥有了一个封装好的工具盒之后，我们来看看标准库所提供的剩下的三个工具的作用。

```cpp
#include <iostream>
#include <tuple>
#include <utility>

#include "tuple_utils.hpp"

int main()
{
    auto t1 = std::make_tuple(10, "abc", 'B', 32);
    int n = 11;
    auto t2 = std::make_tuple("bcd", 'C', 64);
    auto t3 = std::tuple_cat(t1, std::tie(n), t2, std::make_pair("abc", "def"));
    n = 16;
    __utils::print_tuple(t3); std::cout << std::endl;
    // => (10, abc, B, 32, 16, bcd, C, 64, abc, def)

    std::cout << std::tuple_size<decltype(t3)>::value << std::endl;
    // => 10

    std::tuple_element<1, decltype(t3)>::type hoge = "test";
    std::cout << hoge << std::endl; // => "test"

    std::tuple<> t_empty;
    __utils::print_tuple(t_empty);
    // => (Warning! Manipulating an EMPTY tuple.)
    return 0;
}

// filename: ch1-tuple-tools.cpp
// compile this> g++ ch1-tuple-tools.cpp -o ch1-tuple-tools.exe -std=c++14
```

第 12 行测试了 `std::tuple_cat()` 方法，该方法会合并多个 `tuple` 同时保持他们的引用关系。那么既然要保持内部的引用关系，想必在解包时传递的参数应当是一个左值，这样就保证了后续的引用操作依旧有效。第 17 行和第 19 行测试了 `std::tuple_size()` 和 `std::tuple_element()` 方法，分别获取元组内部数据的个数以及指定位置数据类型。注意到这两个方法都是在**编译时**生效的，所以相比较于 `python` 的列表还是逊色了不少。代码的第 23 行测试了我们的 `__utils::print_tuple()` 面对空 `tuple` 的处理方法，对应的代码在 `tuple_utils.hpp` 的第 26-30 行。

既然已经把 `tuple` 看做一种列表了，那我们索性把超纲进行到底。初学 Haskell 的时候大家都遇见过关于列表的四个函数，函数 `head` 返回列表的头部，也就是第一个元素，`tail` 返回列表除去头部的剩下部分，`last` 返回列表的最后一个元素，`init` 返回列表除去最后一个元素的剩余部分。

```haskell
head [5, 4, 3, 2, 1] -- => 5
tail [5, 4, 3, 2, 1] -- => [4, 3, 2, 1]
init [5, 4, 3, 2, 1] -- => [5, 4, 3, 2]
last [5, 4, 3, 2, 1] -- => 1
```

那么如何用 C++ 来实现这四种功能呢？第一种方法就是我们之前提到的递归分解，把大化小，最后在组装回去。不过，如果对变长模板更熟悉一些，我们可以直接在编译期做更多的事情。我们先来看一下 `head` 与 `tail` 这两个函数的实现：

```cpp
#include <iostream>
#include <tuple>
#include <utility>

#include "tuple_utils.hpp"

template<typename T, typename ...Ts>
auto head(std::tuple<T, Ts...> t)
{
    return std::get<0>(t);
}

template<std::size_t ...Ns, typename ...Ts>
auto tail_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t)
{
    return std::make_tuple(std::get<Ns + 1u>(t)...);
}

template <typename ...Ts>
auto tail(std::tuple<Ts...> t)
{
    return tail_impl(std::make_index_sequence<sizeof...(Ts) - 1u>() , t);
}

int main()
{
    auto t = std::make_tuple(2, 3.14, 'c');
    std::cout << head(t) << std::endl; // => 2
    __utils::print_tuple(tail(t)); std::cout << std::endl; // => (3.14, 'c')
    return 0;
}

// filename: ch1-tuple-head-and-tail.cpp
// compile this> g++ ch1-tuple-head-and-tail.cpp -o ch1-tuple-head-and-tail.exe -std=c++14
```

这份代码中我们又借助了一个强大的 C++14 的**编译时**工具 `std::index_sequence`，它表示一串只在**编译期**生效的整数数列。不过一般而言更常用的是 `std::make_index_sequence`，他会在**编译期**生成一串 `0, 1, 2, ...` 这样的数列，这样就更加方便了我们对 `tuple` 的读写操作。比如我们现在需要实现 `tail` 函数的操作，除了第一项以外把剩下的 N-1 个数据返回成 `tuple`，我们就可以先用 `std::make_index_sequence` 制造一个长度为 N-1 的 `0, 1, 2, ...` 数列，然后对每一项 +1 就正好对应上了我们所要的剩下的元素的位置，最后用 `std::get()` 获取元素，顺便在外层包装上 `std::make_tuple()` 即可。既然已经看到了 `std::make_index_sequence` 这个工具，那么就不妨来试一下实现 `init` 和 `last` 函数吧。

```cpp
#include <iostream>
#include <tuple>
#include <utility>

#include "tuple_utils.hpp"

template<std::size_t ...Ns, typename ...Ts>
auto init_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t)
{
    return std::make_tuple(std::get<Ns>(t)...);
}

template<typename ...Ts>
auto init(std::tuple<Ts...> t)
{
    return init_impl(std::make_index_sequence<sizeof...(Ts) - 1u>() , t);
}

template<typename ...Ts>
auto last(std::tuple<Ts...> t)
{
    return std::get<sizeof...(Ts) - 1u>(t);
}

template <class T>
auto size(T t)
{
    return std::tuple_size<T>::value;
}

template <class T>
auto last2(T t)
{
    return std::get<std::tuple_size<T>::value - 1u>(t);
}

int main()
{
    auto t = std::make_tuple(2, 3.14, 'c');
    std::cout << size(t) << std::endl; // => 3
    std::cout << last(t) << std::endl; // => 'c'
    std::cout << last2(t) << std::endl; // => 'c'
    __utils::print_tuple(init(t)); std::cout << std::endl; // => (2, 3.14)
    return 0;
}

// filename: ch1-tuple-init-and-last.cpp
// compile this> g++ ch1-tuple-init-and-last.cpp -o ch1-tuple-init-and-last.exe -std=c++14
```

最后给一个思考题，如果我把上一份代码中 `last2()` 的实现写做

```cpp
template <class T> auto last2(T t)
{
    return std::get<size<T>(t) - 1u>(t);
}
```

这份代码能运行吗？为什么？

先回答这个问题，显然是不可以的。由于模板要求在编译时必须提供给他常量，而 `size()` 函数并不是一个常量，所以在编译时会报错告诉你编译器需要一个常量。当然你也可以用 `#define` 把 `size()` 变成一个宏定义函数，这样确实简单粗暴。不过如果我们知道 C++11 已经提供了常量表达式这一特性之后，这个问题就迎刃而解了。之前我们所接触的 `const` 更多的是指**运行时常量性**，其实更准确的理解方式是**构造运行时的只读变量**。而很多时候我们真正需要的是**编译时**的常量性，比如我们都知道 `size()` 会返回一个**编译时**就**确定的**同时也**不会改变的**值，但是在 C++11 标准出现之前，编译器却不买账。不过现在，我们只需要对 `size()` 函数进行常量表达式修饰就可以解决这个难题。简单地说，这里需要用到 C++11 新给出的关键字 `constexpr` 去修饰函数，同时也需要保证这个函数必须有一个返回值，而且必须是一个常量表达式。具体参考下面的代码：

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
