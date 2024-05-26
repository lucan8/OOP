#include "OutFieldPlayer.h"

shared_ptr<Player> OutFieldPlayer :: clone() const 
{
    return shared_ptr<Player>(new OutFieldPlayer(*this));
}


void OutFieldPlayer :: initStats(){
    initMap(this->stats, Constants :: getStats("OUTFIELD"));
}


void OutFieldPlayer :: printSeasonStats(ostream& out) const{
    Player :: printSeasonStats(out);
    out << "Goals: " << this->s_goals 
    << "\nAssists: " << this->s_assists
    << "\nBlocks: " << this->s_blocks << '\n';
}

void OutFieldPlayer :: resetSeasonStats(){
    Player :: resetSeasonStats();
    this->s_goals = this->s_assists = this->s_blocks = 0;
}
/*

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
*/

