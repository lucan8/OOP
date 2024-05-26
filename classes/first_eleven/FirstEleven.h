#pragma once
#include "../players/outfield/OutFieldPlayer.h"
#include "../players/goalkeeper/Goalkeeper.h"

class FirstEleven{
private:
    string formation;
    unordered_map<string, vector<shared_ptr<Player>>> players;
public:
    FirstEleven(const string& formation, const unordered_map<string, vector<shared_ptr<Player>>>& players)
        : formation(formation), players(players){}
    
    FirstEleven(const string& formation, const unordered_map<string, vector<shared_ptr<Player>>>&& players)
        : formation(formation), players(move(players)){}
    
    const string& getFormation() const{return formation;}
    const unordered_map<string, vector<shared_ptr<Player>>>& getPlayers() const{return players;}
    void setGoalkeeper(shared_ptr<Player> gk) {players["GK"].push_back(move(gk));}
    double getFirstElevenOVR(const unordered_map<string, vector<shared_ptr<Player>>>& first_eleven) const;
    //void print(ostream&) const override;
};
