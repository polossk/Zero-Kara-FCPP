#include <iostream>
#include <string>
#include <utility>
#include <functional>

int main()
{
    std::string names[3] = { "alice", "bob", "carl" };
    int score[3] = { 5, 6, 7 };

    // build a pair from string and int
    std::pair<std::string, int> p0 = std::make_pair(names[0], score[0]);
    std::cout << "The value of p0 is "
              << "(" << p0.first << ", " << p0.second << ")\n";
    // => The value of p0 is (alice, 5)

    // build a pair from string and std::ref(int)
    auto p1 = std::make_pair(names[1], std::ref(score[1]));
    std::cout << "The value of p1 is "
              << "(" << p1.first << ", " << p1.second << ")\n";
    // => The value of p1 is (bob, 6)
    score[1] += 10;
    std::cout << "After score[1] += 10, the value of p1 is "
              << "(" << p1.first << ", " << p1.second << ")\n";
    // => After score[1] += 10, the value of p1 is (bob, 16)
    p1.second += 100;
    std::cout << "After p1.second += 100, the value of p1 is "
              << "(" << p1.first << ", " << p1.second << "), "
              << "while score[1] is " << score[1] << "\n";
    // => After p1.second += 100, the value of p1 is (bob, 116), while score[1] is 116
    names[1] += "bi";
    std::cout << "After names[1] += \"bi\", the value of p1 is "
              << "(" << p1.first << ", " << p1.second << "), "
              << "while names[1] is " << names[1] << "\n";
    // => After names[1] += "bi", the value of p1 is (bob, 116), while names[1] is bobbi
    return 0;
}

// filename: ch1-pair-example-1.cpp
// compile this> g++ ch1-pair-example-1.cpp -o ch1-pair-example-1.exe
