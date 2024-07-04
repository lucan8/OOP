#pragma once
#include <algorithm>
#include "../constants/Constants.h"
#include "../printable/Printable.hpp"
#include "../readable/Readable.hpp"
#include <iostream>

class Human : public Printable, public Readable{
protected:
    string name, nationality;
    uint16_t age;
    double wage;
public:
    Human(){}

    void Age(){age ++;}
    
    uint16_t getAge() const{return age;}
    double getWage() const{return wage;}
    const string& getNationality() const{return nationality;}
    const string& getName() const{return name;}

    void setWage(double wage){this->wage = wage;}
    void setAge(uint16_t age){this->age = age;}
    void setNationality(const string& nationality){this->nationality = nationality;}
    void setName(const string& name){this->name = name;}

    virtual void read(istream&) override;
    virtual void print(ostream&)const override;
    virtual ~Human() = 0;
};