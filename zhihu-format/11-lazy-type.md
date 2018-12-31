惰性求值
========================================

惰性求值（Lazy Evaluation）是相对常用的编程语言中的及早求值而言的另一种求值策略，也被称之为按需调用（call-by-need），或者叫延时求值。简单地讲，惰性求值是在谋求一种平衡，一种在节省开发与节约计算资源的一种平衡策略。一个庞大的类实例可能一次只有一小部分会发生更改，如果把其他的东西都盲目的添加进来，就会额外造成不少的计算资源的浪费。因此，在开发时，开发人员不仅要知道高级语言的语法糖，也需要一定的底层 AST 的实现原理，这样能够避免很多不必要的运行时开销。所以，这里的惰性，更多的是指等待的意思：一旦等到了明确的调用命令，自然会把运行结果正确送出。

那么既然提到了惰性求值，我们先举一个简单的例子来说明。很多人都会用无穷列表来作为惰性求值的第一个例子，我觉得这个例子本身是十分恰当的，你不可能计算一个无穷长度的列表，但是你可以 `take` 或 `drop` 列表前头的东西。不过这个例子在 Haskell 里很容易实现，也很容易讲解，在 C++ 里就颇有些难度：首先得有一定的模板元编程基础，其次还要了解一些 Haskell 求值策略和包装方法（thunk），最后还得重写一套类似的工具来实现这套服务。仅从认识惰性求值而言，这样未免过于头重脚轻。所以我们用一个更简单的例子来进行说明：单一元素的四则运算。

### lazy wrapper

一个变量的主要操作无非两种：初始化和之后的调用。那么既然提到了，惰性求值的内容在于等待，等待明确的调用命令。那么初始化算不算一条明确的调用命令呢？在这里，我认为是不算的。初始化更多的是 policy，你必须遵守初始化的方案，但是如果一个变量自始至终没人调用，你遵不遵守是无所谓的。也就是说，我可以平时啥事都不做，但是把 policy 牢记在心；一旦需要我了，我再去执行这个初始化策略就行了。

更明确地说，我们的惰性变量至少需要三个属性域：初始化策略，真实的值，以及是否完成了初始化的标记。为了表示方便，我们用一个泛型的类，来对元素进行一次打包（wrapper）。

```cpp
template <typename T> class lazy
{
    T value_;
    std::function<T()> policy_;
    bool initialized_;
}
```

注意到我们的 policy 使用了一个 `std::function<T()>` 的函数类型，即要求返回一个 `T` 类型的数据，而且不得有任何参数列表。当然这里只是为了图一个方便，而且参数列表这个事情，我们可以用其他技术来规避掉，这里稍微放下不提。

那么如何在 C++ 这种严格的及早求值的语言中，实现这种操作呢？当然是直接在*调用*上做文章了！也正因为 C++ 是严格的及早求值语言，所以我们只需要在执行*调用*的时候，加上几条语句，判断下是否执行过初始化操作，若是执行了初始化操作，就把初始化后的值放行，反之就执行初始化再放行。

```cpp
template <typename T>
T& lazy::get_value()
{
    if (!initialized_)
    {
        value_ = policy_();
        initialized_ = true;
    }
    return value_;
}
```

最后就是选择一个合适的运算符重载一下就好了。在这里我选择的是函数调用的运算符 `operator()` 以突出调用之意。同时还需要保证一个类型转换函数，即可以通过显式类型转换来把打包的数据类型进行解包。所以我们大致上可以写一个这样的东西出来。

```cpp
template <typename T> class lazy
{
private:
    T value_;
    std::function<T()> policy_;
    bool initialized_;
    static T default_policy()
    {
        throw std::runtime_error("No lazy initiation policy given.");
    }
public:
    lazy() : policy_(default_policy), initialized_(false) {}
    lazy(std::function<T()> _) : policy_(_), initialized_(false) {}
    lazy(const lazy<T>& _) : policy_(_.policy_), initialized_(false) {}

    lazy<T>& operator=(const lazy<T>& _)
    {
        policy_ = _.policy_;
        initialized_ = false;
        return *this;
    }

    T& get_value()
    {
        if (!initialized_)
        {
            value_ = policy_();
            initialized_ = true;
        }
        return value_;
    }

    T& operator()() { return get_value(); }

    operator T() { return get_value(); }
};
```

那么有了这个东西之后我们能做什么呢？来看这样一个例子

```cpp
auto pi = lazy<double>([](){ return acos(-1.0); });
auto area = double(pi) * 5 * 5;
auto perimeter = (double)pi * 2 * 5;
cout << "pi= " << pi() << endl;
```

当程序运行到 `area` 的时候，就会发现有一个“偷懒”的变量 `pi`，这个时候就会强制终止这个变量的摸鱼行为，开始进行计算。计算完 `pi` 的真实值之后，再通过类型转换转换为一个 `double` 类型的值，送给表达式 `double(pi) * 5 * 5`。同样，如果执行到最后一行输出 `pi` 的值的时候，由于已经执行了初始化操作了，所以直接把值送给需要的地方就行了。

等一下，最后一行的这个写法，是不是给人一种记忆化函数调用的感觉？这一点我不反驳，因为两者确实很类似。为了更明显的观察具体的求值顺序，我们编写一份这样的代码：

```cpp
double radius = 5;
auto pi = lazy<double>([](){
    cout << "create var pi." << endl;
    return acos(-1.0);
});
auto helper = lazy<double>([&pi, radius](){
    cout << "create var helper." << endl;
    return pi() * radius;
});
auto area = lazy<double>([&helper, radius](){
    cout << "create var area." << endl;
    return helper() * radius;
});
auto perimeter = lazy<double>([&helper](){
    cout << "create var perimeter." << endl;
    return 2 * helper();
});
```

适才注意到我们使用了 lambda 来注册为初始化的 policy，那么同样我也可以借助 lambda 的变量捕捉方法，来实现加入参数，这就解释了为什么 policy 只设置了 `std::function<T()>` 类型的问题。那么我们写的这 5 条语句，除了第一条是一个及早求值策略的值以外，其他的都是惰性求值变量，所以，先声明的未必是先计算的。请看：

```cpp
cout << "perimeter= " << perimeter << endl;
/* => perimeter= create var perimeter.
create var helper.
create var pi.
31.4159 */
cout << "area= " << area << endl;
/* => area= create var area.
78.5398 */
```

借助我们在 policy 中的说明，我们得以看到惰性求值的具体路线。稍加思考，其实这份路线就是一个手工制作出来的简单版本的 AST ，只不过我们现在只关心求值而非语句到语法树的编译过程。

同样，由于惰性的求值策略，我们可以写出一些奇怪的代码，比如这样：

```cpp
auto a = lazy<int>([]() { return 1; });
auto b = lazy<int>([]() { return 2; });
auto c = a;
cout << "a= " << a() << ", b= " << b() << ", c= " << c() << endl;
// => a= 1, b= 2, c= 1
c() = 5;
cout << "a= " << a() << ", b= " << b() << ", c= " << c() << endl;
// => a= 1, b= 2, c= 5
b = c;
cout << "a= " << a() << ", b= " << b() << ", c= " << c() << endl;
// => a= 1, b= 1, c= 5
```

最后一行，为什么会得到如此奇怪的结果呢？因为我们在整理拷贝构造函数和赋值操作时，都是只复制了 policy 同时把初始化的状态更改为“未初始化”。所以第三行执行 `c = a` 的时候，实际上只是把 `[]() { return 1; }` 这个 lambda 存给了变量 `c`。同样的，最后的那句 `b = c` 又把这一个 lambda 给了变量 `b`。所以就会有明明赋值变成了 5，但是结果却是 1 的神奇操作（这不是 bug，这是 feature）。当然也可以改掉，无非是把 `false` 改成 `true` 罢了，这里不多赘述。

## 写在最后

既然知道了所谓的惰性求值无非是“提出初始化方案，检查是否初始化，返回正确值”的三部曲，那么配合上一些简单的递归操作，就是经常听到的无限列表处理。同样类似，配合上一些数值上的计算设计，细化一些具体计算的语法，就是常见的一些所谓的“数值计算中的惰性求值技巧”了。所谓万变不离其宗，学其根本而悟其道，则虽有万变而不惊也。

理论上这个系列应该是完结了，剩下的都是一些零零碎碎的整理工作，我应该能在年前整理完成。更多的话，我就留在后记里写吧。