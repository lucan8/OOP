#pragma once

#include <filesystem>
#include "../exceptions/FileOpenException.h"
#include "../../functions/functions.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

class Constants{
public:
    static unordered_map<string, uint16_t> values;
    static unordered_map<string, unordered_map<string, uint16_t>>p_gen;
    
    static unordered_map<string, vector<string>> positions, stats, age_stats;
    static unordered_map<string, vector<pair<string, uint16_t>>> stats_ratios;

    static void initValues(const string& file_name);
    static void initPositions(const string& file_name);

    static void initPlayerGen(const string& file_name);
    static void initStatsRatios(const string& file_name);
public:
    static void init();
    static uint16_t getVal(const string& key);

    static const unordered_map<string, uint16_t>& getPlayerGen(const string& key);
    static const vector<string>& getPositions(const string& key);

    static const vector<string>& getStats(const string& key);
    static const vector<string>& getAgeRelatedStats(const string& key);

    static const vector<pair<string, uint16_t>>& getStatsRatios(const string& key);
};
