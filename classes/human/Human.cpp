#include "Human.h"

Human :: ~Human(){}
void Human :: print(ostream& out) const{
    out << "Name: " << this->name << "\nAge: "
    << this->age <<"\nNationality: " << this->nationality
    << "\nWage: " << this->wage << "\n";
    
}

void Human :: read(istream& in){
    in >> this->name >> this->age >> this->nationality >> this->wage;
}