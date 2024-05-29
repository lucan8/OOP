#pragma once

#include "../season/Season.h"

class League{
private:
    //Maybe make this fully immutable
    vector<unique_ptr<Season>> EndedSeasons;
    unique_ptr<Season> ActiveSeason;
public:
    //Also generates the first season
    League(){
        EndedSeasons.reserve(Constants :: getVal("DEFAULT_TEAM_NR") / Constants :: getVal("RELEGATED_NR"));
        ActiveSeason = generateSeaon()
    }

    void newSeason();
};