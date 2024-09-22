#pragma once
#include "../printable/Printable.hpp"
#include "../readable/Readable.hpp"
#include <string>
#include <cstdint>
#include <iostream>

class Human : public Printable, public Readable{
protected:
    std :: string name, nationality;
    uint16_t age;
    float wage;
public:
    Human(){}

    void Age(){age ++;}
    
    uint16_t getAge() const{return age;}
    float getWage() const{return wage;}
    const std :: string& getNationality() const{return nationality;}
    const std :: string& getName() const{return name;}

    void setWage(double wage){this->wage = wage;}
    void setAge(uint16_t age){this->age = age;}
    void setNationality(const std :: string& nationality){this->nationality = nationality;}
    void setName(const std :: string& name){this->name = name;}

    virtual void read(std :: istream&) override;
    virtual void print(std :: ostream&)const override;
    virtual ~Human() = 0;
};