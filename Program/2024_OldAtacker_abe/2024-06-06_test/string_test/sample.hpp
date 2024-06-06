#pragma once

//#include <iostream>
//#include <cMyString>  //実装用
#include<stdlib.h>
#include<stdio.h>
// #include<

using namespace std;

class MyString{
// メンバ変数
    size_t m_capacity;  //最大容量
    size_t m_size;      //文字数
    char *m_data;       //文字列
public:
// 定数
    static const size_t npos = -1;
// メンバ関数
    // コンストラクタ
    MyString();
    MyString(const size_t n, const char);
    MyString(const char*);
    MyString(const char*, const char*);
    MyString(const MyString&);
    // デストラクタ
    ~MyString(){
        delete[] m_data;
    }
    // 演算子
    char &operator [](const size_t);
    const char& operator[](const size_t) const;
    MyString operator + (const char*);
    MyString operator + (const MyString&);
    MyString& operator += (const char*);
    MyString& operator += (const MyString&);
    void push_back(const char);
    bool operator==(const char*);
    // 各種メンバ関数
    bool empty() const;
    size_t size() const;
    size_t length() const;
    size_t capacity() const;
    const char& front() const;
    const char& back() const;
    char& front();
    char& back();
    MyString substr(const size_t, const size_t) const;
    const char* c_str() const;
    const char* data() const;
    size_t find(const char*, const size_t) const;

    void put();
};
// コンストラクタ
MyString::MyString(){   //省略
    m_capacity = m_size = 0;
    m_data = new char[m_size + 1];
    m_data[0] = '\0';
}
MyString::MyString(const size_t n, const char c){   //(n,'x')
    m_capacity = m_size = n;
    m_data = new char[m_size + 1];
    for(size_t i = 0; i < m_size; ++i)
        m_data[i] = c;
    m_data[m_size] = '\0';
}
MyString::MyString(const char *c){  //char* c
    m_capacity = m_size = strlen(c);
    m_data = new char[m_size + 1];
    strcpy(m_data, c);
}
MyString::MyString(const char *f, const char *l){   //char* f,char* l
    m_capacity = m_size = l - f;
    m_data = new char[m_size + 1];
    strncpy(m_data, f, m_size);
    m_data[m_size] = '\0';
}
MyString::MyString(const MyString &s){    //コピーコンストラクタ
    m_capacity = s.m_capacity;
    m_size = s.m_size;
    m_data = new char[m_size + 1];
    strcpy(m_data, s.m_data);
}
// 演算子
const char& MyString::operator[](const size_t n) const{
    return m_data[n];
}
char & MyString::operator [](const size_t n){
    return m_data[n];
}
MyString MyString::operator + (const char *c){
    MyString retval(*this);
    retval += c;
    return retval;
}
MyString MyString::operator + (const MyString &str){
    MyString retval(*this);
    retval += str.m_data;
    return retval;
}
MyString& MyString::operator += (const char *c){
    size_t new_size = this->m_size + strlen(c);
    if(this->m_capacity < new_size){
        while(this->m_capacity < new_size){
            this->m_capacity *= 2;
        }
        char *new_m_data = new char[this->m_capacity + 1];
        memcpy(new_m_data, this->m_data, this->m_size);
        this->m_data = new_m_data;
    }
    strcpy(this->m_data + this->m_size, c);
    this->m_size = new_size;
    return *this;
}
MyString& MyString::operator += (const MyString &str){
    size_t new_size = this->m_size + str.m_size;
    if(this->m_capacity < new_size){
        while(this->m_capacity < new_size){
            this->m_capacity *= 2;
        }
        char *new_m_data = new char[this->m_capacity + 1];
        memcpy(new_m_data, this->m_data, this->m_size);
        this->m_data = new_m_data;
    }
    strcpy(this->m_data + this->m_size, str.m_data);
    this->m_size = new_size;
    return *this;
}
void MyString::push_back(const char c){
    size_t new_size = this->m_size + 1;
    if(this->m_capacity < new_size){
        while(this->m_capacity < new_size){
            this->m_capacity *= 2;
        }
        char *new_m_data = new char[this->m_capacity + 1];
        memcpy(new_m_data, this->m_data, this->m_size);
        this->m_data = new_m_data;
    }
    this->m_data[this->m_size]= c;
    this->m_data[this->m_size + 1] = '\0';
    this->m_size = new_size;
}
bool MyString::operator == (const char *c){
    return m_size == strlen(c) && (memcmp(this->m_data, c, m_size) == 0);
}
// 各種メンバ関数
bool MyString::empty() const{
    return (m_size == 0);
}
size_t MyString::size() const{
    return m_size;
}
size_t MyString::length() const{
    return m_size;
}
size_t MyString::capacity() const{
    return m_capacity;
}
const char& MyString::front() const{
    return m_data[0];
}
const char& MyString::back() const{
    return m_data[m_size - 1];
}
char& MyString::front(){
    return m_data[0];
}
char& MyString::back(){
    return m_data[m_size - 1];
}
MyString MyString::substr(const size_t index, const size_t size) const{
    MyString retval(this->m_data + index, this->m_data + index + size);
    return retval;
}
const char* MyString::c_str() const{
    return this->m_data;
}
const char* MyString::data() const{
    return this->m_data;
}
size_t MyString::find(const char *c, const size_t index) const{
    char *pos = strstr(this->m_data + index, c);
    if(pos == NULL) return MyString::npos;
    return pos - this->m_data;
}

void MyString::put(){
    cout << m_capacity << "|" << m_size << "|" << m_data << "|\n";
}
int main(){
    // コンストラクタ
    MyString str; str.put();
    MyString a10(10, 'a'); a10.put();
    MyString labo_men1("Okabe"); labo_men1.put();
    char assist[] = "Christina"; MyString labo_men2(assist, assist + 5); labo_men2.put();
    MyString l1(labo_men1); l1.put();
    // 演算子
    //[]
    MyString code(10, ' ');
    for(int i = 0; i < 10; ++i)
        code[i] = static_cast<char>('0' + i);
    code.put();
    //+
    MyString labo_men3("Mayushi"), ttr(" TTR"); MyString l3 = labo_men3 + ttr; l3.put();
    MyString labo_men4("Daru"), sh("SuperHacker"); MyString l4 = labo_men4 + " is " + sh; l4.put();
    //+=
    MyString test("t");
    test.put();
    test += "es";
    test.put();
    test.push_back('t');
    test.put();
    if(test == "test"){
        cout << "test == \"test\"" << endl;
    }
    // 各種メンバ関数
    cout << str.empty() << endl;
    cout << labo_men4.size() << endl;
    labo_men4 += ".";
    cout << labo_men4.length() << endl;
    cout << labo_men4.capacity() << endl;
    labo_men4.put();
    cout << labo_men4.front() << " " << labo_men4.back() << endl;
    cout << labo_men4.substr(1, 3).c_str() << endl;
    cout << labo_men3.find("us", 1) << endl;
}
