#include "OutFieldPlayer.h"


unique_ptr<Player> OutFieldPlayer :: clone() const 
{
    return unique_ptr<Player>(new OutFieldPlayer(*this));
}


void OutFieldPlayer :: initStats(){
    for (const auto& stat_name : Constants :: getStats("OUTFIELD"))
        this->stats[stat_name];
}


void OutFieldPlayer :: printSeasonStats(ostream& out) const{
    Player :: printSeasonStats(out);
    out << "Goals: " << this->s_goals 
    << "\nAssists: " << this->s_assists
    << "\nBlocks: " << this->s_blocks << '\n';
}

void OutFieldPlayer :: score(){
    this->s_goals ++;
    pair<string, string> smallest_stats = minStats2(vector<pair<string, double>>{
                                                make_pair((string)"DRI", stats["DRI"]), 
                                                make_pair((string)"PAC", stats["PAC"]), 
                                                make_pair((string)"PHY", stats["PHY"])});
    upgradeStat("SHO");
    upgradeStat(smallest_stats.first);
    upgradeStat(smallest_stats.second);
}


void OutFieldPlayer :: assist(){
    this->s_assists ++;
    pair<string, string> smallest_stats = minStats2(vector<pair<string, double>>{
                                                    make_pair((string)"DRI", stats["DRI"]), 
                                                    make_pair((string)"PAC", stats["PAC"]), 
                                                    make_pair((string)"PHY", stats["PHY"])});
    upgradeStat("PAS");
    upgradeStat(smallest_stats.first);
    upgradeStat(smallest_stats.second);
}


void OutFieldPlayer :: block(){
    this->s_blocks ++;
    pair<string, string> smallest_stats = minStats2(vector<pair<string, double>>{
                                                    make_pair((string)"AGG", stats["AGG"]), 
                                                    make_pair((string)"PAC", stats["PAC"]), 
                                                    make_pair((string)"PHY", stats["PHY"])});
    upgradeStat("DEF");
    upgradeStat(smallest_stats.first);
    upgradeStat(smallest_stats.second);
}

