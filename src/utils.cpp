#include "utils.hpp"

LongNumber arccot(long long value, unsigned int prec=LongNumber::DEFAULT_PRECISION) {
    LongNumber curChange(std::to_string(value * value), 0);
    LongNumber cur = LongNumber::divide(LongNumber("1", 0), LongNumber(std::to_string(value), 0), prec);
    LongNumber sum("0", prec);

    for (int i = 0; !cur.isZero(); i++) {
        sum += cur / LongNumber(std::to_string(2 * i + 1), 0);
        cur = std::move(-cur / curChange);
    }

    return sum;
}

LongNumber calculatePi(unsigned int prec=LongNumber::DEFAULT_PRECISION) {
    unsigned int highPrec = prec + 9;
    LongNumber pi = (
        LongNumber("4", 0) * arccot(5, highPrec) - arccot(239, highPrec)
    ) * LongNumber("4", 0);
    return LongNumber(pi, prec);
}
