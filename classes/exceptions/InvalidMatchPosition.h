#pragma once
#include "MyException.h"
class InvalidMatchPosition : public MyException{
private:
    std :: string m_pos;
public:
    InvalidMatchPosition(const std :: string& func_name, const std :: string& m_pos)
                        : MyException(func_name), m_pos(m_pos){}
    const char* what() const noexcept override{
        return (std :: string(MyException :: what()) + ": Invalid match position: " + m_pos).c_str();
    }
};
