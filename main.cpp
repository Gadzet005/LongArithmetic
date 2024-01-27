#include <iostream>
#include <climits>

#include "long_number.hpp"

int main() {
    LongNumber a(99.99, 2);
    LongNumber b(999.9999, 5);

    cout << a << endl;
    cout << b << endl;
    cout << a * b << endl;

    return 0;
}