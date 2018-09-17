#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <functional>

using namespace std;

template<class T1, class T2>
ostream& operator<<(ostream &out, const pair<T1, T2> &_)
{
    return out << "(" << _.first << ", " << _.second << ")";
}

template<class T1, class T2, class T3>
ostream& operator<<(ostream &out, const tuple<T1, T2, T3> &_)
{
    auto [_1, _2, _3] = _;
    return out << "[" << _1 << ", " << _2 << ", " << _3 << "]";
}

void display_separator() { cout << "--------" << endl; }

int main()
{
    string names[3] = { "alice", "bob", "carl" };
    char ranks[3] = { 'A', 'B', 'C' };
    int score[3] = { 5, 6, 7 };

    int hoge;
    auto student0 = tie(names[0], ranks[0], score[0]);
    // => name: bob, rank: B, score: 6
    tie(std::ignore, std::ignore, hoge) = student0;
    cout << "student0> [hoge, score[0], get<2>(student0)] is "
         << make_tuple(hoge, score[0], get<2>(student0)) << endl;
    hoge += 10;
    cout << "            hoge +=   10 => "
         << make_tuple(hoge, score[0], get<2>(student0)) << endl;
    score[0] += 100;
    cout << "        score[0] +=  100 => "
         << make_tuple(hoge, score[0], get<2>(student0)) << endl;
    get<2>(student0) += 1000;
    cout << "get<2>(student0) += 1000 => "
         << make_tuple(hoge, score[0], get<2>(student0)) << endl;

    display_separator();

    auto student1 = make_tuple(names[1], ranks[1], score[1]);
    // => name: carl, rank: C, score: 7
    auto [_1, _2, piyo] = student1;
    cout << "student1> [piyo, score[1], get<2>(student1)] is "
         << make_tuple(piyo, score[1], get<2>(student1)) << endl;
    piyo += 10;
    cout << "            piyo +=   10 => "
         << make_tuple(piyo, score[1], get<2>(student1)) << endl;
    score[1] += 100;
    cout << "        score[1] +=  100 => "
         << make_tuple(piyo, score[1], get<2>(student1)) << endl;
    get<2>(student1) += 1000;
    cout << "get<2>(student1) += 1000 => "
         << make_tuple(piyo, score[1], get<2>(student1)) << endl;

    display_separator();

    auto student2 = tie(names[2], ranks[2], score[2]);
    // => name: carl, rank: C, score: 7
    auto [_3, _4, fuga] = student2;
    cout << "student2> [fuga, score[2], get<2>(student2)] is "
         << make_tuple(fuga, score[2], get<2>(student2)) << endl;
    fuga += 10;
    cout << "            fuga +=   10 => "
         << make_tuple(fuga, score[2], get<2>(student2)) << endl;
    score[2] += 100;
    cout << "        score[2] +=  100 => "
         << make_tuple(fuga, score[2], get<2>(student2)) << endl;
    get<2>(student2) += 1000;
    cout << "get<2>(student2) += 1000 => "
         << make_tuple(fuga, score[2], get<2>(student2)) << endl;
    return 0;
}

// filename: ch1-tuple-example-3.cpp
// compile this> g++ ch1-tuple-example-3.cpp -o ch1-tuple-example-3.exe -std=c++17
