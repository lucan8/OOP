#pragma once
#include "../player/Player.h"

class OutFieldPlayer : public Player{
private:
    uint16_t s_goals = 0, s_assists = 0, s_blocks = 0;
    void initStats() override;
public:
    OutFieldPlayer() : Player(){ initStats();}
    
    void score();
    void assist();
    void block();

    uint16_t getGoals() const{return s_goals;}
    uint16_t getAssists() const{return s_assists;}
    uint16_t getBlocks() const{return s_blocks;}

    void printSeasonStats(ostream& out) const override;
    void resetSeasonStats() override;
    player_ptr clone() const override;

    ~OutFieldPlayer(){cout << "Outfield destroyed!\n";}
};