#pragma once
#include "../../players/player/Player.h"
#include "../../coordinates/Coordinates.h"
class MatchPlayer;
typedef MatchPlayer* m_player_ptr;
typedef shared_ptr<MatchPlayer> shared_m_player;
typedef unique_ptr<MatchPlayer> unique_m_player;
typedef vector<unique_m_player> unique_m_squad;
//vector of shared_player represented as map(for easier removal)
typedef unordered_map<uint16_t, unique_m_player> unique_m_squad_map;

class MatchPlayer {
protected:
    shared_player player;
    const string position; //Can be either a normal position(subs) or a match position(first eleven)
    Coordinates coords;
    const double OVR;
    unsigned short yellow_cards = 0;
public:
    //Enum for the pitch half in which the player starts
    enum class pitch_half {first, second};
    
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

    //Make triangle from the player's coordinates depending on the half
    unique_ptr<float> getTrianglePositions(pitch_half half) const;

    bool operator <(const MatchPlayer& other) const;
    virtual void p_move() = 0;
    virtual void pass() = 0;
    virtual void block() = 0;
    virtual void tackle() = 0;    
};