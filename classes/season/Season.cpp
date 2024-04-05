#include "Season.h"


Season :: Season(vector<const Team*> :: const_iterator start, vector<const Team*> :: const_iterator end){
    Teams.reserve(end - start);
    
    //Creates new pointers to the same teams
    while (start <= end){
        Teams.push_back(new Team(**start));
        start++;
    }
}


void Season :: resetSeason(){
    this->stage = 1;
    for (auto& t : Teams)
        t->resetSeasonStats();
}


void Season :: restTeams(){
    for (auto& t : Teams)
        t->restPlayers();
}


void Season :: simulateStage(){
    for (int i = 0; i < Teams.size() / 2; i++)
        Teams[i]->playMatch(Teams[Teams.size() - i - 1]);
}


void Season :: trainTeams(){
    for (auto& t : Teams)
        t->trainPlayers();
}


Season :: ~Season(){
    for (auto& t : Teams)
        delete t;
    
    Teams.clear();
    Teams.shrink_to_fit();
}