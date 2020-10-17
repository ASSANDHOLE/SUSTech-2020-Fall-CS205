#include <iostream>
#include <vector>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <thread>
#include <numeric>
#include <functional>
#include <Windows.h>
#include "cblas.h"
#define TIME_COUNTED 2000000

using namespace std;

/**
 * calculate the dot product of two vectors(float) with multithreading
 * @tparam Input type of vectors
 * @param start1 start of the first vector
 * @param end1 end of the first vector
 * @param start2 start of the second vector
 * @return the result of dot product
 */
template <class Input>
float sDotProduct(const Input &start1, const Input &end1, const Input &start2){
    const auto calculate = [start1, start2](int index, Input end, float &result){
        result = std::inner_product(start1+index, end, start2+index, 0.0f);
    };
    const long length = end1 - start1;
    if (length <= 0){
        return 0;
    }
    unsigned long const MIN_VAL = 200;
    unsigned long const MAX_THR = (length + MIN_VAL - 1) / MIN_VAL;
    unsigned long const CONCURRENCY = std::thread::hardware_concurrency();
    unsigned long const THR_NUM = min(CONCURRENCY != 0 ? CONCURRENCY : 2, MAX_THR);
    unsigned long const BLOCK_SIZE = length / THR_NUM;
    std::vector<std::thread> threads(THR_NUM - 1);
    std::vector<float> results(THR_NUM);
    int index = 0;
    for (int i = 0; i < (THR_NUM - 1); i++){
        threads[i] = std::thread(calculate, index, start1+index+BLOCK_SIZE ,std::ref(results[i]));
        index+=BLOCK_SIZE;
    }
    calculate(index, end1, results[THR_NUM-1]);
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return std::accumulate(results.begin(), results.end(), 0.0f);
}

/**
 * a lot like the (float)sDotProduct(...) but the return value is replaced by float &result
 */
template <class Input>
void calculateSProduct(const Input &start1, const Input &end1, const Input &start2, float &result){
    result = sDotProduct(start1, end1, start2);
}

/**
 * see calculateSProduct(...)
 * @param result
 * @return the duration of the calculating process
 */
template <class Input>
double calculateSProductWithTime(const Input &start1, const Input &end1, const Input &start2, float &result){
    LARGE_INTEGER freq, time1, time2;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&time1);
    calculateSProduct(start1, end1, start2, result);
    QueryPerformanceCounter(&time2);
    double duration = (double)(time2.QuadPart - time1.QuadPart)/(double)freq.QuadPart;
    return duration*1000000;
}

/**
 * @return '0' if the number of float int the vec is less than 2,000,000; duration(ns) if more than that
 */
template <class Input>
double sDotProduct(const Input &start1, const Input &end1, const Input &start2, float &result){
    if (start1>=end1){
        return 0;
    }
    int distance = end1 - start1;
    if(distance < TIME_COUNTED){
        calculateSProduct(start1, end1, start2, result);
        return 0;
    } else {
        return calculateSProductWithTime(start1, end1, start2, result);
    }
}

int main() {
    vector<float> v1,v2;
    const int length = TIME_COUNTED/20;

    /*for (int i = 0; i < length; ++i) {
        v1.push_back(i);
        v2.push_back(i);
    }*/
    float f1[] = {1.1,2,3,4,5,6,7,8,9,10};
    float f2[] = {1,2,3,4,5,6,7,8,9,10};

    cout << "prepared\n" << endl;
    double time;
    float value;

    //time= calculateSProductWithTime(v1.begin(),v1.end(),v2.begin(),value);
    time= calculateSProductWithTime(&f1[0],&f1[10],&f2[0],value);
    cout << "Time: " << time << endl << "value: " << value << endl;

    cout << "\n**OPENBLAS**\n" << endl;

    LARGE_INTEGER freq, time1, time2;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&time1);

    float cblas=cblas_sdot(sizeof(f1)/sizeof(f1[0]),f1,0,f2,0);

    QueryPerformanceCounter(&time2);
    double duration1 = (double)(time2.QuadPart - time1.QuadPart)/(double)freq.QuadPart;
    cout << "time: " << duration1*1000000 << "\nvalue: " << cblas << endl;
    cout << "\n**END_OPENBLAS**\n" << endl;
    return 0;
}
