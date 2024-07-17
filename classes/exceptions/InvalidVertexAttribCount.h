#pragma once
#include "Myexception.h"
#include <stdint.h>
class InvalidVertexAttribCount : public MyException{
private:
    uint32_t count;
public:
    InvalidVertexAttribCount(const std :: string& func_name, uint32_t count)
    : MyException(func_name), count(count){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() +
                ("Invalid vertex attribute count: " + std :: to_string(count))).c_str();
    }
};