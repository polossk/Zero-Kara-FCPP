#include <iostream>
#include <tuple>
#include <utility>
#include <functional>
#include <stdexcept>
#include <cmath>

#include "lazy_utils.hpp"

using std::cout;
using std::endl;
using namespace __utils;

int main()
{
    auto pi = lazy<double>([](){ return acos(-1.0); });
    auto area = pi() * 5 * 5;
    auto perimeter = pi() * 2 * 5;
    cout << "pi= " << pi() << endl;
    cout << "area= " << area << endl;
    cout << "perimeter= " << perimeter << endl;
    pi() = 3.14;
    cout << "pi= " << pi() << endl;
    cout << "area= " << area << endl;
    cout << "perimeter= " << perimeter << endl;
    area = pi() * 5 * 5;
    perimeter = pi() * 2 * 5;
    cout << "area= " << area << endl;
    cout << "perimeter= " << perimeter << endl;

    auto b = lazy<int>([]() { return 1; });
    auto c = lazy<int>([]() { return 2; });
    auto a = b;
    cout << a() << " " << b() << " " << c() << endl;
    b = c;
    cout << a() << " " << b() << " " << c() << endl;


    return 0;
}

// filename: ch7-lazy-type-example.cpp
// compile this> g++ ch7-lazy-type-example.cpp -o ch7-lazy-type-example.exe
