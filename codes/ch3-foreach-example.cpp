#include <iostream>
#include <algorithm>
#include <vector>

using std::cout;
using std::endl;

int main()
{
    std::vector<int> a = { 1, 2, 3, 4 };
    int sum = 0, idx = 1, sum2 = 0;
    auto add_to_sum = [&sum](int _){ sum += _; };
    auto add_to_sum_with_idx = [&sum2, &idx](int _){ sum2 += _ * idx++; };
    for (auto e : a) cout << " " << e;
    cout << endl; // => 1 2 3 4
    for_each(a.begin(), a.end(), add_to_sum);
    cout << sum << endl; // => 10
    for_each(a.begin(), a.end(), add_to_sum_with_idx);
    cout << sum2 << endl; // => 30
    return 0;
}

// filename: ch3-foreach-example.cpp
// compile this> g++ ch3-foreach-example.cpp -o ch3-foreach-example.exe
