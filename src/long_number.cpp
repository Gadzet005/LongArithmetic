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
    int start = 0;
    if (str.size() > 1 && str[0] == '-') {
        sign = true;
        start++;
    }
    
    bool intPart = true;
    unsigned int precCounter = 0;
    for (int i = start; i < str.size(); i++) {
        if (prec == 0 && !intPart) {
            break;
        }
        
        char cur = str[i];

        if (!isdigit(cur) && !(cur == '.' && intPart && i != start)) {
            throw std::invalid_argument("В строке должно находиться корректное число");
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
            precCounter++;
            if (precCounter == prec) {
                break;
            }
        }
    }

    for (; precCounter < prec; precCounter++) {
        digits.push_back(0);
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
    long long precDiff = static_cast<long long>(num.precision) - static_cast<long long>(prec);
    for (int i = 0; i < -precDiff; i++) {
        digits.push_back(0);
    }

    long long start = std::max(precDiff, static_cast<long long>(0));
    for (long long i = start; i < num.digits.size(); i++) {
        digits.push_back(num.digits[i]);
    }
}

LongNumber operator ""_LN (long double num) {
    return LongNumber(num);
}

LongNumber operator ""_LN (const char* str, size_t size) {
    return LongNumber(str);
}


// Удаляет незначащие нули из целой части
void LongNumber::removeZeros() {
    while (digits.size() > 1 && digits.back() == 0) {
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
int LongNumber::findDivDigit(LongNumber& num1, const LongNumber& num2) {
    int n = 0;
    while (num1 >= num2) {
        num1 -= num2;
        n++;
    }
    return n;
}

void LongNumber::pushZerosToStr(std::string& str, int& count) const {
    for (int j = 0; j < count; j++) {
        str.push_back('0');
    }
    count = 0;
}

bool LongNumber::isZero() const {
    return digits.size() == 1 && digits[0] == 0;
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
        for (int i = digits.size(); i < idx; i++) {
            digits.push_back(0);
            exp++;
        }
        digits.push_back(value);
        exp++;
    }
}

void LongNumber::changeDigit(int idx, char value) {
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
        char cur = getDigit(-i - 1);
        result.push_back(cur + '0');
    }

    // Дробная часть

    if (precision == 0) {
        return result;
    }

    result.push_back('.');
    int zeroCounter = std::max(-exp, 0);
    bool isEmpty = true;

    for (int i = std::max(0, exp); i < digits.size(); i++) {
        char cur = getDigit(-i - 1);
        if (cur == 0) {
            zeroCounter++;
        } else {
            pushZerosToStr(result, zeroCounter);
            zeroCounter = 0;
            result.push_back(cur + '0');
            isEmpty = false;
        }
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
    
    int offset = result.precision - other.precision;
    int digitsCount = other.precision + std::max(result.exp, other.exp);
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

    int offset = result.precision - right.precision;
    int digitsCount = std::min(result.precision, right.precision) + std::max(result.exp, right.exp);
    for (int i = 0; i < digitsCount; i++) {
        int sm = result.getDigit(offset + i) - right.getDigit(i);
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
    result.precision = right.precision + left.precision;
    result.sign = left.sign != right.sign;

    for (int i = 0; i < left.digits.size(); i++) {
        for (int j = 0; j < right.digits.size(); j++) {
            int change = result.getDigit(i + j) + left.digits[i] * right.digits[j];
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

    const LongNumber ten = LongNumber("10", 0);

    // Целая часть частного
    LongNumber cur("0", 0);
    for (int digitIdx = 0; digitIdx < leftAbs.exp; digitIdx++) {
        LongNumber curDigit = LongNumber(leftAbs.getDigit(-digitIdx - 1), 0);
        cur = ten * cur + curDigit;
        result.digits.push_back(LongNumber::findDivDigit(cur, rightAbs));
        result.exp++;
    }

    // Дробная часть частного
    int precDiff = right.precision - left.precision;
    for (int i = 0; i < prec + precDiff; i++) {
        cur *= ten;
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
