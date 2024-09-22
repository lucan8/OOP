#include "Human.h"
Human :: ~Human(){}

void Human :: print(std :: ostream& out) const{
    out << "Name: " << this->name << "\nAge: "
    << this->age <<"\nNationality: " << this->nationality
    << "\nWage: " << this->wage << "\n";
}

void Human :: read(std :: istream& in){
    in >> this->name >> this->age >> this->nationality >> this->wage;
}