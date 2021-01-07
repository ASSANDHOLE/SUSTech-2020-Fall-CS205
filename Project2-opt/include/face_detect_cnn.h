//
// Created by AnGuangyan on 2020/12/16.
//

#ifndef PROJECT2_FACE_DETECT_CNN_H
#define PROJECT2_FACE_DETECT_CNN_H

#include <exception>
#include "cnn_param.h"

/**
 * Get confidence score if the given pic has face,
 * the larger the value, get greater the chance to have a face
 * @param rgb_arr the input img array sorted by rbg and row major
 * @param rows how many rows the pic have, if it is not 128, an exception occurred
 * @param cols same as {@code rows}
 * @return 0 -> 1 float, the higher the better chance of having face
 */
float GetConfidenceScore128x128rbg(float *rgb_arr, int rows, int cols);

float GetScore128x128Rgb(const float *rbg_arr);

#endif //PROJECT2_FACE_DETECT_CNN_H
