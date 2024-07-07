#pragma once
#include "Myexception.h"
#include <string>
class InvalidPosition : public MyException{
private:
    std :: string pos; 
public:
    InvalidPosition(const std :: string& func_name, const std :: string& pos)
    : MyException(func_name), pos(pos){}
    const char* what() const  noexcept{
        return (std :: string(MyException :: what()) + "Invalid player position: " + pos).c_str();
    }
};