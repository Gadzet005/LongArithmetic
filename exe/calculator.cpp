#include <iostream>

#include "long_number.hpp"

int main() {
    using std::cin, std::cout, std::endl;

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

    LongNumber a("1");
    LongNumber b("7");

    cout << a << endl;
    cout << b << endl;
    cout << a / b << endl;

    return 0;
}