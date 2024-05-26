#pragma once
#include "Myexception.h"
#include <fstream>

class FileOpenException : public MyException{
private:
    std :: string file_name; 
public:
    FileOpenException(const std :: string& func_name, const std :: string& file_name)
    : MyException(func_name), file_name(file_name){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + "Could not open " + file_name).c_str();
    }
};

