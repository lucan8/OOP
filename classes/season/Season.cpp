#include "Season.h"


bool comparePoints(const shared_ptr<Team>& T1, const shared_ptr<Team>& T2){
    return T1->getPoints() < T2->getPoints();
}

vector<shared_ptr<Team>> Season :: cloneTeams() const{
    vector<shared_ptr<Team>> cloned_teams;
    cloned_teams.reserve(this->Teams.size());

    for (const auto& t : this->Teams)
        cloned_teams.emplace_back(*t);
    
    return cloned_teams;
}

void Season :: resetSeason(){
    this->curr_stage = 1;
    for (auto& t : Teams)
        t->resetSeasonStats();
}


void Season :: restTeams(){
    for (auto& t : Teams)
        t->restPlayers();
}


void Season :: simulateStage(){
    for (auto& match : this->Stages[this->curr_stage])
        match->play();
}


void Season :: sortByPoints(){
    sort(Teams.begin(), Teams.end(), comparePoints);
}


void Season :: trainTeams(){
    for (auto& t : Teams)
        t->trainPlayers();
}


