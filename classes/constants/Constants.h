#pragma once

#include <fstream>
#include "../exceptions/FileOpenException.h"
#include <iostream>
#include <optional>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

class Constants{
private:
    static unordered_map<string, unsigned char> values;
    static unordered_map<string, vector<string>> positions;
    static unordered_map<string, vector<string>> stats;
    static unordered_map<string, unordered_map<string, unsigned char>> stats_ratios;

    static void initValues(const string& file_name);
    static void initPositions(const string& file_name);
    static void initStats(const string& file_name);
    static void initStatsRatios(const string& file_name);
public:
    static void init();
    static optional<const unsigned char> getVal(const string& key);
    static optional<const vector<string>&> getPositions(const string& key);
    static optional<const vector<string>&> getStats(const string& key);
    static optional<const unordered_map<string, unsigned char>&> getStatsRatios(const string& key);
};
