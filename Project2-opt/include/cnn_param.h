//
// Created by AnGuangyan on 2020/12/17.
//

#ifndef PROJECT2_CNN_PARAM_H
#define PROJECT2_CNN_PARAM_H

#define ALIGN_512 (std::align_val_t(512))
#define ALIGN_AS_512 __attribute__((aligned(512)))

struct ConvParam {
    int pad;
    int stride;
    int kernel_size;
    int in_channels;
    int out_channels;
    float* p_weight;
    float* p_bias;
};

struct FcParam {
    int in_features;
    int out_features;
    float* p_weight;
    float* p_bias;
};

struct MatParam {
    int in;
    int out;
};

#endif //PROJECT2_CNN_PARAM_H
