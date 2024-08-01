#pragma once
#include "Myexception.h"
class MyRuntimeException : public MyException{
private:
    std :: string message;
public:
    MyRuntimeException(const std :: string& curr_file, const std :: string& curr_func,
                       int curr_line, std :: string stat_name)
     : MyException(curr_file, curr_func, curr_line), message(message){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + message).c_str();
    }
};