//
// Created by AGY on 2020/9/20.
//

#ifndef ASSIGNMENT2_BIGNUMBER_H
#define ASSIGNMENT2_BIGNUMBER_H

#include <string>

using namespace std;

class CBigNumber {
public:
    typedef void (*CBNCALC)(const CBigNumber &, const CBigNumber &, CBigNumber &);

    CBigNumber();

    explicit CBigNumber(string);

    ~CBigNumber();

    string toString();

    void init();

    void strToBigNumber(string);

    void adjustDigs();

    void moveDecimalPoint(int);

    // length of the number hold in this CBigNumber
    int length() const;

    // return 1 if this is bigger, 0 if equals, -1 if lesser
    int compare(const CBigNumber &) const;

    bool isZero() const;

    CBigNumber operator+(const CBigNumber &) const;

    CBigNumber operator-(const CBigNumber &) const;

    CBigNumber operator*(const CBigNumber &) const;

    CBigNumber operator/(const CBigNumber &) const;

    CBigNumber &operator+=(const CBigNumber &);

    CBigNumber &operator-=(const CBigNumber &);

    CBigNumber &operator*=(const CBigNumber &);

    CBigNumber &operator/=(const CBigNumber &);

    bool operator<(const CBigNumber &) const;

    bool operator>(const CBigNumber &) const;

    bool operator==(const CBigNumber &) const;

    bool operator<=(const CBigNumber &) const;

    bool operator>=(const CBigNumber &) const;

    bool operator!=(const CBigNumber &) const;

    static void add(const CBigNumber &, const CBigNumber &, CBigNumber &);

    static void subtract(const CBigNumber &, const CBigNumber &, CBigNumber &);

    static void multiply(const CBigNumber &, const CBigNumber &, CBigNumber &);

    static void divide(const CBigNumber &, const CBigNumber &, CBigNumber &);

    static string result(const string &, const string &, CBNCALC);

private:
    string intStr;
    string decStr;
    int intDigs;
    int decDigs;
    // "+" == 0, "-" == 1
    char sign;
};

//throws when one CBigNumber is divided by 0
class CZeroDivideException : exception {
public:
    const char *what() { /* NOLINT */
        return "CBigNumber: Divided By Zero ! ";;
    }
};


#endif //ASSIGNMENT2_BIGNUMBER_H
