#include <iostream>

using namespace std;

template <int A, int B> struct Pow
{
    enum { result = A * Pow<A, B - 1>::result };
};

template <int A> struct Pow<A, 1> { enum { result = A }; };

template <int A> struct Pow<A, 0> { enum { result = 1 }; };

template <int A, int B> struct ConstPow
{
    constexpr static int result = A * ConstPow<A, B - 1>::result;
};

template <int A> struct ConstPow<A, 1> { constexpr static int result = A; };

template <int A> struct ConstPow<A, 0> { constexpr static int result = 1; };

int pow(int a, int b)
{
    if (b == 0) return 1;
    if (b == 1) return a;
    return a * pow(a, b - 1);
}

int main()
{
    cout << pow(2, 4) << endl; // => 16
    cout << Pow<2, 4>::result << endl; // => 16
    cout << ConstPow<2, 4>::result << endl; // => 16
    return 0;
}

// filename: ch5-pow-metaprogram.cpp
// compile this> g++ ch5-pow-metaprogram.cpp -o ch5-pow-metaprogram.exe
