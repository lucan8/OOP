#pragma once
#include <iostream>
class Readable{
public:
    virtual void read(std :: istream&) = 0;
};

inline std :: istream& operator >>(std :: istream& in, Readable& p){
    p.read(in);
    return in;
}