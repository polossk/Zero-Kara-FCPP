#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <ctime>

using namespace std;

template <int DIM, class T> class __Dot
{
public:
    static T result(T *a, T *b)
    {
        return (*a) * (*b) + __Dot<DIM - 1, T>::result(a + 1, b + 1);
    }
};

template <class T> class __Dot<1, T>
{
public:
    static T result(T *a, T *b) { return (*a) * (*b); }
};

template <int DIM, class T>
T dot_meta(T *a, T *b)
{
    return __Dot<DIM, T>::result(a, b);
}

template <class T> T dot(int dim, T *a, T *b)
{
    int result = T();
    for (int i = 0; i < dim; i++)
        result += a[i] * b[i];
    return result;
}

template <class T> T dot_re(int dim, T *a, T *b)
{
    if (dim == 1) return (*a) * (*b);
    return (*a) * (*b) + dot_re(dim - 1, a + 1, b + 1);
}

vector<int> a = {1, 2, 3, 4}, b = {5, 6, 7, 8};

void benchmark(int cnt)
{
    #define DOT_META_(dim, a, b) dot_meta<dim>(a, b)

    #define TEST_CASE(id, foo) for (int i = 0; i < cnt; i++)\
        for (int j = 0; j < cnt; j++)\
    {\
        int hoge = foo(4, a.data(), b.data());\
        sum##id += hoge * foo(4, a.data(), a.data());\
    }\
    auto t##id = std::chrono::high_resolution_clock::now();

    int sum1 = 0, sum2 = 0, sum3 = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    TEST_CASE(1, dot)
    TEST_CASE(2, dot_re)
    TEST_CASE(3, DOT_META_)
    cout << std::fixed << std::setprecision(2);
    cout << "======== " << cnt << " ========\n";
    cout << "dot():      " << std::chrono::duration<double, std::milli>(t1 - t0).count() << "ms\n";
    cout << "dot_re():   " << std::chrono::duration<double, std::milli>(t2 - t1).count() << "ms\n";
    cout << "dot_meta(): " << std::chrono::duration<double, std::milli>(t3 - t2).count() << "ms\n";
    cout << "(sum1 % 100, sum2 % 100, sum3 % 100)= (";
    cout << (sum1 % 100) << ", " << (sum2 % 100) << ", "<< (sum3 % 100) << ")\n";
    cout << endl;
    #undef TEST_CASE
    #undef DOT_META_
}

int main()
{
    for (int i = 1; i < 10; i += 2)
        benchmark(i * 1000);
    return 0;
}

// filename: ch5-dot-metaprogram.cpp
// compile this> g++ ch5-dot-metaprogram.cpp -o ch5-dot-metaprogram.exe -O2
