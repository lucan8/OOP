#include "Player.h"


unique_ptr<Player> Player :: clone() const{
    return make_unique<Player>(*this);
}


double Player :: getOVR() const{
    double OVR = 0;
    
    const vector<pair<string, uint16_t>>& stats_ratios = Constants :: getStatsRatios(this->position);
    const vector<uint16_t> ratios = getValues(stats_ratios);
    uint16_t weights_sum = reduce(ratios.begin(), ratios.end());

    for (const auto& stat_ratio : stats_ratios){
        OVR += this->stats.at(stat_ratio.first) * stat_ratio.second;
    }

    return OVR / weights_sum;
}

void Player :: initStats(){
    for (const auto& stat_name : Constants :: getStats("OUTFIELD"))
        this->stats[stat_name];
}

void Player :: print(ostream& out) const{
    this->printBasicInfo(out);
    this->printSeasonStats(out);
    this->printStats(out);
}


void Player :: printSeasonStats(ostream& out) const{
    out << "Season Stats:\n"
    << "Goals: " << this->s_goals 
    << "\nAssists: " << this->s_assists << "\nYeallow Cards: " << this->s_yellow_cards
    <<"\nRed Cards: " << this->s_red_cards;
}

void Player :: printStats(ostream& out) const{
    out << "Player Stats:\n";
    for (const auto& stat : stats)
        out << stat.first << ": " << stat.second << '\n';
    
    out << "Potential: " << this->potential_OVR << '\n';
    out << "OVR: " << this->getOVR() << '\n';
}

void Player :: printBasicInfo(ostream& out) const{
    out << "Player Information:\n" 
    << (Human&) (*this) << "Position: " << this->position
    << "\nShirt number: " << this->shirt_nr << "\nStamina: " << this->stamina << "\n"
    << "\nRed Carded: " << boolalpha << (this->red_carded)
    << "\nTransfer eligible: " << boolalpha << this->transfer_eligible
    << "\nForm: " << this->form << "/10\n";
}


void Player :: read(istream& in){
    in >> (Human&)(*this) >> this->position >> this->shirt_nr >> this->potential_OVR;
    this->readStats(in);
}


void Player :: resetSeasonStats(){
    s_goals = s_assists = s_yellow_cards = s_red_cards = form = 0;
    stamina = (double)Constants :: getVal("MAX_STAMINA");
    transfer_eligible = true;
    red_carded = false;
}


void Player :: rest(){
    this->stamina = max((double)Constants::getVal("MAX_STAMINA"), 
                        this->stamina + Constants::getVal("REST_STAMINA_PLUS"));
}


void Player :: readStats(istream& in){
    string stat_name;
    double val;

    for (int i = 0; i < stats.size(); ++i){
        in >> stat_name >> val;
        try{
            stats.at(stat_name) = val;
        } catch(out_of_range& e){
            cout << "Error(readStats), ivalid stat_name: " << stat_name << '\n';
        }
    }
}

void Player :: train(){
    const vector<pair<string, uint16_t>>& stats_ratios = Constants :: getStatsRatios(this->position);

    vector<uint16_t> probabilities = getValues(stats_ratios);
    random_device rd;
    mt19937 gen(rd());

    discrete_distribution<> stat_distibution(probabilities.begin(), probabilities.end());
    uniform_real_distribution<double> train_plus(0.0, 1.0);

    for (int i = 0; i < Constants :: getVal("NR_TRAIN_STATS"); ++i){
        const string& chosen_stat = stats_ratios[stat_distibution(gen)].first;
        this->stats.at(chosen_stat) += train_plus(gen);
    }
}
