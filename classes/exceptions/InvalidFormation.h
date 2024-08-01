#pragma once
#include "Myexception.h"
#include <string>
class InvalidFormation : public MyException{
private:
    std :: string formation; 
public:
    InvalidFormation(const std :: string& curr_file, const std :: string& curr_func,
                     int curr_line, const std :: string& formation)
    : MyException(curr_file, curr_func, curr_line), formation(formation){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + ("Invalid formation: " + formation)).c_str();
    }
};