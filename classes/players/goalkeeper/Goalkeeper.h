#pragma once
#include "../player/Player.h"

class Goalkeeper : public Player{
public:
    double calculateOVR() const;
    void train();
};
