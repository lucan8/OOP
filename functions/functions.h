#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "../classes/constants/Constants.h"

template <typename key, typename val>
vector<key> getKeys(const unordered_map<key, val>& u_map);

template <typename key, typename val>
vector<val> getValues(const unordered_map<key, val>& u_map);

template <typename key, typename val>
vector<key> getKeys(const vector<pair<key, val>>& u_map);

template <typename key, typename val>
vector<val> getValues(const vector<pair<key, val>>& u_map);

pair<string, string> minStats2(const vector<pair<string, double>>& stats);

vector<string> split(const string& str, char sep = ' ');