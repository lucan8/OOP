#include "Team.h"

void Team :: addPoints(const unsigned char p){
    this->points += p;
}


void Team :: clonePlayers(const vector<unique_ptr<Player>>& Players){
    this->Players.reserve(Players.size());
    for (auto& p : Players)
        this->Players.push_back(p->clone());
}

//Not implemented
uint16_t Team :: getChemestry() const{
    return Constants :: getVal("MAX_CHEM").value_or(0);
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


void Team :: trainPlayers(){
    for (auto& p : Players)
        p->train();
}


void Team :: read(istream& op){
    op >> this->name >> this->budget;
    for (auto& p : this->Players)
        op >> *p;
}

void Team :: print(ostream& op) const{
    op << "Name: " << this->name << "\nChemestry: " << this->getChemestry()
    << "\nBudget: " << this->budget
    << "\nPoints: " << this->points << "\n";
}