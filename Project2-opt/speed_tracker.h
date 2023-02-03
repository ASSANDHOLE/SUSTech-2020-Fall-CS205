//
// Created by AnGuangyan on 2021/1/3.
//

#ifndef PROJECT2_SPEED_TRACKER_H
#define PROJECT2_SPEED_TRACKER_H

#include <chrono>
#include <iostream>
#include <cstring>
#include <sstream>

template<typename T>
inline std::string RoundTo(T &&val, int precision = 2) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << val;
    return ss.str();
}

#define TIME_INIT auto _time_start = std::chrono::steady_clock::now();\
auto _time_end = _time_start;\
auto _duration = 0L;
#define TIME_START _time_start = std::chrono::steady_clock::now();
#define TIME_END(NAME) _time_end = std::chrono::steady_clock::now();\
_duration = std::chrono::duration_cast<std::chrono::microseconds>(_time_end - _time_start).count();\
std::cout << (NAME) << " -- Duration: " << RoundTo(float(_duration)/1000.f, 2) << "ms" << std::endl;

#define INIT_GET_NUM std::stringstream _ss;
#define GET_FILES_NUM(vec) _ss.clear();_ss << "ALL " << (vec).size() << " FILES";
#define GET_STR _ss.str()

#endif //PROJECT2_SPEED_TRACKER_H
