#include "FirstTeam.h"

double FirstTeam :: calculateTotalStats()const{
    return this->calculateOVR() + this->calculateChemestry();
}
double FirstTeam :: calculateOVR() const{
    double team_ovr = 0;
    for (const auto& p : this->first_eleven)
        team_ovr += p->getPlayer()->calculateOVR(p->getPosition());
    
    return team_ovr / Constants :: getVal("MATCH_TEAM_SIZE");
}

//TO DO
uint16_t FirstTeam :: calculateChemestry() const{

}