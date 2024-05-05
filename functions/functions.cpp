#include "functions.h"

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

std :: pair<std :: string, std :: string> minStats2
(std :: vector<std :: pair<std :: string, double>>& stats){
    std :: pair<std :: string, double> min1("", Constants :: getVal("MAX_STATS")), 
                                       min2("", Constants :: getVal("MAX_STATS"));

    for (const auto& stat : stats)
        if (stat.second < min1.second){
            min2 = min1;
            min1 = stat;
        }
        else if(stat.second < min2.second)
            min2 = stat;
    
    return make_pair(min1.first, min2.first);
}

std :: vector<std :: string> split(const std :: string& str, char sep){
    std :: istringstream str_stream(str);
    std :: string split_elem;
    std :: vector<std :: string> split_string;

    while (getline(str_stream, split_elem, sep))
        split_string.push_back(split_elem);

    return split_string;
}