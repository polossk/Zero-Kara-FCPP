#include <iostream>

using namespace std;

int main()
{
    int j = 1;
    auto by_val_lambda = [=]{ return j + 1; };
    auto by_ref_lambda = [&]{ return j + 1; };
    auto print = [=]{
        cout << "print by val lambda: " << by_val_lambda() << ", ";
        cout << "by ref lambda: " << by_ref_lambda() << endl;
    };
    print();
    j += 10; print();
    return 0;
}

// filename: ch2-lambda-example-2.cpp
// compile this> g++ ch2-lambda-example-2.cpp -o ch2-lambda-example-2.exe
