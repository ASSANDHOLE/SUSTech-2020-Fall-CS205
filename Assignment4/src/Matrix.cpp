//
// Created by AnGuangyan on 2020/11/26.
//

#include "Matrix.hpp"
#include <string>
#include <vector>
#include <thread>
#include <ctime>
#include <algorithm>
#include <functional>

Matrix::Matrix(unsigned int row, unsigned int column, bool init) {
    if (column > 0 && row > 0) {
        column_ = column;
        row_ = row;
        if (init) {
            matrix_ = new float [row_ * column_]();
        } else {
            matrix_ = new float [row_ * column_];
        }
    } else {
        row_ = 0;
        column_ = 0;
        matrix_ = nullptr;
    }
    reference_counter_ = new unsigned int();
    reference_ = false;
}

Matrix::Matrix(const Matrix &temp, bool copy_reference) {
    row_ = temp.row_;
    column_ = temp.column_;
    if (!copy_reference) {
        const unsigned int kTotal = row_ * column_;
        matrix_ = new float [kTotal];
        memcpy(matrix_, temp.matrix_, kTotal);
        reference_ = false;
        reference_counter_ = new unsigned int();
    } else {
        matrix_ = temp.matrix_;
        reference_ = temp.reference_;
        if (!reference_) {
            reference_counter_ = temp.reference_counter_;
            (*reference_counter_)++;
        }
    }
}

Matrix::Matrix(unsigned int row, unsigned int column, float *matrix, bool copy_reference) {
    row_ = row;
    column_ = column;
    if (copy_reference) {
        matrix_ = matrix;
    } else {
        matrix_ = new float [row_ * column_];
        memcpy(matrix_, matrix, row_ * column_);
    }
    reference_ = copy_reference;
}

Matrix::~Matrix() {
    if (!reference_) {
        if (*reference_counter_ > 0) {
            (*reference_counter_)--;
        } else {
            delete [] matrix_;
            delete reference_counter_;
        }
    }
}

Matrix Matrix::operator+(const Matrix &matrix) const {
    if (row_ != matrix.row_ || column_ != matrix.column_) {
        throw MatrixError();
    }
    Matrix res(row_, column_);
    for (int i = 0; i < row_ * column_; ++i) {
        res.matrix_[i] = matrix_[i] + matrix.matrix_[i];
    }
    return res;
}

Matrix Matrix::operator+(float num) const {
    Matrix res(row_, column_);
    for (int i = 0; i < row_ * column_; ++i) {
        res.matrix_[i] = matrix_[i] + num;
    }
    return res;
}

Matrix Matrix::operator-(const Matrix &matrix) const {
    if (row_ != matrix.row_ || column_ != matrix.column_) {
        throw MatrixError();
    }
    Matrix res(row_, column_);
    for (int i = 0; i < row_ * column_; ++i) {
        res.matrix_[i] = matrix_[i] - matrix.matrix_[i];
    }
    return res;
}

Matrix Matrix::operator-(float num) const {
    Matrix res(row_, column_);
    for (int i = 0; i < row_ * column_; ++i) {
        res.matrix_[i] = matrix_[i] - num;
    }
    return res;
}

#define A(m, n) mat_a[(m) * col1 + (n)]
#define B(m, n) mat_b[(m) * col2 + (n)]
#define C(m, n) c[(m) * col2 + (n)]

size_t hardware_concurrency_ = 1;

void MulBlock(int col1, int col2, float *mat_a, float *mat_b, float *c);

void MulFloatMatThr(int i1, int i2, int col1, int col2, float *mat_a, float *mat_b, float *c) {
    for (int i = i1; i < i2; i += 4) {
        for (int j = 0; j < col2; j += 4) {
            MulBlock(col1, col2, &A(i, 0), &B(0, j), &C(i, j));
        }
    }
}

int Min(int a, int b) {
    return a > b ? b : a;
}

void MulThrMat(int row1, int col1, int col2, float *mat_a, float *mat_b, float *c) {
#ifndef HARDWARE_CONCURRENCY_
    hardware_concurrency_ = std::thread::hardware_concurrency();
#define HARDWARE_CONCURRENCY_
#endif
    const int kMinVal = 4;
    int kMaxThreadR = ((row1 / 4) + kMinVal - 1) / kMinVal;
    int kThreadNum = Min(hardware_concurrency_ != 0 ? hardware_concurrency_ : 2, kMaxThreadR);
    int kBlockSize = ((row1 / 4) / kThreadNum) * 4;
    std::vector<std::thread> threads(kThreadNum - 1);
    for (int i = 0; i < (kThreadNum - 1); i++) {
        threads[i] = std::thread(MulFloatMatThr, i * kBlockSize, (i + 1) * kBlockSize, col1, col2, mat_a, mat_b, c);
    }
    MulFloatMatThr((kThreadNum - 1) * kBlockSize, row1, col1, col2, mat_a, mat_b, c);
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

void MulBlock(int col1, int col2, float *mat_a, float *mat_b, float *c) {
    register float c00, c01, c02, c03,
            c10, c11, c12, c13,
            c20, c21, c22, c23,
            c30, c31, c32, c33,

            a0x, a1x, a2x, a3x,
            bx0, bx1, bx2, bx3;

    float *a0xp, *a1xp, *a2xp, *a3xp;
    c00 = c01 = c02 = c03 = c10 = c11 = c12 = c13 = c20 = c21 = c22 = c23 = c30 = c31 = c32 = c33 = 0.0;
    a0xp = &A(0, 0);
    a1xp = &A(1, 0);
    a2xp = &A(2, 0);
    a3xp = &A(3, 0);
    for (int i = 0; i < col1; ++i) {
        bx0 = B(i, 0);
        bx1 = B(i, 1);
        bx2 = B(i, 2);
        bx3 = B(i, 3);
        a0x = *a0xp++;
        a1x = *a1xp++;
        a2x = *a2xp++;
        a3x = *a3xp++;
        c00 += a0x * bx0;
        c01 += a0x * bx1;
        c02 += a0x * bx2;
        c03 += a0x * bx3;
        c10 += a1x * bx0;
        c11 += a1x * bx1;
        c12 += a1x * bx2;
        c13 += a1x * bx3;
        c20 += a2x * bx0;
        c21 += a2x * bx1;
        c22 += a2x * bx2;
        c23 += a2x * bx3;
        c30 += a3x * bx0;
        c31 += a3x * bx1;
        c32 += a3x * bx2;
        c33 += a3x * bx3;
    }
    C(0, 0) += c00; C(0, 1) += c01; C(0, 2) += c02; C(0, 3) += c03;
    C(1, 0) += c10; C(1, 1) += c11; C(1, 2) += c12; C(1, 3) += c13;
    C(2, 0) += c20; C(2, 1) += c21; C(2, 2) += c22; C(2, 3) += c23;
    C(3, 0) += c30; C(3, 1) += c31; C(3, 2) += c32; C(3, 3) += c33;
}

Matrix Matrix::operator*(const Matrix &matrix) const {
    if (column_ != matrix.row_ || row_ == 0 || column_ == 0 || matrix.column_ == 0) {
        throw MatrixError();
    }
    Matrix res(row_, matrix.column_, true);
    MulThrMat(row_, column_, matrix.column_, matrix_, matrix.matrix_, res.matrix_);
    return res;
}
#undef A
#undef B
#undef C

Matrix Matrix::operator*(float num) const {
    Matrix res(row_, column_);
    for (int i = 0; i < row_ * column_; ++i) {
        res.matrix_[i] = matrix_[i] * num;
    }
    return res;
}

Matrix operator*(float num, const Matrix &matrix) {
    Matrix res(matrix.row_, matrix.column_);
    for (int i = 0; i < matrix.row_ * matrix.column_; ++i) {
        res.matrix_[i] = matrix.matrix_[i] * num;
    }
    return res;
}

Matrix Matrix::operator/(float num) const {
    Matrix res(row_, column_);
    for (int i = 0; i < row_ * column_; ++i) {
        res.matrix_[i] = matrix_[i] / num;
    }
    return res;
}

Matrix Matrix::operator^(const int time) const {
    if (row_ != column_ || row_ == 0) {
        throw MatrixError();
    }
    Matrix res(*this, false);
    Matrix base(*this, false);
    if (time <= 1) {
        return res;
    }
    int temp_time = time - 1;
    while (temp_time > 0) {
        if (temp_time & 1) {
            res = res * base;
        }
        base = base * base;
        temp_time >>= 1;
    }
    return res;
}

Matrix &Matrix::operator=(const Matrix &matrix) {
    if (this == &matrix) {
        return *this;
    }
    row_ = matrix.row_;
    column_ = matrix.column_;
    reference_ = matrix.reference_;
    matrix_ = matrix.matrix_;
    if (!reference_) {
        reference_counter_ = matrix.reference_counter_;
        (*reference_counter_)++;
    }
    return *this;
}

bool Matrix::operator==(const Matrix &matrix) const {
    if (row_ != matrix.row_ || column_ != matrix.column_) {
        return false;
    }
    if (reference_counter_ == matrix.reference_counter_ && matrix_ == matrix.matrix_) {
        return true;
    }
    if (row_ > 0 && column_ > 0) {
        const size_t kTotal = row_ * column_;
        for (int i = 0; i < kTotal; ++i) {
            if (matrix_[i] != matrix.matrix_[i]) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &matrix) const {
    if (row_ != matrix.row_ || column_ != matrix.column_) {
        return true;
    }
    if (reference_counter_ == matrix.reference_counter_ && matrix_ == matrix.matrix_) {
        return false;
    }
    if (row_ > 0 && column_ > 0) {
        const size_t kTotal = row_ * column_;
        for (int i = 0; i < kTotal; ++i) {
            if (matrix_[i] != matrix.matrix_[i]) {
                return true;
            }
        }
    }
    return false;
}

Matrix Matrix::Transpose() const {
    if (column_ == 0 || row_ == 0) {
        throw MatrixError();
    }
    Matrix res(column_, row_);
    for (int i = 0; i < row_; ++i) {
        for (int j = 0; j < column_; ++j) {
            res.matrix_[j * res.column_ + i] = matrix_[i * column_ + j];
        }
    }
    return res;
}

float *Matrix::operator[](int num) const {
    if (num < 0 || num > row_ -1) {
        throw MatrixError();
    }
    return &matrix_[num * column_];
}

std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {
    os << "Matrix<" << matrix.row_ << "*" << matrix.column_ << ">(";
    for (int i = 0; i < matrix.row_; ++i) {
        for (int j = 0; j < matrix.column_; ++j) {
            os << matrix.matrix_[i * matrix.column_ + j];
            if (j != matrix.column_ - 1) {
                os << " ";
            }
        }
        os << ((i == matrix.row_ - 1) ? ")" : "; ");
    }
    return os;
}

