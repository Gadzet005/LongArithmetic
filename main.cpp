#include <iostream>
#include <climits>

#include "long_number.hpp"

int main() {
    LongNumber a(1.23123, 5);
    LongNumber b(8.67, 2);

    cout << a << endl;
    cout << b << endl;
    cout << a + 1231 << endl;

    return 0;
}