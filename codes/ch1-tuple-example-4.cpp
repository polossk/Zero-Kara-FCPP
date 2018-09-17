#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <functional>

using namespace std;

void display_separator() { cout << "--------" << endl; }

int main()
{
    string names[4] = { "alice", "bob", "carl", "dell" };
    char ranks[4] = { 'A', 'B', 'C', 'D' };
    int score[4] = { 5, 6, 7, 8 };

    cout << std::boolalpha;

    int hoge;
    auto student0 = tie(names[0], ranks[0], score[0]);
    // => name: bob, rank: B, score: 6
    tie(std::ignore, std::ignore, hoge) = student0;
    cout << "0> " << std::is_same<int, decltype(hoge)>::value;
    cout << ", " << std::is_same<int&, decltype(hoge)>::value << endl;
    
    display_separator();

    auto student1 = make_tuple(names[1], ranks[1], score[1]);
    // => name: carl, rank: C, score: 7
    auto [_1, _2, piyo] = student1;
    cout << "1> " << std::is_same<int, decltype(piyo)>::value;
    cout << ", " << std::is_same<int&, decltype(piyo)>::value << endl;
    
    display_separator();

    auto student2 = tie(names[2], ranks[2], score[2]);
    // => name: carl, rank: C, score: 7
    auto [_3, _4, fuga] = student2;
    cout << "2> " << std::is_same<int, decltype(fuga)>::value;
    cout << ", " << std::is_same<int&, decltype(fuga)>::value << endl;
    
    display_separator();
    
    auto student3 = make_tuple(names[2], ranks[2], std::ref(score[2]));
    // => name: carl, rank: C, score: 7
    auto [_5, _6, pika] = student3;
    cout << "3> " << std::is_same<int, decltype(pika)>::value;
    cout << ", " << std::is_same<int&, decltype(pika)>::value << endl;
    
    display_separator();

    cout << "bonus> "
         << std::is_same<int&, decltype(std::get<2>(student0))>::value << ", "
         << std::is_same<int&, decltype(std::get<2>(student1))>::value << endl;
    return 0;
}

// filename: ch1-tuple-example-4.cpp
// compile this> g++ ch1-tuple-example-4.cpp -o ch1-tuple-example-4.exe -std=c++17
