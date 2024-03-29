//
// Created by AnGuangyan on 2020/12/17.
//

#include "face_detect_cnn.h"
#include "cnn_param.h"
#include <cmath>
#include <numeric>

/**
 * For the given point(i, j, k) in the output matrices,
 * calculate the convolution result
 * @param mat input mats
 * @param param convolution param
 */
float PartConv(const CnnMatrix &mat, const ConvParam &param, int i, int j, int k) {
    float res = 0;
    for (int l = 0; l < mat.channels_; ++l) {
        for (int m = 0; m < param.kernel_size; ++m) {
            for (int n = 0; n < param.kernel_size; ++n) {
                res += mat.Get(l, j * param.stride - param.pad + m, k * param.stride - param.pad + n) *
                        param.p_weight[i * mat.channels_ * param.kernel_size * param.kernel_size +
                        l * param.kernel_size * param.kernel_size + m * param.kernel_size + n];
            }
        }
    }
    return res;
}

void ConvLayer(const CnnMatrix &in, CnnMatrix &out, const ConvParam &param) {
    out.init(param.out_channels, ((in.rows_ + 2 * param.pad - param.kernel_size) / param.stride + 1),
             ((in.cols_ + 2 * param.pad - param.kernel_size) / param.stride + 1));
    out.data_ = new float [out.Total()];
#ifdef ENABLE_OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < out.channels_; ++i) {
        for (int j = 0; j < out.rows_; ++j) {
            for (int k = 0; k < out.cols_; ++k) {
                out.Set(PartConv(in, param, i, j, k) + param.p_bias[i], i, j, k);
            }
        }
    }
}

void Relu(CnnMatrix &mat) {
#ifdef ENABLE_OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < mat.Total(); ++i) {
        if (mat.data_[i] < 0) {
            mat.data_[i] = 0;
        }
    }
}

inline float Max(float a, float b) {
    return a > b ? a : b;
}

float PartPooling(const CnnMatrix &mat, int i, int j, int k) {
    float res = Max(mat.Get(i, j * 2, k * 2), mat.Get(i, j * 2, k * 2 + 1));
    res = Max(res, mat.Get(i, j * 2 + 1, k * 2));
    res = Max(res, mat.Get(i, j * 2 + 1, k * 2 + 1));
    return res;
}

/**
 * MaxPooling for params of (2, 2)
 */
void MaxPoolingLayer(const CnnMatrix &in, CnnMatrix &out) {
    out.init(in.channels_, in.rows_ / 2, in.cols_ / 2);
    out.data_ = new float [out.Total()];
#ifdef ENABLE_OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < out.channels_; ++i) {
        for (int j = 0; j < out.rows_; ++j) {
            for (int k = 0; k < out.cols_; ++k) {
                out.Set(PartPooling(in, i, j, k), i, j, k);
            }
        }
    }
    Relu(out);
}

/**
 * Optimized inner product function by {@code #pragma simd} for the full connected layer
 * @param index the output is 1x2, so this is the 0 or 1
 */
float DotProduct(const CnnMatrix &in, const FcParam &param, int index) {
    float res = 0;
#pragma simd
    for (int i = 0; i < 2048; ++i) {
        res += in.data_[i] * param.p_weight[i + index];
    }
    return res;
}

/**
 * implementation of the full connected layer
 */
void FcLayer(const CnnMatrix &in, CnnMatrix &out, const FcParam &param) {
    out.init(param.out_features, 1, 1);
    out.data_ = new float [out.Total()];
    for (int i = 0; i < out.channels_; ++i) {
        out.Set(DotProduct(in, param, i * in.Total()) + param.p_bias[i], i, 0, 0);
    }
}

/**
 * Get the score for 2 input(X0 or X1) by (e^X(index))/(e^X0 + e^X1)
 * @param index which input to get result from
 * @return the final confidence score ranged from 0 to 1
 */
float GetScore(CnnMatrix &mat, int index) {
    float res = 0;
    for (int i = 0; i < mat.Total(); ++i) {
        res += std::exp(mat.Get(i, 0, 0));
        if (std::isinf(res)) {
            return i == index ? 1 : 0;
        }
    }
    return (std::exp(mat.Get(index, 0, 0)) / res);
}

float GetConfidenceScore128x128rbg(float *rgb_arr, int rows, int cols) {
    if (rows != 128 && cols != 128) {
        throw CnnInputSizeException();
    }
    const CnnMatrix origin{3, rows, cols, rgb_arr};
    CnnMatrix mats[6];
    extern ConvParam conv_params[3];
    extern FcParam fc_params[1];
    ConvLayer(origin, mats[0], conv_params[0]);
    MaxPoolingLayer(mats[0], mats[1]);
    ConvLayer(mats[1], mats[2], conv_params[1]);
    MaxPoolingLayer(mats[2], mats[3]);
    ConvLayer(mats[3], mats[4], conv_params[2]);
    Relu(mats[4]);
    FcLayer(mats[4], mats[5], fc_params[0]);
    float res = GetScore(mats[5], 1);
    for (const auto & mat : mats) {
        mat.Destroy();
    }
    return res;
}