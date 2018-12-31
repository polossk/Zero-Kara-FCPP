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
    do
    {
        cout << endl << "#1" << endl;
        auto pi = lazy<double>([](){ return acos(-1.0); });
        auto area = double(pi) * 5 * 5;
        auto perimeter = (double)pi * 2 * 5;
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
    } while (0);

    do
    {
        cout << endl << "#2" << endl;
        double radius = 5;
        auto pi = lazy<double>([](){ cout << "create var pi." << endl; return acos(-1.0); });
        auto helper = lazy<double>([&pi, radius]() { cout << "create var helper." << endl; return pi() * radius; });
        auto area = lazy<double>([&helper, radius]() { cout << "create var area." << endl; return helper() * radius; });
        auto perimeter = lazy<double>([&helper]() { cout << "create var perimeter." << endl; return 2 * helper(); });
        cout << "perimeter= " << perimeter << endl;
        cout << "area= " << area << endl;
    } while (0);

    do
    {
        cout << endl << "#3" << endl;
        auto a = lazy<int>([]() { return 1; });
        auto b = lazy<int>([]() { return 2; });
        auto c = a;
        cout << "a= " << a() << ", b= " << b() << ", c= " << c() << endl;
        c() = 5;
        cout << "a= " << a() << ", b= " << b() << ", c= " << c() << endl;
        b = c;
        cout << "a= " << a() << ", b= " << b() << ", c= " << c() << endl;
    } while (0);


    auto a = lazy<int>([]() { return 1; });
    auto b = lazy<int>([]() { return 2; });
    auto c = a;
        // cout << std::boolalpha;

        // cout << a.initialized_ << " " << b.initialized_ << " " << c.initialized_ << endl;
        // cout << "a= " << a() << ", "; cout << a.initialized_ << " " << b.initialized_ << " " << c.initialized_ << endl;
        // cout << "b= " << b() << ", "; cout << a.initialized_ << " " << b.initialized_ << " " << c.initialized_ << endl;
        // cout << "c= " << c() << ", "; cout << a.initialized_ << " " << b.initialized_ << " " << c.initialized_ << endl;
        // c() = 5;
        // cout << "a= " << a() << ", "; cout << a.initialized_ << " " << b.initialized_ << " " << c.initialized_ << endl;
        // cout << "b= " << b() << ", "; cout << a.initialized_ << " " << b.initialized_ << " " << c.initialized_ << endl;
        // cout << "c= " << c() << ", "; cout << a.initialized_ << " " << b.initialized_ << " " << c.initialized_ << endl;
        // b = c;
        // cout << "a= " << a() << ", "; cout << a.initialized_ << " " << b.initialized_ << " " << c.initialized_ << endl;
        // cout << "b= " << b() << ", "; cout << a.initialized_ << " " << b.initialized_ << " " << c.initialized_ << endl;
        // cout << "c= " << c() << ", "; cout << a.initialized_ << " " << b.initialized_ << " " << c.initialized_ << endl;


    return 0;
}

// filename: ch7-lazy-type-example.cpp
// compile this> g++ ch7-lazy-type-example.cpp -o ch7-lazy-type-example.exe
