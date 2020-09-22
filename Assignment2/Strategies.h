//
// Created by AGY on 2020/9/22.
//

#ifndef ASSIGNMENT2_STRATEGIES_H
#define ASSIGNMENT2_STRATEGIES_H

#include <map>
#include <string>

using namespace std;

class CStrategies {
public:
    typedef void(*STRATEGY)();

    CStrategies();

    bool doStrategy(string &);

private:
    map<string, STRATEGY> strategyMap;
};


#endif //ASSIGNMENT2_STRATEGIES_H
