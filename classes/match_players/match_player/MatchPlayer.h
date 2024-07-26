#pragma once
#include "../../players/player/Player.h"
#include "../../coordinates/Coordinates.h"
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
protected:
    shared_player player;
    const string position; //Can be either a normal position(subs) or a match position(first eleven)
    Coordinates coords;
    const double OVR;
    unsigned short yellow_cards = 0;

    //Draws the player's aura circle;
    void drawAura(pitch_half half, Shader& p_shader, const VertexBufferLayout& player_layout,
                       const IBO& p_ibo, float radius) const;

    //Follows principle of getCanvasPositions
    glm :: mat4 getPlayerVertices(pitch_half half, float radius) const;

    //Decides whether to pass or dribble depending on the opponent's stats
    void decidePassDribble(const shared_m_player& opponent);
public:
    
    MatchPlayer(shared_player player = shared_player(),
                const string& position = "", double OVR = 0, const Coordinates& coords = Coordinates())
     : player(move(player)), position(position), coords(coords), OVR(OVR){}

    virtual ~MatchPlayer(){};

    void addYellowCard();
    unsigned short getYellowCards() const{return yellow_cards;}
    string getPosition() const{return position;}
    const shared_player& getPlayer() const{return player;}
    Coordinates getCoords() const{return coords;}

    void setCoordinates(const Coordinates& coordinates){this->coords = coords;}
    double getOVR() const{return OVR;}

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
    bool intersects(const MatchPlayer& other) const;
    
    //Decides what to do with the ball depending on the number of intersections with the opposing players
    //And the opponent's stats(for 0 and 2 intersections there should be no opponent passed)
    void decide(uint16_t nr_intersections, const shared_m_player& opponent = nullptr);
    void advance();
    virtual void pass(){};
    virtual void block() = 0;
    virtual void tackle() = 0; 
    void dribble(){};   
};