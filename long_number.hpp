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
    LongNumber(string str);
    LongNumber(string str, int prec);

    string toString() const;
    LongNumber getAbs() const;
    LongNumber operator - () const;

    LongNumber& operator += (const LongNumber& other);
    LongNumber& operator -= (const LongNumber& other);
    LongNumber& operator *= (const LongNumber& other);
    LongNumber& operator /= (const LongNumber& other);

    friend bool operator > (const LongNumber&, const LongNumber&);
    friend bool operator == (const LongNumber&, const LongNumber&);
    friend LongNumber operator - (const LongNumber&, const LongNumber&);
    friend LongNumber operator + (const LongNumber&, const LongNumber&);
    friend LongNumber operator * (const LongNumber&, const LongNumber&);
    friend LongNumber operator / (const LongNumber&, const LongNumber&);
};

LongNumber operator ""_LN (long double num);
LongNumber operator ""_LN (const char* str, size_t size);

ostream& operator<< (ostream& out, const LongNumber num);

LongNumber operator + (const LongNumber& l, const LongNumber& r);
LongNumber operator - (const LongNumber& l, const LongNumber& r);
LongNumber operator * (const LongNumber& l, const LongNumber& r);
LongNumber operator / (const LongNumber& l, const LongNumber& r);

bool operator == (const LongNumber& l, const LongNumber& r);
bool operator != (const LongNumber& l, const LongNumber& r);
bool operator > (const LongNumber& l, const LongNumber& r);
bool operator < (const LongNumber& l, const LongNumber& r);
bool operator >= (const LongNumber& l, const LongNumber& r);
bool operator <= (const LongNumber& l, const LongNumber& r);
