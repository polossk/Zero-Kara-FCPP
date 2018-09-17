#include <iostream>
#include <string>
#include <tuple>
#include <functional>

int main()
{
    std::string names[3] = { "alice", "bob", "carl" };
    char ranks[3] = { 'A', 'B', 'C' };
    int score[3] = { 5, 6, 7 };

    auto student0 = std::make_tuple(names[0], ranks[0], score[0]);
    std::cout << "student0> name: " << std::get<0>(student0)
              << ", rank: " << std::get<1>(student0)
              << ", score: " << std::get<2>(student0) << std::endl;

    std::string hoge;
    char piyo;
    int fuga;
    auto student1 = std::make_tuple(names[1], ranks[1], score[1]);
    std::tie(hoge, piyo, fuga) = student1;
    std::cout << "student1> name: " << hoge << ", rank: " << piyo
              << ", score: " << fuga << std::endl;

    auto student2 = std::tie(names[2], ranks[2], score[2]);
    auto [a, b, c] = student2; // C++17 structured binding
    std::cout << "student2> name: " << a << ", rank: " << b
              << ", score: " << c << std::endl;
    return 0;
}

// filename: ch1-tuple-example-1.cpp
// compile this> g++ ch1-tuple-example-1.cpp -o ch1-tuple-example-1.exe -std=c++17
