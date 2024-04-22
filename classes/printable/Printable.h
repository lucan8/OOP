#pragma once
#include <iostream>
class Printable{
public:
    virtual void print(std :: ostream&) const = 0;
};

inline std :: ostream& operator <<(std :: ostream& out, const Printable& p){
    p.print(out);
    return out;
}