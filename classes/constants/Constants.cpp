#include "Constants.h"

void Constants :: init(){
    try{
        initValues("values.txt");
        initPositions("positions.txt");
        initStatsRatios("stats_ratios.txt");
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
    unsigned char const_val;

    while (fin >> const_name >> const_val)
        values[const_name] = const_val;
}

void Constants :: initPositions(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(file_name);

    string const_name;
    unsigned char nr_positions;

    while (fin.peek() != EOF){
        fin >> const_name >> nr_positions;
        positions[const_name].reserve(nr_positions);

        while (nr_positions-- >= 1)
            fin >> positions[const_name][nr_positions];
    }
    fin.ignore();
}


void Constants :: initStatsRatios(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(file_name);

    string stats_type, ratio_type, ratios, stat_name;
    unsigned char nr_stats;

    while (fin.peek() != EOF){
        fin >> stats_type >> nr_stats;
        stats[stats_type].reserve(nr_stats);

        fin >> ratio_type;
        while (nr_stats-- >= 1){
            fin >> stat_name;
            stats[stats_type].push_back(stat_name);

            getline(fin, ratios);

            stringstream ratio_stream(ratios);
            while (ratio_stream >> ratios)
                stats_ratios[ratio_type][stat_name] = (unsigned char) ratios[0];
        }
        fin.ignore();
    }

}

optional<const unsigned char> Constants ::  getVal(const string& key){
    try{
        return values.at(key);
    }
    catch(out_of_range& e){
        cerr << e.what() << '\n';
        return {};
    }
}
optional<const vector<string>&> Constants ::getPositions(const string& key){
    try{
        return positions.at(key);
    }
    catch(out_of_range& e){
        cerr << e.what() << '\n';
        return {};
    }
}

optional<const vector<string>&> Constants ::getStats(const string& key){
    try{
        return stats.at(key);
    }
    catch(out_of_range& e){
        cerr << e.what() << '\n';
        return {};
    }
}


optional<const unordered_map<string, unsigned char>&> Constants :: getStatsRatios(const string& key){
    try{
        return stats_ratios.at(key);
    }
    catch(out_of_range& e){
        cerr << e.what() << '\n';
        return {};
    }
}
