#include <iostream>
#include <climits>

#include "long_number.hpp"

int main() {
    LongNumber a(0.8, 5);
    LongNumber b(1.198, 3);

    cout << a << endl;
    cout << b << endl;
    cout << a * b << endl;

    return 0;
}