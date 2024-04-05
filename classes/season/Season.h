#pragma once
#include "../team/Team.h"

class Season{
private:
    vector<Team*> Teams;
    unsigned short stage = 1;
    bool tranfer_window = false;
public:
    Season(); //Generates teams, first season, don't use this yet
    Season(vector<const Team*> :: const_iterator start, vector<const Team*> :: const_iterator end);
    ~Season();

    unsigned short getStage() const{return stage;}

    vector<const Team*> getTeams() const{return vector<const Team*>(Teams.cbegin(), Teams.cend());}
    bool verifTWindowActive() const{return tranfer_window;}

    void setStage(unsigned short stage){this->stage = stage;}
    void changeTranferWindow(){this->tranfer_window = !this->tranfer_window;}

    void simulateStage();
    void trainTeams();
    void restTeams();
    
    void resetSeason();
    void sortByPoints(){sort(Teams.begin(), Teams.end(), comparePoints);}

    friend ostream& operator <<(ostream& op, const Season&);
    friend bool comparePoints(const Team* T1, const Team* T2);
};