#pragma once
#include "Myexception.h"
#include <fstream>

class CompileShaderException : public MyException{
private:
    std :: string message; 
public:
    CompileShaderException(const std :: string& func_name, const std :: string& message)
    : MyException(func_name), message(message){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + "Failed to compile shader: " + message).c_str();
    }
};

