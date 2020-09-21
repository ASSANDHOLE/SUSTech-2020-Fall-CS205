//
// Created by AGY on 2020/9/20.
//

#include "BigNumber.h"
#include <string>
#include <algorithm>

//the max length of decimal numbers when divide happens
const int DIVIDE_MAX_LENGTH = 255;

CBigNumber::CBigNumber() {
    init();
}

CBigNumber::CBigNumber(string str) {
    init();
    strToBigNumber(str);
}

CBigNumber::~CBigNumber() {
    init();
}

void CBigNumber::init() {
    intDigs = 0;
    decDigs = 0;
    intStr = "";
    decStr = "";
    sign = 0;
}

int CBigNumber::length() const {
    int len = intDigs + decDigs;
    if (intDigs == 0) {
        len++;
    }
    if (decDigs) {
        len++;
    }
    if (sign) {
        len++;
    }
    return len;
}

void CBigNumber::strToBigNumber(string numStr) {
    init();
    int pos;
    if (numStr.at(0) == '-') {
        numStr.replace(0, 1, "");
        sign = 1;
    }
    pos = numStr.find('.');
    if (pos != string::npos) {
        intDigs = pos;
        if (pos != 0) {
            intStr = numStr.substr(0, pos);
        } else {
            intDigs++;
            intStr = "0";
        }
        if (pos != numStr.length() - 1) {
            decDigs = (int) numStr.length() - 1 - pos;
            decStr = numStr.substr(pos + 1, decDigs);
        } else {
            decDigs = 0;
            decStr = "";
        }
    } else {
        intDigs = numStr.length();
        intStr = numStr;
        decDigs = 0;
        decStr = "";
    }
    adjustDigs();
    if (!decDigs) {
        if (!intDigs || intDigs == 1 && intStr == "0") {
            sign = 0;
        }
    }
    if (intDigs != 0 && intDigs != 1) {
        reverse(intStr.begin(), intStr.end());
    }
}

string CBigNumber::toString() {
    adjustDigs();
    string str = intStr;
    if (isZero()) {
        return "0";
    }
    if (intDigs != 0 && intDigs != 1) {
        reverse(str.begin(), str.end());
    }
    if (sign == 1) {
        str = "-" + str;
    }
    if (decDigs == 0) {
        return str;
    }
    return str + '.' + decStr;
}

void CBigNumber::adjustDigs() {
    if (intDigs > 1) {
        reverse(intStr.begin(), intStr.end());
        while (intStr.at(0) == '0') {
            intStr.replace(0, 1, "");
            intDigs--;
        }
    }
    while (decDigs > 0 && decStr.at(decDigs - 1) == '0') {
        decStr.replace(decDigs - 1, 1, "");
        decDigs--;
    }
    if (intDigs != 0 && intDigs != 1) {
        reverse(intStr.begin(), intStr.end());
    }
    if (intDigs == 0) {
        intDigs++;
        intStr = "0";
    }
}

void CBigNumber::moveDecimalPoint(int num) {
    if (num == 0) {
        return;
    } else if (num < 0) {
        if (intDigs != 0 && intDigs != 1) {
            reverse(intStr.begin(), intStr.end());
        }
        for (int i = 0; i < -num; ++i) {
            char c;
            if (intDigs < 1) {
                intDigs = 1;
                intStr = "0";
            }
            c = intStr.at(intDigs - 1);
            intStr.replace(intDigs - 1, 1, "");
            intDigs -= intDigs > 0 ? 1 : 0;
            if (decDigs == 0) {
                decDigs++;
                decStr = "0";
            }
            decStr.replace(0, 1, string(1, c) + string(1, decStr.at(0)));
            decDigs++;
        }
    } else {
        if (intDigs != 0 && intDigs != 1) {
            reverse(intStr.begin(), intStr.end());
        }
        for (int i = 0; i < num; ++i) {
            char c;
            if (decDigs > 0) {
                c = decStr.at(0);
            } else {
                c = '0';
            }
            intStr.push_back(c);
            intDigs++;
            if (decDigs > 0) {
                decStr.replace(0, 1, "");
                decDigs--;
            }
        }
    }
    if (intDigs != 0 && intDigs != 1) {
        reverse(intStr.begin(), intStr.end());
    }
    adjustDigs();
}

int CBigNumber::compare(const CBigNumber &BN2) const {
    if (!sign && BN2.sign) {
        return 1;
    } else if (sign && !BN2.sign) {
        return -1;
    } else {
        if (intDigs > BN2.intDigs) {
            return sign ? -1 : 1;
        } else if (intDigs < BN2.intDigs) {
            return sign ? 1 : -1;
        } else {
            for (int i = intDigs - 1; i >= 0; --i) {
                if (intStr.at(i) > BN2.intStr.at(i)) {
                    return sign ? -1 : 1;
                } else if (intStr.at(i) < BN2.intStr.at(i)) {
                    return sign ? 1 : -1;
                }
            }
            for (int i = 0; i < max(decDigs, BN2.decDigs); ++i) {
                if (decStr.at(i) > BN2.decStr.at(i)) {
                    return sign ? -1 : 1;
                } else if (decStr.at(i) < BN2.decStr.at(i)) {
                    return sign ? 1 : -1;
                }
            }
            if (decDigs > BN2.decDigs) {
                return sign ? -1 : 1;
            } else if (decDigs < BN2.decDigs) {
                return sign ? 1 : -1;
            }
            return 0;
        }
    }
}

bool CBigNumber::isZero() const {
    if (decDigs == 0 && intDigs == 0) {
        return true;
    }
    if (decDigs == 0 && intDigs == 1 && intStr == "0") {
        return true;
    }
    return false;
}

void CBigNumber::add(const CBigNumber &BN1, const CBigNumber &BN2, CBigNumber &res) {
    res.init();
    if (BN1.isZero() && BN2.isZero()) {
        return;
    } else if (BN1.isZero() || BN2.isZero()) {
        res = BN1.isZero() ? BN2 : BN1;
        return;
    }
    if (BN1.sign != BN2.sign) {
        CBigNumber NBN2 = BN2;
        NBN2.sign = BN1.sign;
        subtract(BN1, NBN2, res);
    } else {
        int minDecDigs = min(BN1.decDigs, BN2.decDigs);
        int addNum = 0;
        string decString;
        string intString;
        for (int i = minDecDigs - 1; i >= 0; --i) {
            int r = (BN1.decStr.at(i) - '0') + (BN2.decStr.at(i) - '0') + addNum;
            addNum = r / 10;
            decString += to_string(r % 10);
        }
        if (BN1.decDigs > BN2.decDigs) {
            decString = BN1.decStr.substr(minDecDigs, BN1.decDigs - BN2.decDigs) + decString;
            res.decDigs = BN1.decDigs;
        } else if (BN2.decDigs > BN1.decDigs) {
            decString = BN2.decStr.substr(minDecDigs, BN2.decDigs - BN1.decDigs) + decString;
            res.decDigs = BN2.decDigs;
        }
        res.decDigs = max(BN2.decDigs, BN1.decDigs);
        res.decStr = decString;
        int minIntDigs = min(BN1.intDigs, BN2.intDigs);
        for (int j = 0; j < minIntDigs; ++j) {
            int r = (BN1.intStr.at(j) - '0') + (BN2.intStr.at(j) - '0') + addNum;
            addNum = r / 10;
            intString += to_string(r % 10);
        }
        if (BN1.intDigs > BN2.intDigs) {
            for (int j = minIntDigs; j < BN1.intDigs; ++j) {
                int r = (BN1.intStr.at(j) - '0') + addNum;
                addNum = r / 10;
                intString += to_string(r % 10);
            }
        } else if (BN1.intDigs < BN2.intDigs) {
            for (int j = minIntDigs; j < BN2.intDigs; ++j) {
                int r = (BN2.intStr.at(j) - '0') + addNum;
                addNum = r / 10;
                intString += to_string(r % 10);
            }
        }
        res.intDigs = max(BN1.intDigs, BN2.intDigs);
        if (addNum > 0) {
            intString += to_string(addNum);
            res.intDigs++;
        }
        res.intStr = intString;
        if (res.decDigs != 0 && res.decDigs != 1) {
            reverse(res.decStr.begin(), res.decStr.end());
        }
        res.sign = BN1.sign;
        res.adjustDigs();
    }
}

void CBigNumber::subtract(const CBigNumber &BN1, const CBigNumber &BN2, CBigNumber &res) {
    res.init();
    if (BN1.isZero() && BN2.isZero()) {
        return;
    } else if (BN1.isZero()) {
        res = BN2;
        res.sign = (char) (!res.sign);
        return;
    } else if (BN2.isZero()) {
        res = BN1;
        return;
    }
    if (BN1.sign != BN2.sign) {
        CBigNumber NBN2 = BN2;
        NBN2.sign = BN1.sign;
        add(BN1, NBN2, res);
    } else {
        int compared = BN1.compare(BN2);
        if (compared == 0) {
            return;
        }
        int switchSide, minDecDigs, subtract = 0;
        switchSide = BN1.sign == 0 ? compared == -1 : compared == 1;
        const CBigNumber *pBN1 = &BN1, *pBN2 = &BN2;
        if (switchSide) {
            const CBigNumber *temp = pBN1;
            pBN1 = pBN2;
            pBN2 = temp;
        }
        string decString, intString;
        minDecDigs = min(pBN1->decDigs, pBN2->decDigs);
        if (pBN1->decDigs < pBN2->decDigs) {
            for (int i = pBN2->decDigs - 1; i >= pBN1->decDigs; --i) {
                decString += to_string('9' + 1 - subtract - pBN2->decStr.at(i));
                subtract = 1;
            }
        } else if (pBN1->decDigs > pBN2->decDigs) {
            for (int i = pBN1->decDigs - 1; i >= pBN2->decDigs; --i) {
                decString += string(1, pBN1->decStr.at(i));
            }
            subtract = 0;
        }
        for (int i = minDecDigs - 1; i >= 0; --i) {
            if ((pBN1->decStr.at(i) - '0') - subtract < (pBN2->decStr.at(i) - '0')) {
                int r = (pBN1->decStr.at(i) - pBN2->decStr.at(i)) - subtract + 10;
                subtract = 1;
                decString += to_string(r);
            } else {
                int r = (pBN1->decStr.at(i) - pBN2->decStr.at(i)) - subtract;
                subtract = 0;
                decString += to_string(r);
            }
        }
        res.decStr = decString;
        res.decDigs = decString.length();
        for (int i = 0; i < pBN2->intDigs; ++i) {
            if ((pBN1->intStr.at(i) - '0') - subtract < (pBN2->intStr.at(i) - '0')) {
                int r = (pBN1->intStr.at(i) - pBN2->intStr.at(i)) - subtract + 10;
                subtract = 1;
                intString += to_string(r);
            } else {
                int r = (pBN1->intStr.at(i) - pBN2->intStr.at(i)) - subtract;
                subtract = 0;
                intString += to_string(r);
            }
        }
        if (pBN1->intDigs > pBN2->intDigs) {
            for (int i = pBN2->intDigs; i < pBN1->intDigs; ++i) {
                if ((pBN1->intStr.at(i) - '0') - subtract < 0) {
                    int r = (pBN1->intStr.at(i) - '0') - subtract + 10;
                    subtract = 1;
                    intString += to_string(r);
                } else {
                    int r = (pBN1->intStr.at(i) - '0') - subtract;
                    subtract = 0;
                    intString += to_string(r);
                }
            }
        }
        res.intDigs = pBN1->intDigs;
        res.intStr = intString;
        if (res.decDigs != 0 && res.decDigs != 1) {
            reverse(res.decStr.begin(), res.decStr.end());
        }
        if (switchSide) {
            res.sign = (char) (!BN1.sign);
        } else {
            res.sign = BN1.sign;
        }
        res.adjustDigs();
    }
}

void CBigNumber::multiply(const CBigNumber &BN1, const CBigNumber &BN2, CBigNumber &res) {
    if (BN1.isZero() || BN2.isZero()) {
        res.init();
        return;
    }
    CBigNumber temp1 = BN1, temp2 = BN2;
    CBigNumber sum;
    sum.init();
    res.init();
    temp1.moveDecimalPoint(temp1.decDigs);
    temp2.moveDecimalPoint(temp2.decDigs);
    int i, j;
    for (i = 0; i < temp2.intDigs; ++i) {
        CBigNumber temp;
        int addNum = 0;
        temp.intStr += string(temp1.intDigs + temp2.intDigs + 3, '0');
        for (j = 0; j < temp1.intDigs; ++j) {
            int r = (temp2.intStr.at(i) - '0') * (temp1.intStr.at(j) - '0') + addNum;
            addNum = r / 10;
            temp.intStr[i + j] = r % 10 + '0';
        }
        if (addNum) {
            temp.intStr[i + j] = addNum + '0';
        }
        temp.intDigs = temp.intStr.length();
        temp.adjustDigs();
        sum.adjustDigs();
        add(sum, temp, res);
        sum = res;
    }
    res = sum;
    res.moveDecimalPoint(-BN2.decDigs - BN1.decDigs);
    if (BN1.sign != BN2.sign) {
        res.sign = 1;
    }
}

void CBigNumber::divide(const CBigNumber &BN1, const CBigNumber &BN2, CBigNumber &res) {
    CBigNumber temp1 = BN1, temp2 = BN2;
    int moves = temp2.decDigs - temp1.decDigs;
    temp1.moveDecimalPoint(temp1.decDigs);
    temp2.moveDecimalPoint(temp2.decDigs);
    res.init();
    if (BN2.isZero()) {
        throw CZeroDivideException();
    }
    if (BN1.isZero()) {
        return;
    }
    temp1.sign = temp2.sign = 0;
    while (temp1.intDigs != temp2.intDigs) {
        if (temp1.intDigs < temp2.intDigs) {
            temp1.moveDecimalPoint(1);
            moves--;
        } else {
            temp2.moveDecimalPoint(1);
            moves++;
        }
    }
    while (res.decDigs < DIVIDE_MAX_LENGTH) {
        int compared = temp1.compare(temp2);
        int n = 10;
        CBigNumber multiRes, subRes;
        if (compared == -1) {
            temp1.moveDecimalPoint(1);
            res.decStr += '0';
            res.decDigs++;
            continue;
        } else if (compared == 0) {
            res.decStr += '1';
            res.decDigs++;
            break;
        }
        do {
            CBigNumber temp;
            temp.intStr += to_string(--n);
            temp.intDigs = 1;
            multiply(temp2, temp, multiRes);
            compared = multiRes.compare(temp1);
        } while (compared == 1);
        res.decStr += to_string(n);
        res.decDigs++;
        if (compared == 0) {
            break;
        }
        subtract(temp1, multiRes, subRes);
        subRes.moveDecimalPoint(1);
        temp1 = subRes;
    }
    res.moveDecimalPoint(1);
    res.moveDecimalPoint(moves);
    if (BN1.sign != BN2.sign) {
        res.sign = 1;
    }
}

string CBigNumber::result(const string &str1, const string &str2, CBNCALC cbnCalc) {
    CBigNumber BN1, BN2, res;
    BN1.strToBigNumber(str1);
    BN2.strToBigNumber(str2);
    cbnCalc(BN1, BN2, res);
    return res.toString();
}

CBigNumber CBigNumber::operator+(const CBigNumber &number) const {
    CBigNumber res;
    CBigNumber::add(*this, number, res);
    return res;
}

CBigNumber CBigNumber::operator-(const CBigNumber &number) const {
    CBigNumber res;
    CBigNumber::subtract(*this, number, res);
    return res;
}

CBigNumber CBigNumber::operator*(const CBigNumber &number) const {
    CBigNumber res;
    CBigNumber::multiply(*this, number, res);
    return res;
}

CBigNumber CBigNumber::operator/(const CBigNumber &number) const {
    CBigNumber res;
    CBigNumber::divide(*this, number, res);
    return res;
}

CBigNumber &CBigNumber::operator+=(const CBigNumber &number) {
    CBigNumber BN = *this;
    CBigNumber::add(BN, number, *this);
    return *this;
}

CBigNumber &CBigNumber::operator-=(const CBigNumber &number) {
    CBigNumber BN = *this;
    CBigNumber::subtract(BN, number, *this);
    return *this;
}

CBigNumber &CBigNumber::operator*=(const CBigNumber &number) {
    CBigNumber BN = *this;
    CBigNumber::multiply(BN, number, *this);
    return *this;
}

CBigNumber &CBigNumber::operator/=(const CBigNumber &number) {
    CBigNumber BN = *this;
    CBigNumber::divide(BN, number, *this);
    return *this;
}

bool CBigNumber::operator<(const CBigNumber &number) const {
    return compare(number) == -1;
}

bool CBigNumber::operator<=(const CBigNumber &number) const {
    return compare(number) != 1;
}

bool CBigNumber::operator==(const CBigNumber &number) const {
    return compare(number) == 0;
}

bool CBigNumber::operator>=(const CBigNumber &number) const {
    return compare(number) != -1;
}

bool CBigNumber::operator>(const CBigNumber &number) const {
    return compare(number) == 1;
}

bool CBigNumber::operator!=(const CBigNumber &number) const {
    return compare(number) != 0;
}
