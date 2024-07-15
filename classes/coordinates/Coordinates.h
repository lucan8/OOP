#pragma once
#include "../readable/Readable.hpp"
//Coordinates will be used to determine player position in the pitch matrix
//Coordinatex x: 15, y: 10 will be matrix[10][15] and so on
//Also used to hold offsets from one position to another
struct Coordinates : public Readable{
    float x;
    float y;
    Coordinates(float x = 0, float y = 0): x(x), y(y){}
    //Due to the fact that the coordinates are used in a matrix, the x and y values are swapped
    void read(std :: istream& in) override {in >> x >> y;}

    operator bool()const{return x || y;}
    Coordinates operator-()const{return Coordinates(-x, -y);}
    Coordinates operator+(const Coordinates& other)const{return Coordinates(x + other.x, y + other.y);}
    Coordinates operator-(const Coordinates& other)const{return Coordinates(x - other.x, y - other.y);}
};