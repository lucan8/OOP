#pragma once
#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include <random>
#include <glm/glm.hpp>
#include <memory>
//We have two types of players: OUTFIELD AND GOALKEEPER(GK)
//For OUTFIELD there are also detailed types: DEFENDER, MIDFIELDER, ATTACKER
//Each type holds a set of "normal" positions(FULLBACK, CM, WINGER...)
//Each "normal" positions holds a set of match positions(LB/RB, LCM/RCM, LW/RW...)

//Adiacence matrix for player positions nodes
//mpos1, mpos2, vector to be added to mpos1 to get mpos2
typedef std :: unordered_map<std :: string, std :: unordered_map<std :: string, glm :: vec2>> link_matrix;
//Match position, player coordinates in the 'pitch matrix'
typedef std :: unordered_map<std :: string, glm :: vec2> mpos_coords;
class Constants{
private:
    //Holds a vector of match positions and the link matrix
    struct Formation;
    
    //Const name, value(ex: "MATCH_TEAM_SIZE"->11)
    static std :: unordered_map<std :: string, uint16_t> values;
    //Age type(ex: "YOUNG"), const_name(ex: "MIN_AGE"), value(ex: 18
    static std :: unordered_map<std :: string, std :: unordered_map<std :: string, uint16_t>>age_info;
    
    //Detailed player type, vector of normal positions
    static std :: unordered_map<std :: string, std :: vector<std :: string>> positions;
    //Player type, std :: vector of stats
    static std :: unordered_map<std :: string, std :: vector<std :: string>> stats, age_stats;
    //Player type, std :: vector of pairs(stat_name, weight)
    static std :: unordered_map<std :: string, std :: vector<std :: pair<std :: string, uint16_t>>> stats_ratios;
    static std :: vector<std :: string> team_names;

    static std :: unordered_map<std :: string, Formation> formations;
    static mpos_coords p_coords;

    //match_pos, normal_pos
    //ex: "LCB"->"CB", "LCM"->"CM", "LB"->"FULLBACK"
    static std :: unordered_map<std :: string, std :: string> pos_equivalence;

    //Detailed position, nr_players
    static std :: unordered_map<std :: string, uint16_t> subs_layout;
    //const_name, array of vertices
    //static std :: unordered_map<std :: string, unique_ptr<GLfloat>> vertices;

    //Geometric shape, indices for the vertex positions
    static std :: unordered_map<std :: string, std :: unique_ptr<GLuint>> vertex_indices;
    //Entity type, entity number
    static std :: unordered_map<std :: string, uint16_t> entity_types;
    static std :: mt19937 rng;
    static void initValues(const std :: string& file_name);
    static void initPositions(const std :: string& file_name);

    static void initPlayerGen(const std :: string& file_name);
    static void initStatsRatios(const std :: string& file_name);

    static void initTeamNames(const std :: string& file_name);
    static void initFormations(const std :: string& file_name);

    static void initMPosCoords(const std :: string& file_name);

    static void initPositionEquivalence(const std :: string& file_name);
    static void initSubsLayout(const std :: string& file_name);

    //static void initVertices(const std :: string& file_name);
    static void initVertexIndices(const std :: string& file_name);

    static void initEntityTypes(const std :: string& file_name);
    static void initRNG();
public:
    static void init();

    static uint16_t getVal(const std :: string& const_name);
    static std :: vector<std :: string> getPositions(const std :: string& det_p_type);
    //Retrieves all positions
    static std :: vector<std :: string> getPositions();
    //static std :: unordered_map<std :: string, std :: vector<std :: string>> getPositions1(){return positions;};

    static std :: unordered_map<std :: string, uint16_t> getAllAgeInfo(const std :: string& age_type);
    static uint16_t getAgeInfo(const std :: string& age_type, const std :: string& const_name);

    //The player stats depending on the player type
    //ex: "GK"->{"DIV", "HAN", ...}
    static std :: vector<std :: string> getStats(const std :: string& p_type);
    static std :: vector<std :: string> getAgeRelatedStats(const std :: string& p_type);
    
    //GK, OUTFILED
    static std :: vector<std :: string> getPlayerTypes();
    //GK, DEFENDER, MIDFIELDER, ATTACKER
    static std :: vector<std :: string> getDetailedPTypes();

    //Returns all positions which have the same detailed player type as pos
    //ex: pos = "cm"->{"cm", "cdm", "cam", "sidemid"}
    static std :: vector<std :: string> getSameDetType(const std :: string& pos);
    static std :: vector<std :: string> getAgeTypes();

    static std :: vector<std :: pair<std :: string, uint16_t>> getStatsRatios(const std :: string& p_pos);
    static const std :: vector<std :: string>& getTeamNames();

    static std :: vector<std :: string>getFormationsNames();
    static const link_matrix& getLinkMatrix(const std :: string& formation_name);
    
    //Retrieves glm :: vec2 for a match position
    static const glm :: vec2& getMPosCoords(const std :: string& m_pos);
    static const std :: vector<std :: string>& getFormationPositions(const std :: string& formation_name);

    //Retrives the normal position for a match position
    static const std :: string& getPosEquivalence(const std :: string& m_pos);
    //static const std :: unordered_map<std :: string, std :: string>& getPosEquivalence();

    //Position, nr_players for that position
    //TO DO:
    static std :: unordered_map<std :: string, uint16_t> getSplitFormation(const std :: string& formation_name);
    static const std :: unordered_map<std :: string, uint16_t>& getSubsLayout();

    //Retrieves the array of vertices for a certain constant name(ex: "PITCH" )
    //Don't call delete
    //static GLfloat* getVertices(const std :: string& const_name);
    
    //Retrieves the array of vertex indices for a certain constant name(ex: "SQUARE" )
    //Don't call delete
    static GLuint* getVertexIndices(const std :: string& const_name);

    static uint16_t getEntityNumber(const std :: string& entity_type);
    //Returns a random real number between min and max
    static float generateRealNumber(float min, float max);
    //Returns a random positive integer between min and max
    static uint16_t generateNaturalNumber(uint16_t min, uint16_t max);
    //Returns index of chosen element from the weights std :: vector
    static uint16_t generateDiscreteNumber(const std :: vector<uint16_t>& weights);

    //Retruns the projection matrix for the pitch coordinate system
    //center is 0,0
    static glm :: mat4 getPitchProj();
    //Returns the projection matrix for the normal pixel coordinate system
    //top left corner is 0,0
    static glm :: mat4 getPixelProj();
    //Returns the projection matrix for the fragment shader pixel coordinate system
    //bottom left corner is 0,0
    static glm :: mat4 getPixelFragProj();
};
