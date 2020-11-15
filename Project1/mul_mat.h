//
// Created by AnGuangyan on 2020/11/15.
//

#ifndef PROJECT1_MUL_MAT_H
#define PROJECT1_MUL_MAT_H
#include <vector>
#include <thread>
#include <ctime>
#include <algorithm>
#include <functional>

#define A(m, n) mat_a[(m) * col1 + (n)]
#define B(m, n) mat_b[(m) * col2 + (n)]
#define C(m, n) c[(m) * col2 + (n)]

size_t hardware_concurrency_ = 1;

void MulBlock(int col1, int col2, float *mat_a, float *mat_b, float *c);

void MulFloatMat(int row1, int col1, int col2, float *mat_a, float *mat_b, float *c) {
    for (int i = 0; i < row1; i += 4) {
        for (int j = 0; j < col2; j += 4) {
            MulBlock(col1, col2, &A(i, 0), &B(0, j), &C(i, j));
        }
    }
}

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
#endif //PROJECT1_MUL_MAT_H
