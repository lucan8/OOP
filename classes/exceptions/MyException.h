#pragma once
#include <exception>
#include <string>
class MyException : public std :: exception{
protected:
    std :: string curr_file, curr_func;
    int curr_line;
public:
    MyException(const std :: string& curr_file, const std :: string& curr_func,
                int curr_line)
     : curr_file(curr_file), curr_func(curr_func), curr_line(curr_line){}
    virtual const char* what() const noexcept{
        return ("ERROR in file: " + curr_file + ", function: " + curr_func + ", line: " + std :: to_string(curr_line) + "\n").c_str();
    }
};