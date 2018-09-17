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

void display_separator() { cout << "--------" << endl; }

int main()
{
    string name = "alice";
    char rank = 'A';
    int score = 5;

    auto student = make_tuple(name, rank, score);
    // => name: alice, rank: A, score: 5
    cout << "student> name: " << get<0>(student)
              << ", rank: " << get<1>(student)
              << ", score: " << get<2>(student) << endl;
    cout << "Here, (score, get<2>(student)) is "
         << make_pair(score, get<2>(student)) << endl;
    score += 10;
    cout << "        score   +=  10 => "
         << make_pair(score, get<2>(student)) << endl;
    get<2>(student) += 100;
    cout << "get<2>(student) += 100 => "
         << make_pair(score, get<2>(student)) << endl;
    return 0;
}

// filename: ch1-tuple-example-2.cpp
// compile this> g++ ch1-tuple-example-2.cpp -o ch1-tuple-example-2.exe -std=c++17
