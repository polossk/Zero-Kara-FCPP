#include <iostream>
#include <vector>

using namespace std;

int main()
{
    auto addition = [](int _1, int _2)->int{ return _1 + _2; };
    vector<int> a = { 1, 2, 3, 4, 5 };
    vector<int> b = { 6, 7, 8, 9, 10 };
    for (int i = 0, sz = a.size(); i < sz; i++)
    {
        a[i] = addition(a[i], b[i]);
    }
    for (auto e : a) cout << " " << e; cout << endl;
    // => 7 9 11 13 15
    return 0;
}

// filename: ch2-lambda-example-1.cpp
// compile this> g++ ch2-lambda-example-1.cpp -o ch2-lambda-example-1.exe
