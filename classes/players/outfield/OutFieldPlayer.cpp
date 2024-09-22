#include "OutFieldPlayer.h"
#include "../../constants/Constants.h"
#include "../../../functions/functions.h"

player_ptr OutFieldPlayer :: clone() const 
{
    return new OutFieldPlayer(*this);
}


void OutFieldPlayer :: initStats(){
    initMap(this->stats, Constants :: getStats("OUTFIELD"));
}


void OutFieldPlayer :: printSeasonStats(std :: ostream& out) const{
    Player :: printSeasonStats(out);
    out << "Goals: " << this->s_goals 
    << "\nAssists: " << this->s_assists
    << "\nBlocks: " << this->s_blocks << '\n';
}

void OutFieldPlayer :: resetSeasonStats(){
    Player :: resetSeasonStats();
    this->s_goals = this->s_assists = this->s_blocks = 0;
}


