#pragma once
#include <vector>
#include <unordered_map>

template <typename key, typename val>
vector<key> getKeys(const unordered_map<key, val>& u_map);

template <typename key, typename val>
vector<val> getValues(const unordered_map<key, val>& u_map);

template <typename key, typename val>
vector<key> getKeys(const vector<pair<key, val>>& u_map);

template <typename key, typename val>
vector<val> getValues(const vector<pair<key, val>>& u_map);