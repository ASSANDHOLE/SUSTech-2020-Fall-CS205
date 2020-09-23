//
// Created by AGY on 2020/9/22.
//

#include "Strategies.h"
#include <map>
#include <string>
#include <iostream>

using namespace std;

void clearStorage();

void listVars();

void forStatement();

void forStatementN();

void deleteFunction(string &);

void clearFunctions();

void strategyHelp() {
    cout
            << "Commands:" << endl
            << "    Exit: exit the calculator (exit Big Number mode if in)" << endl
            << "    Clear: clear variable storage" << endl
            << "    List: list all variable with value" << endl
            << "    For: use \"for\" method, more info when entered" << endl
            << "    ForN: \"For\" in big number mode (can be accessed any where)" << endl
            << "    BigN: big number mode, functions not supported" << endl
            << "    Example: examples of using variables" << endl
            << "Functions:" << endl
            << R"(    "sqrt", "sin", "cos", "tan", "sinh", "cosh", "tanh", "log", "logTen";)" << endl
            << R"(    where "log" == log2 & "logTen" == log10 )" << endl
            << "Special:" << endl
            << "    Define (function name),(expression)" << endl
            << "    Define -help for more detail" << endl
            << "    Delete (function name)" << endl;

}

void strategyVariableExample(){
    cout
            << "the following are ordered inputs:" << endl
            << "x=2" << endl
            << "y=x*3.5+2" << endl
            << "z=sqrt(y)" << endl
            << "z+x" << endl
            << "the output is 5" << endl
            << "if you want to see the value of one variable, " << endl
            << "just input it's name" << endl
            << "x" << endl
            << "the output is 2" << endl;
}

void strategyDefineExample(){
    cout
            << "use 'x' as function variables, "
            << "the following are ordered inputs:" << endl
            << "Define funcA,3*3+x" << endl
            << "3*3+funcA(11)" << endl
            << "the output is 29" << endl
            << "if you want to delete one function, " << endl
            << "just use Delete (function name)" << endl
            << "example: Delete funcA" << endl;
}

CStrategies::CStrategies() {
    this->strategyMap["help"] = strategyHelp;
    this->strategyMap["Help"] = strategyHelp;
    this->strategyMap["Example"] = strategyVariableExample;
    this->strategyMap["Clear"] = clearStorage;
    this->strategyMap["List"] = listVars;
    this->strategyMap["For"] = forStatement;
    this->strategyMap["ForN"] = forStatementN;
    this->strategyMap["Clear Function"] = clearFunctions;
    this->strategyMap["Define -help"] = strategyDefineExample;
}

bool CStrategies::doStrategy(string &key) {
    map<string, STRATEGY>::iterator iterator;
    if ((iterator = this->strategyMap.find(key)) != this->strategyMap.end()) {
        iterator->second();
        return true;
    }
    return false;
}
