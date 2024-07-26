#pragma once
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <stdint.h>
#include "../classes/exceptions/FileOpenException.h"
#include "../classes/coordinates/Coordinates.h"
#include <glm/glm.hpp>

//Splits a string by a separator
std :: vector<std :: string> split(const std :: string& str, char sep = ' ');
//Reads file content and returns it as a string
std :: string ReadFile(const std :: string& file_path);
//Returns the positions for the square canvas(center is x, y and 2 * radius is the side of the square)
glm :: mat4x2 getCanvasPositions(Coordinates coords, float radius);

//Initializes a map with keys from a vector
template <typename key, typename val>
void initMap(std :: unordered_map<key, val>& u_map, std :: vector<key> keys){
    for (const auto& k : keys)
        u_map[k];
}

//Returns a vector of keys from an unordered map
template <typename key, typename val>
std :: vector<key> getKeys(const std :: unordered_map<key, val>& u_map){
    std :: vector<key> keys;
    keys.reserve(u_map.size());

    for (const auto& k_v : u_map)
        keys.push_back(k_v.first);
    return keys;
}

//Returns a vector of values from an unordered map
template <typename key, typename val>
std :: vector<val> getValues(const std :: unordered_map<key, val>& u_map){
    std :: vector<val> values;
    values.reserve(u_map.size());

    for (const auto& k_v : u_map)
        values.push_back(k_v.second);
    return values;
}

//Returns a vector of keys from a vector of pairs
template <typename key, typename val>
std :: vector<key> getKeys(const std :: vector<std :: pair<key, val>>& u_map){
    std :: vector<key> keys;
    keys.reserve(u_map.size());

    for (const auto& k_v : u_map)
        keys.push_back(k_v.first);
    return keys;
}

//Returns a vector of values from a vector of pairs
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
std :: vector<std :: shared_ptr<T>> toSharedVector(const std :: vector<T*>& vec){
    std :: vector<std :: shared_ptr<T>> shared_vec;
    shared_vec.reserve(vec.size());

    for (const auto ptr : vec)
        shared_vec.emplace_back(ptr);
    return shared_vec;
}

//Takes a vector of pointers and returns a vector of unique pointers
template <typename T>
std :: vector<std :: unique_ptr<T>> toUniqueVector(const std :: vector<T*>& vec){
    std :: vector<std :: unique_ptr<T>> unique_vec;
    unique_vec.reserve(vec.size());

    for (const auto ptr : vec)
        unique_vec.emplace_back(ptr);
    return unique_vec;
}

//Only works if T has a clone method
template <typename T>
std :: vector<std :: shared_ptr<T>> clonePtrVector(const std :: vector<std :: shared_ptr<T>>& vec){
    std :: vector<std :: shared_ptr<T>> cloned_vec;
    cloned_vec.reserve(vec.size());

    for (const auto& ptr : vec)
        cloned_vec.emplace_back(ptr->clone());
    return cloned_vec;
}

//Convetrs a vector to an unordered map with keys being the index of the vector
template <typename T>
std :: unordered_map <uint16_t, T> toUMap(const std :: vector<T>& vec){
    std :: unordered_map <uint16_t, T> u_map;

    for (uint16_t i = 0; i < vec.size(); ++i)
        u_map[i] = vec[i];
    return u_map;
}


//Converts matrices to 4x4 matrices(remaining elements are set to 0)
template <typename matrix_type>
glm :: mat4 toMat4(const matrix_type& matrix){
    glm :: mat4 res_mat;
    
    for (uint16_t i = 0; i < matrix.length(); ++i)
        for (uint16_t j = 0; j < matrix[i].length(); ++j)
            res_mat[i][j] = matrix[i][j];

    return res_mat;
}

