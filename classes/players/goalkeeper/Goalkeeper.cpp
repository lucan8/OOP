#include "Goalkeeper.h"


player_ptr Goalkeeper :: clone() const{
    return new Goalkeeper(*this);
}


void Goalkeeper :: initStats(){
    initMap(this->stats, Constants :: getStats("GK"));
}

void Goalkeeper :: printSeasonStats(ostream& out) const{
    out << "Saves: " << this->nr_saves << '\n';
    Player :: printSeasonStats(out);
}


void Goalkeeper :: resetSeasonStats(){
    Player :: resetSeasonStats();
    this->nr_saves = 0;
}




