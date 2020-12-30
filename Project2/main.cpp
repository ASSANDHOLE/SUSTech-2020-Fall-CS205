#include <iostream>
#include "human_detect_cnn.h"
#include <vector>
#include "opencv2/opencv.hpp"
#include <io.h>

//Get file name by it's full path
std::string GetFileName(std::string full_path) {
    full_path.replace(0, full_path.find_last_of('\\') + 1, "");
    return full_path;
}

//Get score by image in given full path and Print
void PrintImage(const std::string &name) {
    using namespace cv;
    Mat mat = imread(name, IMREAD_COLOR);
    std::vector<Mat> img_channels;
    split(mat, img_channels);
    auto *img_data = new float [(int)(img_channels[0].total() + img_channels[1].total()
                                      + img_channels[2].total())];
    for (int i = 0; i < img_channels[2].total(); ++i) {
        img_data[i] = (float)img_channels[2].data[i] / 255.0f;
    }
    for (int i = 0; i < img_channels[1].total(); ++i) {
        img_data[i + img_channels[2].total()] = (float)img_channels[1].data[i] / 255.0f;
    }
    for (int i = 0; i <img_channels[0].total(); ++i) {
        img_data[i + img_channels[2].total() + img_channels[1].total()] =
                (float)img_channels[0].data[i] / 255.0f;
    }
    std::cout << "img_name: ";
    float out = GetConfidenceScore128x128rbg(img_data, img_channels[0].rows, img_channels[0].cols);
    std::cout << GetFileName(name) << "\nScore:    " << out << "\n\n";
}

//Get all file in the path, recursively = (recursively)
void GetAllFiles(const std::string &path, std::vector<std::string>& files, bool recursively = false) {
    long h_file;
    _finddata_t file_info{};
    std::string p;
    if((h_file = _findfirst(p.assign(path).append("\\*").c_str(), &file_info)) != -1) {
        do {
            if((file_info.attrib & _A_SUBDIR)) {
                if((strcmp(file_info.name, ".") != 0 && strcmp(file_info.name, "..") != 0) && recursively) {
                    GetAllFiles(p.assign(path).append("\\").append(file_info.name), files);
                }
            } else {
                files.push_back(p.assign(path).append("\\").append(file_info.name));
            }
        } while(_findnext(h_file, &file_info) == 0);
        _findclose(h_file);
    }
}

int main() {
    std::vector<std::string> file_names;
    GetAllFiles(R"(G:\ComputerScience\C\CS205\Project2\samples)", file_names);
    for (const auto &file_name : file_names) {
        PrintImage(file_name);
    }
    std::cout.flush();
    return 0;
}
