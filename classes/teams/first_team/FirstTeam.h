#pragma once
#include "../../match_players/match_outfield/MatchOutfield.h"
#include "../../match_players/match_goalkeeper/MatchGoalkeeper.h"
#include "../team/Team.h"
#include "../../shader/Shader.h"
#include "../../font/Font.h"
#include "../../VB_layout/VertexBufferLayout.h"
#include "../../ibo/IBO.h"
#include "../../exceptions/MyRuntimeException.h"
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
    MatchPlayer :: pitch_half side = MatchPlayer :: pitch_half :: first;

    //Returns a vector of passing options for the player passed as argument
    MatchPlayer :: passing_options getPassingOptions(const MatchPlayer& player, const FirstTeam& opp_team) const;

    //Returns the starting position for the substitutes text(in pitch coordinates) 
    glm :: vec2 getSubsTextStartPos() const;

    //tackling player tries to tackle the player with the ball
    void tryTackle(glm :: vec2 ball_coords, MatchPlayer& tackling_player, MatchPlayer& player_with_ball);

    //Marking player marks the closest unmarked player(or the one he is already marking)
    void markPlayer(MatchPlayer& marking_player, const FirstTeam& opp_team);
    
    //Moves the player in the passing range of the player with the ball
    //Or towards the opponent's goal if already in the passing range
    void movePlayerWithoutBall(MatchPlayer& player, const MatchPlayer& player_with_ball, const FirstTeam& opp_team);

    //If near the ball, player tries to get possesion, otherwise he moves towards the it
    void getPossesion(glm :: vec2 ball_coords, MatchPlayer& player, const MatchPlayer& closest_ball);
public:
    //Uses the player with the index according to the match situation(returns true if a goal was scored)
    bool usePlayer(uint16_t index, glm :: vec2& ball_coords, const FirstTeam& opp_team);

    //Moves players in the passing range of the player with the ball and towards ooponent's goal
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
    MatchPlayer& getGK() const{return *first_eleven.back();};

    //Moves all players to the other side of the pitch
    void changeSide();
    //From vertical pitch to horizontal pitch(and vice versa)
    void changeSide1();
    //Draws the players on the pitch as circles
    void drawPlaying(MatchPlayer :: pitch_half half, Shader& p_shader, const IBO& player_ibo,
                     const VertexBufferLayout& player_layout,
                     const VertexBufferLayout& player_aura_layout) const;
    //Draws the players off the pitch as text of their information
    void drawUnplaying(Shader& shader, const IBO& ibo, const Font& font) const;

    static shared_m_player getClosestPlayer(const glm :: vec2& target, const shared_m_squad& players);
    shared_m_squad getUnmarkedPlayers(const FirstTeam& opp_team) const;
    //If the team is not in possession of the ball, the player with the ball is nullptr
    shared_m_player getPlayerWithBall() const;
    //Moves the players to their starting positions
    void resetPlayers();

    bool operator<(const FirstTeam& other) const;
};
