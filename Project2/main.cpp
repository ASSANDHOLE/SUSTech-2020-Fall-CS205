#include <iostream>
#include "face_detect_cnn.h"
#include <vector>
#include "opencv2/opencv.hpp"
#include "timer_local.h"

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
    for (int i = 0; i < img_channels[2].total(); ++i) {
        img_data[i] = (float) img_channels[2].data[i] / 255.0f;
    }
    for (int i = 0; i < img_channels[1].total(); ++i) {
        img_data[i + img_channels[2].total()] = (float) img_channels[1].data[i] / 255.0f;
    }
    for (int i = 0; i < img_channels[0].total(); ++i) {
        img_data[i + img_channels[2].total() + img_channels[1].total()] =
                (float) img_channels[0].data[i] / 255.0f;
    }
    std::cout << "img_name: ";
    float out = GetConfidenceScore128x128rbg(img_data, img_channels[0].rows, img_channels[0].cols);
    std::cout << GetFileName(name) << "\nScore:    " << out << "\n\n";
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
    const std::string kImagesPath = R"(D:\samples)";

    std::vector<std::string> file_names;
    GetAllFiles(kImagesPath, file_names);
    TIME_INIT

    TIME_START
    for (const auto &file_name : file_names) {
        PrintImage(file_name);
    }
    TIME_END("Ori")

    std::cout.flush();
    return 0;
}