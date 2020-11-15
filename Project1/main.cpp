#include <iostream>
#include "matrix.h"
#include "matrix.cpp"
#include <functional>

using namespace std;

class TestClass {
public:
    explicit TestClass(int i = 0) : abc(i) {}
    TestClass operator+(TestClass add) const {
        return TestClass(abc + add.abc);
    }
    TestClass &operator+=(TestClass add) {
        abc += add.abc;
        return *this;
    }
    TestClass operator-(TestClass sub) const {
        return TestClass(abc - sub.abc);
    }
    TestClass operator*(TestClass mul) const {
        return TestClass(abc * mul.abc);
    }
    TestClass &operator=(const TestClass &a) {
        if (this == &a) {
            return *this;
        }
        abc = a.abc;
        return *this;
    }
    TestClass &operator=(const int a) {
        abc = a;
        return *this;
    }
    friend std::ostream &operator<<(std::ostream &os, const TestClass &a) {
        os << a.abc;
        return os;
    }
private:
    int abc;
};

int main() {
    Matrix<TestClass> m1(10,3), m2(3, 10);
    auto m3 = m1 * m2;
    m3 = m3 + m3.Transpose();
    m3.Transpose();
    cout << m3 << m3[2][2] << endl;
    Matrix<float> m_f1(10000, 10000), m_f2(10000, 10000);
    for (int i = 0; i < 10000; ++i) {
        for (int j = 0; j < 10000; ++j) {
            m_f1[i][j] = i * j;
            m_f2(i + 1, j + 1) = i + j;
        }
    }
    auto m_f3 = m_f1 * m_f2;
    //cout << m_f3.Format(Matrix<float>::kSimpleOneLine);
}
