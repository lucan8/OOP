#include "Team.h"

void Team :: addPoints(const unsigned char p){
    this->points += p;
}

unsigned short Team :: getChemestry() const{
    return Constants :: getVal("MAX_CHEM").value();
}

void Team :: playMatch(Team* other){
 
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

bool comparePoints(const Team* T1, const Team* T2){
    return T1->getPoints() < T2->getPoints();
}

void Team :: sortByOVR(){
    sort(Players.begin(), Players.end(), compareOVR);
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

bool compareOVR(const Player* P1, const Player* P2){
    return P1->getOVR() > P2->getOVR();
}

istream& operator >>(istream& op, Team& T){
    op >> T.name >> T.budget;
    for (auto& p : T.Players)
        op >> *p;
    return op;
}

ostream& operator <<(ostream& op, const Team& T){
    op << "Name: " << T.name << "\nChemestry: " << T.getChemestry()
    << "\nBudget: " << T.budget
    << "\nPoints: " << T.points << "\n";
    
    return op;
}