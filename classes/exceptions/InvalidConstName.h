#pragma once
#include "Myexception.h"
#include <string>
class InvalidConstName : public MyException{
private:
    std :: string const_name; 
public:
    InvalidConstName(const std :: string& curr_file, const std :: string& curr_func,
                     int curr_line, const std :: string& const_name)
    : MyException(curr_file, curr_func, curr_line), const_name(const_name){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + ("Invalid const name: " + const_name)).c_str();
    }
};