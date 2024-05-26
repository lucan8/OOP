#pragma once
#include "Myexception.h"
#include <string>
class InvalidAgeType : public MyException{
private:
    std :: string type; 
public:
    InvalidAgeType(const std :: string& func_name, const std :: string& type)
    : MyException(func_name), type(type){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + ("Invalid age type: " + type)).c_str();
    }
};