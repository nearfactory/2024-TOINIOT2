#include<iostream>

using uint8_t = unsigned char;

using namespace std;

int main(){
    double val[] = {0.8, 1.6, 3.2, 6.4, 12.8, 25.6};

    // double配列の先頭アドレスをuint8_t*に変換してptrへ代入
    auto ptr = reinterpret_cast<uint8_t*>(val);
    // 後から復元するために元配列のサイズ(バイト)を保存
    auto element = sizeof(val) / sizeof(double);
    auto size = (sizeof(double) / sizeof(uint8_t)) * element;

    // ポインタを再度変換して復元する
    auto *reverse = reinterpret_cast<double*>(ptr);

    // 元の値を出力
    cout << "val:";
    for(auto x : val) cout << x << " ";
    cout << endl;

    // 元配列のサイズを出力
    cout << "size:" << size << endl;

    // 変換後の配列を文字列として表示
    cout << "str:";
    for(int i=0;i<size;i++) cout << ptr[i];
    cout << endl;

    // 元配列を復元して出力
    cout << "reverse:";
    for(int i=0;i<element;i++) cout << reverse[i] << " ";

    return 0;
}