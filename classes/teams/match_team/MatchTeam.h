#pragma once
#include "../team/Team.h"

class MatchTeam{
private:
    unique_ptr<FirstEleven> first_team;
    uint16_t goals_scored = 0, nr_chances = 0;
    shared_ptr<Team> team;
public:
    MatchTeam(unique_ptr<FirstEleven> first_team, shared_ptr<Team> team)
        : first_team(move(first_team)), team(move(team)){}
    
    const unique_ptr<FirstEleven>& getFirstTeam(){return first_team;}
    const shared_ptr<Team>& getTeam(){return team;}
    uint16_t getGoalsScored(){return goals_scored;}
    uint16_t getNrChances(){return nr_chances;}
};