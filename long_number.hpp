#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>


using namespace std;

class LongNumber {
public:
    static const int DEFAULT_PRECISION = 5;

private:
    vector<char> digits;
    int exp;
    int precision;
    bool sign;

    void normilizeDigits();
    void reverseDigits();
    
    int getDigit (int idx) const;
    void setDigit (int idx, char value);

public:
    LongNumber();
    LongNumber(long double num);
    LongNumber(long double num, int prec);

    string toString() const;

    LongNumber operator - () const;

    LongNumber& operator += (const LongNumber& other);
    LongNumber& operator -= (const LongNumber& other);
    LongNumber& operator *= (const LongNumber& other);
    LongNumber& operator /= (const LongNumber& other);

    friend bool operator > (const LongNumber& l, const LongNumber& r);
    friend bool operator < (const LongNumber& l, const LongNumber& r);
};

LongNumber operator ""_LN (long double num);

ostream& operator<< (ostream& out, const LongNumber num);

LongNumber operator + (const LongNumber& r, const LongNumber& l);
LongNumber operator - (const LongNumber& r, const LongNumber& l);
LongNumber operator * (const LongNumber& r, const LongNumber& l);
LongNumber operator / (const LongNumber& r, const LongNumber& l);

bool operator == (const LongNumber& l, const LongNumber& r);
bool operator != (const LongNumber& l, const LongNumber& r);
bool operator > (const LongNumber& l, const LongNumber& r);
bool operator < (const LongNumber& l, const LongNumber& r);
