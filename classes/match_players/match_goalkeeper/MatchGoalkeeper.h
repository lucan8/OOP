#pragma once
#include "../match_player/MatchPlayer.h"

class MatchGoalkeeper: public MatchPlayer{
public:
    MatchGoalkeeper(shared_ptr<Player> player, const string& position, const Coordinates& coordinates)
        : MatchPlayer(move(player), position, coordinates){}
    void p_move() override;
    void pass() override;
    void block() override;
    void tackle() override;
};