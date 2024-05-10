#include "generate_player.h"
using ng = dasmig :: ng;

optional<unique_ptr<Player>> generatePlayer(const string& p_type, const string& age_type){
    unique_ptr<Player> new_player;
    try{
        new_player = createPlayer(p_type);
    } catch(InvalidPlayerType& e){
        cerr << "Error(generatePlayer): " << e.what() << '\n';
        return nullopt;
    }

    loadNamegenResources();

    ng :: culture r_culture = ng::instance().get_rand_culture();

    new_player->setNationality(ng::instance().culture_to_string(r_culture));
    new_player->setName(ng::instance().get_name(ng::gender::m, r_culture).append_surname());


    new_player->setAge(generateAge(
                                    Constants :: getAgeInfo(age_type, "MIN_AGE").value_or(0), 
                                    Constants :: getAgeInfo(age_type, "MAX_AGE").value_or(0)
                                  ));
    new_player->setStats(generateStats(
                                    Constants :: getStats(p_type).value_or(vector<string>()),
                                    (double)Constants :: getAgeInfo(age_type, "MIN_STAT").value_or(0),
                                    (double)Constants :: getAgeInfo(age_type, "MAX_STAT").value_or(0)
                                    ));
    new_player->setPosition(determinePreferedPos(p_type, new_player));

    new_player->setPotential(generatePotential(
                                    new_player->getOVR(new_player->getPosition()) + 
                                    (double)Constants :: getAgeInfo(age_type, "POT_MIN_PLUS").value_or(0),
                                    (double)Constants :: getVal("MAX_STATS").value_or(0)
                                    ));
    new_player->setTrainNerf();

    return new_player;
}

unique_ptr<Player> createPlayer(const string& p_type){
    if (p_type == "GK")
        return unique_ptr<Player>(new Goalkeeper);
    else if (p_type == "OUTFIELD")
        return unique_ptr<Player>(new OutFieldPlayer);
    else
        throw InvalidPlayerType(p_type);
}

string determinePreferedPos(const string&  p_type, const unique_ptr<Player>& player){
    uint16_t max_ovr = 0;
    string pref_pos;

    for (const auto& pos : Constants :: getPositions(p_type).value_or(vector<string>())){
        uint16_t ovr = player->getOVR(pos);
        if (ovr > max_ovr){
            max_ovr = ovr;
            pref_pos = pos; 
        }
    }

    return pref_pos;
}

uint16_t generateAge(uint16_t min_age, uint16_t max_age){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> age_dist(min_age, max_age);
                                        
    return age_dist(gen);
}

double generatePotential(double min_pot, double max_pot){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> pot_dist(min_pot, max_pot);

    return pot_dist(gen);
}

unordered_map<string, double> generateStats(const vector<string>& stats_names, 
                                            double min_stat, double max_stat){
    unordered_map<string, double> stats;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> stat_dist(min_stat, max_stat);

    for (const auto& stat_name : stats_names)
        stats[stat_name] = stat_dist(gen);
    
    return stats;
}

void loadNamegenResources(){
    filesystem :: path resources_path = 
                    filesystem :: current_path().parent_path() / "dasmig-src\\resources";
    ng :: instance().load(resources_path);
}