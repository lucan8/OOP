#include "Team.h"

void Team :: addPoints(const unsigned char p){
    this->points += p;
}

team_ptr Team :: clone() const{
    return new Team(*this);
}


void Team :: setAuraColor(const glm :: vec4& color){
    aura_color = normalizeColor(color); 
    aura_color.a = 0.5f;
}


shared_squad_split Team :: splitTeamPos() const{
    unordered_map<string, shared_squad> split_team;
    initMap(split_team, Constants :: getPositions());

    for (const auto& p : this->Players)
        split_team[p->getPosition()].push_back(p);
    
    return split_team;
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
    op << "Name: " << this->name
    << "\nBudget: " << this->budget
    << "\nPoints: " << this->points << "\n";
}

unordered_map<string, float> Team :: getTeamStats() const{
    unordered_map<string, float> stats;
    
    for (const auto& p : this->Players)
        for (const auto& stat : p->getStats())
            stats[stat.first] += stat.second;
    
    for (auto& stat : stats)
        stat.second = (stat.second) / this->Players.size();
    
    return stats;
}