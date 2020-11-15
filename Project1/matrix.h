//
// Created by AGY on 2020/10/21.
//

#ifndef PROJECT1_MATRIX_H
#define PROJECT1_MATRIX_H

#include <exception>
#include <ostream>
#include <istream>
#include <sstream>
#include <string>

//TODO 1. 矩阵乘法ijk -> ikj 最快, 2. 使用一维数组二维索引储存矩阵, 方便且保证连续性, 3. 多线程

/**
 * Matrix class
 * Example:
 *     Matrix<int> matrix(3, 5);
 *     for (int i = 0; i < 3; ++i) {
 *         for (int j = 0; j < 5; ++j) {
 *             matrix[i][j] = i * j;
 *         }
 *     }
 *     Matrix<int> matrixCopy(matrix);
 *     cout << matrix * matrixCopy.Transpose() << endl;
 *     std::string matrix_string = matrix.format(Matrix<int>::kSimpleOneLine);
 */
template<typename T>
class Matrix {
  public:
    //4 types of output style used in Format()
    enum OutputStyle {
        // Matrix<typename>[row*column](a b c; d e...)
        kMatrixTypeNumOneLine,
        // Matrix<typename>[row*column]
        // (a b c;
        //  d e ...)
        kMatrixTypeNumMultipleLines,
        // [a b c; d e f; ...]
        kSimpleOneLine,
        // [a b c;
        //  d e ...]
        kSimpleMultipleLines
    };

    //if the bool value is true, it initialize types like int, long ... to 0.
    explicit Matrix(int row = 0, int column = 0, bool to_zero_for_basic_types = false);
    Matrix(const Matrix<T> &);
    Matrix(int row, int column, T *matrix);

    ~Matrix();

    Matrix operator+(const Matrix<T> &) const;
    Matrix operator+(const T &) const;

    Matrix operator-(const Matrix<T> &) const;
    Matrix operator-(const T &) const;

    Matrix operator*(const Matrix<T> &) const;
    Matrix operator*(const T &) const;

    Matrix operator/(const T &) const;
    //an operator to calculate power
    Matrix operator^(int time) const;

    Matrix &operator=(const Matrix<T> &);

    bool operator==(const Matrix<T> &) const;
    bool operator!=(const Matrix<T> &) const;
    
    Matrix Transpose() const;

    std::string Format(OutputStyle) const;

    T *operator[](int) const;

    T &operator()(int, int) const;

    //Get(x, y) = Matrix[x - 1][y - 1]
    T &Get(int real_row, int real_column) const;

    //the output style is OutputStyle.kMatrixTypeNumOneLine
    template<typename Type>
    friend std::ostream &operator<<(std::ostream &, const Matrix<Type> &);
    template<typename Type>
    friend std::istream &operator>>(std::istream &, const Matrix<Type> &);

  private:
    //row number and column number
    int row_, column_;
    //pointer to the row_*column T[], if row_ or column is 0 -> nullptr
    T *matrix_1d_;

    void initToZero();
};

class MatrixNotFitException : public std::exception {
    const char *what() const noexcept override {
        return "Matrix Not Fit!";
    }
};

class IndexOutOfMatrixException : public std::exception {
    const char *what() const noexcept override {
        return "The Index Is Out Of The Matrix Size!";
    }
};

void MulMat(const float *mat1, int row1, int col1, const float *mat2, int col2, float *res);


#endif //PROJECT1_MATRIX_H
