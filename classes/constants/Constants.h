#pragma once

#include <algorithm>
#include <filesystem>
#include "../exceptions/FileOpenException.h"
#include "../exceptions/InvalidPosition.h"
#include "../exceptions/InvalidConstName.h"
#include "../exceptions/InvalidAgeType.h"
#include "../exceptions/InvalidPlayerType.h"
#include "../exceptions/InvalidFormation.h"
#include "../exceptions/InvalidMatchPosition.h"
#include "../../functions/functions.h"
#include "../coordinates/Coordinates.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <vector>

using namespace std;

//We have two types of players: OUTFIELD AND GOALKEEPER(GK)
//For OUTFIELD there are also detailed types: DEFENDER, MIDFIELDER, ATTACKER
//Each type holds a set of "normal" positions(FULLBACK, CM, WINGER...)
//Each "normal" positions holds a set of match positions(LB/RB, LCM/RCM, LW/RW...)

//Adiacence matrix for player positions nodes
//Coordinates represent the vector to be added to pos1 coordinates in order to get pos2 coordinates
typedef unordered_map<string, unordered_map<string, Coordinates>> link_matrix;
//Match position, coordinates
typedef unordered_map<string, Coordinates> mpos_coords;
class Constants{
private:
    //Holds a vector of match positions and the link matrix
    struct Formation;
    
    //Const name, value(ex: "MATCH_TEAM_SIZE"->11)
    static unordered_map<string, uint16_t> values;
    //Age type(ex: "YOUNG"), const_name(ex: "MIN_AGE"), value(ex: 18
    static unordered_map<string, unordered_map<string, uint16_t>>age_info;
    
    //Detailed player type, vector of normal positions
    static unordered_map<string, vector<string>> positions;
    //Player type, vector of stats
    static unordered_map<string, vector<string>> stats, age_stats;
    //Player type, vector of pairs(stat_name, weight)
    static unordered_map<string, vector<pair<string, uint16_t>>> stats_ratios;
    static vector<string> team_names;
    static unordered_map<string, Formation> formations;
    static mpos_coords p_coords;

    //const_name, array of vertex_positions
    static unordered_map<string, unique_ptr<float>> vertex_positions;

    //match_pos, normal_pos
    //ex: "LCB"->"CB", "LCM"->"CM", "LB"->"FULLBACK"
    static unordered_map<string, string> pos_equivalence;

    //Detailed position, nr_players
    static unordered_map<string, uint16_t> subs_layout;
    
    static void initValues(const string& file_name);
    static void initPositions(const string& file_name);

    static void initPlayerGen(const string& file_name);
    static void initStatsRatios(const string& file_name);

    static void initTeamNames(const string& file_name);
    static void initFormations(const string& file_name);

    static void initMPosCoords(const string& file_name);

    static void initPositionEquivalence(const string& file_name);
    static void initSubsLayout(const string& file_name);

    static void initVertexPositions(const string& file_name);
public:
    static void init();

    static uint16_t getVal(const string& const_name);
    static vector<string> getPositions(const string& p_type);
    //Retrieves all positions
    static vector<string> getPositions();
    //static unordered_map<string, vector<string>> getPositions1(){return positions;};

    static unordered_map<string, uint16_t> getAllAgeInfo(const string& age_type);
    static uint16_t getAgeInfo(const string& age_type, const string& const_name);

    //The player stats depending on the player type
    //ex: "GK"->{"DIV", "HAN", ...}
    static vector<string> getStats(const string& p_type);
    static vector<string> getAgeRelatedStats(const string& p_type);
    
    //GK, OUTFILED
    static vector<string> getPlayerTypes();
    //GK, DEFENDER, MIDFIELDER, ATTACKER
    static vector<string> getDetailedPTypes();

    //Returns all positions which have the same detailed player type as pos
    //ex: pos = "cm"->{"cm", "cdm", "cam", "sidemid"}
    static vector<string> getSameDetType(const string& pos);
    static vector<string> getAgeTypes();

    static vector<pair<string, uint16_t>> getStatsRatios(const string& p_pos);
    static const vector<string>& getTeamNames();

    static vector<string>getFormationsNames();
    static const link_matrix& getLinkMatrix(const string& formation_name);
    
    //Retrieves coordinates for a match position
    static const Coordinates& getMPosCoords(const string& m_pos);
    static const vector<string>& getFormationPositions(const string& formation_name);

    //Retrives the normal position for a match position
    static const string& getPosEquivalence(const string& m_pos);
    //static const unordered_map<string, string>& getPosEquivalence();

    //Position, nr_players for that position
    //TO DO:
    static unordered_map<string, uint16_t> getSplitFormation(const string& formation_name);
    static const unordered_map<string, uint16_t>& getSubsLayout();

    //Retrieves the vertex positions for a certain constant name(ex: "PITCH" )
    static float* getVertexPositions(const string& const_name);
};
