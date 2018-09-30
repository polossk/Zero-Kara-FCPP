#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>

using std::cout;
using std::endl;

int main()
{
    std::vector<int> a = { 97, 98, 99, 100, 101, 102, 103 };
    for (auto e : a) cout << " " << e;
    cout << endl; // => 97 98 99 100 101 102 103
    std::for_each(a.begin(), a.end(), [](const int &_){ cout << " " << _; });
    cout << endl; // => 97 98 99 100 101 102 103
    std::vector<int> b, c(10, 122);
    std::transform(a.begin(), a.end(), std::back_inserter(b),
        [](int _)->int{ return _ + 3; });
    std::transform(a.begin(), a.end(), c.begin() + 1,
        [](int _)->int{ return _ + 7;});
    std::vector<char> d, e;
    std::transform(a.begin(), a.end(), c.begin(), std::back_inserter(d),
        [](int _1, int _2){ return std::toupper(char(std::max(_1, _2))); });
    std::transform(c.begin(), c.end(), a.begin(), std::back_inserter(e),
        [](int _1, int _2){ return std::toupper(char(std::min(_1, _2))); });

    auto print_char = [](auto v){ for (auto e : v) cout << char(e); cout << endl; };
    auto print_value = [](auto v){ for (auto e : v) cout << " " << int(e); cout << endl; };
    #define DISPLAY_CHAR(v) do { cout << #v ": "; print_char(v); } while (0);
    #define DISPLAY_VALUE(v) do { cout << #v ":"; print_value(v); } while (0);
    DISPLAY_CHAR(a); // => a: abcdefg
    DISPLAY_CHAR(b); // => b: defghij
    DISPLAY_CHAR(c); // => c: zhijklmnzz
    DISPLAY_CHAR(d); // => d: ZHIJKLM
    DISPLAY_VALUE(d); // => d: 90 72 73 74 75 76 77
    DISPLAY_CHAR(e); // => e: ZHIJKLM
    DISPLAY_VALUE(e); // => e: 65 66 67 68 69 70 71 0 0 0
    #undef DISPLAY_CHAR
    #undef DISPLAY_VALUE
    return 0;
}

// filename: ch3-transform-example.cpp
// compile this> g++ ch3-transform-example.cpp -o ch3-transform-example.exe
