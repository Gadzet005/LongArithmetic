#include <iostream>

#include "long_number.hpp"

int main() {
    using namespace std;
    using namespace lngAr;

    while (true) {
        LongNumber a, b;
        char op;
        cin >> a >> op >> b;

        switch (op) {
        case '+':
            cout << a + b << endl;
            break;
        case '-':
            cout << a - b << endl;
            break;
        case '*':
            cout << a * b << endl;
            break;
        case '/':
            cout << a / b << endl;
            break;
        default:
            throw std::runtime_error("Доступные операции: + - * /");
            break;
        }
    }

    return 0;
}