#pragma once
#include "Myexception.h"
#include <string>
class InvalidPlayerType : public MyException{
private:
    std :: string type; 
public:
    InvalidPlayerType(const std :: string& curr_file, const std :: string& curr_func,
                      int curr_line, const std :: string& type)
    : MyException(curr_file, curr_func, curr_line), type(type){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + ("Invalid player type: " + type)).c_str();
    }
};