#pragma once
#include "Myexception.h"
#include <string>
class InvalidEntityType : public MyException{
private:
    std :: string entity_type; 
public:
    InvalidEntityType(const std :: string& func_name, const std :: string& entity_type)
    : MyException(func_name), entity_type(entity_type){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + ("Invalid entity type: " + entity_type)).c_str();
    }
};