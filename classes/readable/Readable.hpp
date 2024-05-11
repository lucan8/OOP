#pragma once
#include <iostream>
#include <typeinfo>
class Readable{
public:
    virtual void read(std :: istream&) = 0;
};

inline std :: istream& operator >>(std :: istream& in, Readable& p){
    cout << "Reading:" << typeid(p).name() << '\n';
    p.read(in);
    return in;
}