#pragma once
#include "../match/Match.h"

//Make comparator for teams
class Season{
private:
    vector<shared_ptr<Team>> Teams;
    vector<vector<unique_ptr<Match>>> Stages;
    uint16_t curr_stage = 1;
    bool tranfer_window = false;

    //Backtracking time(Maybe put in in generate Season)
    void generateStages();

    vector<shared_ptr<Team>> cloneTeams() const ;
public:
    Season(); //Generates teams, first season, don't use this yet
    Season(const vector<shared_ptr<Team>>& Teams) : Teams(Teams){};
    ~Season();

    uint16_t getStage() const{return curr_stage;}

    const vector<shared_ptr<Team>>& getTeams() const{return Teams;}
    bool verifTWindowActive() const{return tranfer_window;}

    void setStage(uint16_t stage){this->curr_stage = stage;}
    void changeTranferWindow(){this->tranfer_window = !this->tranfer_window;}

    void simulateStage();
    void trainTeams();
    void restTeams();
    
    void resetSeason();
    void sortByPoints();

    friend bool comparePoints(const shared_ptr<Team>& T1, const shared_ptr<Team>& T2);
};