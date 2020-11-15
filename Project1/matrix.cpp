//
// Created by AGY on 2020/10/21.
//

#include "matrix.h"
#include <algorithm>
#include <thread>
#include <vector>
#include <functional>
#include "mul_mat.h"

template<typename T>
Matrix<T>::Matrix(const int row, const int column, bool to_zero_for_basic_types) {
    if (column > 0 && row > 0) {
        column_ = column;
        row_ = row;
        matrix_1d_ = new T[row_ * column_];
        if (to_zero_for_basic_types) {
            initToZero();
        }
    } else {
        row_ = 0;
        column_ = 0;
        matrix_1d_ = nullptr;
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T> &temp) {
    row_ = temp.row_;
    column_ = temp.column_;
    const size_t kTotal = row_ * column_;
    matrix_1d_ = new T[kTotal];
    for (int i = 0; i < kTotal; ++i) {
        matrix_1d_[i] = temp.matrix_1d_[i];
    }
}

template<typename T>
Matrix<T>::Matrix(int row, int column, T *matrix) {
    row_ = row;
    column_ = column;
    matrix_1d_ = matrix;
}

template<typename T>
Matrix<T>::~Matrix<T>() {
    if (matrix_1d_) {
        delete[] matrix_1d_;
    }
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &mat2) const {
    if (row_ != mat2.row_ || column_ != mat2.column_) {
        throw MatrixNotFitException();
    }
    Matrix<T> res(row_, column_);
    const size_t kTotal = row_ * column_;
    for (int i = 0; i < kTotal; ++i) {
        res.matrix_1d_[i] = matrix_1d_[i] + mat2.matrix_1d_[i];
    }
    return res;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const T &t) const {
    Matrix<T> res(row_, column_);
    const size_t kTotal = row_ * column_;
    for (int i = 0; i < kTotal; ++i) {
        res.matrix_1d_[i] = matrix_1d_[i] + t;
    }
    return res;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &mat2) const {
    if (row_ != mat2.row_ || column_ != mat2.column_) {
        throw MatrixNotFitException();
    }
    Matrix<T> res(row_, column_);
    const size_t kTotal = row_ * column_;
    for (int i = 0; i < kTotal; ++i) {
        res.matrix_1d_[i] = matrix_1d_[i] - mat2.matrix_1d_[i];
    }
    return res;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const T &t) const {
    Matrix<T> res(row_, column_);
    const size_t kTotal = row_ * column_;
    for (int i = 0; i < kTotal; ++i) {
        res.matrix_1d_[i] = matrix_1d_[i] - t;
    }
    return res;
}

template<class T>
const T &Min(const T &a, const T &b) {
    return a < b ? a : b;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &mat2) const {
    if (column_ != mat2.row_ || row_ == 0 || column_ == 0 || mat2.column_ == 0) {
        throw MatrixNotFitException();
    }
    Matrix<T> res(row_, mat2.column_, true);
    if (typeid(T) == typeid(float)) {
        MulThrMat(row_, column_, mat2.column_, matrix_1d_, mat2.matrix_1d_, res.matrix_1d_);
        return res;
    }
    const auto calculate = [this, &res, &mat2](int start, int end) {
        for (int i = start; i < end; ++i) {
            for (int k = 0; k < column_; ++k) {
                for (int j = 0; j < res.column_; ++j) {
                    res.matrix_1d_[i * res.column_ + j] = res.matrix_1d_[i * res.column_ + j] +
                                                       matrix_1d_[i * column_ + k] * mat2.matrix_1d_[k * mat2.column_ + j];
                }
            }
        }
    };
    unsigned long const kMinVal = 200;
    unsigned long const kMaxThread = (res.row_ + kMinVal - 1) / kMinVal;
    unsigned long const kHardwareConcurrency = std::thread::hardware_concurrency();
    unsigned long const kThreadNum = Min(kHardwareConcurrency != 0 ? kHardwareConcurrency : 2, kMaxThread);
    unsigned long const kBlockSize = res.row_ / kThreadNum;
    std::vector<std::thread> threads(kThreadNum - 1);
    for (int i = 0; i < (kThreadNum - 1); i++) {
        threads[i] = std::thread(calculate, i * kBlockSize, (i + 1) * kBlockSize);
    }
    calculate((kThreadNum - 1) * kBlockSize, res.row_);
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return res;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const T &t) const {
    Matrix<T> res(row_, column_);
    const size_t kTotal = row_ * column_;
    for (int i = 0; i < kTotal; ++i) {
        res.matrix_1d_[i] = matrix_1d_[i] * t;
    }
    return res;
}

template<typename T>
Matrix<T> Matrix<T>::operator/(const T &t) const {
    Matrix<T> res(row_, column_);
    const size_t kTotal = row_ * column_;
    for (int i = 0; i < kTotal; ++i) {
        res.matrix_1d_[i] = matrix_1d_[i] / t;
    }
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator^(const int time) const {
    if (row_ != column_ || row_ == 0) {
        throw MatrixNotFitException();
    }
    Matrix<T> res(*this);
    Matrix<T> base(*this);
    if (time <= 1) {
        return res;
    }
    int temp_time = time - 1;
    while (temp_time > 0) {
        if (temp_time & 1) {
            res = res * base;
        }
        base = base * base;
        temp_time /= 2;
    }
    return res;
}

template<typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &matrix) {
    if (this == &matrix) {
        return *this;
    }
    if (matrix_1d_) {
        delete[] matrix_1d_;
    }
    row_ = matrix.row_;
    column_ = matrix.column_;
    if (row_ > 0 && column_ > 0) {
        const size_t kTotal = row_ * column_;
        matrix_1d_ = new T[kTotal];
        for (int i = 0; i < kTotal; ++i) {
            matrix_1d_[i] = matrix.matrix_1d_[i];
        }
    } else {
        row_ = 0;
        column_ = 0;
        matrix_1d_ = nullptr;
    }
    return *this;
}

template<typename T>
bool Matrix<T>::operator==(const Matrix<T> &mat2) const {
    if (row_ != mat2.row_ || column_ != mat2.column_) {
        return false;
    }
    if (row_ > 0 && column_ > 0) {
        const size_t kTotal = row_ * column_;
        for (int i = 0; i < kTotal; ++i) {
            if (matrix_1d_[i] != mat2.matrix_1d_[i]) {
                return false;
            }
        }
    }
    return true;
}

template<typename T>
bool Matrix<T>::operator!=(const Matrix<T> &mat2) const {
    if (row_ != mat2.row_ || column_ != mat2.column_) {
        return true;
    }
    if (row_ > 0 && column_ > 0) {
        const size_t kTotal = row_ * column_;
        for (int i = 0; i < kTotal; ++i) {
            if (matrix_1d_[i] != mat2.matrix_1d_[i]) {
                return true;
            }
        }
    }
    return false;
}

template<typename T>
Matrix<T> Matrix<T>::Transpose() const {
    if (column_ <= 0 || row_ <= 0) {
        throw MatrixNotFitException();
    }
    Matrix<T> res(column_, row_);
    for (int i = 0; i < row_; ++i) {
        for (int j = 0; j < column_; ++j) {
            res.matrix_1d_[j * res.column_ + i] = matrix_1d_[i * column_ + j];
        }
    }
    return res;
}

template<typename T>
std::string Matrix<T>::Format(Matrix::OutputStyle output_style) const {
    std::stringstream string_stream;
    switch (output_style) {
        case kMatrixTypeNumOneLine:
            string_stream << "Matrix<" << typeid(T).name() << ">[" << row_ << "*" << column_ << "](";
            for (int i = 0; i < row_; ++i) {
                for (int j = 0; j < column_; ++j) {
                    string_stream << matrix_1d_[i * column_ + j];
                    if (j != column_ - 1) {
                        string_stream << " ";
                    }
                }
                string_stream << ((i == row_ - 1) ? ")" : "; ");
            }
            break;
        case kMatrixTypeNumMultipleLines:
            string_stream << "Matrix<" << typeid(T).name() << ">[" << row_ << "*" << column_ << "]\n(";
            for (int i = 0; i < row_; ++i) {
                for (int j = 0; j < column_; ++j) {
                    string_stream << matrix_1d_[i * column_ + j];
                    if (j != column_ - 1) {
                        string_stream << " ";
                    }
                }
                string_stream << ((i == row_ - 1) ? ")" : ";\n ");
            }
            break;
        case kSimpleOneLine:
            string_stream << "(";
            for (int i = 0; i < row_; ++i) {
                for (int j = 0; j < column_; ++j) {
                    string_stream << matrix_1d_[i * column_ + j];
                    if (j != column_ - 1) {
                        string_stream << " ";
                    }
                }
                string_stream << ((i == row_ - 1) ? ")" : "; ");
            }
            break;
        case kSimpleMultipleLines:
            string_stream << "(";
            for (int i = 0; i < row_; ++i) {
                for (int j = 0; j < column_; ++j) {
                    string_stream << matrix_1d_[i * column_ + j];
                    if (j != column_ - 1) {
                        string_stream << " ";
                    }
                }
                string_stream << ((i == row_ - 1) ? ")" : ";\n ");
            }
            break;
        default:;
    }
    return string_stream.str();
}

template<typename T>
T *Matrix<T>::operator[](const int addr) const {
    if (addr >= row_ || addr < 0) {
        throw IndexOutOfMatrixException();
    }
    T *res = &matrix_1d_[addr * column_];
    return res;
}

template <typename T>
T &Matrix<T>::operator()(int real_row, int real_column) const {
    return Get(real_row, real_column);
}

template<typename T>
T &Matrix<T>::Get(int real_row, int real_column) const {
    if (real_row < 1 || real_column < 1 || real_row > row_ || real_column > column_) {
        throw IndexOutOfMatrixException();
    }
    return matrix_1d_[(real_row - 1) * column_ + (real_column - 1)];
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &matrix) {
    os << "Matrix<" << typeid(T).name() << ">[" << matrix.row_ << "*" << matrix.column_ << "](";
    for (int i = 0; i < matrix.row_; ++i) {
        for (int j = 0; j < matrix.column_; ++j) {
            os << matrix.matrix_1d_[i * matrix.column_ + j];
            if (j != matrix.column_ - 1) {
                os << " ";
            }
        }
        os << ((i == matrix.row_ - 1) ? ")" : "; ");
    }
    return os;
}

template<typename T>
std::istream &operator>>(std::istream &is, const Matrix<T> &matrix) {
    if (matrix.row_ <= 0 || matrix.column_ <= 0) {
        throw MatrixNotFitException();
    }
    for (int i = 0; i < matrix.row_; ++i) {
        for (int j = 0; j < matrix.column_; ++j) {
            is >> matrix.matrix_1d_[i * matrix.column_ + j];
        }
    }
    return is;
}

template<typename T>
void Matrix<T>::initToZero() {
    const size_t kTotal = row_ * column_;
    for (int i = 0; i < kTotal; ++i) {
        matrix_1d_[i] = T();
    }
}

