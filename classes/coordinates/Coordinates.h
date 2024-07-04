#pragma once
#include "../readable/Readable.hpp"
struct Coordinates : public Readable{
    int x;
    int y;
    Coordinates(int x, int y): x(x), y(y){}
    //Due to the fact that the coordinates are used in a matrix, the x and y values are swapped
    void read(std :: istream& in) override {in >> y >> x;}
    operator bool()const{return x || y;}
    Coordinates operator-()const{return Coordinates(-x, -y);}
    Coordinates operator+(const Coordinates& other)const{return Coordinates(x + other.x, y + other.y);}
    Coordinates operator-(const Coordinates& other)const{return Coordinates(x - other.x, y - other.y);}
};