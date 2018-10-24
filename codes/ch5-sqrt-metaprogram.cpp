#include <iostream>

using namespace std;

template <bool Flag, class MaybeA, class MaybeB> class IfElse;

template <class MaybeA, class MaybeB>
class IfElse<true, MaybeA, MaybeB>
{
public:
    using ResultType = MaybeA;
};

template <class MaybeA, class MaybeB>
class IfElse<false, MaybeA, MaybeB>
{
public:
    using ResultType = MaybeB;
};

template <int N, int L, int R> struct __Sqrt
{
    enum { mid = (L + R + 1) / 2 };

    using ResultType = typename IfElse<(N < mid * mid),
        __Sqrt<N, L, mid - 1>, __Sqrt<N, mid, R> >::ResultType;

    enum { result = ResultType::result };
};

template <int N, int L> struct __Sqrt<N, L, L> { enum { result = L }; };

template <int N> struct Sqrt { enum { result = __Sqrt<N, 1, N>::result }; };

/* compile error
template <int N, int L, int R> struct __Sqrt
{
    enum { mid = (L + R + 1) / 2 };
    enum { result = (N < mid * mid) ? __Sqrt<N, L, mid - 1>::result
                                    : __Sqrt<N, mid, R>::result };
};

template <int N, int L> struct __Sqrt<N, L, L> { enum { result = L }; };

template <int N> struct Sqrt { enum { result = __Sqrt<N, 1, N>::result }; };
*/

int __sqrt(int n, int l, int r)
{
    if (l == r) return l;
    int mid = (l + r + 1) / 2;
    if (n < mid * mid)
        return __sqrt(n, l, mid - 1);
    else return __sqrt(n, mid, r);
}

inline int sqrt(int n) { return __sqrt(n, 0, n); }

int main()
{
    cout << sqrt(15) << endl; // => 3
    cout << sqrt(16) << endl; // => 4
    cout << sqrt(17) << endl; // => 4
    cout << Sqrt<15>::result << endl; // => 3
    cout << Sqrt<16>::result << endl; // => 4
    cout << Sqrt<17>::result << endl; // => 4
    return 0;
}

// filename: ch5-sqrt-metaprogram.cpp
// compile this> g++ ch5-sqrt-metaprogram.cpp -o ch5-sqrt-metaprogram.exe
