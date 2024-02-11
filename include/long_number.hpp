#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "exceptions.hpp"


class LongNumber {
public:
    static const unsigned int DEFAULT_PRECISION = 10;
    static const unsigned long long BASE = 1e9;
    static const unsigned int LBASE = 9;

private:
    std::vector<long long> digits;
    int exp;
    unsigned int precision;
    bool sign;

    void removeZeros();
    void reverseDigits();
    LongNumber removePoint() const;
    void pushZerosToStr(std::string& str, int count) const;
    void addToDigit(long long& digit, char c) const;
    void pushDigit(long long& digit, int& digitCounter);

    static long long findDivDigit(LongNumber& num1, const LongNumber& num2);
    
    long long getDigit(int idx) const;
    void setDigit(int idx, long long value);
    void changeDigit(int idx, long long value);

public:
    LongNumber();
    explicit LongNumber(long double num);
    explicit LongNumber(long double num, unsigned int prec);
    explicit LongNumber(const std::string& str);
    explicit LongNumber(const std::string& str, unsigned int prec);
    explicit LongNumber(const LongNumber& num, unsigned int prec);

    static LongNumber divide(const LongNumber& left, const LongNumber& right, int prec);

    std::string toString() const;
    LongNumber abs() const;
    unsigned int getPrecision() const;
    unsigned int digitsAfterPoint() const;
    bool isZero() const;

    std::strong_ordering operator <=> (const LongNumber& right) const;
    bool operator == (const LongNumber& right) const;

    LongNumber operator - () const;

    LongNumber& operator += (const LongNumber& other);
    LongNumber& operator -= (const LongNumber& other);
    LongNumber& operator *= (const LongNumber& other);
    LongNumber& operator /= (const LongNumber& other);

    friend LongNumber operator + (const LongNumber& left, const LongNumber& right);
    friend LongNumber operator - (const LongNumber& left, const LongNumber& right);
    friend LongNumber operator * (const LongNumber& left, const LongNumber& right);
    friend LongNumber operator / (const LongNumber& left, const LongNumber& right);
};

LongNumber operator ""_LN (long double num);
LongNumber operator ""_LN (const char* str, size_t size);

std::ostream& operator << (std::ostream& out, const LongNumber& num);
std::istream& operator >> (std::istream& in, LongNumber& num);
