//
// Created by AnGuangyan on 2020/12/17.
//

#ifndef PROJECT2_CNN_PARAM_H
#define PROJECT2_CNN_PARAM_H

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

#endif //PROJECT2_CNN_PARAM_H
