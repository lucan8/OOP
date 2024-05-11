#include "Constants.h"


unordered_map<string, uint16_t>  Constants :: values;
unordered_map<string, unordered_map<string, uint16_t>> Constants :: age_info;
unordered_map<string, vector<string>> Constants :: positions;
unordered_map<string, vector<string>> Constants :: age_stats;
unordered_map<string, vector<string>> Constants :: stats;
unordered_map<string, vector<pair<string, uint16_t>>> Constants :: stats_ratios;
vector<string> Constants :: team_names;

void Constants :: init(){
    string input_path = filesystem :: current_path().parent_path().string() + "\\classes\\constants\\";
    try{
        initValues(input_path + "values.txt");
        initPlayerGen(input_path + "player_generation.txt");
        initPositions(input_path + "positions.txt");
        initStatsRatios(input_path + "stats_ratios.txt");
        initTeamNames(input_path + "team_names.txt");
    }
    catch (FileOpenException e){
        cerr << e.what() << '\n';
    }
}
void Constants :: initValues(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(file_name);

    string const_name;
    uint16_t const_val;

    while (fin >> const_name >> const_val){
        values[const_name] = const_val;
    }
}

void Constants :: initPositions(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(file_name);

    string const_name;
    uint16_t nr_positions;

    while (fin.peek() != EOF){
        fin >> const_name >> nr_positions;
        //Adding key-value pair, and initializing a vector of empty string to avoid read exception
        positions.emplace(const_name, vector<string>(nr_positions, ""));

        while (nr_positions-- >= 1)
            fin >> positions[const_name][nr_positions];
    }
    fin.ignore();
}

void Constants :: initPlayerGen(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(file_name);

    string player_type, const_name;
    uint16_t nr_player_type, nr_consts, const_val;
    fin >> nr_player_type >> nr_consts;

    while (nr_player_type--){
        fin >> player_type;
        for (int i = 0; i < nr_consts; ++i){
            fin >> const_name >> const_val;
            age_info[player_type][const_name] = const_val;
        }
        fin.ignore();
    }
}


void Constants :: initStatsRatios(const string& file_name){
    ifstream fin(file_name);
    //Checking file opening
    if (!fin.is_open())
        throw FileOpenException(file_name);

    string player_type, aux_player_positions, stat_name, age_related;
    uint16_t nr_stats, stat_ratio;

    //Filling stats and ratios at the same time
    while (fin.peek() != EOF){
        fin >> player_type >> nr_stats;
        fin.ignore();

        //Readding all ratio_types at once for specific position_type
        getline(fin, aux_player_positions);
        vector<string> player_positions = split(aux_player_positions);

        //Iterating over the stats and setting the ratios
        while (nr_stats-- >= 1){
            fin >> stat_name >> age_related;
            stats[player_type].push_back(stat_name);

            if (age_related == "age_related")
                age_stats[player_type].push_back(stat_name);

            for (const auto& p_poz : player_positions){
                fin >> stat_ratio;
                stats_ratios[p_poz].push_back(make_pair(stat_name, stat_ratio));
            }
        }
        fin.ignore();
    }
}

void Constants :: initTeamNames(const string& file_name){
    
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException("team_names");
    
    uint16_t nr_names;
    string t_name;
    fin >> nr_names;
    fin.ignore();
    team_names.reserve(nr_names);

    while (getline(fin, t_name))
        team_names.push_back(t_name);
}

optional<uint16_t> Constants ::  getVal(const string& key){
    try{
        return values.at(key);
    }
    catch(out_of_range& e){
        cerr << "Error(getVal), key not found: " << key << '\n';
        return nullopt;
    }
}

optional<unordered_map<string, uint16_t>> Constants ::  getAllAgeInfo(const string& key){
    try{
        return age_info.at(key);
    }
    catch(out_of_range& e){
        cerr << "Error(getAllAgeInfo), key not found: " << key << '\n';
        return nullopt;
    }
}

optional<uint16_t> Constants :: getAgeInfo(const string& p_type, const string& key){
    try{
        return getAllAgeInfo(p_type).value_or(unordered_map<string, uint16_t>()).at(key);
    }
    catch(out_of_range& e){
        cerr << "Error(getAgeInfo), key not found: " << key << '\n';
        return nullopt;
    }

}

optional<vector<string>> Constants :: getPositions(const string& key){
    try{
        return positions.at(key);
    }
    catch(out_of_range& e){
        cerr << "Error(getPositions), key not found: " << key << '\n';
        return nullopt;
    }
}

optional<vector<string>> Constants :: getStats(const string& key){
    try{
        return stats.at(key);
    }
    catch(out_of_range& e){
        cerr << "Error(getStats), key not found: " << key << '\n';
        return nullopt;
    }
}

optional<vector<string>> Constants :: getAgeRelatedStats(const string& key){
    try{
        return age_stats.at(key);
    }
    catch(out_of_range& e){
        cerr << "Error(getAgeRelatedStats), key not found: " << key << '\n';
        return nullopt;
    }
}


optional<vector<pair<string, uint16_t>>> Constants :: getStatsRatios(const string& key){
    try{
        return stats_ratios.at(key);
    }
    catch(out_of_range& e){
        cerr << "Error(getStatsRatios), key not found: " << key << '\n';
        return nullopt;
    }
}

vector<string> Constants :: getPlayerTypes(){
        return getKeys(positions);
    }

vector<string> Constants :: getAgeTypes(){
        return getKeys(age_info);
    }