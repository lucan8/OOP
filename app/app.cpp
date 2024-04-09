#include <iostream>
#include "../classes/constants/Constants.h"
//Make printing and copying virtual for each type of player
//Write splitting function
int main(){

}

template <typename T>
ostream& operator <<(ostream& op, vector<T*> v){
    for (auto& x : v)
        op << *x << '\n';
    return op;
}

istream& operator >>(istream& op, unordered_map<string, double>& stats){
    for (auto& s : stats){
       op >> s.second;
    }
    return op;
}




