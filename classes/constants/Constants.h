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
#include "../exceptions/InvalidEntityType.h"
#include "../../functions/functions.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <GL/glew.h>
#include <random>
using namespace std;

//We have two types of players: OUTFIELD AND GOALKEEPER(GK)
//For OUTFIELD there are also detailed types: DEFENDER, MIDFIELDER, ATTACKER
//Each type holds a set of "normal" positions(FULLBACK, CM, WINGER...)
//Each "normal" positions holds a set of match positions(LB/RB, LCM/RCM, LW/RW...)

//Adiacence matrix for player positions nodes
//mpos1, mpos2, vector to be added to mpos1 to get mpos2
typedef unordered_map<string, unordered_map<string, glm :: vec2>> link_matrix;
//Match position, player coordinates in the 'pitch matrix'
typedef unordered_map<string, glm :: vec2> mpos_coords;
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

    //match_pos, normal_pos
    //ex: "LCB"->"CB", "LCM"->"CM", "LB"->"FULLBACK"
    static unordered_map<string, string> pos_equivalence;

    //Detailed position, nr_players
    static unordered_map<string, uint16_t> subs_layout;
    //const_name, array of vertices
    //static unordered_map<string, unique_ptr<GLfloat>> vertices;

    //Geometric shape, indices for the vertex positions
    static unordered_map<string, unique_ptr<GLuint>> vertex_indices;
    //Entity type, entity number
    static unordered_map<string, uint16_t> entity_types;
    static mt19937 rng;
    static void initValues(const string& file_name);
    static void initPositions(const string& file_name);

    static void initPlayerGen(const string& file_name);
    static void initStatsRatios(const string& file_name);

    static void initTeamNames(const string& file_name);
    static void initFormations(const string& file_name);

    static void initMPosCoords(const string& file_name);

    static void initPositionEquivalence(const string& file_name);
    static void initSubsLayout(const string& file_name);

    //static void initVertices(const string& file_name);
    static void initVertexIndices(const string& file_name);

    static void initEntityTypes(const string& file_name);
    static void initRNG();
public:
    static void init();

    static uint16_t getVal(const string& const_name);
    static vector<string> getPositions(const string& det_p_type);
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
    
    //Retrieves glm :: vec2 for a match position
    static const glm :: vec2& getMPosCoords(const string& m_pos);
    static const vector<string>& getFormationPositions(const string& formation_name);

    //Retrives the normal position for a match position
    static const string& getPosEquivalence(const string& m_pos);
    //static const unordered_map<string, string>& getPosEquivalence();

    //Position, nr_players for that position
    //TO DO:
    static unordered_map<string, uint16_t> getSplitFormation(const string& formation_name);
    static const unordered_map<string, uint16_t>& getSubsLayout();

    //Retrieves the array of vertices for a certain constant name(ex: "PITCH" )
    //Don't call delete
    //static GLfloat* getVertices(const string& const_name);
    
    //Retrieves the array of vertex indices for a certain constant name(ex: "SQUARE" )
    //Don't call delete
    static GLuint* getVertexIndices(const string& const_name);

    static uint16_t getEntityNumber(const string& entity_type);
    //Returns a random real number between min and max
    static float generateRealNumber(float min, float max);
    //Returns a random positive integer between min and max
    static uint16_t generateNaturalNumber(uint16_t min, uint16_t max);
    //Returns index of chosen element from the weights vector
    static uint16_t generateDiscreteNumber(const vector<uint16_t>& weights);
};
