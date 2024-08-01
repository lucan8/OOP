#include "generate_player.h"
#include "../../classes/exceptions/InvalidPlayerType.h"
using ng = dasmig :: ng;

player_ptr generatePlayer(const string& p_type, const string& age_type){
    player_ptr new_player;
    new_player = createPlayer(p_type);
    
    loadNamegenResources();

    ng :: culture r_culture = ng::instance().get_rand_culture();

    //Human data
    new_player->setNationality(ng::instance().culture_to_string(r_culture));
    new_player->setName(ng::instance().get_name(ng::gender::m, r_culture).append_surname());
    new_player->setAge(generateAge(age_type));

    //Player data
    new_player->setStats(generateStats(p_type, age_type));
    new_player->setPosition(determinePreferedPos(p_type, new_player));
    new_player->setPotential(generatePotential(age_type, new_player->calculateOVR(new_player->getPosition())));
    new_player->setTrainNerf();

    return new_player;
}

player_ptr createPlayer(const string& p_type){
    if (p_type == "GK")
        return new Goalkeeper;
    else if (p_type == "OUTFIELD")
        return new OutFieldPlayer;
    else
        throw InvalidPlayerType(__FILE__, __func__, __LINE__, p_type);
}

uint16_t generateAge(const string& age_type){
    return Constants :: generateNaturalNumber(Constants :: getAgeInfo(age_type, "MIN_AGE"), 
                                              Constants :: getAgeInfo(age_type, "MAX_AGE"));
}

float generatePotential(const string& age_type, float player_ovr){
    return Constants :: generateRealNumber(player_ovr + Constants :: getAgeInfo(age_type, "POT_MIN_PLUS"),
                                           Constants :: getVal("MAX_STATS"));
}


string determinePreferedPos(const string& p_type, const player_ptr player){
    uint16_t max_ovr = 0;
    string pref_pos;

    if (p_type == "GK")
        return p_type;


    //Looks only at the outfield positions, calculates ovr for that position, chooses best pos
    for (const auto& pos : Constants :: getPositions()){
        if (pos != "GK"){
            uint16_t ovr = player->calculateOVR(pos);
            if (ovr > max_ovr){
                max_ovr = ovr;
                pref_pos = pos; 
            }
        }
    }

    return pref_pos;
}

unordered_map<string, float> generateStats(const string& p_type, const string& age_type){
    unordered_map<string, float> stats;

    for (const auto& stat_name : Constants :: getStats(p_type))
        stats[stat_name] = Constants :: generateRealNumber(Constants :: getAgeInfo(age_type, "MIN_STAT"),
                                                           Constants :: getAgeInfo(age_type, "MAX_STAT"));
    
    return stats;
}

void loadNamegenResources(){
    filesystem :: path resources_path = 
                    filesystem :: current_path().parent_path() / "dasmig-src\\resources";
    ng :: instance().load(resources_path);
}