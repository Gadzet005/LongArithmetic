#include <iostream>

#include "long_number.hpp"

int main() {
    LongNumber a = "213.321121311111111111111111111111111111234314199999999999999999999"_LN;
    LongNumber b("213321121311111111111111111111111111111234314199999999999999999999", 10);

    cout << a << endl;
    cout << b << endl;
    cout << a * b << endl;

    return 0;
}