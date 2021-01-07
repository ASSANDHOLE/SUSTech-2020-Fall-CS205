#include <iostream>
#include "face_detect_cnn.h"
#include <vector>
#include "opencv2/opencv.hpp"
#include "speed_tracker.h"

#ifdef WIN32
#include <io.h>
#else
#include <memory.h>
#include <dirent.h>
#endif

std::string GetFileName(std::string full_path) {
    full_path.replace(0, full_path.find_last_of('\\') + 1, "");
    return full_path;
}

void PrintImage(const std::string &name) {
    using namespace cv;
    Mat mat = imread(name, IMREAD_COLOR);
    std::vector<Mat> img_channels;
    split(mat, img_channels);
    auto *img_data = new float[(int) (img_channels[0].total() + img_channels[1].total()
                                      + img_channels[2].total())];
#ifdef ENABLE_OPENMP
#pragma omp simd
#endif
    for (int i = 0; i < img_channels[2].total(); ++i) {
        img_data[i] = (float) img_channels[2].data[i] / 255.0f;
    }
#ifdef ENABLE_OPENMP
#pragma omp simd
#endif
    for (int i = 0; i < img_channels[1].total(); ++i) {
        img_data[i + img_channels[2].total()] = (float) img_channels[1].data[i] / 255.0f;
    }
#ifdef ENABLE_OPENMP
#pragma omp simd
#endif
    for (int i = 0; i < img_channels[0].total(); ++i) {
        img_data[i + img_channels[2].total() + img_channels[1].total()] =
                (float) img_channels[0].data[i] / 255.0f;
    }
    std::cout << "img_name: ";
    float out = GetScore128x128Rgb(img_data);
    std::cout << GetFileName(name) << "\nScore:    " << out << "\n\n";
}

void GetAllFilesRbgArr(std::vector<std::string> &file_names, std::vector<float*> &arrs) {
    using namespace cv;
    for (const auto &file_name : file_names) {
        Mat mat = imread(file_name, IMREAD_COLOR);
        std::vector<Mat> img_channels;
        split(mat, img_channels);
        auto *img_data = new float[(int) (img_channels[0].total() + img_channels[1].total()
                                          + img_channels[2].total())];
#ifdef ENABLE_OPENMP
#pragma omp simd
#endif
        for (int i = 0; i < img_channels[2].total(); ++i) {
            img_data[i] = (float) img_channels[2].data[i] / 255.0f;
        }
#ifdef ENABLE_OPENMP
#pragma omp simd
#endif
        for (int i = 0; i < img_channels[1].total(); ++i) {
            img_data[i + img_channels[2].total()] = (float) img_channels[1].data[i] / 255.0f;
        }
#ifdef ENABLE_OPENMP
#pragma omp simd
#endif
        for (int i = 0; i < img_channels[0].total(); ++i) {
            img_data[i + img_channels[2].total() + img_channels[1].total()] =
                    (float) img_channels[0].data[i] / 255.0f;
        }
        arrs.push_back(img_data);
    }
}

void GetAllFiles(const std::string &path, std::vector<std::string> &files, bool recursively = false) {
#ifdef WIN32
    long h_file;
    _finddata_t file_info{};
    std::string p;
    if ((h_file = _findfirst(p.assign(path).append("\\*").c_str(), &file_info)) != -1) {
        do {
            if ((file_info.attrib & _A_SUBDIR)) {
                if ((strcmp(file_info.name, ".") != 0 && strcmp(file_info.name, "..") != 0) && recursively) {
                    GetAllFiles(p.assign(path).append("\\").append(file_info.name), files);
                }
            } else {
                files.push_back(p.assign(path).append("\\").append(file_info.name));
            }
        } while (_findnext(h_file, &file_info) == 0);
        _findclose(h_file);
    }
#else
    DIR *dir = opendir(path.c_str());
    if (dir == nullptr) {
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (entry->d_type == DT_DIR){
            if (!recursively || strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            std::string dir_new = path + "/" + entry->d_name;
            std::vector<std::string> temp_path;
            GetFilesList(dir_new, temp_path);
            files.insert(files.end(), temp_path.begin(), temp_path.end());
        }else {
            std::string name = entry->d_name;
            std::string img_dir = path;
            img_dir += "/" + name;
            files.push_back(img_dir);
        }
    }
    closedir(dir);
#endif
}

int main() {
    //the dir witch contains all the img files
    const std::string kImagesPath = "path/to/imgs";

    std::vector<std::string> file_names;
    std::vector<float*> rgb_arrs;
    GetAllFiles(kImagesPath, file_names);
    INIT_GET_NUM
    GET_FILES_NUM(file_names)
    TIME_INIT

#ifdef SPEED_RUN
    GetAllFilesRbgArr(file_names, rgb_arrs);
    for (int i = 0; i < 3; ++i) {
        for (float *arr : rgb_arrs) {
            GetScore128x128Rgb(arr);
        }
    }
    TIME_START
    for (float *arr : rgb_arrs) {
        GetScore128x128Rgb(arr);
    }
#else
    TIME_START
    for (auto & file_name : file_names) {
        PrintImage(file_name);
    }
#endif

    TIME_END(GET_STR)
    std::cout.flush();
    return 0;
}