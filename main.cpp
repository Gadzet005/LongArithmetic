#include <iostream>
#include <climits>

#include "long_number.hpp"

int main() {
    LongNumber a(0.01, 2);
    LongNumber b(412.00021312, 5);

    cout << a << endl;
    cout << b << endl;
    cout << b * a << endl;

    return 0;
}