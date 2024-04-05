#include "Team.h"

void Team :: addPoints(M_POINTS p){
    this->points += p;
}

unsigned short Team :: getChemestry() const{
    return Constants :: getVal("MAX_CHEM");
}

void Team :: playMatch(Team* other){
    M_RESULT result = M_RESULT(rand() % 3);
    switch (result)
    {
        case WIN:
            this->addPoints(W_POINTS);
            break;
        case DRAW:
            this->addPoints(D_POINTS);
            other->addPoints(D_POINTS);
            break;
        case LOSS:
            other->addPoints(W_POINTS);
            break;
        default:
            break; //Throw error
    }
}

void Team :: resetSeasonStats(){
    this->points = 0;
    for (auto& p : this->Players)
        p->resetSeasonStats();
}

void Team :: restPlayers(){
    for (auto&p : Players)
        p->rest();
}

vector<const Player*> Team :: sortedByOVR() const{   
    vector<const Player*>& aux = getPlayers();
    sort(aux.begin(), aux.end(), compareOVR);
    return aux;
}


void Team :: trainPlayers(){
    for (auto& p : Players)
        p->train();
}

Team :: ~Team(){
    for (auto& p :Players)
        delete p;

    this->Players.clear();
    Players.shrink_to_fit();
}
