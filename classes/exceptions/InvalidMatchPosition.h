#pragma once
#include "MyException.h"
class InvalidMatchPosition : public MyException{
private:
    std :: string m_pos;
public:
    InvalidMatchPosition(const std :: string& curr_file, const std :: string& curr_func,
                         int curr_line, const std :: string& m_pos)
    : MyException(curr_file, curr_func, curr_line), m_pos(m_pos){}
    const char* what() const noexcept override{
        return (std :: string(MyException :: what()) + ": Invalid match position: " + m_pos).c_str();
    }
};
