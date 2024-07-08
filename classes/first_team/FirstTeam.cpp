#include "FirstTeam.h"

double FirstTeam :: getTotalStats()const{
    return this->getOVR() + this->getChemestry();
}
double FirstTeam :: getOVR() const{
    double team_ovr = 0;
    for (const auto& p : this->first_eleven)
        team_ovr += p->getOVR();
    
    return team_ovr / Constants :: getVal("MATCH_TEAM_SIZE");
}

//TO DO
uint16_t FirstTeam :: getChemestry() const{
    return 0;
}