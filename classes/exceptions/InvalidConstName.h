#pragma once
#include "Myexception.h"
#include <string>
class InvalidConstName : public MyException{
private:
    std :: string const_name; 
public:
    InvalidConstName(const std :: string& func_name, const std :: string& const_name)
    : MyException(func_name), const_name(const_name){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + ("Invalid const name: " + const_name)).c_str();
    }
};