#pragma once
#include "../../players/player/Player.h"
#include "../../shader/Shader.h"
#include "../../VB_layout/VertexBufferLayout.h"
#include "../../IBO/IBO.h"
#include "../../font/Font.h"
#include <glm/glm.hpp>

//TODO: Maybe hold the vbo and vao of the player
class MatchPlayer;
typedef MatchPlayer* m_player_ptr;
typedef std :: shared_ptr<MatchPlayer> shared_m_player;
typedef std :: unique_ptr<MatchPlayer> unique_m_player;
typedef std :: vector<unique_m_player> unique_m_squad;
typedef std :: vector<shared_m_player> shared_m_squad;
//vector of shared_player represented as map(for easier removal)
typedef std :: unordered_map<uint16_t, unique_m_player> unique_m_squad_map;
//typedef vector<vector<shared_m_player>> shared_m_matrix;

class MatchPlayer {
public:
    //Enum for the pitch half in which the player starts
    enum pitch_half{first, second};
    friend pitch_half operator!(pitch_half half){return half == pitch_half :: first ? pitch_half :: second : pitch_half :: first;}
    //Struct holding the number of players marking this one and a pointer to the opponent(if there is one)
    //Possible nr of intersections: 0, 1, 2(everything above 2 is considered 2)
    //For 0 and 2 intersections there is shoudl be no opponent
    struct OpponentIntersections{
        uint16_t nr_intersections;
        shared_m_player opponent;
    };


    //Struct used for passing options(should only be used in FirstTeam and MatchPlayer)
    //Contains a teammate ovr, the chance of receiving a succesful pass 
    //And the distance from the player to the goal
    struct PassingInfo{
        shared_m_player team_mate;
        float pass_success_chance;
        float distance_from_goal;

        OpponentIntersections opp_intersections;
    };
    
    typedef std :: vector<PassingInfo> passing_options;
protected:
    //Enum for the type of radius used in the intersects method
    enum class radius_type {player, aura, ball};

    shared_player player;
    const std ::string position; //Can be either a normal position(subs) or a match position(first eleven)
    glm :: vec2 coords;
    const float OVR;
    unsigned short yellow_cards = 0;
    bool has_ball = false;
    //Draws the player's aura circle;
    void drawAura(pitch_half half, Shader& p_shader, const VertexBufferLayout& player_layout,
                       const IBO& p_ibo, float radius) const;

    //Follows principle of getCanvasPositions
    glm :: mat4 getPlayerVertices(pitch_half half, float radius) const;

    //Decides whether to pass or dribble depending on the opponent's stats and passing options
    void decidePassDribble(glm :: vec2& ball_coords, const MatchPlayer& opp_intersections,
                           const passing_options& passing_options, const glm :: vec2& opp_gk_coords);
    
    //Should be part of an entity class
    static bool isOutsidePitch(const glm :: vec2& coords);
    std :: vector<glm :: vec2> getMoveOptions() const;
    OpponentIntersections getOpponentIntersections(const shared_m_squad& opponents, radius_type r_type) const;
    //Target should be another player's coords for r_type = {player, aura}
    //And the ball's coords for r_type = ball
    bool intersects(const glm :: vec2& target, radius_type r_type) const;
    //ovr, position, surname and shirt number paired with the number of with the space ocupied by each(in pixels)
    //the space is calculated as max number of characters * max char width
    std :: array<std :: pair<std :: string, uint16_t>, 4> getUnplayingInfo(pitch_half side, const Font& font) const;
public:
    
    MatchPlayer(shared_player player = shared_player(),
                const std ::string& position = "", float OVR = 0, const glm :: vec2& coords = glm :: vec2())
     : player(move(player)), position(position), coords(coords), OVR(OVR){}

    virtual ~MatchPlayer(){};

    void addYellowCard();
    unsigned short getYellowCards() const{return yellow_cards;}
    std ::string getPosition() const{return position;}
    const shared_player& getPlayer() const{return player;}
    glm :: vec2 getCoords() const{return coords;}
    glm :: vec2 getPitchMatrixcoords()const;
    float getOVR() const{return OVR;}
    bool hasBall() const{return has_ball;}
    void setCoords(const glm :: vec2& coords){this->coords = coords;}
    void setPossesion(bool has_ball){this->has_ball = has_ball;}

    //Moves the player to the other side of the pitch
    //Should be used only at the start of any of the halves
    void changeSide();
    //From vertical pitch to horizontal pitch(and vice versa)
    void changeSide1();

    //Draws the player as a circle(used for players in the first eleven)
    void drawPlaying(pitch_half half, Shader& p_shader, const IBO& player_ibo,
                     const VertexBufferLayout& player_layout, const VertexBufferLayout& player_aura_layout,
                     const Font& font) const;
    
    //Draws the player's information as text(used for players in the subs)
    void drawUnplaying(Shader& shader, const IBO& ibo, const Font& font, glm :: vec2 s_text_pos, pitch_half side) const;
    
    //Decides what to do with the ball depending on the number of intersections with the opposing players
    //The the opponent's stats and passing options(for 0 and 2 intersections there should be no opponent passed)
    void decide(glm :: vec2& ball_coords, const OpponentIntersections& opp_intersections,
                const passing_options& passing_options, const glm :: vec2& opp_gk_coords,
                const MatchPlayer& closest_team_mate);

    //Passes the ball to the best option
    virtual void pass(glm :: vec2& ball_coords, const passing_options& passing_options);
    //Gets the chence of sending a succesful pass to a teammate
    float getPassChance(const MatchPlayer& team_mate) const;
    //Calcualtes the chance of choosing this PassingInfo option
    float getFinalPassChance(const PassingInfo& pass_info) const;
    float getPassingRange() const;

    //Chooses the best move option taking into account the pass_chance and the distance to the teammate
    void p_move(const MatchPlayer& player_with_ball, const MatchPlayer& closest_team_mate,
                const shared_m_squad& opponents);
    //Chooses the best move option taking into account the distance to the target and teammate
    void p_move(const glm :: vec2& target, const MatchPlayer& closest_team_mate);
    void moveTowards(const glm :: vec2& target, const glm :: vec2& move_values);

    virtual void block() = 0;
    virtual void tackle(glm :: vec2& ball_coords, MatchPlayer& opponent);
    void dribble(glm :: vec2& ball_coords, const MatchPlayer& opponent, const glm :: vec2& opp_gk_coords);

    void shoot(glm :: vec2& ball_coords, MatchPlayer& opp_gk);
    float getScoringChance(const MatchPlayer& opp_gk) const;
    
    bool inTackleRange(const MatchPlayer& opponent) const;
    float getTackleChance(const MatchPlayer& opponent) const;
    OpponentIntersections getMarkingIntersections(const shared_m_squad& opponents) const;
    OpponentIntersections getTacklingIntersections(const shared_m_squad& opponents) const;

    //Verifies if the player has the same detailed player type as det_p_type(DEF, MID, ATT)
    bool verifDetPType(const std ::string& det_p_type) const;
    bool isPassedMidline(pitch_half half) const;
    bool isNearBall(const glm :: vec2& ball_coords) const;
    //Compere two players by their OVR
    bool operator <(const MatchPlayer& other) const;
};