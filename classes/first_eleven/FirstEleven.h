#pragma once
#include "../match_players/match_outfield/MatchOutfield.h"
#include "../match_players/match_goalkeeper/MatchGoalkeeper.h"

class FirstEleven{
private:
    string formation;
    vector<shared_ptr<MatchPlayer>> first_team;
    vector<shared_ptr<MatchPlayer>> subs;
public:
    FirstEleven(const string& formation, const vector<shared_ptr<MatchPlayer>>& first_team, 
                const vector<shared_ptr<MatchPlayer>>& subs)
        : formation(formation), first_team(move(first_team)), subs(move(subs)){}
    
    const string& getFormation() const{return formation;}
    const unordered_map<string, vector<shared_ptr<Player>>>& getPlayers() const{return players;}
    void setGoalkeeper(shared_ptr<Player> gk) {players["GK"].push_back(move(gk));}
    double getFirstElevenOVR(const unordered_map<string, vector<shared_ptr<Player>>>& first_eleven) const;
    //void print(ostream&) const override;
};
