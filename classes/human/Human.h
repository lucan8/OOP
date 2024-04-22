#pragma once
#include <algorithm>
#include "../constants/Constants.h"
#include "../printable/Printable.h"
#include "../readable/Readable.h"
#include <iostream>
#include <unordered_map>
#include "../../functions/functions.h"
#include <random>
#include <numeric>

class Human : public Printable, public Readable{
protected:
    string name, nationality;
    unsigned short age;
    double wage;
public:
    Human(){}
    Human(const string& name, unsigned short age, double wage, const string& nationality) : 
        name(name), age(age), wage(wage), nationality(nationality){}

    void Age(){age ++;}
    
    unsigned short getAge() const{return age;}
    double getWage() const{return wage;}
    const string& getNationality() const{return nationality;}
    const string& getName() const{return name;}

    void setWage(double wage){this->wage = wage;}
    void setAge(unsigned short age){this->age = age;}

    virtual void read(istream&) override;
    virtual void print(ostream&)const override;
};