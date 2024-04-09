#include <iostream>
//Make printing and copying virtual for ach type of player
#include "../classes/league/League.h"

int main(){
    //Cmake
    Constants :: init();  

}

template <typename T>
ostream& operator <<(ostream& op, vector<T*> v){
    for (auto& x : v)
        op << *x << '\n';
    return op;
}
string boolToString(bool B){
    if (B)
        return "YES";
    else
        return "NO";
}


istream& operator >>(istream& op, unordered_map<string, double>& stats){
    for (auto& s : stats){
       op >> s.second;
    }
    return op;
}




