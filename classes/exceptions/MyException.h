#pragma once
#include <exception>
#include <string>
class MyException : public std :: exception{
protected:
    std :: string func_name;
public:
    MyException(const std :: string& func_name) : func_name(func_name){}
    virtual const char* what() const noexcept{return ("ERROR at " + func_name + ": ").c_str();}
};