#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    vector<char> vec;
   
    string str = "hello world!!";
 
    for(auto c : str) {
        vec.push_back(c);
    }

    for(auto c : str) {
        cout << c;
    }

    cout << endl;

    return 0;
}