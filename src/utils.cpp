#include "utils.hpp"

// arccot(x) = arctg(1/x) = 1/x - 1/3*x**3 + 1/5*x**5 - ...
LongNumber arccot(int value, unsigned int prec=LongNumber::DEFAULT_PRECISION) {
    LongNumber curChange(value * value, 0);
    LongNumber cur = LongNumber::divide(LongNumber(1, 0), LongNumber(value, 0), prec);
    LongNumber sum(0, prec);

    int i = 0;
    while (cur != LongNumber(0, 0)) {
        sum += cur / LongNumber(2 * i + 1, 0);
        cur = -cur / curChange;
        i++;
    }

    return sum;
}

// Формула Гаусса: pi/4 = 12*arccot(18) + 8*arccot(57) - 5*arccot(239)
LongNumber calculatePi(unsigned int prec=LongNumber::DEFAULT_PRECISION) {
    unsigned int highPrec = prec + 10;
    LongNumber pi = (
        LongNumber(12, 0) * arccot(18, highPrec) + 
        LongNumber(8, 0) * arccot(57, highPrec) +
        LongNumber(-5, 0) * arccot(239, highPrec)
    ) * LongNumber(4, 0);
    return LongNumber(pi, prec);
}
