#include "FirstTeam.h"


void FirstTeam :: changeSide(){
    for (auto& p : this->first_eleven)
        p->changeSide();
}

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

unique_ptr<float> FirstTeam :: getTrianglePositions(MatchPlayer :: pitch_half half) const {
    //Allocating 6 floats for each player
    unique_ptr<float> positions(new float[6 * this->first_eleven.size()]);
    unique_ptr<float> triangle_positions;
    //Copying the positions of each player at the end of the buffer
    for (int i = 0; i < this->first_eleven.size(); ++i){
        triangle_positions = this->first_eleven[i]->getTrianglePositions(half);
        memcpy(positions.get() + i * 6, triangle_positions.get(), 6 * sizeof(float));
    }
    return positions;
}