#pragma once
#include "../match_players/match_outfield/MatchOutfield.h"
#include "../match_players/match_goalkeeper/MatchGoalkeeper.h"
class FirstTeam;
typedef unique_ptr<FirstTeam> unique_first_team;
//player1, player2, distance
typedef unordered_map<shared_m_player, unordered_map<shared_m_player, double>> m_adjacency_matrix;

class FirstTeam{
private:
    string formation;
    shared_m_squad first_eleven;
    m_adjacency_matrix adjacency_matrix;
    unique_m_squad subs;

    //Helper function for initPlayersPositions from Match
    //Sets the players' positions in the players_positions map
    //void initPlayersPositions(unordered_map<float, unordered_map<float, bool>>& players_positions, bool team) const;
public:
    FirstTeam(const string& formation, shared_m_squad first_team, unique_m_squad subs)
              : formation(formation), first_eleven(move(first_team)), subs(move(subs)){setAdjacencyMatrix();}
                
    FirstTeam(){}
    
    const string& getFormation() const{return formation;}
    const shared_m_squad& getFirstEleven() const{return first_eleven;}
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

    //From vertical pitch to horizontal pitch(and vice versa)
    void changeSide1();
    
    void drawPlayers(MatchPlayer :: pitch_half half, Shader& p_shader, const IBO& player_ibo,
                     const VertexBufferLayout& player_layout,
                     const VertexBufferLayout& player_aura_layout) const;
    
    //Takes every pair of players and calculates the distance between them
    void setAdjacencyMatrix();
    
    //Determines the number of players that intersect with the player passed as argument
    //Returns the number of intersections and, if there is one opponent, a pointer to him
    //Argument: player from the other team contained in Match class
    pair<uint16_t, shared_m_player> getOpponentIntersections(const shared_m_player& player) const;
};
