#pragma once
#include "../readable/Readable.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//Coordinates will be used to determine player position in the pitch matrix
//Coordinates x: 15, y: 10 will be matrix[10][15] and so on
//Also used to hold offsets from one position to another
struct Coordinates : public Readable{
    float x;
    float y;
    Coordinates(float x = 0, float y = 0): x(x), y(y){}
    void read(std :: istream& in) override {in >> x >> y;}

    operator bool() const {return x || y;}
    bool operator==(const Coordinates& other) const {return x == other.x && y == other.y;}

    friend float distance(const Coordinates& c1, const Coordinates& c2)
    {return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));}

    Coordinates operator+(const Coordinates& other) const {return Coordinates(x + other.x, y + other.y);}
    Coordinates operator-(const Coordinates& other) const {return Coordinates(x - other.x, y - other.y);}
    Coordinates operator-() const {return Coordinates(-x, -y);}

};
