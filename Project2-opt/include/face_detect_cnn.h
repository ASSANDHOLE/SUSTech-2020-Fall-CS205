//
// Created by AnGuangyan on 2020/12/16.
//

#ifndef PROJECT2_FACE_DETECT_CNN_H
#define PROJECT2_FACE_DETECT_CNN_H

#include <exception>
#include <tuple>

#include <cnn_param.h>

/**
 * Get the RGB array from image, if available.
 * If the image is not available,
 * throw an std::runtime_error with brief description
 * @param file_name the image file's path
 * @return (data array, (channel, width, height))
 */
std::tuple<float*, std::tuple<int, int, int>>
GetRgbArrayFromImage(const std::string &file_name);

/**
 * Get confidence score if the given pic has face,
 * the larger the value, get greater the chance to have a face
 * @param rgb_arr the input img array sorted by rbg and row major
 * @return 0 -> 1 float, the higher the better chance of having face
 */
float GetScore128x128Rgb(const float *rbg_arr);

#endif //PROJECT2_FACE_DETECT_CNN_H
