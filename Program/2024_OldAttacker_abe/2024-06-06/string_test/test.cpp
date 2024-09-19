#include<iostream>

using namespace std;

#include"sample.hpp"

int main(){
    MyString str="あいうえお";
    str += "abc" + "123";

    cout << "str:" << str << endl;
    cout << "c_str" << str.c_str() << endl;    

    return 0;
}