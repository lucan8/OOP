#pragma once
#include "Myexception.h"
#include <string>
class InvalidPosition : public MyException{
private:
    std :: string pos; 
public:
    InvalidPosition(const std :: string& curr_file, const std :: string& curr_func,
                    int curr_line, const std :: string& pos)
    : MyException(curr_file, curr_func, curr_line), pos(pos){}
    const char* what() const  noexcept{
        return (std :: string(MyException :: what()) + "Invalid player position: " + pos).c_str();
    }
};