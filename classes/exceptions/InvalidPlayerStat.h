#pragma once
#include "Myexception.h"
class InvalidPlayerStat : public MyException{
private:
    std :: string stat_name;
public:
    InvalidPlayerStat(const std :: string& curr_file, const std :: string& curr_func,
                      int curr_line, std :: string stat_name)
    : MyException(curr_file, curr_func, curr_line), stat_name(stat_name){}
    const char* what() const  noexcept{
        return ((std :: string)MyException :: what() +
                ("Invalid player stat name: " + stat_name)).c_str();
    }
};