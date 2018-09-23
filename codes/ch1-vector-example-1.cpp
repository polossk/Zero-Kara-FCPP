#include <iostream>
#include <string>
#include <vector>

template<class T>
std::ostream& operator<<(std::ostream &os, const std::vector<T> &xs)
{
    os << '[';
    for (auto x : xs) os << " " << x;
    return os << ']';
}

int main()
{
    std::vector<std::string> echo(3, "Hello");
    std::cout << echo << std::endl; // => [ Hello Hello Hello]
    std::vector<int> fib = {1, 1, 2, 3, 5, 8, 13};
    std::cout << fib << std::endl; // => [ 1 1 2 3 5 8 13]
    fib.push_back(fib.back());
    std::cout << fib << std::endl; // => [ 1 1 2 3 5 8 13 13]
    fib.back() = fib.front();
    std::cout << fib << std::endl; // => [ 1 1 2 3 5 8 13 1]
    fib.pop_back();
    
    for (int i = 0, len = fib.size(); i < len; i++)
    {
        std::cout << " " << fib[i];
    }
    std::cout << std::endl; // => 1 1 2 3 5 8 13

    for (std::vector<int>::iterator it = fib.begin(); it != fib.end(); it++)
    {
        (*it) = (*it) + 1;
    }

    for (std::vector<int>::reverse_iterator ri = fib.rbegin(); ri != fib.rend(); ri++)
    {
        std::cout << " " << (*ri);
    }
    std::cout << std::endl; // =>  14 9 6 4 3 2 2
    return 0;
}

// filename: ch1-vector-example-1.cpp
// compile this> g++ ch1-vector-example-1.cpp -o ch1-vector-example-1.exe
