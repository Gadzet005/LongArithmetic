#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>


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

public:
    LongNumber();
    LongNumber(double num);
    LongNumber(double num, int prec);

    string toString() const;

    LongNumber& operator+= (const LongNumber& other);
    LongNumber& operator-= (const LongNumber& other);
    LongNumber& operator*= (const LongNumber& other);
    LongNumber& operator/= (const LongNumber& other);

    bool operator== (const LongNumber& other) const;
    bool operator!= (const LongNumber& other) const;
    bool operator> (const LongNumber& other) const;
    bool operator< (const LongNumber& other) const;
};

ostream& operator<< (ostream& out, const LongNumber num);

LongNumber operator+ (const LongNumber& r, const LongNumber& l);
LongNumber operator- (const LongNumber& r, const LongNumber& l);
LongNumber operator* (const LongNumber& r, const LongNumber& l);
LongNumber operator/ (const LongNumber& r, const LongNumber& l);