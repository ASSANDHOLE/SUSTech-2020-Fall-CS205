#include <iostream>
#include "src/Matrix.hpp"

using namespace std;

int main() {
    Matrix matrix1(4, 6);
    for (int i = 0; i < 24; ++i) {
        matrix1[0][i] = i;
    }
    Matrix matrix5 = matrix1;
    Matrix matrix3 = matrix1 * matrix1.Transpose();
    matrix5 = 3 * matrix5;
    matrix5 = matrix5 / 3;
    auto *matrix_arr = new float [18]();
    Matrix matrix_by_arr(3, 6, matrix_arr);
    matrix_by_arr[2][3] = 3;
    cout << matrix_arr[2 * 6 + 3] << endl;
    cout << (matrix5 == matrix1) << endl;
    cout << matrix3 << endl;
    system("pause");
}
