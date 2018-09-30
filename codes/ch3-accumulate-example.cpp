#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

using std::cout;
using std::endl;

int main()
{
    auto print_value = [](auto v){ for (auto e : v) cout << " " << int(e); cout << endl; };
    #define DISPLAY_VALUE(v) do { cout << #v ":"; print_value(v); } while (0);
    std::vector<int> a = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int sum = std::accumulate(a.begin(), a.end(), 0,
        [](int acc, int _){ return acc + _; });
    cout << sum << ", "; DISPLAY_VALUE(a);
    // => 55, a: 1 2 3 4 5 6 7 8 9 10

    int prod = std::accumulate(a.begin(), a.end(), 1,
        [](int acc, int _){ return acc * _; });
    cout << prod << ", "; DISPLAY_VALUE(a);
    // => 3628800 a: 1 2 3 4 5 6 7 8 9 10

    std::vector<int> a_re = std::accumulate(a.begin(), a.end(), std::vector<int>(),
        [](std::vector<int> acc, int _)->std::vector<int>{
            std::vector<int> __; __.push_back(_);
            std::copy(acc.begin(), acc.end(), std::back_inserter(__));
            return __;
        });
    DISPLAY_VALUE(a_re); // => a_re: 10 9 8 7 6 5 4 3 2 1
    #undef DISPLAY_VALUE
    return 0;
}

// filename: ch3-accumulate-example.cpp
// compile this> g++ ch3-accumulate-example.cpp -o ch3-accumulate-example.exe
