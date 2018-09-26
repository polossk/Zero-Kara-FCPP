#include <iostream>
#include <functional>

using namespace std;

int main()
{
    std::function<int(int)> fact;
    fact = [&fact](int x)->int{ return x <= 1 ? x : x * fact(x - 1); };
    cout << fact(5) << endl; // => 120

    std::function<int(int, int)> gcd = [&](int a, int b)->int{
        return b == 0 ? a : gcd(b, a % b);
    };
    cout << gcd(24, 36) << endl; // => 12
    return 0;
}

// filename: ch2-lambda-example-3.cpp
// compile this> g++ ch2-lambda-example-3.cpp -o ch2-lambda-example-3.exe
