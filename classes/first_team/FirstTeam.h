#pragma once
#include "../match_players/match_outfield/MatchOutfield.h"
#include "../match_players/match_goalkeeper/MatchGoalkeeper.h"
#include "../textures/Textures.h"
class FirstTeam;
typedef unique_ptr<FirstTeam> unique_first_team;
typedef FirstTeam* first_team_ptr;
//player -> opponent intersections
typedef unordered_map<shared_m_player, MatchPlayer :: OpponentIntersections> intersections_map;

class FirstTeam{
private:
    string formation;
    shared_m_squad first_eleven;
    unique_m_squad subs;
    glm :: vec4 aura_color;
    

    //Returns a vector of passing options for the player passed as argument
    MatchPlayer :: passing_options getPassingOptions(const shared_m_player& player, const glm :: vec2& opp_gk_coords,
                                      const intersections_map& opp_intersections) const;
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
    float getElevenTotalStats() const;

    //Average OVR of the first eleven
    float getElevenOVR() const;
    uint16_t getChemestry() const;

    //Average OVR of the substitutes
    float getSubsOVR() const;
    bool hasBall(glm :: vec2 ball_coords) const;

    //Moves all players to the other side of the pitch
    void changeSide();

    //From vertical pitch to horizontal pitch(and vice versa)
    void changeSide1();
    
    void drawPlayers(MatchPlayer :: pitch_half half, Shader& p_shader, const IBO& player_ibo,
                     const VertexBufferLayout& player_layout,
                     const VertexBufferLayout& player_aura_layout) const;
    
    void movePlayerWithBall(glm :: vec2& ball_coords, shared_m_player& player,
                            const intersections_map& opp_intersections, const glm :: vec2& opp_gk_coords);

    //Moves players in the passing range of the player with the ball and towards ooponent's goal
    void movePlayersWithoutBall(const shared_m_player& player_with_ball, const glm :: vec2& opp_gk_coords,
                                const intersections_map& opp_intersections);
    //Moves closest player towards ball, the rest towards opponent's goal
    void movePlayersWithoutBall(glm :: vec2 ball_coords, const glm :: vec2& opp_gk_coords);

    

    pair<shared_m_player, shared_m_player> getClosest2Players(const glm :: vec2& target) const;
    shared_m_player getClosestPlayer(const glm :: vec2& target) const;

    //Determines the number of players that intersect with the player passed as argument
    //Returns the number of intersections and, if there is one opponent, a pointer to him
    //Argument: player from the other team contained in Match class
    MatchPlayer :: OpponentIntersections getOpponentIntersections(const shared_m_player& player) const;

    //Sets the aura color for the players, the color is normalized, alpha is 0.5
    void setAuraColor(const glm :: vec4& color);
    const glm :: vec4& getAuraColor() const{return aura_color;}

    //Use the pitch_matrix from Match class
    void setPitchMatrix(shared_m_matrix& pitch_matrix) const;
    //Returns the number of intersections for all the players in the first eleven
    intersections_map getOpponentIntersections(const unique_first_team& t2);
};
