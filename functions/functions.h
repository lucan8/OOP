#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

template <typename key, typename val>
std :: vector<key> getKeys(const std :: unordered_map<key, val>& u_map){
    std :: vector<key> keys;
    keys.reserve(u_map.size());

    for (const auto& k_v : u_map)
        keys.push_back(k_v.first);
    return keys;
}

template <typename key, typename val>
std :: vector<val> getValues(const std :: unordered_map<key, val>& u_map){
    std :: vector<val> values;
    values.reserve(u_map.size());

    for (const auto& k_v : u_map)
        values.push_back(k_v.second);
    return values;
}

template <typename key, typename val>
std :: vector<key> getKeys(const std :: vector<std :: pair<key, val>>& u_map){
    std :: vector<key> keys;
    keys.reserve(u_map.size());

    for (const auto& k_v : u_map)
        keys.push_back(k_v.first);
    return keys;
}

template <typename key, typename val>
std :: vector<val> getValues(const std :: vector<std :: pair<key, val>>& u_map){
    std :: vector<val> values;
    values.reserve(u_map.size());

    for (const auto& k_v : u_map)
        values.push_back(k_v.second);
    return values;
}

std :: vector<std :: string> split(const std :: string& str, char sep = ' ');