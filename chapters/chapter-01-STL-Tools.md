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
* `pair` 只包含两个数据成员，分别是 `first` 与 `second`，其类型和数据值按照声明时的**代码书写顺序**依次对应，注意到这个顺序并非我们所熟悉的**C++中函数参数压栈**顺序；
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





## 基础序列式容器 `vector`

一般而言我们把一组可以排序的（但是未必是有序的）数据称之为序列式的数据，自然“盛放”这些数据的容器，也被称为序列式容器。STL 提供了很多简单轻巧的容器，包含典型的序列式容器如向量 `vector`，链表 `list`，双端队列 `deque`，以及其他的关联式容器如集合 `set`，映射表 `map`，散列表 `hash_set` 等等。
