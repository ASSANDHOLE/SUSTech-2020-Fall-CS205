//
// Created by AnGuangyan on 2021/1/3.
//

#ifndef PROJECT2_TIMER_LOCAL_H
#define PROJECT2_TIMER_LOCAL_H

#include <chrono>
#include <iostream>

#define TIME_INIT auto _time_start = std::chrono::steady_clock::now();\
auto _time_end = _time_start;\
auto _duration = 0L;
#define TIME_START _time_start = std::chrono::steady_clock::now();
#define TIME_END(NAME) _time_end = std::chrono::steady_clock::now();\
_duration = std::chrono::duration_cast<std::chrono::milliseconds>(_time_end - _time_start).count();\
std::cout << (NAME) << " -- Duration: " << _duration << "ms" << std::endl;

#endif //PROJECT2_TIMER_LOCAL_H
