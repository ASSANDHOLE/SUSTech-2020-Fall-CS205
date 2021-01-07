#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"
//
// Created by AnGuangyan on 2021/1/6.
//

#include "face_detect_cnn.h"
#include "cnn_param.h"

#include <cmath>
#include <cstring>

float *Pad01(const float *arr) {
    auto res = new float [50700]();
    const int size = sizeof(float) * 128;
#ifdef ENABLE_OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 128; ++j) {
            memcpy(&res[i * 16900 + j * 130 + 1], &arr[i * 16384 + j * 128], size);
        }
    }
    return res;
}

float *Pad02(const float *arr) {
    auto res = new float [9248]();
    const int size = sizeof(float) * 15;
#ifdef ENABLE_OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 15; ++j) {
            memcpy(&res[i * 289 + (j + 1) * 17 + 1], &arr[i * 225 + j * 15], size);
        }
    }
    return res;
}

#define AXP(a, b) arr[(a)] * conv_param.p_weight[(b)]

float *ConvCpu01(const float *arr, const ConvParam &conv_param) {
    auto res = new float [65536]();
#ifdef ENABLE_OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < 16; ++i) {
        int inner_arr_index;
        int inner_weight_index;
        for (int j = 0; j < 64; ++j) {
            for (int k = 0; k < 64; ++k) {
                inner_arr_index = j * 260  + k * 2;
                inner_weight_index = i * 27;
                res[i * 4096 + j * 64 + k] +=
                AXP(inner_arr_index, inner_weight_index) +
                AXP(inner_arr_index + 1, inner_weight_index + 1) +
                AXP(inner_arr_index + 2, inner_weight_index + 2) +
                AXP(inner_arr_index + 130, inner_weight_index + 3) +
                AXP(inner_arr_index + 131, inner_weight_index + 4) +
                AXP(inner_arr_index + 132, inner_weight_index + 5) +
                AXP(inner_arr_index + 260, inner_weight_index + 6) +
                AXP(inner_arr_index + 261, inner_weight_index + 7) +
                AXP(inner_arr_index + 262, inner_weight_index + 8) +
                AXP(inner_arr_index + 16900, inner_weight_index + 9) +
                AXP(inner_arr_index + 16901, inner_weight_index + 10) +
                AXP(inner_arr_index + 16902, inner_weight_index + 11) +
                AXP(inner_arr_index + 17030, inner_weight_index + 12) +
                AXP(inner_arr_index + 17031, inner_weight_index + 13) +
                AXP(inner_arr_index + 17032, inner_weight_index + 14) +
                AXP(inner_arr_index + 17160, inner_weight_index + 15) +
                AXP(inner_arr_index + 17161, inner_weight_index + 16) +
                AXP(inner_arr_index + 17161, inner_weight_index + 17) +
                AXP(inner_arr_index + 33800, inner_weight_index + 18) +
                AXP(inner_arr_index + 33801, inner_weight_index + 19) +
                AXP(inner_arr_index + 33802, inner_weight_index + 20) +
                AXP(inner_arr_index + 33930, inner_weight_index + 21) +
                AXP(inner_arr_index + 33931, inner_weight_index + 22) +
                AXP(inner_arr_index + 33932, inner_weight_index + 23) +
                AXP(inner_arr_index + 34060, inner_weight_index + 24) +
                AXP(inner_arr_index + 34061, inner_weight_index + 25) +
                AXP(inner_arr_index + 34062, inner_weight_index + 26);
            }
        }
    }
    for (int i = 0; i < 16; ++i) {
        const float bias = conv_param.p_bias[i];
        for (int j = 0; j < 4096; ++j) {
            res[i * 4096 + j] += bias;
        }
    }
    return res;
}

float *ConvCpu02(const float *arr, const ConvParam &conv_param) {
    auto res = new float [28800]();
#ifdef ENABLE_OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < 32; ++i) {
        int inner_arr_index;
        int inner_weight_index;
            for (int j = 0; j < 30; ++j) {
                for (int k = 0; k < 30; ++k) {
                    const int res_index = i * 900 + j * 30 + k;
                    for (int channel = 0; channel < 16; ++channel) {
                        inner_arr_index = channel * 1024 + j * 32 + k;
                        inner_weight_index = i * 144 + channel * 9;
                        res[res_index] +=
                                AXP(inner_arr_index, inner_weight_index) +
                                AXP(inner_arr_index + 1, inner_weight_index + 1) +
                                AXP(inner_arr_index + 2, inner_weight_index + 2) +
                                AXP(inner_arr_index + 32, inner_weight_index + 3) +
                                AXP(inner_arr_index + 33, inner_weight_index + 4) +
                                AXP(inner_arr_index + 34, inner_weight_index + 5) +
                                AXP(inner_arr_index + 64, inner_weight_index + 6) +
                                AXP(inner_arr_index + 65, inner_weight_index + 7) +
                                AXP(inner_arr_index + 66, inner_weight_index + 8);
                    }
                }
            }
    }
    for (int i = 0; i < 32; ++i) {
        const float bias = conv_param.p_bias[i];
        for (int j = 0; j < 900; ++j) {
            res[i * 900 + j] += bias;
        }
    }
    return res;
}

float *ConvCpu03(const float *arr, const ConvParam &conv_param) {
    auto res = new float [2048]();
#ifdef ENABLE_OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < 32; ++i) {
        int inner_arr_index;
        int inner_weight_index;
        for (int j = 0; j < 8; ++j) {
            for (int k = 0; k < 8; ++k) {
                const int res_index = i * 64 + j * 8 + k;
                for (int channel = 0; channel < 32; ++channel) {
                    inner_arr_index = channel * 289 + j * 34 + k * 2;
                    inner_weight_index = i * 288 + channel * 9;
                    res[res_index] +=
                            AXP(inner_arr_index, inner_weight_index) +
                            AXP(inner_arr_index + 1, inner_weight_index + 1) +
                            AXP(inner_arr_index + 2, inner_weight_index + 2) +
                            AXP(inner_arr_index + 17, inner_weight_index + 3) +
                            AXP(inner_arr_index + 18, inner_weight_index + 4) +
                            AXP(inner_arr_index + 19, inner_weight_index + 5) +
                            AXP(inner_arr_index + 34, inner_weight_index + 6) +
                            AXP(inner_arr_index + 35, inner_weight_index + 7) +
                            AXP(inner_arr_index + 36, inner_weight_index + 8);
                }
            }
        }
    }
    for (int i = 0; i < 32; ++i) {
        const float bias = conv_param.p_bias[i];
        for (int j = 0; j < 64; ++j) {
            res[i * 64 + j] += bias;
        }
    }
    return res;
}

inline float Max(float a, float b) {
    return a > b ? a : b;
}

float *MaxPool01(const float *arr) {
    auto res = new float [16384];
    int base;
#ifdef ENABLE_OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 32; ++j) {
            for (int k = 0; k < 32; ++k) {
                base = i * 4096 + j * 128 + k * 2;
                res[i * 1024 + j * 32 + k] =
                        Max(Max(Max(arr[(base)], arr[(base) + 1]),
                                arr[(base) + 64]), arr[(base) + 65]);
            }
        }
    }
    return res;
}

float *MaxPool02(const float *arr) {
    auto res = new float [7200];
    int base;
#ifdef ENABLE_OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 15; ++j) {
            for (int k = 0; k < 15; ++k) {
                base = i * 900 + j * 60 + k * 2;
                res[i * 225 + j * 15 + k] =
                        Max(Max(Max(arr[(base)], arr[(base) + 1]),
                                arr[(base) + 30]), arr[(base) + 31]);
            }
        }
    }
    return res;
}

void Relu(float *arr, int length) {
#ifdef ENABLE_OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < length; ++i) {
        if (arr[i] < 0) {
            arr[i] = 0;
        }
    }
}

float DotProduct2048(const float *arr_a, const float *arr_b) {
    float res = 0;
#ifdef ENABLE_OPENMP
#pragma omp simd
#else
#pragma simd
#endif
    for (int i = 0; i < 2048; ++i) {
        res += arr_a[i] * arr_b[i];
    }
    return res;
}

float *FullConnected(float *arr, const FcParam &fc_param) {
    auto res = new float [2];
    res[0] = DotProduct2048(arr, fc_param.p_weight) + fc_param.p_bias[0];
    res[1] = DotProduct2048(arr, &fc_param.p_weight[2048]) + fc_param.p_bias[1];
    return res;
}

float SoftMax(const float *arr) {
    float exp0 = std::exp(arr[0]);
    float exp1 = std::exp(arr[1]);
    if (std::isinf(exp0)) {
        return 0;
    } else if (std::isinf(exp1)) {
        return 1;
    }
    return (exp1 / (exp0 + exp1));
}

float GetScore128x128Rgb(const float *rbg_arr) {
    extern ConvParam conv_params[3];
    extern FcParam fc_params[1];
    auto res01 = Pad01(rbg_arr);
    auto res02 = ConvCpu01(res01, conv_params[0]);
    auto res03 = MaxPool01(res02);
    Relu(res03, 16384);
    delete [] res01;
    delete [] res02;
    auto res04 = ConvCpu02(res03, conv_params[1]);
    auto res05 = MaxPool02(res04);
    Relu(res05, 7200);
    delete [] res03;
    delete [] res04;
    auto res06 = Pad02(res05);
    auto res07 = ConvCpu03(res06, conv_params[2]);
    Relu(res07, 2048);
    auto res08 = FullConnected(res07, fc_params[0]);
    auto res = SoftMax(res08);
    delete [] res05;
    delete [] res06;
    delete [] res07;
    delete [] res08;
    return res;
}
#pragma clang diagnostic pop