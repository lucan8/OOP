#pragma once

#include <algorithm>
#include <filesystem>
#include "../exceptions/FileOpenException.h"
#include "../exceptions/InvalidPosition.h"
#include "../exceptions/InvalidConstName.h"
#include "../exceptions/InvalidAgeType.h"
#include "../exceptions/InvalidPlayerType.h"
#include "../exceptions/InvalidFormation.h"
#include "../../functions/functions.h"
#include "../coordinates/Coordinates.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <vector>

using namespace std;

//Adiacence matrix for player positions nodes
//Coordinates represent the vector to be added to pos1 coordinates in order to get pos2 coordinates
typedef unordered_map<string, unordered_map<string, Coordinates>> link_matrix;
typedef unordered_map<string, Coordinates> players_coords;
class Constants{
private:
    struct Formation;
    static unordered_map<string, uint16_t> values;
    static unordered_map<string, unordered_map<string, uint16_t>>age_info;
    
    static unordered_map<string, vector<string>> positions, stats, age_stats;
    static unordered_map<string, vector<pair<string, uint16_t>>> stats_ratios;
    static vector<string> team_names;
    static unordered_map<string, Formation> formations;
    
    static void initValues(const string& file_name);
    static void initPositions(const string& file_name);

    static void initPlayerGen(const string& file_name);
    static void initStatsRatios(const string& file_name);

    static void initTeamNames(const string& file_name);
    static void initFormations(const string& file_name);
public:
    static void init();

    static uint16_t getVal(const string& const_name);
    static vector<string> getPositions(const string& p_type);
    //Retrieves all positions
    static vector<string> getPositions();

    static unordered_map<string, uint16_t> getAllAgeInfo(const string& age_type);
    static uint16_t getAgeInfo(const string& age_type, const string& const_name);

    static vector<string> getStats(const string& p_type);
    static vector<string> getAgeRelatedStats(const string& p_type);

    static vector<string> getPlayerTypes();
    static vector<string> getDetailedPTypes();
    //Returns all positions which have the same detailed player type as pos
    static vector<string> getSameDetType(const string& pos);
    static vector<string> getAgeTypes();

    static vector<pair<string, uint16_t>> getStatsRatios(const string& p_pos);
    static const vector<string>& getTeamNames();

    static vector<string>getFormationsNames();
    static const link_matrix& getLinkMatrix(const string& formation_name);
    
    static const players_coords& getPlayersCoords(const string& formation_name);
    static const vector<string>& getFormationPositions(const string& formation_name);
};
