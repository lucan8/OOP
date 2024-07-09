#include "FirstTeam.h"

double FirstTeam :: getElevenTotalStats()const{
    return this->getElevenOVR() + this->getChemestry();
}


double FirstTeam :: getElevenOVR() const{
    double eleven_ovr = 0;
    for (const auto& p : this->first_eleven)
        eleven_ovr += p->getOVR();
    
    return eleven_ovr / Constants :: getVal("MATCH_TEAM_SIZE");
}


double FirstTeam :: getSubsOVR()const{
     double subs_ovr = 0;
    for (const auto& p : this->subs)
        subs_ovr += p->getOVR();
    
    return subs_ovr / Constants :: getVal("NR_PLAYERS_BENCH");
}


//TO DO
uint16_t FirstTeam :: getChemestry() const{
    return 0;
}