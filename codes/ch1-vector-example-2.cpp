#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<class T>
ostream& operator<<(ostream &os, const vector<T> &xs)
{
    os << "size: " << xs.size() << ", ";
    os << "capacity: " << xs.capacity() << ", ";
    os << "begin(): " << xs.data() << ", ";
    os << "by now: " << (xs.data() + xs.size()) << ", ";
    return os << "end(): " << (xs.data() + xs.capacity());
}

int main()
{
    vector<int> a = { 1, 2 };
    cout << a << endl;
    for (int i = 0; i < 10; i++)
    {
        a.push_back(i); cout << a << endl;
    }
    cout << "----------------" << endl;
    vector<int> b = { 1, 2, 3 };
    cout << b << endl;
    for (int i = 0; i < 10; i++)
    {
        b.push_back(i); cout << b << endl;
    }
    return 0;
}

// filename: ch1-vector-example-2.cpp
// compile this> g++ ch1-vector-example-2.cpp -o ch1-vector-example-2.exe
