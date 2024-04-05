#pragma once

#include "../season/Season.h"

class League{
private:
    vector<const Season*> EndedSeasons;
    Season* ActiveSeason;
public:
    //Also generates the first season
    League(){
        EndedSeasons.reserve(Constants :: getVal("DEFAULT_TEAM_NR") / Constants :: getVal("RELEGATED_NR"));
        ActiveSeason = new Season;
    }

    ~League();
    void newSeason();
    friend ostream& operator <<(ostream& op, const League&);
};