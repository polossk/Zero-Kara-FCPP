#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

using namespace std;

int fib_it(int n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    int ret = 1, a = 1, b = 0;
    for (int i = 2; i <= n; i++)
    {
        ret = a + b; b = a; a = ret;
    }
    return ret;
}

int fib_re(int n)
{
    if (n == 0) return 0;
    else if (n == 1) return 1;
    else return fib_re(n - 1) + fib_re(n - 2);
}

int fib_tr(int n, int ret = 0, int tmp = 1)
{
    if (n == 0) return ret;
    else return fib_tr(n - 1, ret + tmp, ret);
}

void benchmark(int cnt)
{
    #define TEST_CASE(id, foo) int sum##id = 0;\
    for (int i = 0; i < cnt; i += 1000)\
    {\
        sum##id += (i) * foo(i);\
    }\
    auto t##id = std::chrono::high_resolution_clock::now();

    auto t0 = std::chrono::high_resolution_clock::now();
    TEST_CASE(1, fib_tr)
    TEST_CASE(2, fib_it)
    cout << std::fixed << std::setprecision(2);
    cout << "======== " << cnt << " ========\n";
    cout << "fib_tr(): " << std::chrono::duration<double, std::milli>(t1 - t0).count() << "ms\n";
    cout << "fib_it(): " << std::chrono::duration<double, std::milli>(t2 - t1).count() << "ms\n";
    cout << "(sum1, sum2) % 11 = (";
    cout << (sum1 % 11) << ", " << (sum2 % 11) << ")\n";
    cout << endl;

    #undef TEST_CASE
}

int main()
{
    for (int i = 0; i < 10; i++)
    {
        cout << "\t" << fib_it(i);
        cout << "\t" << fib_re(i);
        cout << "\t" << fib_tr(i) << "\n";
    }

    for (int i = 1; i < 10; i += 2)
        benchmark(i * 100000);
    return 0;
}

// filename: ch6-fib-benchmark.cpp
// compile this> g++ ch6-fib-benchmark.cpp -o ch6-fib-benchmark.exe -O2
