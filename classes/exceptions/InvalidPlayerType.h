#pragma once
#include "Myexception.h"
#include <string>
class InvalidPlayerType : public MyException{
private:
    std :: string type; 
public:
    InvalidPlayerType(const std :: string& func_name, const std :: string& type)
    : MyException(func_name), type(type){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + ("Invalid player type: " + type)).c_str();
    }
};