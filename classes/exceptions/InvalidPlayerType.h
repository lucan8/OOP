#pragma once
#include <exception>
#include <string>
class InvalidPlayerType : public std :: exception{
private:
    std :: string type; 
public:
    InvalidPlayerType(std :: string type) : type(type){}
    const char* what() const  noexcept{return ("Invalid player type: " + type).c_str();}
};