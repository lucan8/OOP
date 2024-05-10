#pragma once

#include <filesystem>
#include "../exceptions/FileOpenException.h"
#include "../../functions/functions.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <optional>

using namespace std;

class Constants{
private:
    static unordered_map<string, uint16_t> values;
    static unordered_map<string, unordered_map<string, uint16_t>>age_info;
    
    static unordered_map<string, vector<string>> positions, stats, age_stats;
    static unordered_map<string, vector<pair<string, uint16_t>>> stats_ratios;
    static vector<string> team_names;

    static void initValues(const string& file_name);
    static void initPositions(const string& file_name);

    static void initPlayerGen(const string& file_name);
    static void initStatsRatios(const string& file_name);

    static void initTeamNames(const string& file_name);
public:
    static void init();

    static optional<uint16_t> getVal(const string& key);
    static optional<vector<string>> getPositions(const string& key);

    static optional<unordered_map<string, uint16_t>> getAllAgeInfo(const string& age_type);
    static optional<uint16_t> getAgeInfo(const string& age_type, const string& key);

    static optional<vector<string>> getStats(const string& key);
    static optional<vector<string>> getAgeRelatedStats(const string& key);

    static vector<string> getPlayerTypes();
    static vector<string> getAgeTypes();

    static optional<vector<pair<string, uint16_t>>> getStatsRatios(const string& key);
    static const vector<string>& getTeamNames(){return team_names;}
};
