//
// Created by AnGuangyan on 2020/12/16.
//

#ifndef PROJECT2_HUMAN_DETECT_CNN_H
#define PROJECT2_HUMAN_DETECT_CNN_H

#include <exception>

class CnnMatrix {
public:
    float Get(int channel, int row, int col) const {
        if (row < 0 || row > rows_ - 1 || col < 0 || col > cols_ - 1) {
            return 0;
        }
        return data_[channel * cols_ * rows_ + row * cols_ + col];
    }

    float &Set(float val, int channel, int row, int col) const {
        return (data_[channel * cols_ * rows_ + row * cols_ + col] = val);
    }

    int Total() const {
        return channels_ * rows_ * cols_;
    }

    void Destroy() const {
        delete [] data_;
    }

    void init(int channels, int rows, int cols, float *data = nullptr) {
        channels_ = channels;
        rows_ = rows;
        cols_ = cols;
        data_ = data;
    }
    int channels_;
    int cols_;
    int rows_;
    float *data_;
};

class CnnInputSizeException : public std::exception {
    const char *what() const noexcept override {
        return "Input size is not 3x128x128";
    }
};

float GetConfidenceScore128x128rbg(float *rgb_arr, int rows, int cols);

#endif //PROJECT2_HUMAN_DETECT_CNN_H
