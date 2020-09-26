#include <iostream>
#include <stack>
#include <string>
#include <exception>
#include <cmath>
#include "BigNumber.h"
#include "Strategies.h"
#include "Methods.h"
#include <Windows.h>

using namespace std;

int getType(string &);

void systemCalc(string &);

void initFunc(string &);

void initVar(string &);

void initVarN(string &);

void exchangeVar(string &);

void exchangeFunction(string &);

bool check(string);

void listVars();

bool isNumber(char);

string solveFunc(int, string);

double calculate(string);

CBigNumber calculateN(string);

void addZero(string &);

void forStatement();

void removeSpace(string &);

int compareOpe(char);

string removeExtraZeros(string);

void clearStorage();

void bigNumberMode();

void deleteFunction(string &);

void clearFunctions();

void clearStack();

/* store variables with name and value */
struct Var {
    string name;
    string value;
};
// the max variable number
const int MAX = 100;
const int INIT_VAR = 1;
const int CALC = 2;
const int DEFINE_FUNC = 3;
const int DELETE_FUNC = 4;
const int SYSTEM_CALC = 5;
stack<double> numStack; /* NOLINT */
stack<CBigNumber> bigNumStack; /* NOLINT */
stack<char> symStack; /* NOLINT */
CMethods methods; /* NOLINT */
//variables
Var vars[MAX];
//number of variables
int varNums = 0;
CStrategies strategies = CStrategies(); /* NOLINT */
string functionNames[10] = { /* NOLINT */
        "sqrt", "sin", "cos", "tan", "sinh",
        "cosh", "tanh", "log", "logTen", "gamma"
};
const int FUNCTION_NUMS = sizeof(functionNames) / sizeof(functionNames[0]);

class CInputTypeErrorExc : public exception {
public:
    static const char *message() {
        return "ERROR: input error";
    }
};

class CInitVarErrorExc : public exception {
public:
    static const char *message() {
        return "ERROR: init variable error";
    }
};

/* handle input functions */
class CFunctions {
public:
    /**
     * check if the (@param varName) is in the "string functionName[]"
     * @param varName the name of the function to check
     * @return return the number of the position of the name in the list. -1 if not in
     */
    static int checkName(string &varName) {
        for (int i = 0; i < FUNCTION_NUMS; ++i) {
            if (varName == functionNames[i]) {
                return i;
            }
        }
        return -1;
    }

    /**
     * calculate by function and value
     * @param funcNum the return number of "check(string &)"
     * @param value the value to be calculated
     * @return the ans, the same as (@param value) if (@param funcNum) == -1
     */
    static double proceed(int funcNum, double value) {
        switch (funcNum) {
            case 0:
                return sqrt(value);
            case 1:
                return sin(value);
            case 2:
                return cos(value);
            case 3:
                return tan(value);
            case 4:
                return sinh(value);
            case 5:
                return cosh(value);
            case 6:
                return tanh(value);
            case 7:
                return log(value);
            case 8:
                return log10(value);
            case 9:
                return tgamma(value);
            default:
                return value;
        }
    }
};

int main() {
    cout << "welcome to the calculator" << endl;
    cout << "try to use \"Help\" to get some help" << endl;
    while (true) {
        numStack = stack<double>();
        symStack = stack<char>();
        string expression;
        getline(cin, expression);
        if (expression == "Exit") {
            cout << "EXIT..." << endl;
            return 0;
        }
        if (expression == "BigN") {
            bigNumberMode();
            continue;
        }
        if (strategies.doStrategy(expression)) {
            continue;
        }
        try {
            int type = getType(expression);
            if (type == INIT_VAR) {
                initVar(expression);
            } else if (type == CALC) {
                exchangeVar(expression);
                exchangeFunction(expression);
                cout << removeExtraZeros(solveFunc(-1, expression)) << endl;
            } else if (type == DEFINE_FUNC) {
                initFunc(expression);
            } else if (type == DELETE_FUNC) {
                deleteFunction(expression);
            } else {
                systemCalc(expression);
            }
        } catch (CInputTypeErrorExc &e) {
            cout << CInputTypeErrorExc::message() << endl;
            clearStack();
            continue;
        } catch (CInitVarErrorExc &e) {
            cout << CInitVarErrorExc::message() << endl;
            clearStack();
            continue;
        } catch (exception &e) {
            cout << e.what() << endl;
            cout << "sorry, but all the storage has been cleared" << endl;
            clearStorage();
            continue;
        }
    }
}

int getType(string &in) {
    if (in.length() > 10 && in.substr(0, 7) == "Define ") {
        return DEFINE_FUNC;
    }
    if (in.length() > 7 && in.substr(0, 7) == "Delete ") {
        return DELETE_FUNC;
    }
    if (in.length() > 11 && in.substr(0, 11) == "USE_SYSTEM ") {
        return SYSTEM_CALC;
    }
    int pos = in.find('=');
    if (pos > 0 && pos < in.length() - 1) {
        return INIT_VAR;
    }
    return CALC;
}

void systemCalc(string &exp) {
    int interval = 1000;
    exp = exp.substr(11);
    int pos;
    pos = exp.find(',');
    if (pos != string::npos) {
        interval = stoi(exp.substr(pos + 1));
        exp = exp.substr(0, pos);
    }
    removeSpace(exp);
    system("CALC");
    Sleep(interval);
    for (char i : exp) {
        if (i == '(') {
            keybd_event(VK_SHIFT, 0, 0, 0);
            keybd_event('9', 0, 0, 0);
            keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event('9', 0, KEYEVENTF_KEYUP, 0);
        } else if (i == ')') {
            keybd_event(VK_SHIFT, 0, 0, 0);
            keybd_event('0', 0, 0, 0);
            keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event('0', 0, KEYEVENTF_KEYUP, 0);
        } else if (i == '+') {
            keybd_event(VK_ADD, 0, 0, 0);
            keybd_event(VK_ADD, 0, KEYEVENTF_KEYUP, 0);
        } else if (i == '-') {
            keybd_event(VK_SUBTRACT, 0, 0, 0);
            keybd_event(VK_SUBTRACT, 0, KEYEVENTF_KEYUP, 0);
        } else if (i == '*') {
            keybd_event(VK_MULTIPLY, 0, 0, 0);
            keybd_event(VK_MULTIPLY, 0, KEYEVENTF_KEYUP, 0);
        } else if (i == '/') {
            keybd_event(VK_DIVIDE, 0, 0, 0);
            keybd_event(VK_DIVIDE, 0, KEYEVENTF_KEYUP, 0);
        } else if (i == '.') {
            keybd_event(VK_OEM_PERIOD, 0, 0, 0);
            keybd_event(VK_OEM_PERIOD, 0, KEYEVENTF_KEYUP, 0);
        } else {
            keybd_event((byte) i, 0, 0, 0);
            keybd_event((byte) i, 0, KEYEVENTF_KEYUP, 0);
        }
    }
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
    system("pause");
}

void initFunc(string &exp) {
    exp = exp.substr(7);
    removeSpace(exp);
    int pos = exp.find(',');
    string name = exp.substr(0, pos);
    if (CFunctions::checkName(name) >= 0) {
        cout << "ERROR: the function name: \"" << name <<
             "\" is a predefined function name" << endl;
        return;
    }
    for (int i = 0; i < varNums; ++i) {
        if (name == vars[i].name) {
            cout << "ERROR: the function name: \"" << name <<
                 "\" is defined as a variable" << endl;
            return;
        }
    }
    string expression = exp.substr(pos + 1);
    if (!methods.add(name, expression)) {
        cout << "unable to add function" << endl;
    }
}

/**
 * initialize variable, change variable value if the variable is previously defined
 * @param exp full expression
 */
void initVar(string &exp) {
    string toInit;
    removeSpace(exp);
    int pos = exp.find('=');
    toInit = exp.substr(0, pos);
    if (CFunctions::checkName(toInit) >= 0) {
        cout << "ERROR: the variable name: \"" << toInit <<
             "\" is predefined as function name" << endl;
        return;
    }
    if (!methods.check(toInit).empty()) {
        cout << "ERROR: the variable name: \"" << toInit <<
             "\" is defined as a user function" << endl;
        return;
    }
    int index = varNums;
    bool existence = false;
    for (int i = 0; i < varNums; ++i) {
        if (toInit == vars[i].name) {
            index = i;
            existence = true;
            break;
        }
    }
    exp = exp.substr(pos + 1);
    int isPureNum = 1;
    for (char c:exp) {
        if (!isNumber(c)) {
            isPureNum = 0;
            break;
        }
    }
    if (isPureNum) {
        vars[index] = {toInit, exp};
        varNums += existence ? 0 : 1;
        return;
    }
    exchangeVar(exp);
    exchangeFunction(exp);
    try {
        vars[index] = {toInit, solveFunc(-1, exp)};
        varNums += existence ? 0 : 1;
    } catch (CInputTypeErrorExc &e) {
        throw CInitVarErrorExc();
    }
}

/**
 * initVar(string &) in big number mode
 */
void initVarN(string &exp) {
    string toInit;
    removeSpace(exp);
    int pos = exp.find('=');
    toInit = exp.substr(0, pos);
    if (CFunctions::checkName(toInit) >= 0) {
        cout << "ERROR: the variable name: \"" << toInit <<
             "\" is predefined as function name" << endl;
        return;
    }
    if (!methods.check(toInit).empty()) {
        cout << "ERROR: the variable name: \"" << toInit <<
             "\" is defined as a user function" << endl;
        return;
    }
    int index = varNums;
    bool existence = false;
    for (int i = 0; i < varNums; ++i) {
        if (toInit == vars[i].name) {
            index = i;
            existence = true;
            break;
        }
    }
    exp = exp.substr(pos + 1);
    int isPureNum = 1;
    for (char c:exp) {
        if (!isNumber(c)) {
            isPureNum = 0;
            break;
        }
    }
    if (isPureNum) {
        vars[index] = {toInit, exp};
        varNums += existence ? 0 : 1;
        return;
    }
    exchangeVar(exp);
    exchangeFunction(exp);
    try {
        vars[index] = {toInit, calculateN(exp).toString()};
        varNums += existence ? 0 : 1;
    } catch (CInputTypeErrorExc &e) {
        throw CInitVarErrorExc();
    }
}

/**
 * print all variables
 */
void listVars() {
    if (varNums > 0) {
        for (int i = 0; i < varNums; ++i) {
            cout << vars[i].name << " = " << removeExtraZeros(vars[i].value) << endl;
        }
    } else {
        cout << "no variable defined" << endl;
    }
}

/**
 * change the variable name to it's value in the expression
 * @param exp full expression
 */
void exchangeVar(string &exp) {
    if (exp.empty()) {
        return;
    }
    string temp;
    int length = 0;
    for (int i = 0; i < exp.length(); ++i) {
        char c = exp.at(i);
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (!temp.empty() && c >= 0 && c <= 9)) {
            temp += c;
            length++;
        } else {
            if (!temp.empty()) {
                for (int j = 0; j < varNums; ++j) {
                    if (vars[j].name == temp) {
                        exp.replace(i - length, length, vars[j].value);
                        i = (i - length - 1 >= 0) ? i - length - 1 : 0;
                        length = 0;
                        temp = "";
                        break;
                    }
                }
                temp = "";
                length = 0;
            }
        }
    }
    if (!temp.empty()) {
        for (int i = 0; i < varNums; ++i) {
            if (vars[i].name == temp) {
                exp.replace(exp.length() - length, length, vars[i].value);
            }
        }
    }
}

void exchangeFunction(string &exp) {
    if (exp.empty()) {
        return;
    }
    string temp;
    int length = 0;
    for (int i = 0; i < exp.length(); ++i) {
        char c = exp.at(i);
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (!temp.empty() && c >= 0 && c <= 9)) {
            temp += c;
            length++;
        } else {
            if (!temp.empty() && c == '(') {
                int count = 1;
                int endLength = 0;
                for (int j = i + 1; j < exp.length(); ++j) {
                    char ch = exp.at(j);
                    if (ch == '(') {
                        count++;
                    } else if (ch == ')') {
                        count--;
                    }
                    if (count == 0) {
                        endLength = j - i + 1;
                        break;
                    }
                }
                string sub = exp.substr(i + 1, endLength - 2);
                string expression = methods.check(temp);
                if (!expression.empty()) {
                    CMethods::replace(expression, sub);
                    exp.replace(i - temp.length(),
                                endLength + temp.length(), expression);
                    i = (i - temp.length() - 1 < 1) ? i - (int) temp.length() - 1 : 0;
                    temp = "";
                    length = 0;
                }
            }
        }
    }
}

/**
 * check if the expression is calculable
 * @param exp expression with a "=" at the end
 */
bool check(string exp) {
    if (exp.empty()) {
        return false;
    }
    stack<char> stackC;
    int check = 0;
    for (char c : exp) {
        if (!(isNumber(c) || '(' == c || ')' == c || '+' == c
              || '-' == c || '*' == c || '/' == c || '=' == c)) {
            return false;
        }
        if ('(' == c) {
            stackC.push(c);
        }
        if (')' == c) {
            if (stackC.empty() || '(' != stackC.top()) {
                return false;
            }
            stackC.pop();
        }
        if ('=' == c) {
            if (check) {
                return false;
            }
            check = 1;
        }
    }
    if (!stackC.empty()) {
        return false;
    }
    if ('=' != exp.at(exp.length() - 1)) {
        return false;
    }
    return true;
}

bool isNumber(char c) {
    if (c >= '0' && c <= '9' || c == '.') {
        return true;
    }
    return false;
}

/**
 * solve expression with functions in expression, variables are not accepted
 * @param funcNum could get by "CFunction::check(string &)", should be -1 at beginning
 * @return the final answer as string
 */
string solveFunc(int funcNum, string exp) {
    if (exp.empty()) {
        return exp;
    }
    string temp;
    int length = 0;
    for (int i = 0; i < exp.length(); ++i) {
        char c = exp.at(i);
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            temp += c;
            length++;
        } else {
            if (!temp.empty() && c == '(') {
                int count = 1;
                int endLength = 0;
                int checkFunc = CFunctions::checkName(temp);
                for (int j = i + 1; j < exp.length(); ++j) {
                    char ch = exp.at(j);
                    if (ch == '(') {
                        count++;
                    } else if (ch == ')') {
                        count--;
                    }
                    if (count == 0) {
                        endLength = j - i + 1;
                        break;
                    }
                }
                string sub = exp.substr(i + 1, endLength - 2);
                exp.replace(i - functionNames[checkFunc].length(),
                            endLength + functionNames[checkFunc].length(), solveFunc(checkFunc, sub));
            }
            temp = "";
            length = 0;
        }
    }
    if (funcNum == -1) {
        return to_string(CFunctions::proceed(funcNum, calculate(exp)));
    } else {
        return "(" + to_string(CFunctions::proceed(funcNum, calculate(exp))) + ")";
    }
}

/**
 * calculate the calculable expression, no function and variable accepted
 * @return the answer
 */
double calculate(string exp) {
    removeSpace(exp);
    int isPureNum = 1;
    for (char cc:exp) {
        if (!isNumber(cc)) {
            isPureNum = 0;
        }
    }
    if (isPureNum) {
        return stod(exp);
    }
    addZero(exp);
    if (exp.length() > 1 && '=' != (exp.at(exp.length() - 1))) {
        exp += "=";
    }
    if (!check(exp)) {
        throw CInputTypeErrorExc();
    }
    string temp;
    for (char c : exp) {
        if (isNumber(c)) {
            temp += c;
        } else {
            if (!temp.empty()) {
                numStack.push(stod(temp));
                temp = "";
            }
            while (!compareOpe(c) && !symStack.empty()) {
                double b = numStack.top();
                numStack.pop();
                double a = numStack.top();
                numStack.pop();
                switch (symStack.top()) {
                    case '+':
                        numStack.push(a + b);
                        break;
                    case '-':
                        numStack.push(a - b);
                        break;
                    case '*':
                        numStack.push(a * b);
                        break;
                    case '/':
                        numStack.push(a / b);
                        break;
                    default:
                        break;
                }
                symStack.pop();
            }
            if (c != '=') {
                symStack.push(c);
                if (c == ')') {
                    symStack.pop();
                    symStack.pop();
                }
            }
        }
    }
    return numStack.top();
}

/**
 * calculate(string) in big number mode
 * @return the answer in CBigNumber
 */
CBigNumber calculateN(string exp) {
    removeSpace(exp);
    int isPureNum = 1;
    for (char cc:exp) {
        if (!isNumber(cc)) {
            isPureNum = 0;
        }
    }
    if (isPureNum) {
        return CBigNumber(exp);
    }
    addZero(exp);
    if (exp.length() > 1 && '=' != (exp.at(exp.length() - 1))) {
        exp += "=";
    }
    if (!check(exp)) {
        throw CInputTypeErrorExc();
    }
    string temp;
    for (char c : exp) {
        if (isNumber(c)) {
            temp += c;
        } else {
            if (!temp.empty()) {
                bigNumStack.push(CBigNumber(temp));
                temp = "";
            }
            while (!compareOpe(c) && !symStack.empty()) {
                CBigNumber b = bigNumStack.top();
                bigNumStack.pop();
                CBigNumber a = bigNumStack.top();
                bigNumStack.pop();
                switch (symStack.top()) {
                    case '+':
                        bigNumStack.push(a + b);
                        break;
                    case '-':
                        bigNumStack.push(a - b);
                        break;
                    case '*':
                        bigNumStack.push(a * b);
                        break;
                    case '/':
                        bigNumStack.push(a / b);
                        break;
                    default:
                        break;
                }
                symStack.pop();
            }
            if (c != '=') {
                symStack.push(c);
                if (c == ')') {
                    symStack.pop();
                    symStack.pop();
                }
            }
        }
    }
    return bigNumStack.top();
}

/**
 * replace -123... by 0-123... and ...(-123... by ...(0-123...
 */
void addZero(string &str) {
    char pre = str.at(0);
    if (pre == '-') {
        str.replace(0, 1, "0-");
    }
    for (int i = 1; i < str.length(); ++i) {
        char c = str.at(i);
        if (pre == '(' && c == '-') {
            str.replace(i, 1, "0-");
            pre = '-';
            i++;
            continue;
        }
        pre = c;
    }
}

void forStatement() {
    const string ef = "exit FOR ...\n";
    cout << "please input 3 numbers indicate "
            "the start point, the end point, and step length" << endl;
    double start, end, step;
    cin >> start >> end >> step;
    string line;
    getline(cin, line);
    if (step == 0) {
        cout << "ERROR: step length == 0" << endl << ef;
        return;
    } else if ((start < end && step < 0) || (start > end && step > 0)) {
        cout << "ERROR: end is unable to reach" << endl << ef;
        return;
    } else if (start == end) {
        cout << "ERROR: start == end" << endl << ef;
    }
    cout << "please input 2 string indicate the variable "
            "(different from the already defined) and the expression" << endl;
    cout << "use comma (,) to separate 2 strings" << endl;
    getline(cin, line);
    int pos = line.find(',');
    string var = line.substr(0, pos);
    string exp = line.substr(pos + 1, line.length() - pos - 1);
    removeSpace(var);
    removeSpace(exp);
    for (int i = 0; i < varNums; ++i) {
        if (var == vars[i].name) {
            cout << "ERROR: the variable: \"" << var << "\" is already used" << endl << ef;
            return;
        }
    }
    for (int i = 0; i < ((end - start) / step) + 1; ++i) {
        string tempExp = exp;
        try {
            string str = var + "=" + to_string(start + i * step);
            initVar(str);
            exchangeVar(tempExp);
            cout << var << " = " << start + i * step << " :  " <<
                 removeExtraZeros(solveFunc(-1, tempExp)) << endl;
        } catch (CInputTypeErrorExc &e) {
            cout << CInputTypeErrorExc::message() << endl << ef;
            clearStack();
            break;
        } catch (CInitVarErrorExc &e) {
            cout << CInitVarErrorExc::message() << endl << ef;
            clearStack();
            break;
        } catch (exception &e) {
            cout << e.what() << endl << ef;
            clearStorage();
            break;
        }
    }
    varNums--;
}

/**
 * forStatement() in big number mode
 */
void forStatementN() {
    const string ef = "exit FOR ...\n";
    cout << "please input 3 numbers indicate "
            "the start point, the end point, and step length" << endl;
    double start, end, step;
    cin >> start >> end >> step;
    string line;
    getline(cin, line);
    if (step == 0) {
        cout << "ERROR: step length == 0" << endl << ef;
        return;
    } else if ((start < end && step < 0) || (start > end && step > 0)) {
        cout << "ERROR: end is unable to reach" << endl << ef;
        return;
    } else if (start == end) {
        cout << "ERROR: start == end" << endl << ef;
    }
    cout << "please input 2 string indicate the variable "
            "(different from the already defined) and the expression" << endl;
    cout << "use comma (,) to separate 2 strings" << endl;
    getline(cin, line);
    int pos = line.find(',');
    string var = line.substr(0, pos);
    string exp = line.substr(pos + 1, line.length() - pos - 1);
    removeSpace(var);
    removeSpace(exp);
    for (int i = 0; i < varNums; ++i) {
        if (var == vars[i].name) {
            cout << "ERROR: the variable: \"" << var << "\" is already used" << endl << ef;
            return;
        }
    }
    for (int i = 0; i < ((end - start) / step) + 1; ++i) {
        string tempExp = exp;
        try {
            string str = var + "=" + to_string(start + i * step);
            initVarN(str);
            exchangeVar(tempExp);
            cout << var << " = " << start + i * step << " :  " <<
                 calculateN(tempExp).toString() << endl;
        } catch (CInputTypeErrorExc &e) {
            cout << CInputTypeErrorExc::message() << endl << ef;
            clearStack();
            break;
        } catch (CInitVarErrorExc &e) {
            cout << CInitVarErrorExc::message() << endl << ef;
            clearStack();
            break;
        } catch (exception &e) {
            cout << e.what() << endl << ef;
            clearStorage();
            break;
        }
    }
    varNums--;
    bigNumStack = stack<CBigNumber>();
    symStack = stack<char>();
}

/**
 * remove all blank space(" ") in the expression
 */
void removeSpace(string &str) {
    int index = 0;
    if (!str.empty()) {
        while ((index = str.find(' ', index)) != string::npos) {
            str.erase(index, 1);
        }
    }
}

/**
 * compare the priority of (@param c) and the one at the top of the "symStack"
 * @param c the target operator
 * @return 1 if (@param c) has higher priority, 0 for the rest
 */
int compareOpe(char c) {
    if (symStack.empty()) {
        return 1;
    }
    char top = symStack.top();
    if (top == '(') {
        return 1;
    }
    switch (c) {
        case '(':
            return 1;
        case '*':
        case '/':
            return (top == '+' || top == '-') ? 1 : 0;
        case '+':
        case '-':
        case ')':
            //end of the expression
        case '=':
            return 0;
        default:
            break;
    }
    return 1;
}

/**
 * remove extra zeros at the beginning and the end of the string
 */
string removeExtraZeros(string str) {
    int pos = str.find('.');
    if (pos != string::npos) {
        bool end = false;
        while (str.at(str.length() - 1) == '0' || str.at(str.length() - 1) == '.') {
            if (str.at(str.length() - 1) == '.') {
                end = true;
            }
            str.replace(str.length() - 1, 1, "");
            if (end) {
                break;
            }
        }
    }
    return str;
}

/**
 * clear the stacks and variables
 */
void clearStorage() {
    numStack = stack<double>();
    bigNumStack = stack<CBigNumber>();
    symStack = stack<char>();
    memset(vars, 0, sizeof(Var) * MAX);
    varNums = 0;
}

/**
 * calculate in CBigNumber
 */
void bigNumberMode() {
    cout << "you are entering the big number mode, math functions are disabled. " << endl;
    cout << "your variables won't being deleted, you should clear them manually" << endl;
    cout << "are you sure to get in [y/n]" << endl;
    string expression;
    while (true) {
        getline(cin, expression);
        if (expression == "y") {
            cout << "you are in big number mode" << endl;
            break;
        }
        if (expression == "n") {
            cout << "exit big number..." << endl;
            return;
        }
    }
    while (true) {
        clearStack();
        getline(cin, expression);
        if (expression == "Exit" || expression == "exit" ||
            expression == "EXIT") {
            cout << "EXIT BIG NUMBER..." << endl;
            return;
        }
        if (strategies.doStrategy(expression)) {
            continue;
        }
        try {
            int type = getType(expression);
            if (type == INIT_VAR) {
                initVarN(expression);
            } else if (type == CALC) {
                exchangeVar(expression);
                exchangeFunction(expression);
                cout << calculateN(expression).toString() << endl;
            } else if (type == DEFINE_FUNC) {
                initFunc(expression);
            } else if (type == DELETE_FUNC) {
                deleteFunction(expression);
            } else {
                systemCalc(expression);
            }
        } catch (CInputTypeErrorExc &e) {
            cout << CInputTypeErrorExc::message() << endl;
            clearStack();
            continue;
        } catch (CInitVarErrorExc &e) {
            cout << CInitVarErrorExc::message() << endl;
            clearStack();
            continue;
        } catch (CZeroDivideException &e) {
            cout << e.what() << endl;
            clearStack();
            continue;
        } catch (exception &e) {
            cout << "ERROR: " << e.what() << endl;
            cout << "sorry, but the storage must be cleared" << endl;
            clearStorage();
            continue;
        }
    }
}

void deleteFunction(string &exp) {
    string name = exp.substr(7);
    if (methods.check(name).empty()) {
        return;
    }
    methods.erase(name);
}

void clearFunctions() {
    methods.clear();
}

void clearStack() {
    numStack = stack<double>();
    bigNumStack = stack<CBigNumber>();
    symStack = stack<char>();
}