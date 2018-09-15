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
    std::pair<std::string, int> p1 = std::make_pair(names[1], score[1]);
    std::cout << "The value of p1 is "
              << "(" << p1.first << ", " << p1.second << ")\n";
    // => The value of p1 is (bob, 6)

    // build a pair from string and int
    auto p2 = std::make_pair(names[1], std::ref(score[1]));
    std::cout << "Before modification, the value of p2 is "
              << "(" << p2.first << ", " << p2.second << ")\n";
    // => Before modification, the value of p2 is (bob, 6)
    score[1] += 10;
    std::cout << "After modification, the value of p2 is "
              << "(" << p2.first << ", " << p2.second << ")\n";
    // => After modification, the value of p2 is (bob, 16)
    names[1] += "bi";
    std::cout << "names[1] = " << names[1]
              << ", p2.first = " << p2.first << "\n";
    // => names[1] = bobbi, p2.first = bob
    return 0;
}

// filename: ch1-pair-example-1.cpp
// compile this> g++ ch1-pair-example-1.cpp -o ch1-pair-example-1.exe
```

在这份代码当中，有以下几点需要额外注明：

* 第 3-4 行添加了必要的头文件，其中 `std::pair<>` 以及 `std::make_pair()` 在头文件 `<utility>` 当中，`std::ref()` 在头文件 `<functional>` 当中；
* 第 8-9 行预定义了一些数据，第 12 行显式的定义了一个元组 `p1`，并在第 13-14 行分别使用 `p1.first` 和 `p1.second` 选择其数据并输出；
* 第 18 行的前半段利用编译时的类型推导功能同样定义了一个元组 `p2`，关键字 `auto` 表示使用类型推导；
* 第 18 行的后半段用 `std::ref()` 制造了一个 `score[1]` 的引用类型，可以注意到在第 22 行对其进行修改后，元组 `p2` 的值也发生了改变；
* 第 26 行虽然我们对 `names[1]` 的值进行了修改，但是并没有影响到 `p2` 的内部取值。

从上面这个代码中，我们不难推测出一些关于 `pair` 简单的特性。

* `pair` 是一个仅包含**两个元素**的一个模板类，使用时需要通过**显式**的声明或者**隐式**的推导对其进行模板特化，从而制造出我们需要的特定类型，换言之，任何的类型推导错误都会导致其出现编译时错误；
* `pair` 只包含两个数据成员，分别是 `first` 与 `second`，其类型和数据值按照声明时的**代码书写顺序**依次对应，注意到这个顺序并非我们所熟悉的**C++中函数参数压栈**顺序；
* `auto` 在进行类型推导时会默认使用**基础类型**，而非指针与引用，只有在初始化时使用了显式的引用（如 `std::ref()`）时才会将类型变更为引用类型。

在有以上思考的基础上，我们就不难阅读 `pair` 的定义声明了。



## 包装多个数据的元组 `tuple`


## 基础序列式容器 `vector`

一般而言我们把一组可以排序的（但是未必是有序的）数据称之为序列式的数据，自然“盛放”这些数据的容器，也被称为序列式容器。STL 提供了很多简单轻巧的容器，包含典型的序列式容器如向量 `vector`，链表 `list`，双端队列 `deque`，以及其他的关联式容器如集合 `set`，映射表 `map`，散列表 `hash_set` 等等。
