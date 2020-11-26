//
// Created by AnGuangyan on 2020/11/26.
//

#ifndef ASSIGNMENT4_MATRIX_HPP
#define ASSIGNMENT4_MATRIX_HPP


#include <exception>
#include <ostream>

class Matrix {
public:
    Matrix(unsigned int row, unsigned int column, bool init = false);
    Matrix(const Matrix &, bool copy_reference = true);
    Matrix(unsigned int row, unsigned int column, float *matrix, bool copy_reference = true);
    ~Matrix();

    Matrix operator+(const Matrix &) const;
    Matrix operator+(float) const;

    Matrix operator-(const Matrix &) const;
    Matrix operator-(float) const;

    Matrix operator*(const Matrix &) const;
    Matrix operator*(float) const;
    friend Matrix operator*(float, const Matrix &);

    Matrix operator/(float) const;
    //an operator to calculate power
    Matrix operator^(int time) const;

    Matrix &operator=(const Matrix &);

    bool operator==(const Matrix &) const;
    bool operator!=(const Matrix &) const;

    Matrix Transpose() const;

    float *operator[](int) const;

    friend std::ostream &operator<<(std::ostream &, const Matrix &);

private:
    unsigned int row_, column_;
    float *matrix_;
    bool reference_;
    unsigned int *reference_counter_ = nullptr;
};

class MatrixError : public std::exception {
    const char *what() const noexcept final {
        return "Matrices with such rows or columns cannot be calculated";
    }
};


#endif //ASSIGNMENT4_MATRIX_HPP
