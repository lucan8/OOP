#pragma once
#include "Myexception.h"
class InvalidPlayerStat : public MyException{
private:
    std :: string stat_name;
public:
    InvalidPlayerStat(const std :: string& func_name, std :: string stat_name)
     : MyException(func_name), stat_name(stat_name){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() +
                ("Invalid player stat name: " + stat_name)).c_str();
    }
};