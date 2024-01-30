#include "long_number.hpp"


LongNumber::LongNumber() :
    exp(1), sign(), digits(1, 0), precision(LongNumber::DEFAULT_PRECISION) {}

LongNumber::LongNumber(long double num, int prec): exp(), precision(prec) {
    if (prec < 0) {
        throw invalid_argument("Точность должна быть >= 0");
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
    if (digits.empty()) {
        digits.push_back(0);
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

LongNumber::LongNumber(long double num) : 
    LongNumber(num, LongNumber::DEFAULT_PRECISION) {}

LongNumber::LongNumber(const string& str, int prec) : sign(), exp(), precision(prec) {
    if (prec < 0) {
        throw invalid_argument("Точность должна быть >= 0");
    }

    int start = 0;
    if (str.size() > 0 && str[0] == '-') {
        sign = true;
        start++;
    }
    
    bool intPart = true;
    for (int i = start; i < str.size(); i++) {
        if (prec == 0 && !intPart) {
            break;
        }
        
        char cur = str[i];

        if (!isdigit(cur) && !(cur == '.' && intPart)) {
            throw invalid_argument("В строке должно находиться корректное число");
        }

        if (cur == '.') {
            intPart = false;
            continue;
        } else {
            digits.push_back(cur - '0');
        }

        if (intPart) {
            exp++;
        } else {
            prec--;
        }
    }

    while (prec > 0) {
        digits.push_back(0);
        prec--;
    }

    reverseDigits();
    removeZeros();
}

LongNumber::LongNumber(const string& str) {
    size_t idx = str.find('.');
    int prec;
    if (idx == string::npos) {
        prec = 0;
    } else {
        prec = str.size() - idx - 1;
    }
    *this = LongNumber(str, prec);
}

LongNumber operator ""_LN (long double num) {
    return LongNumber(num);
}

LongNumber operator ""_LN (const char* str, size_t size) {
    return LongNumber(str);
}


// Удаляет незначащие нули из целой части
void LongNumber::removeZeros() {
    while (exp > 1 && digits.size() > 0 && getDigit(-1) == 0) {
        digits.pop_back();
        exp--;
    }
}

// Меняет порядок разрядов на противоположный
void LongNumber::reverseDigits() {
    for (int i = 0; i < digits.size() / 2; i++) {
        swap(digits[i], digits[digits.size() - i - 1]);
    }
}

// Делает дробную часть целой
LongNumber LongNumber::removePoint() const {
    LongNumber tmp = *this;
    tmp.exp += tmp.precision;
    tmp.precision = 0;
    tmp.removeZeros();
    return tmp;
}

// Вспомогаетльный метод для поиска очередной цифры при делении
int LongNumber::findDivDigit(LongNumber& num1, const LongNumber& num2) {
    int n = 0;
    while (num1 >= num2) {
        num1 -= num2;
        n++;
    }
    return n;
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


LongNumber LongNumber::Abs() const {
    LongNumber tmp = *this;
    tmp.sign = 0;
    return tmp;
}


string LongNumber::toString() const {
    string result;
    if (sign) {
        result.push_back('-');
    }

    int zeroCounter = 0;
    bool haveDigitAfterPoint = true;
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
    if (precision == 0) {
        for (int j = 0; j < zeroCounter; j++) {
            result.push_back('0');
        }
    }

    return result;
}

ostream& operator << (ostream& out, const LongNumber& num) {
    out << num.toString();
    return out;
}

istream& operator >> (istream& in, LongNumber& num) {
    string s;
    in >> s;
    num = LongNumber(s);
    return in;
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

bool operator == (const LongNumber& l, const LongNumber& r) {
    if (l.sign != r.sign || l.exp != r.exp) {
        return false;
    }

    int digitsCount = l.exp + max(l.precision, r.precision);
    for (int i = 0; i < digitsCount; i++) {
        if (l.getDigit(-i - 1) != r.getDigit(-i - 1)) {
            return false;
        }
    }
    return true;
}

bool operator != (const LongNumber& l, const LongNumber& r) {
    return !(l == r);
}

bool operator >= (const LongNumber& l, const LongNumber& r) {
    return (l > r) || (l == r);
}

bool operator <= (const LongNumber& l, const LongNumber& r) {
    return r >= l;
}


LongNumber operator + (const LongNumber& l, const LongNumber& r) {
    // Если разные знаки, сводим сложение к вычитанию
    if (l.sign != r.sign) {
        return l - (-r);
    }

    // Выбираем число с наибольшей точностью
    LongNumber result = l.precision > r.precision ? l : r;
    const LongNumber& other = l.precision > r.precision ? r : l;
    
    int offset = abs(l.precision - r.precision);
    int digitsCount = min(l.precision, r.precision) + max(l.exp, r.exp);
    for (int i = 0; i < digitsCount; i++) {
        int sum = result.getDigit(offset + i) + other.getDigit(i);
        // Текущий разряд
        result.setDigit(offset + i, sum % 10);
        // Перенос единицы на следующий разряд
        if (sum >= 10) {
            result.changeDigit(offset + i + 1, 1);
        }
    }

    result.removeZeros();
    return result;
}

LongNumber& LongNumber::operator += (const LongNumber& other) {
    *this = *this + other;
    return *this;
}


LongNumber operator - (const LongNumber& l, const LongNumber& r) {
    // Если разные знаки, сводим вычитание к сложению
    if (l.sign != r.sign) {
        return l + (-r);
    }

    // Вычитаем из большего меньшее, если это не так, то меняем слагаемые местами со сменой знака
    if ((l.sign && l > r) || (!l.sign && l < r)) {
        return -(r - l);
    }

    // Результат будет иметь наибольшую точность
    LongNumber result = LongNumber(0, max(l.precision, r.precision)) + l;

    int offset = result.precision - r.precision;
    int digitsCount = min(result.precision, r.precision) + max(result.exp, r.exp);
    for (int i = 0; i < digitsCount; i++) {
        int sm = result.getDigit(offset + i) - r.getDigit(i);
        if (sm < 0) {
            result.setDigit(offset + i, sm + 10);
            result.changeDigit(offset + i + 1, -1);
        } else {
            result.setDigit(offset + i, sm);
        }
    }

    result.removeZeros();
    return result;
}

LongNumber& LongNumber::operator -= (const LongNumber& other) {
    *this = *this - other;
    return *this;
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

    result.exp -= result.precision;
    result.removeZeros();
    return result;
}

LongNumber& LongNumber::operator *= (const LongNumber& other) {
    *this = *this * other;
    return *this;
}


// Деление с заданной точностью
LongNumber LongNumber::divide(const LongNumber& l, const LongNumber& r, int prec) {
    if (r == 0) {
        throw runtime_error("Деление на 0");
    }

    LongNumber result(0, 0);
    result.digits.clear();
    result.exp = 0;
    result.sign = l.sign != r.sign;
    result.precision = prec;

    // Считаем, что числа положительные и целые
    LongNumber left = l.removePoint().Abs();
    LongNumber right = r.removePoint().Abs();

    const LongNumber ten = LongNumber(10, 0);

    // Целая часть частного
    int digitIdx = 0;
    LongNumber cur(0, 0);
    while (digitIdx < left.exp) {
        LongNumber curDigit = LongNumber(left.getDigit(-digitIdx - 1), 0);
        cur = ten * cur + curDigit;
        result.digits.push_back(LongNumber::findDivDigit(cur, right));
        result.exp++;
        digitIdx++;
    }

    // Дробная часть частного
    int precDiff = r.precision - l.precision;
    for (int i = 0; i < prec + precDiff; i++) {
        cur *= ten;
        result.digits.push_back(LongNumber::findDivDigit(cur, right));
    }

    result.reverseDigits();

    for (int i = 0; i <= -(result.exp + precDiff); i++) {
        result.digits.push_back(0);
    }
    result.exp = max(result.exp + precDiff, 1);

    result.removeZeros();
    return result;
}

LongNumber operator / (const LongNumber& l, const LongNumber& r) {
    int defaultPrecision = LongNumber::DEFAULT_PRECISION;
    int maxPrecision = max(max(l.precision, r.precision), defaultPrecision);
    return LongNumber::divide(l, r, maxPrecision);
}

LongNumber& LongNumber::operator /= (const LongNumber& other) {
    *this = *this / other;
    return *this;
}

