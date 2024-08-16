#pragma once
#include "../../players/player/Player.h"
#include "../../renderer/Renderer.h"
class MatchPlayer;
typedef MatchPlayer* m_player_ptr;
typedef shared_ptr<MatchPlayer> shared_m_player;
typedef unique_ptr<MatchPlayer> unique_m_player;
typedef vector<unique_m_player> unique_m_squad;
typedef vector<shared_m_player> shared_m_squad;
typedef vector<vector<shared_m_player>> shared_m_matrix;
//vector of shared_player represented as map(for easier removal)
typedef unordered_map<uint16_t, unique_m_player> unique_m_squad_map;

class MatchPlayer {
public:
    //Enum for the pitch half in which the player starts
    enum class pitch_half {first, second};
    friend pitch_half operator!(pitch_half half){return half == pitch_half :: first ? pitch_half :: second : pitch_half :: first;}

    //Struct holding the number of players marking this one and a pointer to the opponent(if there is one)
    //Possible nr of intersections: 0, 1, 2(everything above 2 is considered 2)
    //For 0 and 2 intersections there should be no opponent passed
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
    
    typedef vector<PassingInfo> passing_options;
protected:
    shared_player player;
    const string position; //Can be either a normal position(subs) or a match position(first eleven)
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
    void decidePassDribble(glm :: vec2& ball_coords, MatchPlayer& opponent,
                           const passing_options& passing_options);
    
    //Should be part of an entity class
    static bool isOutsidePitch(const glm :: vec2& coords);
    vector<glm :: vec2> getMoveOptions() const;
public:
    
    MatchPlayer(shared_player player = shared_player(),
                const string& position = "", float OVR = 0, const glm :: vec2& coords = glm :: vec2())
     : player(move(player)), position(position), coords(coords), OVR(OVR){}

    virtual ~MatchPlayer(){};

    void addYellowCard();
    unsigned short getYellowCards() const{return yellow_cards;}
    string getPosition() const{return position;}
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

    //Draws the player as a circle
    void draw(pitch_half half, Shader& p_shader, const IBO& player_ibo,
              const VertexBufferLayout& player_layout, const VertexBufferLayout& player_aura_layout) const;

    //Compere two players by their OVR
    bool operator <(const MatchPlayer& other) const;
    //Two players intersect if their aura circles intersect
    bool intersects(const MatchPlayer& other) const;
    
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
    //Used to retrieve the closest team mate/opponent
    //shared_m_player getClosestPlayer(const shared_m_squad& players) const;
    virtual void block() = 0;
    virtual void tackle(glm :: vec2& ball_coords, MatchPlayer& opponent);
    void dribble(glm :: vec2& ball_coords, MatchPlayer& opponent);

    //Verifies if the player has the same detailed player type as det_p_type(DEF, MID, ATT)
    bool verifDetPType(const string& det_p_type) const;

    //Determines the number of players that intersect with this player
    //Returns the number of intersections and, if there is one opponent, a pointer to him
    OpponentIntersections getOpponentIntersections(const shared_m_squad& opponents) const;

    bool isPassedMidline(pitch_half half) const;
    bool isNearBall(const glm :: vec2& ball_coords) const;
    bool inTackleRange(const MatchPlayer& opponent) const;
    float getTackleChance(const MatchPlayer& opponent) const;

};