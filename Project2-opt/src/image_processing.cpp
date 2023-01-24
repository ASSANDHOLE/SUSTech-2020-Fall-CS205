//
// Created by An Guangyan on 2023-01-24.
//

#include <face_detect_cnn.h>

#include <opencv2/opencv.hpp>

std::tuple<float*, std::tuple<int, int, int>>
GetRgbArrayFromImage(const std::string &file_name) {
    using namespace cv;
    Mat mat = imread(file_name, IMREAD_COLOR);
    std::vector<Mat> img_channels;
    if (mat.empty()) {
        throw std::runtime_error("Image is empty");
    }
    if (mat.channels() != 3) {
        throw std::runtime_error(
                std::string("Image channel is not 3, but ") +
                std::to_string(mat.channels())
                );
    }
    cv::split(mat, img_channels);
    const size_t total_0 = img_channels[0].total();
    const size_t total_1 = img_channels[1].total();
    const size_t total_2 = img_channels[2].total();

    auto *img_data = new ALIGN_512 float[total_0 + total_1 + total_2];
    for (int i = 0; i < total_2; ++i) {
        img_data[i] = (float) img_channels[2].data[i] / 255.0f;
    }

    for (int i = 0; i < total_1; ++i) {
        img_data[i + total_2] = (float) img_channels[1].data[i] / 255.0f;
    }

    const size_t total_1_2 = total_1 + total_2;
    for (int i = 0; i < img_channels[0].total(); ++i) {
        img_data[i + total_1_2] = (float) img_channels[0].data[i] / 255.0f;
    }
    return {img_data, {mat.channels(), mat.rows, mat.cols}};
}
