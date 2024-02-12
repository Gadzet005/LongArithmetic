#include <iostream>

#include "long_number.hpp"

int main() {
    using std::cin, std::cout, std::endl;

    LongNumber a;
    LongNumber b("0");

    cout << a << endl;
    cout << b << endl;
    cout << (a == b) << endl;

    // while (true) {
    //     LongNumber a, b;
    //     char op;
    //     cin >> a >> op >> b;

    //     switch (op) {
    //     case '+':
    //         cout << a + b << endl;
    //         break;
    //     case '-':
    //         cout << a - b << endl;
    //         break;
    //     case '*':
    //         cout << a * b << endl;
    //         break;
    //     case '/':
    //         cout << a / b << endl;
    //         break;
    //     default:
    //         throw std::runtime_error("Доступные операции: + - * /");
    //         break;
    //     }
    // }

    return 0;
}