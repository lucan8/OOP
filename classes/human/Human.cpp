#include "Human.h"


ostream& operator <<(ostream& op, const Human& H){
    op << "Name: " << H.name << "\nAge: "
    << H.age <<"\nNationality: " << H.nationality
    << "\nWage: " << H.wage << "\n";
    
    return op;
}

istream& operator>>(istream& op, Human& H){
    op >> H.name >> H.age >> H.nationality >> H.wage;
    return op;
}