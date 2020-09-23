//
// Created by AGY on 2020/9/23.
//

#ifndef ASSIGNMENT2_METHODS_H
#define ASSIGNMENT2_METHODS_H

#include <string>
#include <map>

using namespace std;

class CMethods {
public:
    bool add(string &, string &);
    string check(string &);
    void erase(string &);
    void clear();
    static void replace(string &, string &);

private:
    int currentIndex = 0;
    map<string,string> methodMap;
};


#endif //ASSIGNMENT2_METHODS_H
