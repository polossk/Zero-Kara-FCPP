#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

using namespace std;

int frac_it(int n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    int ret = 1;
    for (int i = 2; i <= n; ret *= i++);
    return ret;
}

int frac_re(int n)
{
    if (n == 0) return 1;
    else if (n == 1) return 1;
    else return n * frac_re(n - 1);
}

int frac_tr(int n, int ret = 1)
{
    if (n == 0) return ret;
    else if (n == 1) return ret;
    else return frac_tr(n - 1, n * ret);
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
    TEST_CASE(1, frac_re)
    TEST_CASE(2, frac_tr)
    TEST_CASE(3, frac_it)
    cout << std::fixed << std::setprecision(2);
    cout << "======== " << cnt << " ========\n";
    cout << "frac_re(): " << std::chrono::duration<double, std::milli>(t1 - t0).count() << "ms\n";
    cout << "frac_tr(): " << std::chrono::duration<double, std::milli>(t2 - t1).count() << "ms\n";
    cout << "frac_it(): " << std::chrono::duration<double, std::milli>(t3 - t2).count() << "ms\n";
    cout << "(sum1, sum2, sum3) % 11 = (";
    cout << (sum1 % 11) << ", " << (sum2 % 11) << ", " << (sum3 % 11) << ")\n";
    cout << endl;

    #undef TEST_CASE
}

int main()
{
    for (int i = 1; i < 10; i += 2)
        benchmark(i * 100000);
    return 0;
}

// filename: ch6-frac-benchmark.cpp
// compile this> g++ ch6-frac-benchmark.cpp -o ch6-frac-benchmark.exe -O2
