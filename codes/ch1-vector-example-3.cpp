#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<class T>
ostream& operator<<(ostream &os, const vector<T> &xs)
{
    os << '[';
    for (auto x : xs) os << " " << x;
    return os << ']';
}

int main()
{
    vector<int> a = { 1, 2, 3, 4 };
    a.clear(); cout << a << endl; // => []
    a.insert(a.begin(), 5);
    cout << a << endl; // => [ 5]
    for (int i = 0; i < 3; i++) a.push_back(7);
    cout << a << endl; // => [ 5 7 7 7]
    a.insert(a.begin(), 2, 9);
    cout << a << endl; // => [ 9 9 5 7 7 7]
    
    vector<int> b = {10, 11, 12};
    a.insert(a.begin() + 1, b.begin(), b.end());
    cout << a << endl; // => [ 9 10 11 12 9 5 7 7 7]

    int c[5] = {20, 21, 22, 999, 999};
    a.insert(a.begin(), c, c + 3);
    cout << a << endl; // => [ 20 21 22 9 10 11 12 9 5 7 7 7]

    a.erase(a.begin());
    cout << a << endl; // => [ 21 22 9 10 11 12 9 5 7 7 7]
    a.erase(a.begin() + 2, a.begin() + 5);
    cout << a << endl; // => [ 21 22 12 9 5 7 7 7]

    // erase all number gather than 10
    for (auto it = a.begin(); it != a.end(); )
    {
        it = (*it > 10) ? a.erase(it) : it + 1;
    }
    cout << a << endl; // => [ 9 5 7 7 7]
    return 0;
}

// filename: ch1-vector-example-3.cpp
// compile this> g++ ch1-vector-example-3.cpp -o ch1-vector-example-3.exe
