#pragma once
#include "../match_player/MatchPlayer.h"

class MatchOutfield : public MatchPlayer{
public:
    MatchOutfield(shared_player player = shared_player(),
                const string& position = "", double OVR = 0 ,  const glm :: vec2& coords = glm :: vec2())
     : MatchPlayer(move(player), position, OVR, coords){}
    //void p_move() override{};
    //void pass() override{};
    void block() override{};
    //void tackle();
    void sprint(){};
};