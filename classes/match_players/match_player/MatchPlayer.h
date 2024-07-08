#pragma once
#include "../../players/player/Player.h"
#include "../../coordinates/Coordinates.h"
class MatchPlayer;
typedef MatchPlayer* m_player_ptr;
typedef shared_ptr<MatchPlayer> shared_m_player;
typedef unique_ptr<MatchPlayer> unique_m_player;
typedef vector<unique_m_player> unique_m_squad;

class MatchPlayer {
protected:
    shared_player player;
    const string position; //Can be either a normal position(subs) or a match position(first eleven)
    Coordinates coordinates;
    const double OVR;
    unsigned short yellow_cards = 0;
public:
    MatchPlayer(shared_player player = shared_player(),
                const string& position = "", double OVR = 0, const Coordinates& coordinates = Coordinates())
     : player(move(player)), position(position), coordinates(coordinates), OVR(OVR){}

    virtual ~MatchPlayer(){};

    void addYellowCard();
    unsigned short getYellowCards() const{return yellow_cards;}
    string getPosition() const{return position;}
    const shared_player& getPlayer() const{return player;}
    Coordinates getCoordinates() const{return coordinates;}

    void setCoordinates(const Coordinates& coordinates){this->coordinates = coordinates;}
    double getOVR() const{return OVR;}

    bool operator <(const MatchPlayer& other) const;
    virtual void p_move() = 0;
    virtual void pass() = 0;
    virtual void block() = 0;
    virtual void tackle() = 0;    
};