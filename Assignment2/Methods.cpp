//
// Created by AGY on 2020/9/23.
//

#include "Methods.h"
#include <string>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

bool CMethods::add(string &name, string &expression) {
    if (expression.empty()) {
        return false;
    }
    this->methodMap[name] = expression;
    ++this->currentIndex;
    return true;
}

string CMethods::check(string &key) {
    string ans;
    map<string, string>::iterator iterator;
    if ((iterator = this->methodMap.find(key)) != this->methodMap.end()) {
        ans = iterator->second;
    }
    return ans;
}

void CMethods::erase(string &key) {
    map<string, string>::iterator iterator;
    if ((iterator = this->methodMap.find(key)) != this->methodMap.end()) {
        methodMap.erase(iterator);
    }
}

void CMethods::clear() {
    this->currentIndex = 0;
    if (!this->methodMap.empty()) {
        this->methodMap.erase(this->methodMap.begin(), this->methodMap.end());
    }
}

void CMethods::replace(string &expression, string &var) {
    vector<string> expParts;
    stringstream expStream(expression);
    string part;
    while (getline(expStream, part, 'x')) {
        expParts.push_back(part);
    }
    if (expression.at(expression.length() - 1) == 'x') {
        expParts.emplace_back("");
    }
    string ans = expParts.at(0);
    if (expParts.size() > 1) {
        ans = expParts.at(0);
        for (int i = 1; i < expParts.size(); ++i) {
            ans += "(" + var + ")" + expParts.at(i);
        }
        expression = ans;
        expression = "(" + expression + ")";
    }
}