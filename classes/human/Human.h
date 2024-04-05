#pragma once
#include <algorithm>
#include "../constants/Constants.h"
#include <iostream>
#include <unordered_map>

class Human{
protected:
    string name, nationality;
    unsigned short age;
    double wage;
public:
    Human(const string& name, unsigned short age, double wage, const string& nationality) : 
        name(name), age(age), wage(wage), nationality(nationality){}

    void Age(){age ++;}
    
    unsigned short getAge() const{return age;}
    double getWage() const{return wage;}
    const string& getNationality() const{return nationality;}
    const string& getName() const{return name;}

    void setWage(double wage){this->wage = wage;}
    void setAge(unsigned short age){this->age = age;}

    friend ostream& operator <<(ostream&, const Human&);
    friend istream& operator >>(istream& op, Human& H);
};