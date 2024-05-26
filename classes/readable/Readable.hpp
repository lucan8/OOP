#pragma once
#include <iostream>
#include <typeinfo>
class Readable{
public:
    virtual void read(std :: istream&) = 0;
};

inline std :: istream& operator >>(std :: istream& in, Readable& p){
    p.read(in);
    return in;
}