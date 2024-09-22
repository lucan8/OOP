#include "Player.h"
#include <numeric>
#include <iomanip>
#include "../../exceptions/InvalidPlayerStat.h"
#include "../../constants/Constants.h"
#include "../../../functions/functions.h"
using namespace std;

//TO DO: make exception for stats as well
/*
void Player :: Age(){
    if (this->age >= Constants :: getVal("MIN_YOUNG") && 
        this->age >= Constants :: getVal("MAX_YOUNG"))
        age_upgrade();
    else if(this->age >= Constants :: getVal("MIN_OLD") && 
            this->age >= Constants :: getVal("MAX_OLD"))
        age_downgrade();
    Human :: Age();
    for (const auto& stat : Constants)
        
}
*/

float Player :: getStat(const string& stat_name) const{
    try{
        return this->stats.at(stat_name);
    } catch(out_of_range& e){
        throw InvalidPlayerStat(__FILE__, __func__, __LINE__, stat_name);
    }
}


float Player :: getAttackingOVR() const{
    return (this->getStat("SHO") + this->getStat("DRI") + this->getStat("PAC") + this->getStat("PHY")) / 4;
}


void Player :: eliminateMaxes(vector<pair<string, uint16_t>>& weights) const{
    vector<pair<string, uint16_t>> :: iterator it = weights.begin();
    uint16_t max_stat = Constants :: getVal("MAX_STATS");

    while(it != weights.end()){ 
        const string& stat_name = it->first;
        //Removing the stat if it's a max stat
        if (this->getStat(stat_name) == max_stat)
            it = weights.erase(it);
        else
            ++it;
    }
}

float Player :: calculateOVR(const string& p_pos) const{
    float OVR = 0;
    
    vector<pair<string, uint16_t>>stats_ratios = Constants :: getStatsRatios(p_pos);
    const vector<uint16_t> ratios = getValues(stats_ratios);
    uint16_t weights_sum = reduce(ratios.begin(), ratios.end());

    for (const auto& stat_ratio : stats_ratios){
        OVR += this->getStat(stat_ratio.first) * stat_ratio.second;
    }

    return OVR / weights_sum;
}

float Player :: getTrainPlus() const{
    float ovr = this->calculateOVR(this->position);
    return this->potential_OVR / ovr * 
        ((this->potential_OVR - ovr - this->train_nerf) / this->remaining_sessions);
}

pair<string, string> Player :: minStats2() const{
    pair<string, float> min1("", Constants :: getVal("MAX_STATS")), 
                         min2("", Constants :: getVal("MAX_STATS"));

    for (const auto& stat : this->stats)
        if (stat.second < min1.second){
            min2 = min1;
            min1 = stat;
        }
        else if(stat.second < min2.second)
            min2 = stat;
    
    return make_pair(min1.first, min2.first);
}

void Player :: print(ostream& out) const{
    this->printBasicInfo(out);
    this->printSeasonStats(out);
    this->printStats(out);
}

void Player :: printEssentials(ostream& out) const{
    out << this->name << ' ' << this->age << ' '
        << this->position << ' ' << fixed << setprecision(2) << this->calculateOVR(this->position)
        << ' ' <<  this->potential_OVR << '\n';
}

void Player :: printSeasonStats(ostream& out) const{
    out << "Season Stats:\n"
    << "\nYellow Cards: " << this->s_yellow_cards
    <<"\nRed Cards: " << this->s_red_cards << '\n';
}

void Player :: printStats(ostream& out) const{
    out << "Player Stats:\n";
    for (const auto& stat : stats)
        out << stat.first << ": " << stat.second << '\n';
    
    out << "Potential: " << this->potential_OVR << '\n';
    out << "OVR: " << this->calculateOVR(this->position) << '\n';
}

void Player :: printBasicInfo(ostream& out) const{
    out << "Player Information:\n";
    this->::Human :: print(out);
    out << "Position: " << this->position
    << "\nShirt number: " << this->shirt_nr << "\nStamina: " << this->stamina << "\n"
    << "\nRed Carded: " << boolalpha << (this->red_carded)
    << "\nTransfer eligible: " << boolalpha << this->transfer_eligible
    << "\nForm: " << this->form << "/10\n";
}


void Player :: read(istream& in){
    this->Human :: read(in);
    in >> this->position >> this->shirt_nr >> this->potential_OVR;
    this->readStats(in);
}


void Player :: rest(){
    this->stamina = max((float)Constants::getVal("MAX_STAMINA"), 
                        this->stamina + Constants::getVal("REST_STAMINA_PLUS"));
}


void Player :: readStats(istream& in){
    string stat_name;
    float val;

    for (int i = 0; i < stats.size(); ++i){
        in >> stat_name >> val;
        this->setStat(stat_name, val);
    }
}

void Player :: resetSeasonStats(){
    s_yellow_cards = s_red_cards = form = 0;
    stamina = (float)Constants::getVal("MAX_STAMINA");
    transfer_eligible = true;
    red_carded = false;
}

void Player :: setTrainNerf(){
    this->train_nerf = (this->potential_OVR - this->calculateOVR(this->position)) / 
                        Constants :: getVal("TRAIN_NERF_DISPENSER");
}


void Player :: setStats(unordered_map<string, float> Stats){
    for (const auto& stat : Stats)
        this->setStat(stat.first, stat.second);
}

void Player :: setStat(const string& stat_name, float stat_val){
    try{
        this->stats.at(stat_name) = stat_val;
    } catch(out_of_range& e){
        throw InvalidPlayerStat(__FILE__, __func__, __LINE__, stat_name);
    }
}

void Player :: upgradeStat(const string& stat_name, float stat_plus){
    stat_plus = Constants :: generateRealNumber(0, 2 * stat_plus);
    uint16_t max_stat = Constants :: getVal("MAX_STATS");

    float upgraded_stat = min(this->getStat(stat_name) + stat_plus, (float)max_stat);
    this->setStat(stat_name, upgraded_stat);
}


void Player :: train(){
    float ovr_pl = this->getTrainPlus();

    vector<pair<string, uint16_t>>stats_ratios = Constants :: getStatsRatios(this->position);
    eliminateMaxes(stats_ratios);

    vector<uint16_t> stats_weights = getValues(stats_ratios);

    for (int i = 0; i < Constants :: getVal("NR_TRAIN_STATS") && !stats_weights.empty(); ++i){
        uint16_t stat_poz = Constants :: generateDiscreteNumber(stats_weights);
        const string& chosen_stat = stats_ratios.at(stat_poz).first;
        upgradeStat(chosen_stat, ovr_pl);

        //Removing the stat so that we don't choose it again
        stats_ratios.erase(stats_ratios.begin() + stat_poz);
        stats_weights.erase(stats_weights.begin() + stat_poz);
    }
    this->remaining_sessions --;
}
