#pragma once
#include "../player/Player.h"

class Goalkeeper : public Player{
private:
    uint16_t nr_saves = 0;
    void initStats() override;
public:
    Goalkeeper() : Player(){initStats();}
    player_ptr clone() const override;
    void resetSeasonStats() override;
    void printSeasonStats(std :: ostream&) const override;
    ~Goalkeeper(){cout << "GK destroyed!\n";}
};
