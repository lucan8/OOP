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
    double getElevenTotalStats()const;

    //Average OVR of the first eleven
    double getElevenOVR() const;
    uint16_t getChemestry() const;

    //Average OVR of the substitutes
    double getSubsOVR() const;

    //Moves all players to the other side of the pitch
    void changeSide();
    
    void drawPlayers(MatchPlayer :: pitch_half half, const Shader& p_shader,
                     const VertexBufferLayout& player_layout) const;
};
