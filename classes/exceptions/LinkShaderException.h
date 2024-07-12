#pragma once
#include "Myexception.h"
#include <fstream>

class LinkShaderException : public MyException{
private:
    std :: string message; 
public:
    LinkShaderException(const std :: string& func_name, const std :: string& message)
    : MyException(func_name), message(message){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + "Failed to link shader: " + message).c_str();
    }
};

