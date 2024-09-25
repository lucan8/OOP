#pragma once
#include "../../match_players/match_outfield/MatchOutfield.h"
#include "../../match_players/match_goalkeeper/MatchGoalkeeper.h"
#include "../../shader/Shader.h"
#include "../../VB_layout/VertexBufferLayout.h"
#include "../../ibo/IBO.h"
#include "../team/Team.h"
#include <glm/glm.hpp>
class FirstTeam;
typedef std :: unique_ptr<FirstTeam> unique_first_team;
typedef std :: shared_ptr<FirstTeam> shared_first_team;
typedef FirstTeam* first_team_ptr;

class FirstTeam{
private:
    std :: string formation;
    shared_m_squad first_eleven;
    unique_m_squad subs;
    shared_team team;

    //bool in_possesion = false;
    MatchPlayer :: pitch_half side = MatchPlayer :: pitch_half :: first;

    //Returns a vector of passing options for the player passed as argument
    MatchPlayer :: passing_options getPassingOptions(const MatchPlayer& player, const FirstTeam& opp_team) const;
    //Moves players in the passing range of the player with the ball and towards ooponent's goal
    void movePlayersWithoutBall(const MatchPlayer& player_with_ball, const FirstTeam& opp_team);
public:
    FirstTeam(){}
    FirstTeam(const std :: string& formation, shared_m_squad first_eleven, unique_m_squad subs, shared_team team)
     : formation(formation), first_eleven(std :: move(first_eleven)), subs(std :: move(subs)), team(std :: move(team)){}

    const std :: string& getFormation() const{return formation;}
    const shared_m_squad& getFirstEleven() const{return first_eleven;}
    const shared_team& getTeam() const{return team;}
    MatchPlayer :: pitch_half getSide() const{return side;}
    const unique_m_squad& getSubs() const{return subs;}
    bool inPossesion() const{return getPlayerWithBall() != nullptr;}

    //Sums the OVR with chem
    float getElevenTotalStats() const;
    //Average OVR of the first eleven
    float getElevenOVR() const;
    uint16_t getChemestry() const;
    //Average OVR of the substitutes
    float getSubsOVR() const;

    //Returns the goalkeeper's coordinates
    glm :: vec2 getGKCoords() const{return first_eleven.back()->getCoords();};

    //Moves all players to the other side of the pitch
    void changeSide();
    //From vertical pitch to horizontal pitch(and vice versa)
    void changeSide1();
    
    void drawPlayers(MatchPlayer :: pitch_half half, Shader& p_shader, const IBO& player_ibo,
                     const VertexBufferLayout& player_layout,
                     const VertexBufferLayout& player_aura_layout) const;
    
    //Moves the player with the ball and the rest either towards the opponent's goal 
    //Or in the passing range of the player with the ball
    void attack(glm :: vec2& ball_coords, const FirstTeam& opp_team);
    //Moves players towards unmarked players(or stay on their mark)
    void defend(glm :: vec2& ball_coords, const FirstTeam& opp_team);
    //Moves closest player towards ball, the rest towards opponent's goal(used when no team is in possesion)
    void getPossesion(glm :: vec2 ball_coords, const glm :: vec2& opp_gk_coords);


    std :: pair<shared_m_player, shared_m_player> getClosest2Players(const glm :: vec2& target) const;
    static shared_m_player getClosestPlayer(const glm :: vec2& target, const shared_m_squad& players);

    shared_m_squad getUnmarkedPlayers(const FirstTeam& opp_team) const;
    //If the team is not in possession of the ball, the player with the ball is nullptr
    shared_m_player getPlayerWithBall() const;

    bool operator<(const FirstTeam& other) const;

    //Use the pitch_matrix from Match class
    //void setPitchMatrix(shared_m_matrix& pitch_matrix) const;
};
