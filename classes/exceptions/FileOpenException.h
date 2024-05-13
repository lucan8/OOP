#pragma once
#include <exception>
#include <fstream>

class FileOpenException : public std :: exception{
private:
    std :: string file_name; 
public:
    FileOpenException(std :: string file_name) : file_name(file_name){}
    const char* what() const  noexcept{return ("Could not open " + file_name).c_str();}
};

