#pragma once
#include "../match_player/MatchPlayer.h"

class MatchOutfield : public MatchPlayer{
public:
    MatchOutfield(shared_player player = shared_player(),
                const string& position = "", double OVR = 0 ,  const Coordinates& coordinates = Coordinates())
     : MatchPlayer(move(player), position, OVR, coordinates){}
    void p_move() override;
    void pass() override;
    void block() override;
    void tackle() override;
    void sprint();
};