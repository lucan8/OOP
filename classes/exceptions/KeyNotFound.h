#pragma once
#include <exception>
#include <string>
class KeyNotFound : public std :: exception{
private:
    std :: string key; 
public:
    KeyNotFound(std :: string key) : key(key){}
    const char* what() const  noexcept{return ("Key not found: " + key).c_str();}
};