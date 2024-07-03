#pragma once
#include "../readable/Readable.hpp"
struct Coordinates : public Readable{
    int x;
    int y;
    Coordinates(int x, int y): x(x), y(y){}
    void read(istream& in) override {in >> x >> y;}
    operator bool()const{return x || y;}
};