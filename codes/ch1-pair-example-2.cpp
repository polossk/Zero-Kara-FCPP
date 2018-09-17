#include <iostream>
#include <string>
#include <utility>
#include <functional>

using namespace std;

template<class T1, class T2>
ostream& operator<<(ostream &out, const pair<T1, T2> &_)
{
    return out << "(" << _.first << ", " << _.second << ")";
}

void display_separator() { cout << "--------" << endl; }

int main()
{
    pair<int, double> p1; // => (0, 0)
    cout << "Default constructor: " << p1 << endl;
    pair<int, double> p2(65, 1.1); // => (65, 1.1)
    cout << "Initialized with two values: " << p2 << endl;
    pair<int, double> p3(p2); // => (65, 1.1)
    cout << "Copy constructor: " << p3 << endl;
    pair<char, int> p4(p2); // => (A, 1)
    cout << "Copy then convert: " << p4 << endl;

    display_separator();

    pair<char, int> p5 = make_pair('B', 2);
    cout << "Before swap: p4 is " << p4 << " , p5 is " << p5 << endl;
    // => Before swap: p4 is (A, 1) , p5 is (B, 2)
    p5.swap(p4);
    cout << "After swap: p4 is " << p4 << " , p5 is " << p5 << endl;
    // => After swap: p4 is (B, 2) , p5 is (A, 1)

    display_separator();

    pair<char, int> p6 = make_pair('B', 1);
    pair<char, int> p7 = make_pair('A', 2);
    cout << "p4 is " << p4 << " , p5 is " << p5 << ", "
         << "p6 is " << p6 << " , p7 is " << p7 << endl;
    // => p4 is (B, 2) , p5 is (A, 1), p6 is (B, 1) , p7 is (A, 2)
    cout << std::boolalpha;
    cout << (p4 == p5) << endl; // => false
    cout << (p4 != p5) << endl; // => true
    cout << (p5 < p7) << endl; // => true
    cout << (p4 < p6) << endl; // => false
    cout << (p5 < p6) << endl; // => true
    cout << (p6 < p7) << endl; // => false
    return 0;
}

// filename: ch1-pair-example-2.cpp
// compile this> g++ ch1-pair-example-2.cpp -o ch1-pair-example-2.exe
