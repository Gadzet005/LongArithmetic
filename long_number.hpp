#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>


using namespace std;

class LongNumber {
public:
    static const int DEFAULT_PRECISION = 10;

private:
    vector<char> digits;
    int exp;
    int precision;
    bool sign;

    void removeZeros();
    void reverseDigits();
    LongNumber removePoint() const;

    static int findDivDigit(LongNumber& num1, const LongNumber& num2);
    
    int getDigit(int idx) const;
    void setDigit(int idx, char value);
    void changeDigit(int idx, char value);

public:
    LongNumber();
    LongNumber(long double num);
    LongNumber(long double num, int prec);
    LongNumber(const string& str);
    LongNumber(const string& str, int prec);

    static LongNumber divide(const LongNumber& l, const LongNumber& r, int prec);

    string toString() const;
    LongNumber Abs() const;

    LongNumber operator - () const;

    LongNumber& operator += (const LongNumber& other);
    LongNumber& operator -= (const LongNumber& other);
    LongNumber& operator *= (const LongNumber& other);
    LongNumber& operator /= (const LongNumber& other);

    friend LongNumber operator + (const LongNumber& l, const LongNumber& r);
    friend LongNumber operator - (const LongNumber& l, const LongNumber& r);
    friend LongNumber operator * (const LongNumber& l, const LongNumber& r);
    friend LongNumber operator / (const LongNumber& l, const LongNumber& r);

    friend bool operator > (const LongNumber& l, const LongNumber& r);
    friend bool operator == (const LongNumber& l, const LongNumber& r);
};

LongNumber operator ""_LN (long double num);
LongNumber operator ""_LN (const char* str, size_t size);

ostream& operator << (ostream& out, const LongNumber& num);
istream& operator >> (istream& in, LongNumber& num);

bool operator != (const LongNumber& l, const LongNumber& r);
bool operator < (const LongNumber& l, const LongNumber& r);
bool operator >= (const LongNumber& l, const LongNumber& r);
bool operator <= (const LongNumber& l, const LongNumber& r);
