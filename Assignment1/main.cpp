#include <iostream>

using namespace std;

int main() {
    long long a, b;
    cout << "Please input two integers" << endl;
    cin >> a >> b;
    if (cin.good()==1){
        cout << a*b;
    } else {
        cout << "input type error";
    }
}
