#pragma once
#include "../teams/match_team/MatchTeam.h"
#include <random>

//Ignoring player match stamina for the sake of simplicity
class Match{
private:
    //Team, nr_goals
    unique_ptr<MatchTeam> t1, t2;

    void decidePossesion(const unordered_map<string, double>& t1_stats,
                         const unordered_map<string, double>& t2_stats){
        uint16_t t1_pos_chance = t1
    }
    bool decideBreakThrough();
    decideGoal();
public:
    Match(const shared_ptr<Team>& t1, const shared_ptr<Team>& t2) : t1(t1, 0), t2(t2, 0){
        this->nr_chances = Constants :: getVal("NR_CHANCES");
        uint32_t chances_offset = Constants :: getVal("NR_CHANCES_OFFSET");

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> chance_dist(
                                                nr_chances - chances_offset, 
                                                nr_chances + chances_offset
                                            );
        this->nr_chances = chance_dist(gen);
    }

    void play(){
        unordered_map<string, double>t1_stats = t1.first->getTeamStats(),
                                     t2_stats = t2.first->getTeamStats();
        
        decideAttack()

        //Apply rules
        for (uint16_t i = 0; i < this->nr_chances; ++i){
            
        }
    }




    
};