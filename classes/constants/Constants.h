#pragma once

#include <filesystem>
#include "../exceptions/FileOpenException.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

class Constants{
public:
    static unordered_map<string, uint16_t> values;
    static unordered_map<string, vector<string>> positions;
    static unordered_map<string, vector<string>> stats;
    static unordered_map<string, vector<pair<string, uint16_t>>> stats_ratios;

    static void initValues(const string& file_name);
    static void initPositions(const string& file_name);
    static void initStats(const string& file_name);
    static void initStatsRatios(const string& file_name);
public:
    static void init();
    static uint16_t getVal(const string& key);
    static const vector<string>& getPositions(const string& key);
    static const vector<string>& getStats(const string& key);
    static const vector<pair<string, uint16_t>>& getStatsRatios(const string& key);
};
