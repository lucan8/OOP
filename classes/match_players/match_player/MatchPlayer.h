#pragma once
#include "../../players/player/Player.h"
#include "../../coordinates/Coordinates.h"
class MatchPlayer {
protected:
    shared_ptr<Player> player;
    unsigned short yellow_cards = 0;
    Coordinates coordinates;
    string position;
public:
    MatchPlayer(shared_ptr<Player> player, const string& position, const Coordinates& coordinates)
     : player(move(player)), position(position), coordinates(coordinates){}
    void addYellowCard();
    unsigned short getYellowCards() const{return yellow_cards;}
    shared_ptr<Player> getPlayer() const{return player;}

    virtual void p_move() = 0;
    virtual void pass() = 0;
    virtual void block() = 0;
    virtual void tackle() = 0;    
};