#include "Goalkeeper.h"


unique_ptr<Player> Goalkeeper :: clone() const{
    return unique_ptr<Player>(new Goalkeeper(*this));
}


void Goalkeeper :: initStats(){
    for (const auto& stat_name : Constants :: getStats("GK").value_or(vector<string>()))
        this->stats[stat_name];
}

void Goalkeeper :: printSeasonStats(ostream& out) const{
    out << "Saves: " << this->nr_saves << '\n';
    Player :: printSeasonStats(out);
}


void Goalkeeper :: resetSeasonStats(){
    Player :: resetSeasonStats();
    this->nr_saves = 0;
}




