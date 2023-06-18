#include<iostream>
#include<vector>
#include<string>
using namespace std;

int main() {
    vector<char> data;
    string strs = "Hello World!";

    for (char& str: strs) {
        data.push_back(str);
    }

    for (char& str:data) {
        cout << str;
    }

    cout << endl;
    
    return 0;
}