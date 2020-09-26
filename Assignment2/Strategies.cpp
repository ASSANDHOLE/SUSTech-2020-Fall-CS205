//
// Created by AGY on 2020/9/22.
//

#include "Strategies.h"
#include <map>
#include <string>
#include <iostream>
#include <Windows.h>

using namespace std;

void clearStorage();

void listVars();

void forStatement();

void forStatementN();

void deleteFunction(string &);

void clearFunctions();

void strategyHelp() {
    MessageBoxW(GetForegroundWindow(),L"Commands:\n    Exit: exit the calculator (exit Big Number mode if in)\n    Clear: clear variable storage\n    List: list all variable with value\n    For: use \"for\" method, more info when entered\n    ForN: \"For\" in big number mode (can be accessed any where)\n    BigN: big number mode, pre defined functions not supported\n    CALC: open system's calculator\nPredefined functions:\n    \"sqrt\", \"sin\", \"cos\", \"tan\", \"sinh\"\n    \"cosh\", \"tanh\", \"log\", \"logTen\", \"gamma\"\n    where \"log\" == log2 & \"logTen\" == log10)\nSpecial:\n    Define (function name),(expression)\n    Delete (function name)\n    Define -help for more detail\n    USE_SYSTEM (expression),(*delay) to use the system calculator\n        - where delay time(ms)-(default: 1000) is the interval of opening calc and start input expression\n          this usually depends on the speed of your computer\n",L"Help",1);
}

void strategyVariableExample() {
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

void strategyDefineExample() {
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

void strategyCalc() {
    system("calc");
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
    this->strategyMap["CALC"] = strategyCalc;
}

bool CStrategies::doStrategy(string &key) {
    map<string, STRATEGY>::iterator iterator;
    if ((iterator = this->strategyMap.find(key)) != this->strategyMap.end()) {
        iterator->second();
        return true;
    }
    return false;
}
