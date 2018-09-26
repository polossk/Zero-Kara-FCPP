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
