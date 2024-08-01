#pragma once
#include "Myexception.h"
#include <fstream>

class FileOpenException : public MyException{
private:
    std :: string file_name; 
public:
    FileOpenException(const std :: string& curr_file, const std :: string& curr_func,
                      int curr_line, const std :: string& file_name)
    : MyException(curr_file, curr_func, curr_line), file_name(file_name){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + "Could not open " + file_name).c_str();
    }
};

