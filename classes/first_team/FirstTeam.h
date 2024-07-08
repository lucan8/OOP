#pragma once
#include "../match_players/match_outfield/MatchOutfield.h"
#include "../match_players/match_goalkeeper/MatchGoalkeeper.h"

class FirstTeam;
typedef unique_ptr<FirstTeam> unique_first_team;
class FirstTeam{
private:
    string formation;
    unique_m_squad first_eleven;
    unique_m_squad subs;
public:
    FirstTeam(const string& formation, unique_m_squad first_team, unique_m_squad subs)
              : formation(formation), first_eleven(move(first_team)), subs(move(subs)){}
                
    FirstTeam(){}
    
    const string& getFormation() const{return formation;}
    const unique_m_squad& getFirstEleven() const{return first_eleven;}
    const unique_m_squad& getSubs() const{return subs;}

    //Sums the OVR with chem
    double getTotalStats()const;

    //Average OVR of the first eleven
    double getOVR() const;
    uint16_t getChemestry() const;
    //void print(ostream&) const override;
};
