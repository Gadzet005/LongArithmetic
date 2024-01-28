#include <iostream>

#include "long_number.hpp"

int main() {
    LongNumber a = "213.321"_LN;
    LongNumber b = "91.1"_LN;

    cout << a << " " << b << endl;

    cout << a + b << endl;
    cout << a - b << endl;
    cout << a * b << endl;
    cout << a / b << endl;

    return 0;
}