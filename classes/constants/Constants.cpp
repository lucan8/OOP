#include "Constants.h"


unordered_map<string, uint16_t>  Constants :: values;
unordered_map<string, vector<string>> Constants :: positions;
unordered_map<string, vector<string>> Constants :: stats;
unordered_map<string, vector<pair<string, uint16_t>>> Constants :: stats_ratios;

void Constants :: init(){
    string input_path = filesystem :: current_path().parent_path().string() + "\\classes\\constants\\";
    try{
        initValues(input_path + "values.txt");
        initPositions(input_path + "positions.txt");
        initStatsRatios(input_path + "stats_ratios.txt");
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


void Constants :: initStatsRatios(const string& file_name){
    ifstream fin(file_name);
    //Checking file opening
    if (!fin.is_open())
        throw FileOpenException(file_name);

    string stats_type, ratio_types, ratio_type, ratios, stat_name;
    uint16_t nr_stats;

    //Filling stats and ratios at the same time
    while (fin.peek() != EOF){
        fin >> stats_type >> nr_stats;
        fin.ignore();

        //Readding all ratio_types at once for specific position_type
        getline(fin, ratio_types);

        //Adding key-value pair, and initializing a vector of empty string to avoid read exception
        stats.emplace(stats_type, vector<string>(nr_stats, ""));

        //Iterating over the stats and setting the ratios
        while (nr_stats-- >= 1){
            fin >> stats[stats_type][nr_stats];
            stat_name = stats[stats_type][nr_stats];
            
            getline(fin, ratios);

            //rtype gets rewritten every iteration with the same thing
            stringstream ratio_stream(ratios), rtype_stream(ratio_types);
            while (ratio_stream >> ratios && rtype_stream >> ratio_type)
                stats_ratios[ratio_type].push_back({stat_name, stoi(ratios)});
        }
        fin.ignore();
    }
}

uint16_t Constants ::  getVal(const string& key){
    try{
        return Constants :: values.at(key);
    }
    catch(out_of_range& e){
        cerr << "Error(getVal), key not found: " << key << '\n';
        return -1;
    }
}
const vector<string>& Constants :: getPositions(const string& key){
    try{
        return positions.at(key);
    }
    catch(out_of_range& e){
        cerr << "Error(getPositions), key not found: " << key << '\n';
        return {};
    }
}

const vector<string>& Constants :: getStats(const string& key){
    try{
        return stats.at(key);
    }
    catch(out_of_range& e){
        cerr << "Error(getStats), key not found: " << key << '\n';
        return {};
    }
}


const vector<pair<string, uint16_t>>& Constants :: getStatsRatios(const string& key){
    try{
        return stats_ratios.at(key);
    }
    catch(out_of_range& e){
        cerr << "Error(getStatsRations), key not found: " << key << '\n';
        return {};
    }
}
