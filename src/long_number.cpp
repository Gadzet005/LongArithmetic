#include "long_number.hpp"


LongNumber::LongNumber() :
    exp(1), sign(), digits(1, 0), precision(LongNumber::DEFAULT_PRECISION) {}

LongNumber::LongNumber(long double num, unsigned int prec): exp(), precision(prec) {
    sign = num < 0;
    num = std::abs(num);

    long double intPart;
    num = std::modf(num, &intPart);
    while (intPart >= 1) {
        long double mod = std::fmod(intPart, 10);
        digits.push_back(mod);
        intPart /= 10;
        exp++;
    }
    if (digits.empty()) {
        digits.push_back(0);
        exp++;
    }

    reverseDigits();

    for (int i = 0; i < prec; i++) {
        num = std::modf(num * 10, &intPart);
        digits.push_back(intPart);
    }

    reverseDigits();
    removeZeros();
}

LongNumber::LongNumber(long double num) : LongNumber(num, LongNumber::DEFAULT_PRECISION) {}

LongNumber::LongNumber(const std::string& str, unsigned int prec) : sign(), exp(), precision(prec) {
    int startIdx = 0;
    if (str.size() > 1 && str[0] == '-') {
        sign = true;
        startIdx++;
    }

    size_t pointIdx = str.find('.');
    if (pointIdx == std::string::npos) {
        pointIdx = str.size();
    }

    if (pointIdx == 0) {
        throw std::invalid_argument("Некорректная строка");
    }

    // Целая часть числа

    long long curDigit = 0;
    int digitCounter = 0;

    int mod = (pointIdx - startIdx) % LBASE;
    for (int i = startIdx; i < startIdx + mod; i++) {
        addToDigit(curDigit, str[i]);
    }
    startIdx += mod;
    pushDigit(curDigit, digitCounter);
    exp++;

    for (int i = startIdx; i < pointIdx; i++) {
        addToDigit(curDigit, str[i]);
        digitCounter++;

        if (digitCounter == LBASE || i + 1 == pointIdx) {
            pushDigit(curDigit, digitCounter);
            exp++;
        }
    }

    // Дробная часть числа

    unsigned int precCounter = 0;
    for (int i = pointIdx + 1; i < str.size() && precCounter < prec; i++) {
        addToDigit(curDigit, str[i]);
        digitCounter++;
        precCounter++;

        if (digitCounter == LBASE) {
            pushDigit(curDigit, digitCounter);
        }
    }

    if (digitCounter != 0) {
        for (int i = digitCounter; i < LBASE; i++) {
            curDigit *= 10;
        }
        pushDigit(curDigit, digitCounter);
    }
    
    if (prec > precCounter) {
        double stop = static_cast<double>(prec - precCounter) / LBASE;
        for (double i = 0; i < stop; i++) {
            digits.push_back(0);
        }
    }

    reverseDigits();
    removeZeros();
}

LongNumber::LongNumber(const std::string& str) {
    size_t idx = str.find('.');
    int prec;
    if (idx == std::string::npos) {
        prec = 0;
    } else {
        prec = str.size() - idx - 1;
    }
    *this = LongNumber(str, prec);
}

LongNumber::LongNumber(const LongNumber& num, unsigned int prec) : 
    exp(num.exp), sign(num.sign), precision(prec) 
{
    long long precDiff = static_cast<long long>(num.digitsAfterPoint()) - 
                         static_cast<long long>(digitsAfterPoint());
    for (int i = 0; i < -precDiff; i++) {
        digits.push_back(0);
    }

    long long start = std::max(precDiff, static_cast<long long>(0));
    for (long long i = start; i < num.digits.size(); i++) {
        digits.push_back(num.digits[i]);
    }

    if (prec % LBASE != 0) {
        digits[0] -= digits[0] % static_cast<long long>(pow(10, LBASE - prec % LBASE));
    }

    removeZeros();
}

LongNumber operator ""_LN (long double num) {
    return LongNumber(num);
}

LongNumber operator ""_LN (const char* str, size_t size) {
    return LongNumber(str);
}


// Удаляет незначащие нули
void LongNumber::removeZeros() {
    while (digits.size() > 1 && getDigit(-1) == 0) {
        digits.pop_back();
        exp--;
    }
}

// Меняет порядок разрядов на противоположный
void LongNumber::reverseDigits() {
    for (int i = 0; i < digits.size() / 2; i++) {
        std::swap(digits[i], digits[digits.size() - i - 1]);
    }
}

// Делает дробную часть целой
LongNumber LongNumber::removePoint() const {
    LongNumber tmp = *this;
    tmp.exp = tmp.digits.size();
    tmp.precision = 0;
    return tmp;
}

// Вспомогаетльный метод для поиска очередной цифры при делении
long long LongNumber::findDivDigit(LongNumber& num1, const LongNumber& num2) {
    if (num1 < num2) {
        return 0;
    }

    long long left = 0;
    long long right = BASE;

    while (right > left) {
        long long mid = (right + left) / 2;

        LongNumber mult = num2 * LongNumber(std::to_string(mid), 0);

        if (mult <= num1 && mult + num2 > num1) {
            num1 -= mult;
            return mid;
        } else if (mult > num1) {
            right = mid;
        } else {
            left = mid;
        }
    }
    
    num1 -= LongNumber(std::to_string(left), 0) * num2;
    return left;
}

void LongNumber::pushZerosToStr(std::string& str, int count) const {
    str += std::move(std::string(count, '0'));
}

void LongNumber::addToDigit(long long& digit, char c) const {
    if (!isdigit(c)) {
        throw std::invalid_argument("Некорректная строка");
    }
    digit = 10 * digit + (c - '0');
}

void LongNumber::pushDigit(long long& digit, int& digitCounter) {
    digits.push_back(digit);
    digit = 0;
    digitCounter = 0;
}

bool LongNumber::isZero() const {
    return digits.size() == 1 && digits[0] == 0;
}


long long LongNumber::getDigit(int idx) const {
    if (idx < 0 && idx >= -digits.size()) {
        return digits[digits.size() + idx];
    }
    if (idx >= 0 && idx < digits.size()) {
        return digits[idx];
    }
    return 0;
}

void LongNumber::setDigit(int idx, long long value) {
    if (idx < digits.size()) {
        digits[idx] = value;
    } else {
        for (int i = digits.size(); i < idx; i++) {
            digits.push_back(0);
            exp++;
        }
        digits.push_back(value);
        exp++;
    }
}

void LongNumber::changeDigit(int idx, long long value) {
    setDigit(idx, getDigit(idx) + value);
}


LongNumber LongNumber::abs() const {
    LongNumber tmp = *this;
    tmp.sign = 0;
    return tmp;
}

unsigned int LongNumber::getPrecision() const {
    return precision;
}

unsigned int LongNumber::digitsAfterPoint() const {
    return ceil(static_cast<double>(precision) / LBASE);
}

std::string LongNumber::toString() const {
    std::string result;
    if (sign) {
        result.push_back('-');
    }

    // Целая часть

    if (exp <= 0) {
        result.push_back('0');
    }

    for (int i = 0; i < exp; i++) {
        long long cur = getDigit(-i - 1);
        std::string s = std::to_string(cur);
        if (i != 0) {
            pushZerosToStr(result, LBASE - s.size());
        }
        result += std::move(s);
    }

    // Дробная часть

    if (precision == 0) {
        return result;
    }

    result.push_back('.');
    int zeroCounter = std::max(-exp, 0) * LBASE;
    bool isEmpty = true;

    for (int i = std::max(0, exp); i < digits.size(); i++) {
        long long cur = getDigit(-i - 1);
        if (cur == 0) {
            zeroCounter += LBASE;
            continue;
        }

        std::string s = std::to_string(cur);
        pushZerosToStr(result, LBASE - s.size() + zeroCounter);
        zeroCounter = 0;

        while (s.back() == '0') {
            s.pop_back();
            zeroCounter++;
        }

        result += std::move(s);
        isEmpty = false;
    }

    if (isEmpty) {
        result.push_back('0');
    }

    return result;
}

std::ostream& operator << (std::ostream& out, const LongNumber& num) {
    out << num.toString();
    return out;
}

std::istream& operator >> (std::istream& in, LongNumber& num) {
    std::string s;
    in >> s;
    num = LongNumber(s);
    return in;
}


std::strong_ordering LongNumber::operator <=> (const LongNumber& right) const {
    const std::strong_ordering less = std::strong_ordering::less;
    const std::strong_ordering greater = std::strong_ordering::greater;
    const LongNumber& left = *this;

    if (left.isZero() && right.isZero()) {
        return std::strong_ordering::equal;
    }

    if (left.sign != right.sign) {
        return left.sign ? less : greater;
    }

    if (left.sign == right.sign && left.sign) {
        return (-right <=> -left);
    }

    if (left.exp != right.exp) {
        return left.exp < right.exp ? less : greater;
    }

    int digitsCount = std::max(left.digits.size(), right.digits.size());
    for (int i = 0; i < digitsCount; i++) {
        int leftDight = left.getDigit(-i - 1);
        int rightDigit = right.getDigit(-i - 1);
        
        if (leftDight != rightDigit) {
            return leftDight < rightDigit ? less : greater;
        }
    }

    return std::strong_ordering::equal;
}

bool LongNumber::operator == (const LongNumber& right) const {
    return ((*this) <=> right) == std::strong_ordering::equal;
}


LongNumber operator + (const LongNumber& left, const LongNumber& right) {
    // Если разные знаки, сводим сложение к вычитанию
    if (left.sign != right.sign) {
        return left - (-right);
    }

    // Выбираем число с наибольшей точностью
    LongNumber result = left.precision > right.precision ? left : right;
    const LongNumber& other = left.precision > right.precision ? right : left;
    
    int offset = result.digitsAfterPoint() - other.digitsAfterPoint();
    int digitsCount = other.digitsAfterPoint() + std::max(result.exp, other.exp);
    for (int i = 0; i < digitsCount; i++) {
        long long sum = result.getDigit(offset + i) + other.getDigit(i);
        // Текущий разряд
        result.setDigit(offset + i, sum % LongNumber::BASE);
        // Перенос единицы на следующий разряд
        if (sum >= LongNumber::BASE) {
            result.changeDigit(offset + i + 1, 1);
        }
    }

    result.removeZeros();
    return result;
}

LongNumber& LongNumber::operator += (const LongNumber& other) {
    *this = std::move(*this + other);
    return *this;
}


LongNumber operator - (const LongNumber& left, const LongNumber& right) {
    // Если разные знаки, сводим вычитание к сложению
    if (left.sign != right.sign) {
        return left + (-right);
    }

    // Вычитаем из большего меньшее, если это не так, то меняем слагаемые местами со сменой знака
    if ((left.sign && left > right) || (!left.sign && left < right)) {
        return -(right - left);
    }

    // Результат будет иметь наибольшую точность
    LongNumber result = LongNumber(left, std::max(left.precision, right.precision));

    int offset = result.digitsAfterPoint() - right.digitsAfterPoint();
    int digitsCount = std::min(result.digitsAfterPoint(), right.digitsAfterPoint()) + std::max(result.exp, right.exp);
    for (int i = 0; i < digitsCount; i++) {
        long long sm = result.getDigit(offset + i) - right.getDigit(i);
        if (sm < 0) {
            result.setDigit(offset + i, sm + LongNumber::BASE);
            result.changeDigit(offset + i + 1, -1);
        } else {
            result.setDigit(offset + i, sm);
        }
    }

    result.removeZeros();
    return result;
}

LongNumber& LongNumber::operator -= (const LongNumber& other) {
    *this = std::move(*this - other);
    return *this;
}

LongNumber LongNumber::operator - () const {
    LongNumber tmp = *this;
    tmp.sign = !tmp.sign;
    return tmp;
}


LongNumber operator * (const LongNumber& left, const LongNumber& right) {
    LongNumber result("0", 0);
    result.sign = left.sign != right.sign;

    for (int i = 0; i < left.digits.size(); i++) {
        for (int j = 0; j < right.digits.size(); j++) {
            long long change = result.getDigit(i + j) + left.digits[i] * right.digits[j];
            long long div = change / LongNumber::BASE;
            long long mod = change % LongNumber::BASE;

            result.setDigit(i + j, mod);
            result.changeDigit(i + j + 1, div);
        }
    }

    result.exp = left.exp + right.exp;
    result.removeZeros();
    result.precision = (result.digits.size() - std::max(0, result.exp)) * LongNumber::LBASE;
    result = std::move(LongNumber(result, left.precision + right.precision));

    return result;
}

LongNumber& LongNumber::operator *= (const LongNumber& other) {
    *this = std::move(*this * other);
    return *this;
}


// Деление с заданной точностью
LongNumber LongNumber::divide(const LongNumber& left, const LongNumber& right, int prec) {
    if (right.isZero()) {
        throw divisionByZero();
    }

    LongNumber result("0", 0);
    result.sign = left.sign != right.sign;

    // Считаем, что числа положительные и целые
    LongNumber leftAbs = left.removePoint().abs();
    LongNumber rightAbs = right.removePoint().abs();

    const LongNumber base = LongNumber(std::to_string(BASE), 0);

    // Целая часть частного
    LongNumber cur("0", 0);
    for (int digitIdx = 0; digitIdx < leftAbs.exp; digitIdx++) {
        LongNumber curDigit = LongNumber(std::to_string(leftAbs.getDigit(-digitIdx - 1)), 0);
        cur = base * cur + curDigit;
        result.digits.push_back(LongNumber::findDivDigit(cur, rightAbs));
        result.exp++;
    }

    // Дробная часть частного
    int precDiff = (right.digitsAfterPoint() - left.digitsAfterPoint());
    int end = ceil(static_cast<double>(prec) / LBASE) + precDiff;
    for (int i = 0; i < end; i++) {
        cur *= base;
        result.digits.push_back(LongNumber::findDivDigit(cur, rightAbs));
    }

    result.reverseDigits();
    result.exp += precDiff;
    result.removeZeros();
    result.precision = std::max(prec, -precDiff);

    return LongNumber(result, prec);
}

LongNumber operator / (const LongNumber& left, const LongNumber& right) {
    unsigned int defaultPrecision = LongNumber::DEFAULT_PRECISION;
    int maxPrecision = std::max(std::max(left.precision, right.precision), defaultPrecision);
    return LongNumber::divide(left, right, maxPrecision);
}

LongNumber& LongNumber::operator /= (const LongNumber& other) {
    *this = std::move(*this / other);
    return *this;
}
