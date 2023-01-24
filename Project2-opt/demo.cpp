#include <iostream>
#include <vector>
#include <filesystem>
#include <sstream>

#include <face_detect_cnn.h>
#include "speed_tracker.h"

namespace fs = std::filesystem;

std::string GetFileName(const std::string &full_path) {
    return fs::path(full_path).filename().string();
}

void PrintImage(const std::string &name) {
    auto [img_data, size] = GetRgbArrayFromImage(name);
    auto [channel, width, height] = size;
    if (channel != 3 or width != 128 or height != 128) {
        std::cout << "Image size is not 3x128x128, skip it: " << name << std::endl;
        return;
    }
    std::cout << "img_name: ";
    float out = GetScore128x128Rgb(img_data);
    std::cout << GetFileName(name) << "\nScore:    " << out << "\n\n";
    free(img_data);
}

void GetAllFilesRbgArr(const std::vector<std::string> &file_names, std::vector<float*> &arrs) {
    for (const auto &file_name : file_names) {
        auto [img_data, size] = GetRgbArrayFromImage(file_name);
        auto [channel, width, height] = size;
        if (channel != 3 or width != 128 or height != 128) {
            std::cout << "Image size is not 3x128x128, skip it: " << file_name << std::endl;
            continue;
        }
        arrs.push_back(img_data);
    }
}

void GetAllFiles(const std::string &path,
                 std::vector<std::string> &files,
                 bool recursively = false,
                 bool sort = true) {
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path());
        } else if (recursively && entry.is_directory()) {
            GetAllFiles(entry.path(), files, true, false);
        }
    }
    if (sort) {
        std::sort(files.begin(), files.end());
    }
}

int main(int argc, char *argv[]) {
    // the dir witch contains all the img files
    bool speed_run = false;
    std::string image_path =
            fs::path(__FILE__).parent_path().string() +
            fs::path::preferred_separator + "samples";

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (strcmp(argv[i], "-s") == 0) {
                speed_run = true;
            } else if (strcmp(argv[i], "-h") == 0) {
                std::cout << "Usage: " << argv[0] << " [-s] [-h] [-d <dir>]\n"
                             "  -s: speed run\n"
                             "  -h: print this help\n"
                             "  -d: the dir witch contains all the img files\n";
                return 0;
            } else if (strcmp(argv[i], "-d") == 0) {
                if (i + 1 < argc) {
                    image_path = argv[i + 1];
                    ++i;
                } else {
                    std::cout << "Error: -d must be followed by a dir\n";
                    return 1;
                }
            }
        }
    }

    std::vector<std::string> file_names;
    std::vector<float*> rgb_arrs;
    GetAllFiles(image_path, file_names);

    INIT_GET_NUM
    GET_FILES_NUM(file_names)

    TIME_INIT

    if (speed_run) {
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
    } else {
        TIME_START
        for (auto & file_name : file_names) {
            PrintImage(file_name);
        }
        std::cout << "\nN.B. The 'Duration' includes IO time (print, file io).\n";
    }

    TIME_END(GET_STR)
    return 0;
}