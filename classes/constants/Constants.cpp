#include "Constants.h"

//Throw other exceptions when met with outside bounds
unordered_map<string, uint16_t>  Constants :: values;
unordered_map<string, unordered_map<string, uint16_t>> Constants :: age_info;
unordered_map<string, vector<string>> Constants :: positions;
unordered_map<string, vector<string>> Constants :: age_stats;
unordered_map<string, vector<string>> Constants :: stats;
unordered_map<string, vector<pair<string, uint16_t>>> Constants :: stats_ratios;
vector<string> Constants :: team_names;
unordered_map<string, unordered_map<string, uint16_t>> Constants :: formations;

void Constants :: init(){
    string input_path = filesystem :: current_path().parent_path().string() + "\\classes\\constants\\";
    try{
        initValues(input_path + "values.txt");
        initPlayerGen(input_path + "player_generation.txt");
        initPositions(input_path + "positions.txt");
        initStatsRatios(input_path + "stats_ratios.txt");
        initTeamNames(input_path + "team_names.txt");
        initFormations(input_path + "formations.txt");
    }
    catch (FileOpenException e){
        cerr << e.what() << '\n';
    }
}
void Constants :: initValues(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);

    string const_name;
    uint16_t const_val;

    while (fin >> const_name >> const_val){
        values[const_name] = const_val;
    }
}

void Constants :: initPositions(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);

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
        throw FileOpenException(__func__, file_name);

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
        throw FileOpenException(__func__, file_name);

    string player_type, aux_player_positions, stat_name, age_related, match_related;
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
            fin >> stat_name >> age_related >> match_related;
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
        throw FileOpenException(__func__, file_name);
    
    uint16_t nr_names;
    string t_name;
    fin >> nr_names;
    fin.ignore();
    team_names.reserve(nr_names);

    while (getline(fin, t_name))
        team_names.push_back(t_name);
}

void Constants :: initFormations(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);
    
    string form_layout, formation_name, pos;
    uint16_t nr_pairs, nr_players;

    while (fin >> formation_name >> nr_pairs){
        while (nr_pairs--){
            fin >> pos >> nr_players;
            formations[formation_name][pos] = nr_players;
        }
    }
}

uint16_t Constants ::  getVal(const string& const_name){
    try{
        return values.at(const_name);
    } catch(out_of_range& e){
        throw InvalidConstName(__func__, const_name);
    }
}

unordered_map<string, uint16_t> Constants ::  getAllAgeInfo(const string& age_type){
    try{
        return age_info.at(age_type);
    } catch(out_of_range& e){
        throw InvalidAgeType(__func__, age_type);
    }
}

uint16_t Constants :: getAgeInfo(const string& age_type, const string& const_name){
    try{
        return getAllAgeInfo(age_type).at(const_name);
    }
    catch(out_of_range& e){
        throw InvalidConstName(__func__, const_name);
    }
}

vector<string> Constants :: getPositions(const string& p_type){
    try{
        return positions.at(p_type);
    }catch(out_of_range& e){
        throw InvalidPlayerType(__func__, p_type);
    }
}

vector<string> Constants :: getPositions(){
    return getKeys(stats_ratios);
}

vector<string> Constants :: getStats(const string& p_type){
    try{
        return stats.at(p_type);
    }catch(out_of_range& e){
        throw InvalidPlayerType(__func__, p_type);
    }
}

vector<string> Constants :: getAgeRelatedStats(const string& p_type){
    try{
        return age_stats.at(p_type);
    }catch(out_of_range& e){
        throw InvalidPlayerType(__func__, p_type);
    }
    
}

vector<pair<string, uint16_t>> Constants :: getStatsRatios(const string& p_pos){
    try{
        return stats_ratios.at(p_pos);
    }catch(out_of_range& e){
        throw InvalidPosition(__func__, p_pos);
    }
    
}

vector<string> Constants :: getPlayerTypes(){
        return getKeys(stats);
}

vector<string> Constants :: getDetailedPTypes(){
    return getKeys(positions);
}

vector<string> Constants :: getSameDetType(const string& pos){
    for (auto& p_t : positions)
        if (find(p_t.second.begin(), p_t.second.end(), pos) != p_t.second.end())
            return p_t.second;

    throw InvalidPosition(__func__, pos);
}
vector<string> Constants :: getAgeTypes(){
    return getKeys(age_info);
}

const vector<string>& Constants :: getTeamNames(){
    return team_names;
}

const unordered_map<string, unordered_map<string, uint16_t>>& Constants :: getFormations(){
    return formations;
}

const unordered_map<string, uint16_t>& Constants :: getFormation(const string& form_name){
    try{
        return formations.at(form_name);
    }catch(out_of_range& e){
        throw InvalidFormation(__func__, form_name);
    }
}