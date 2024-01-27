#include "long_number.hpp"


LongNumber::LongNumber():
    exp(), sign(), precision(LongNumber::DEFAULT_PRECISION) {}

LongNumber::LongNumber(double num, int prec): exp(), precision(prec) {
    sign = num < 0;
    num = abs(num);

    double intPart;
    num = modf(num, &intPart);
    while (intPart >= 1) {
        double mod = fmod(intPart, 10);
        digits.push_back(static_cast<char>(mod));
        intPart /= 10;
        exp++;
    }

    reverseDigits();

    while (prec > 0) {
        num *= 10;
        num = modf(num, &intPart);
        double mod = fmod(intPart, 10);
        digits.push_back(static_cast<char>(mod));
        prec--;
    }

    reverseDigits();
    normilizeDigits();
}

LongNumber::LongNumber(double num): 
    LongNumber(num, LongNumber::DEFAULT_PRECISION) {}


void LongNumber::normilizeDigits() {
    while (digits.size() > 0 && digits[digits.size() - 1] == 0 && exp > 0) {
        digits.pop_back();
        exp--;
    }
}

void LongNumber::reverseDigits() {
    for (int i = 0; i < digits.size() / 2; i++) {
        swap(digits[i], digits[digits.size() - i - 1]);
    }
}


string LongNumber::toString() const {
    string result;
    if (sign) {
        result.push_back('-');
    }
    if (exp == 0) {
        result.push_back('0');
    }

    for (int i = 0; i < digits.size(); i++) {
        if (i == exp) {
            result.push_back('.');
        }
    
        char curDigit = digits[digits.size() - i - 1];
        result.push_back('0' + curDigit);
    }

    return result;
}

ostream& operator<< (ostream& out, const LongNumber num) {
    out << num.toString();
    return out;
}


LongNumber& LongNumber::operator+= (const LongNumber& other) {
    for (int i = exp; i <= other.exp; i++) {
        digits.push_back(0);
        exp++;
    }

    int precDiff = precision - other.precision;
    int thisOffset = precDiff > 0 ? precDiff : 0;
    int otherOffset = precDiff < 0 ? -precDiff : 0;
    
    int digitsCount = min(precision, other.precision) + exp - 1;
    for (int i = 0; i < digitsCount; i++) {
        int sm = digits[thisOffset + i] + other.digits[otherOffset + i];
        digits[thisOffset + i] = sm % 10;
        digits[thisOffset + i + 1] += sm / 10;
    }

    normilizeDigits();

    return *this;
}

LongNumber operator+ (const LongNumber& r, const LongNumber& l) {
    LongNumber temp(r);
    temp += l;
    return temp;
}


