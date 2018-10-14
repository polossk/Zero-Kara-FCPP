#include <iostream>
#include <functional>

using std::cout;
using std::endl;

void f(int x, int y, const int &z)
{
    cout << "=> f(" << x << ", " << y << ", " << z << ")" << endl;
}

int g(int x, int y, int z) { return x + y * z; }

struct Foo
{
    int data = -1;
    int addition(int a, int b) { return data + a + b; }
};

int main()
{
    using namespace std::placeholders;
    int n = 5;
    auto fyx5 = std::bind(f, _2, _1, n);
    fyx5(1, 2); // => f(2, 1, 5)
    auto f5yy = std::bind(f, n, std::ref(n), std::cref(n));
    f5yy(); // => f(5, 5, 5)
    n += 2; f5yy(); // => f(5, 7, 7)

    auto gxxx = std::bind(g, _1, _1, _1);
    cout << "5 + 5 * 5= " << gxxx(5) << endl; // => 5 + 5 * 5= 30
    auto gx45 = std::bind(std::bind(g, _1, _2, 5), _1, 4);
    cout << "3 + 4 * 5= " << gx45(3) << endl; // => 3 + 4 * 5= 23

    Foo foo_object;
    auto foo_data = std::bind(&Foo::data, _1);
    cout << foo_data(foo_object) << endl; // => -1

    auto foo_member = std::bind(&Foo::addition, &foo_object, 1, _1);
    cout << foo_member(10) << endl; // => 10

    return 0;
}

// filename: ch4-bind-example.cpp
// compile this> g++ ch4-bind-example.cpp -o ch4-bind-example.exe
