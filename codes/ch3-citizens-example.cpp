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
