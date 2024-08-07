#pragma once
#include "../../players/player/Player.h"
#include "../../renderer/Renderer.h"
class MatchPlayer;
typedef MatchPlayer* m_player_ptr;
typedef shared_ptr<MatchPlayer> shared_m_player;
typedef unique_ptr<MatchPlayer> unique_m_player;
typedef vector<unique_m_player> unique_m_squad;
typedef vector<shared_m_player> shared_m_squad;
//vector of shared_player represented as map(for easier removal)
typedef unordered_map<uint16_t, unique_m_player> unique_m_squad_map;

class MatchPlayer {
public:
    //Enum for the pitch half in which the player starts
    enum class pitch_half {first, second};

    //Struct used for passing options(should only be used in FirstTeam and MatchPlayer)
    //Contains a teammate ovr, the chance of receiving a succesful pass 
    //And the distance from the player to the goal
    struct PassingInfo{
        shared_m_player team_mate;
        float pass_success_chance;
        float distance_from_goal;
    };
protected:
    shared_player player;
    const string position; //Can be either a normal position(subs) or a match position(first eleven)
    glm :: vec2 coords;
    const float OVR;
    unsigned short yellow_cards = 0;

    //Draws the player's aura circle;
    void drawAura(pitch_half half, Shader& p_shader, const VertexBufferLayout& player_layout,
                       const IBO& p_ibo, float radius) const;

    //Follows principle of getCanvasPositions
    glm :: mat4 getPlayerVertices(pitch_half half, float radius) const;

    //Decides whether to pass or dribble depending on the opponent's stats and passing options
    void decidePassDribble(glm :: vec2& ball_coords, const shared_m_player& opponent,
                           const vector<PassingInfo>& passing_options);
    
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
    float getOVR() const{return OVR;}

    void setCoords(const glm :: vec2& coords){this->coords = coords;}

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
    void decide(glm :: vec2& ball_coords, uint16_t nr_intersections, const vector<PassingInfo>& passing_options,
                const shared_m_player& opponent, const glm :: vec2& opp_gk_coords, const shared_m_player& closest_team_mate);

    //Passes the ball to the best option
    virtual void pass(glm :: vec2& ball_coords, const vector<PassingInfo>& passing_options);
    //Gets the chence of sending a succesful pass to a player at a certain distance
    float getPassChance(float pass_distance) const ;
    //Calcualtes the chance of choosing this PassingInfo option
    float getFinalPassChance(const PassingInfo& pass_info) const;
    float getPassingRange() const;

    //Moves the player towards the target and away from the closest team mate
    void p_move(const glm :: vec2& target, const shared_m_player& closest_team_mate);
    virtual void block() = 0;
    virtual void tackle() = 0; 
    void dribble();   
};