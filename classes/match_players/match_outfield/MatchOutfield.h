#pragma once
#include "../match_player/MatchPlayer.h"

class MatchOutfield : public MatchPlayer{
public:
    MatchOutfield(shared_ptr<Player> player, const string& position, const Coordinates& coordinates)
     : MatchPlayer(move(player), position, coordinates){}
    void p_move() override;
    void pass() override;
    void block() override;
    void tackle() override;
    void sprint();
};