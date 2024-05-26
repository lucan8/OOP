#pragma once
#include "Myexception.h"
#include <string>
class InvalidFormation : public MyException{
private:
    std :: string formation; 
public:
    InvalidFormation(const std :: string& func_name, const std :: string& formation)
    : MyException(func_name), formation(formation){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + ("Invalid formation: " + formation)).c_str();
    }
};