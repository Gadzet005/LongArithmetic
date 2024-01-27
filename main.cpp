#include <iostream>
#include <climits>

#include "long_number.hpp"

int main() {
    LongNumber a(0.01, 2);
    LongNumber b(0.0012, 5);

    cout << a << endl;
    cout << b << endl;
    cout << -a - b << endl;

    return 0;
}