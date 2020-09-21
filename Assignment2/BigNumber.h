//
// Created by AGY on 2020/9/20.
//

#ifndef ASSIGNMENT2_BIGNUMBER_H
#define ASSIGNMENT2_BIGNUMBER_H

#include <string>

using namespace std;

class CBigNumber {
public:
    typedef void (*CBNCALC) (const CBigNumber&,const CBigNumber&,CBigNumber&);
    CBigNumber();
    CBigNumber(string);
    ~CBigNumber();
    string toString();
    void releaseString();
    void init();
    void strToBigNumber(string);
    void adjustDigs();
    void moveDecimalPoint(int);
    int length() const;
    int compare(const CBigNumber&) const;
    int isZero() const;
    CBigNumber operator+(const CBigNumber&) const;
    CBigNumber operator-(const CBigNumber&) const;
    CBigNumber operator*(const CBigNumber&) const;
    CBigNumber operator/(const CBigNumber&) const;
    CBigNumber& operator+=(const CBigNumber&);
    CBigNumber& operator-=(const CBigNumber&);
    CBigNumber& operator*=(const CBigNumber&);
    CBigNumber& operator/=(const CBigNumber&);
    bool operator<(const CBigNumber&) const;
    bool operator>(const CBigNumber&) const;
    bool operator==(const CBigNumber&) const;
    bool operator<=(const CBigNumber&) const;
    bool operator>=(const CBigNumber&) const;
    bool operator!=(const CBigNumber&) const;
    static void add(const CBigNumber&,const CBigNumber&,CBigNumber&);
    static void subtract(const CBigNumber&,const CBigNumber&,CBigNumber&);
    static void multiply(const CBigNumber&,const CBigNumber&,CBigNumber&);
    static void divide(const CBigNumber&,const CBigNumber&,CBigNumber&);
    static string result(const string &,const string &,CBNCALC);
private:
    string intStr;
    string decStr;
    int intDigs;
    int decDigs;
    char sign;
};

class CZeroDivideException:exception{
public:
    static string message();
};


#endif //ASSIGNMENT2_BIGNUMBER_H
