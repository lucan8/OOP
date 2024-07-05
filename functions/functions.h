#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>


template <typename key, typename val>
void initMap(std :: unordered_map<key, val>& u_map, std :: vector<key> keys){
    for (const auto& k : keys)
        u_map[k];
}

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

//Takes a vector of pointers and returns a vector of shared pointers
template <typename T>
vector<shared_ptr<T>> toSharedVector(const vector<T*>& vec){
    vector<shared_ptr<T>> shared_vec;
    shared_vec.reserve(vec.size());

    for (const auto ptr : vec)
        shared_vec.emplace_back(ptr);
    return shared_vec;
}

//Takes a vector of pointers and returns a vector of unique pointers
template <typename T>
vector<unique_ptr<T>> toUniqueVector(const vector<T*>& vec){
    vector<unique_ptr<T>> unique_vec;
    unique_vec.reserve(vec.size());

    for (const auto ptr : vec)
        unique_vec.emplace_back(ptr);
    return unique_vec;
}

//Only works if T has a clone method
template <typename T>
vector<shared_ptr<T>> clonePtrVector(const vector<shared_ptr<T>>& vec){
    vector<shared_ptr<T>> cloned_vec;
    cloned_vec.reserve(vec.size());

    for (const auto& ptr : vec)
        cloned_vec.emplace_back(ptr->clone());
    return cloned_vec;
}
std :: vector<std :: string> split(const std :: string& str, char sep = ' ');