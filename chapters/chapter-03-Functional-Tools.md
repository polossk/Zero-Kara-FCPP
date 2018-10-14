函数式编程工具箱
========================================

## 把函数看做一等公民

上一节中介绍了 lambda 函数，同时也开拓了一种全新的编程思路，即把函数作为一等公民来对待。那么我们反复说的这个一等公民到底是什么样的意思呢？

传统的 C++ 把承载了数据的变量作为一等公民，这些变量享有以下权利：

* 可以承载数据，也可以存在于更高级的数据结构中；
* 可以作为右值参与计算、函数调用；
* 可以作为左值参与赋值操作。

那么，如果把函数也当做一等公民，那么，函数也应当享有以下的权利：

* 函数就是数据，一个函数变量可以承载函数，也可以存在于更高级的数据结构中；
* 可以作为右值参与计算，或参与更高级的函数调用；
* 可以作为左值参与赋值操作。

为了更方便理解，我们介绍这样一个例子。假设我们想要实现一个简单的计算器，实现实数的加减乘除与乘方运算。一种最直接的想法是通过 if-else 判断运算符，然后把参数转发给对应的函数。现在我们也可以这样写：

```cpp
#include <iostream>
#include <functional>
#include <cmath>
#include <map>

using std::cout;
using std::endl;

int main()
{
    std::map<char, std::function<double(double, double)> > host;
    host['+'] = [](double a, double b)->double{ return a + b; };
    host['-'] = [](double a, double b)->double{ return a - b; };
    host['*'] = [](double a, double b)->double{ return a * b; };
    host['/'] = [](double a, double b)->double{ return a / b; };
    host['^'] = [](double a, double b)->double{ return pow(a, b); };
    cout << "1.1 + 2.2= " << host['+'](1.1, 2.2) << endl; // => 3.3
    cout << "1.1 - 2.2= " << host['-'](1.1, 2.2) << endl; // => -1.1
    cout << "1.1 * 2.2= " << host['*'](1.1, 2.2) << endl; // => 2.42
    cout << "1.1 / 2.2= " << host['/'](1.1, 2.2) << endl; // => 0.5
    cout << "1.1 ^ 2.2= " << host['^'](1.1, 2.2) << endl; // => 1.23329
    return 0;
}

// filename: ch3-citizens-example.cpp
// compile this> g++ ch3-citizens-example.cpp -o ch3-citizens-example.exe
```

在这个例子里，我们使用了标准库所提供的 `std::map` 这一数据结构来存储我们的核心匹配关系，即一个运算符对应一个运算关系。`map` 所储存的数据现在不再是实实在在的数字，而是一个具体的函数，一种具体的运算方法。这便是刚才所提到的公民权利的第一条和第三条。那么第二条，作为右值参与计算或参与更高级的函数调用，又应该怎么解释呢。这就要提到所有生成自己支持函数式编程或类函数式编程的最基础的三个工具了：map，filter，fold。

## 最基础的三个工具

不同的编程语言对这三个方法有不同的名字，但是本质还是一样的。map，filter，fold 分别表示对集合而言的三种操作：映射，筛选，折叠。下面我们依次介绍一遍。

### 映射操作

映射操作顾名思义就是对一个列表 `vec` 内的所有元素依照某种运算法则 `f` 进行运算，从而得到一个全新的列表 `[f(x) for x in vec]`。有人一定会说，这不就是一句 `for` 循环的事么。确实，事实上 `map` 不论是用递归定义实现的也好，还是用普通的循环实现的已好，从代码的逻辑上讲他确确实实是一次基于列表的循环操作。但是这里需要额外说明的是，我们在提到“函数式编程”的时候，会反复强调甚至是反复洗脑两个原则。其一是 DRY 原则，“Don't Repeat Yourself”，一个组件有且只有一次实现，绝不额外的特化过多的实现。其二是重视逻辑推演而非执行过程，程序员的任务不再是既需要考虑怎么算又需要考虑怎么写，而是更侧重怎么算，怎么通过逻辑上的组合、推演，来完备的定义问题的解，至于执行过程则交给计算机本身即可。所以虽然结果一样，但是思考问题的角度和解决问题的道路并不完全一样。

STL 中提供了两个工具来实现这一操作，`std::for_each` 更像是一个没有下标的 `for` 循环而且写起来也不那么友善，`std::transform` 相对而言就友善了一些，甚至可以支持两个列表同时计算（你甚至不需要提前 zip 两个列表）。为什么我会对 `for_each` 带有偏见而 `transform` 则态度不同呢，我们先看这样一个例子。

```cpp
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>

using std::cout;
using std::endl;

int main()
{
    std::vector<int> a = { 97, 98, 99, 100, 101, 102, 103 };
    for (auto e : a) cout << " " << e;
    cout << endl; // => 97 98 99 100 101 102 103
    std::for_each(a.begin(), a.end(), [](const int &_){ cout << " " << _; });
    cout << endl; // => 97 98 99 100 101 102 103
    std::vector<int> b, c(10, 122);
    std::transform(a.begin(), a.end(), std::back_inserter(b),
        [](int _)->int{ return _ + 3; });
    std::transform(a.begin(), a.end(), c.begin() + 1,
        [](int _)->int{ return _ + 7;});
    std::vector<char> d, e;
    std::transform(a.begin(), a.end(), c.begin(), std::back_inserter(d),
        [](int _1, int _2){ return std::toupper(char(std::max(_1, _2))); });
    std::transform(c.begin(), c.end(), a.begin(), std::back_inserter(e),
        [](int _1, int _2){ return std::toupper(char(std::min(_1, _2))); });

    auto print_char = [](auto v){ for (auto e : v) cout << char(e); cout << endl; };
    auto print_value = [](auto v){ for (auto e : v) cout << " " << int(e); cout << endl; };
    #define DISPLAY_CHAR(v) do { cout << #v ": "; print_char(v); } while (0);
    #define DISPLAY_VALUE(v) do { cout << #v ":"; print_value(v); } while (0);
    DISPLAY_CHAR(a); // => a: abcdefg
    DISPLAY_CHAR(b); // => b: defghij
    DISPLAY_CHAR(c); // => c: zhijklmnzz
    DISPLAY_CHAR(d); // => d: ZHIJKLM
    DISPLAY_VALUE(d); // => d: 90 72 73 74 75 76 77
    DISPLAY_CHAR(e); // => e: ZHIJKLM
    DISPLAY_VALUE(e); // => e: 65 66 67 68 69 70 71 0 0 0
    #undef DISPLAY_CHAR
    #undef DISPLAY_VALUE
    return 0;
}

// filename: ch3-transform-example.cpp
// compile this> g++ ch3-transform-example.cpp -o ch3-transform-example.exe
```

我们首先分析 `for_each`。注意第 12 行和第 14 行，`for_each` 不仅写法比 C++11 的按范围循环的 `for` 复杂（需要提供迭代器表示起点终点），而且需要提供一个函数来做应用，更关键的是，`for_each` 保证这个代码按照从 `begin` 到 `end` 的顺序依次应用。那么它和 `for` 循环相比而言可以说优势全无，不仅写法麻烦，而且毫无新意。但是需要注意的一点是，正因为 `for_each` 保证了在执行时是依次执行的，同时，`for_each` 并不要求一定有一个返回值。所以这使得它可以完成 `transform` 不能完成的任务。`for_each` 是标准库中提供的，可以应用一个非纯函数的工具。纯函数是指那些，没有副作用，没有后继的状态变化，对于相同的输入总会反馈相同的结果的一类特殊函数。某种意义上，函数式编程要求，或者尽量要求大家使用纯函数从而方便测试（反馈唯一）、维护（没有副作用）、自动并行化（没有后继的状态变化）。但是这并不影响在我们初学的时候借助一些特殊的函数帮助我们学习与理解这一全新的范式。比如我们可以借助 `for_each` 实现这种另类的求和：

```cpp
#include <iostream>
#include <algorithm>
#include <vector>

using std::cout;
using std::endl;

int main()
{
    std::vector<int> a = { 1, 2, 3, 4 };
    int sum = 0, idx = 1, sum2 = 0;
    auto add_to_sum = [&sum](int _){ sum += _; };
    auto add_to_sum_with_idx = [&sum2, &idx](int _){ sum2 += _ * idx++; };
    for (auto e : a) cout << " " << e;
    cout << endl; // => 1 2 3 4
    for_each(a.begin(), a.end(), add_to_sum);
    cout << sum << endl; // => 10
    for_each(a.begin(), a.end(), add_to_sum_with_idx);
    cout << sum2 << endl; // => 30
    return 0;
}

// filename: ch3-foreach-example.cpp
// compile this> g++ ch3-foreach-example.cpp -o ch3-foreach-example.exe
```

上一节提到了， lambda 函数可以通过引用传递从而把数据带进函数内部，函数内部的修改同时也会影响外部的数据。所以在这个例子中，`sum` 和 `sum2` 用来记录求和的结果，`idx` 则相当于提供了随着 `for_each` 执行时的一个下标变量。所以自然地，第一个 `sum` 就是计算了 `1 + 2 + 3 + 4` 的结果，而 `sum2` 则相当于计算了 `1 * 1 + 2 * 2 + 3 * 3 + 4 * 4` 的结果。这就是一个简单的非纯函数的例子。

回到之前的 `transform`，刚才既然提到了非纯函数和按序执行这两条，那么有必要对 `transform` 进行一番额外解释。首先就是按序执行这一条。这一条是不被保证的。至于函数到底是纯函数还是非纯函数这一点，`transform` 只要求函数不对迭代器做手脚（比如delete）或者修改范围内部的元素（比如把下一个元素的值+1）。同时，`transform` 要求有返回值，每执行一次都会把结果存储在一个新的列表中。`transform` 允许同时接受两个列表，但是此时要求后者的长度**必须大于等于**前者的长度，否则会产生未定义行为（Undefined Behavior，下文用 ub 代替），因为没有规定说一个不存在的迭代器位置的值应当是多少。基于这些理解，再来观察刚才的例子就十分容易了。其中，第 29-30 行定义了两个宏来辅助输出，并且在第 31-37 行集中展示了我们的 4 个例子。注意到由于最后一个例子是 ub 所以不同的平台的运行结果可能不同，而其他的例子中我们所用到的都是纯函数应用，即没有副作用，没有后继的状态变化，对于相同的输入总会反馈相同的结果。当然为了方便展示，我们把每一次的运行结果都存放到了一个新的变量中，实际上 `transform` 也可以实现就地操作，只需要把输出的迭代器位置变换成自己的 `begin` 就行了。

### 筛选操作

映射操作是所有操作的根基，因为所有的操作都可以用映射操作进行解释，筛选与折叠也不例外。筛选，顾名思义，对一个列表内的所有元素应用一个谓词（predicate，谓词特指判断事物返回True或False的函数），然后取出所有结果为 True 的元素。简单地说就是 for 循环加 if-else 判断。

STL 提供了两个比较类似的函数用来实现筛选这一操作，`copy_if` 和 `remove_if`。从字面上我们就可以知道，前者是把满足条件的给拷贝走，后者是把满足条件的删除。那么具体的使用方法我们还是结合例子来看：

```cpp
#include <iostream>
#include <algorithm>
#include <vector>

using std::cout;
using std::endl;

int main()
{
    auto print_value = [](auto v){ for (auto e : v) cout << " " << int(e); cout << endl; };
    #define DISPLAY_VALUE(v) do { cout << #v ":"; print_value(v); } while (0);
    std::vector<int> a = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::vector<int> b;
    std::copy_if(a.begin(), a.end(), std::back_inserter(b),
        [](int _){ return _ % 2; });
    DISPLAY_VALUE(a); // => a: 1 2 3 4 5 6 7 8 9 10
    DISPLAY_VALUE(b); // => b: 1 3 5 7 9
    a.erase(std::remove_if(a.begin(), a.end(), [](int _){ return _ % 2; }),
        a.end());
    DISPLAY_VALUE(a); // => a: 2 4 6 8 10
    #undef DISPLAY_VALUE
    return 0;
}

// filename: ch3-filter-example.cpp
// compile this> g++ ch3-filter-example.cpp -o ch3-filter-example.exe
```

在这里需要额外注意的是，`copy_if` 需要额外开一个空间出来，而 `remove_if` 则返回了需要删除掉的元素的迭代器，然后再通过 `erase` 方法彻底删除元素。与 `transform` 类似，这里的谓词函数同样要求不得修改原始的数据，最好有 `const &` 修饰。

### 折叠操作

折叠操作是指，对一个列表和一个初始值应用一个二元函数，然后依次按某一特定方向将整个列表折叠起来的操作。折叠一开始时，会取列表里规则所规定的特定方向的第一个元素和初始值来应用二元函数，把得到的返回值作为新的累加值。然后用新的累加值去和列表剩下元素的第一个元素来应用二元函数，直到整个列表都被遍历完毕。比如求和函数就是最简单的折叠函数：初始值为 0，二元函数是加法运算，方向则无所谓（加法满足交换律，所以既可以从前往后加也可以从后往前加）。求列表的长度则是一个较复杂的折叠操作：先把列表的元素全部映射成 1，然后求和。把列表翻转也是一个折叠操作：初始值为空列表，二元函数还是列表的元素插入，只不过需要把之前的结果拼接到后面。这种例子数不胜数，但是我们还是从最基本的工具开始学习。

STL 提供了两个折叠函数，一个是 `accumulate`，另一个是 `inner_product`。两者都是左折叠函数，即从头到尾进行折叠。从名字里我们就可以知道，两个分别是执行求和和计算向量内积的函数，而后者实际上是两层应用：先对两个列表的元素应用一个二元运算（默认的是乘法）把结果存入一个列表当中，再对这个列表折叠（默认求和）。所以后者就不多加以介绍了（等价于 `accumulate` 和 `transform` 的二次封装）。而前者 `accumulate` 我们还是通过例子来观察一下：

```cpp
#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

using std::cout;
using std::endl;

int main()
{
    auto print_value = [](auto v){ for (auto e : v) cout << " " << int(e); cout << endl; };
    #define DISPLAY_VALUE(v) do { cout << #v ":"; print_value(v); } while (0);
    std::vector<int> a = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int sum = std::accumulate(a.begin(), a.end(), 0,
        [](int acc, int _){ return acc + _; });
    cout << sum << ", "; DISPLAY_VALUE(a);
    // => 55, a: 1 2 3 4 5 6 7 8 9 10

    int prod = std::accumulate(a.begin(), a.end(), 1,
        [](int acc, int _){ return acc * _; });
    cout << prod << ", "; DISPLAY_VALUE(a);
    // => 3628800 a: 1 2 3 4 5 6 7 8 9 10

    std::vector<int> a_re = std::accumulate(a.begin(), a.end(), std::vector<int>(),
        [](std::vector<int> acc, int _)->std::vector<int>{
            std::vector<int> __; __.push_back(_);
            std::copy(acc.begin(), acc.end(), std::back_inserter(__));
            return __;
        });
    DISPLAY_VALUE(a_re); // => a_re: 10 9 8 7 6 5 4 3 2 1
    #undef DISPLAY_VALUE
    return 0;
}

// filename: ch3-accumulate-example.cpp
// compile this> g++ ch3-accumulate-example.cpp -o ch3-accumulate-example.exe
```

这里选择了两个最常见的例子，求和与求积，注意到同样所有的函数不得对原始元素修改，最好使用 `const &` 进行修饰。第三个例子是一个简单的用左折叠的思路对列表进行反序的结果。简单地讲，所应用的二元函数的第一个参数是累加值，第二个元素是列表中取出的值，返回值的类型应当与初值和累加值的类型相同，所以第三个例子中我显式的声明了所有的类型（自己熟练之后就可以用 `auto` 代替了）。

总结一下，至此我们已经成功的把函数作为一个特殊的变量使用，不仅可以给函数赋值，同时可以把函数作为参数去调用更高级的函数。另外，一但自己的思路打开，重点关注代码的逻辑关系而非逻辑实现，可以大量的减少不必要的思维时间，从而更快速，更敏捷的思考问题，把握重点。最后需要注意的是，由于 C++ 依旧是一个基于**面向过程编程**和**面向对象编程**所设计出的语言，所以在函数式编程上的优化可能不如函数式语言更为专业。而且由于 C++ 会按语句依次执行，所以如果我们大量的堆叠 `transform` 语句会不会造成性能的下降呢？这就是之后将要引入的内容：是否需要引入简单的lazy求值，以及简单的柯里化操作。
