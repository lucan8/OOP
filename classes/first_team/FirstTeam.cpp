#include "FirstTeam.h"


void FirstTeam :: changeSide(){
    for (auto& p : this->first_eleven)
        p->changeSide();
}


void FirstTeam :: changeSide1(){
    for (auto& p : this->first_eleven)
        p->changeSide1();
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

void FirstTeam :: drawPlayers(MatchPlayer :: pitch_half half, Shader& p_shader,
                              const VertexBufferLayout& player_layout)const {
    for (const auto& p : this->first_eleven)
        p->drawCircle(half, p_shader, player_layout);
}

void FirstTeam :: setAdjacencyMatrix(){
    for (uint16_t i = 0 ; i < first_eleven.size(); ++i)
            for (uint16_t j = i + 1; j < first_eleven.size(); ++j){
                double distance = first_eleven[i]->getCoords().distance(first_eleven[j]->getCoords());
                adjacency_matrix[first_eleven[i]][first_eleven[j]] = distance;
                adjacency_matrix[first_eleven[j]][first_eleven[i]] = distance;
            }
}
void FirstTeam :: movePlayers(){
    for (auto& p : this->first_eleven)
        p->p_move();
}