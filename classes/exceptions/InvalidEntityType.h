#pragma once
#include "Myexception.h"
#include <string>
class InvalidEntityType : public MyException{
private:
    std :: string entity_type; 
public:
    InvalidEntityType(const std :: string& curr_file, const std :: string& curr_func,
                      int curr_line, const std :: string& entity_type)
    : MyException(curr_file, curr_func, curr_line), entity_type(entity_type){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() + ("Invalid entity type: " + entity_type)).c_str();
    }
};