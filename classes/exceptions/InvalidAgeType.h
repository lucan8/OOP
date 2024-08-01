#pragma once
#include "Myexception.h"
#include <string>
class InvalidAgeType : public MyException{
private:
    std :: string type; 
public:
    InvalidAgeType(const std :: string& curr_file, const std :: string& curr_func,
                   int curr_line, const std :: string& type)
    : MyException(curr_file, curr_func, curr_line), type(type){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + ("Invalid age type: " + type)).c_str();
    }
};