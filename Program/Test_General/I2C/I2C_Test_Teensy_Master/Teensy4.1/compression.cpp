#include<iostream>

using namespace std;

struct RawData{
    double val[64];
};

int main(){
    cout << "comparession";
    
    double val[64];

    for(int i=0;i<64;i++) val[i] = i;
    
    auto p=reinterpret_cast<char*>(&val);
    for(int i=0;i<256;i++) cout << p[i];

    return 0;
}

// ポインタを無理やり変換(成功)
// double d=194853.194857;
// auto p = reinterpret_cast<const char*>(&d);
// for(int i=0;i<8;i++) cout << p[i];

// ポインタの変換を配列に適用
// double val[64];
// 
// for(int i=0;i<64;i++) val[i] = i;
// 
// auto p=reinterpret_cast<char*>(&val);
// for(int i=0;i<256;i++) cout << p[i];