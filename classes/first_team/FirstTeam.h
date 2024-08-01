#pragma once
#include "../match_players/match_outfield/MatchOutfield.h"
#include "../match_players/match_goalkeeper/MatchGoalkeeper.h"
class FirstTeam;
typedef unique_ptr<FirstTeam> unique_first_team;
//player1, player2, distance
typedef unordered_map<shared_m_player, unordered_map<shared_m_player, float>> m_adjacency_matrix;

class FirstTeam{
private:
    string formation;
    shared_m_squad first_eleven;
    unique_m_squad subs;

    //Returns a vector of passing options for the player passed as argument
    vector<MatchPlayer :: PassingInfo> getPassingOptions(const shared_m_player& player,
                                                        const glm :: vec2& opp_gk_coords) const;
    
public:
    FirstTeam(const string& formation, shared_m_squad first_team, unique_m_squad subs)
              : formation(formation), first_eleven(move(first_team)), subs(move(subs)){}
                
    FirstTeam(){}
    
    const string& getFormation() const{return formation;}
    const shared_m_squad& getFirstEleven() const{return first_eleven;}

    //Returns the goalkeeper's coordinates
    glm :: vec2 getGKCoords() const{return first_eleven.back()->getCoords();};
    const unique_m_squad& getSubs() const{return subs;}

    //Sums the OVR with chem
    float getElevenTotalStats()const;

    //Average OVR of the first eleven
    float getElevenOVR() const;
    uint16_t getChemestry() const;

    //Average OVR of the substitutes
    float getSubsOVR() const;

    //Moves all players to the other side of the pitch
    void changeSide();

    //From vertical pitch to horizontal pitch(and vice versa)
    void changeSide1();
    
    void drawPlayers(MatchPlayer :: pitch_half half, Shader& p_shader, const IBO& player_ibo,
                     const VertexBufferLayout& player_layout,
                     const VertexBufferLayout& player_aura_layout) const;
    
    void movePlayerWithBall(glm :: vec2& ball_coords, shared_m_player& player, uint16_t intersections,
                            const shared_m_player& opponent, const glm :: vec2& opp_gk_coords);

    //Moves players in the passing range of the player with the ball and towards ooponent's goal
    void movePlayersWithoutBall(const shared_m_player& player_with_ball, const glm :: vec2& opp_gk_coords);
    //Moves closest player towards ball, the rest towards opponent's goal
    void movePlayersWithoutBall(glm :: vec2 ball_coords, const glm :: vec2& opp_gk_coords);

    pair<shared_m_player, shared_m_player> getClosest2Players(const glm :: vec2& target) const;
    shared_m_player getClosestPlayer(const glm :: vec2& target) const;

    //Determines the number of players that intersect with the player passed as argument
    //Returns the number of intersections and, if there is one opponent, a pointer to him
    //Argument: player from the other team contained in Match class
    pair<uint16_t, shared_m_player> getOpponentIntersections(const shared_m_player& player) const;
};
