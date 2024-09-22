#pragma once
#include "../match_player/MatchPlayer.h"

class MatchGoalkeeper: public MatchPlayer{
public:
    MatchGoalkeeper(shared_player player = shared_player(),
                    const std :: string& position = "", double OVR = 0,const glm :: vec2& coords = glm :: vec2())
        : MatchPlayer(move(player), position, OVR, coords){}
    //void p_move() override{};
    //void pass() override{};
    void block() override{};
    //void tackle() override{};
};