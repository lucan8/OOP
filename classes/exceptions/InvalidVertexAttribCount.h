#pragma once
#include "Myexception.h"
#include <stdint.h>
class InvalidVertexAttribCount : public MyException{
private:
    uint32_t count;
public:
    InvalidVertexAttribCount(const std :: string& curr_file, const std :: string& curr_func,
                             int curr_line, uint32_t count)
    : MyException(curr_file, curr_func, curr_line), count(count){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() +
                ("Invalid vertex attribute count: " + std :: to_string(count))).c_str();
    }
};