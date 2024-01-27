#include "long_number.hpp"


LongNumber::LongNumber():
    exp(), sign(), precision(LongNumber::DEFAULT_PRECISION) {}

LongNumber::LongNumber(long double num, int prec): exp(), precision(prec) {
    if (prec <= 0) {
        throw invalid_argument("Точность должна быть > 0");
    }

    sign = num < 0;
    num = abs(num);

    long double intPart;
    num = modf(num, &intPart);
    while (intPart >= 1) {
        long double mod = fmod(intPart, 10);
        digits.push_back(mod);
        intPart /= 10;
        exp++;
    }

    reverseDigits();

    while (prec > 0) {
        num *= 10;
        num = modf(num, &intPart);
        digits.push_back(intPart);
        prec--;
    }

    reverseDigits();
    removeZeros();
}

LongNumber::LongNumber(long double num): 
    LongNumber(num, LongNumber::DEFAULT_PRECISION) {}

LongNumber operator ""_LN (long double num) {
    return LongNumber(num);
}


// Удаляет незначащие нули из целой части
void LongNumber::removeZeros() {
    while (exp > 0 && digits.size() > 0 && getDigit(-1) == 0) {
        digits.pop_back();
        exp--;
    }
}

void LongNumber::reverseDigits() {
    for (int i = 0; i < digits.size() / 2; i++) {
        swap(digits[i], digits[digits.size() - i - 1]);
    }
}


int LongNumber::getDigit(int idx) const {
    if (idx < 0 && idx >= -digits.size()) {
        return digits[digits.size() + idx];
    }
    if (idx >= 0 && idx < digits.size()) {
        return digits[idx];
    }
    return 0;
}

void LongNumber::setDigit(int idx, char value) {
    if (idx < digits.size()) {
        digits[idx] = value;
    } else {
        digits.push_back(value);
        exp++;
    }
}

void LongNumber::changeDigit(int idx, char value) {
    setDigit(idx, getDigit(idx) + value);
}


string LongNumber::toString() const {
    string result;
    if (sign) {
        result.push_back('-');
    }
    if (exp == 0) {
        result.push_back('0');
    }

    int zeroCounter = 0;
    bool haveDigitAfterPoint = false;
    for (int i = 0; i < digits.size(); i++) {
        // Разделяем целую и дробную часть
        if (i == exp) {
            for (int j = 0; j < zeroCounter; j++) {
                result.push_back('0');
            }
            zeroCounter = 0;
            haveDigitAfterPoint = false;
            result.push_back('.');
        }
    
        char curDigit = getDigit(-i - 1);
        if (curDigit == 0) {
            zeroCounter++;
        } else {
            for (int j = 0; j < zeroCounter; j++) {
                result.push_back('0');
            }
            zeroCounter = 0;
            haveDigitAfterPoint = true;
            result.push_back('0' + curDigit);
        }
    }

    if (!haveDigitAfterPoint) {
        result.push_back('0');
    }

    return result;
}

ostream& operator << (ostream& out, const LongNumber num) {
    out << num.toString();
    return out;
}


bool operator > (const LongNumber& l, const LongNumber& r) {
    if (l.sign != r.sign) {
        return l.sign < r.sign;
    }
    if (l.sign == r.sign && l.sign) {
        return (-r > -l);
    }
    if (l.exp != r.exp) {
        return l.exp > r.exp;
    }

    int digitsCount = l.exp + max(l.precision, r.precision);
    for (int i = 0; i < digitsCount; i++) {
        int ld = l.getDigit(-i - 1);
        int rd = r.getDigit(-i - 1);
        
        if (ld != rd) {
            return ld > rd;
        }
    }

    return false;
}

bool operator < (const LongNumber& l, const LongNumber& r) {
    return r > l;
}


LongNumber& LongNumber::operator += (const LongNumber& other) {
    // Если разные знаки, сводим сложение к вычитанию
    if (sign != other.sign) {
        *this -= -other;
        return *this;
    }

    int precDiff = precision - other.precision;
    int thisOffset = precDiff > 0 ? precDiff : 0;
    int otherOffset = precDiff < 0 ? -precDiff : 0;
    
    int digitsCount = min(precision, other.precision) + max(exp, other.exp);
    for (int i = 0; i < digitsCount; i++) {
        int sum = getDigit(thisOffset + i) + other.getDigit(otherOffset + i);

        // Текущий разряд
        setDigit(thisOffset + i, sum % 10);

        // Перенос единицы на следующий разряд
        if (sum >= 10) {
            changeDigit(thisOffset + i + 1, 1);
        }
    }

    removeZeros();
    return *this;
}

LongNumber operator + (const LongNumber& l, const LongNumber& r) {
    LongNumber temp(r);
    temp += l;
    return temp;
}


LongNumber& LongNumber::operator -= (const LongNumber& other) {
    // Если разные знаки, сводим вычитание к сложению
    if (sign != other.sign) {
        *this += -other;
        return *this; 
    }

    // Вычитаем из большего меньшее, если это не так, то меняем слагаемые местами со сменой знака
    if ((!sign && other > *this) || (sign && other < *this)) {
        LongNumber tmp = other;
        tmp -= *this;
        *this = tmp;
        this->sign = !(this->sign);
        return *this;
    }

    int precDiff = precision - other.precision;
    int thisOffset = precDiff > 0 ? precDiff : 0;
    int otherOffset = precDiff < 0 ? -precDiff : 0;
    
    int digitsCount = min(precision, other.precision) + max(exp, other.exp);
    for (int i = 0; i < digitsCount; i++) {
        int sm = getDigit(thisOffset + i) - other.getDigit(otherOffset + i);
        if (sm < 0) {
            sm += 10;
            changeDigit(thisOffset + i + 1, -1);
        }
        setDigit(thisOffset + i, sm);
    }

    removeZeros();

    return *this;
}

LongNumber operator - (const LongNumber& l, const LongNumber& r) {
    LongNumber temp(r);
    temp -= l;
    return temp;
}

LongNumber LongNumber::operator - () const {
    LongNumber tmp = *this;
    tmp.sign = !tmp.sign;
    return tmp;
}


LongNumber operator * (const LongNumber& l, const LongNumber& r) {
    LongNumber result;
    result.precision = r.precision + l.precision;
    result.sign = l.sign != r.sign;

    for (int i = 0; i < l.digits.size(); i++) {
        for (int j = 0; j < r.digits.size(); j++) {
            int change = result.getDigit(i + j) + l.digits[i] * r.digits[j];
            result.setDigit(i + j, change % 10);
            if (change >= 10) {
                result.changeDigit(i + j + 1, change / 10);
            }
        }
    }

    result.exp = max(result.exp - result.precision, 0);
    result.removeZeros();
    return result;
}

LongNumber& LongNumber::operator *= (const LongNumber& other) {
    *this = *this * other;
    return *this;
}