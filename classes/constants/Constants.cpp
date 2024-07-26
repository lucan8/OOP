#include "Constants.h"

unordered_map<string, uint16_t>  Constants :: values;
unordered_map<string, unordered_map<string, uint16_t>> Constants :: age_info;
unordered_map<string, vector<string>> Constants :: positions;
unordered_map<string, vector<string>> Constants :: age_stats;
unordered_map<string, vector<string>> Constants :: stats;
unordered_map<string, vector<pair<string, uint16_t>>> Constants :: stats_ratios;
vector<string> Constants :: team_names;
unordered_map<string, Constants :: Formation> Constants :: formations;
mpos_coords Constants :: p_coords;
unordered_map<string, string> Constants :: pos_equivalence;
unordered_map<string, uint16_t> Constants :: subs_layout;
unordered_map<string, unique_ptr<GLfloat>> Constants :: vertices;
unordered_map<string, unique_ptr<GLuint>> Constants :: vertex_indices;
unordered_map<string, uint16_t> Constants :: entity_types;

//Holds  match_positions, the link_matrix, and the coordinates for each position
struct Constants :: Formation{
    vector<string> positions;
    link_matrix matrix;
    //mpos_coords coords;

    Formation(){}

    void readLinkMatrix(ifstream& fin);
    //Moved outside of formaation class as player positions as independent of formation
    //void initPlayersCoords(ofstream& fout);
private:
    //Normalizes the coordinates to be between -1 and 1(used in initPlayersCoords)
    //Reading from file now, not needed
    //void normalizeCoords();
};
void Constants :: init(){
    string constants_path = (filesystem :: current_path().parent_path() / "resources" / "constants" / "").string();
    try{
        initValues(constants_path + "values.txt");
        initPlayerGen(constants_path + "player_generation.txt");
        initPositions(constants_path + "positions.txt");
        initStatsRatios(constants_path + "stats_ratios.txt");
        initTeamNames(constants_path + "team_names.txt");
        initFormations(constants_path + "formations.txt");
        initMPosCoords(constants_path + "match_pos_coords.txt");
        initPositionEquivalence(constants_path + "position_equivalence.txt");
        initSubsLayout(constants_path + "subs_layout.txt");
        initVertices(constants_path + "vertices.txt");
        initVertexIndices(constants_path + "vertex_indices.txt");
        initEntityTypes(constants_path + "entity_types.txt");
    }
    catch (FileOpenException& e){
        cerr << e.what() << '\n';
    }
}
void Constants :: initValues(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);

    string const_name;
    uint16_t const_val;

    while (fin >> const_name >> const_val){
        values[const_name] = const_val;
    }
}

void Constants :: initPositions(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);

    string const_name;
    uint16_t nr_positions;

    while (fin.peek() != EOF){
        fin >> const_name >> nr_positions;
        //Adding key-value pair, and initializing a vector of empty string to avoid read exception
        positions.emplace(const_name, vector<string>(nr_positions, ""));

        while (nr_positions-- >= 1)
            fin >> positions[const_name][nr_positions];
    }
    fin.ignore();
}

void Constants :: initPlayerGen(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);

    string player_type, const_name;
    uint16_t nr_player_type, nr_consts, const_val;
    fin >> nr_player_type >> nr_consts;

    while (nr_player_type--){
        fin >> player_type;
        for (int i = 0; i < nr_consts; ++i){
            fin >> const_name >> const_val;
            age_info[player_type][const_name] = const_val;
        }
        fin.ignore();
    }
}

void Constants :: initStatsRatios(const string& file_name){
    ifstream fin(file_name);
    //Checking file opening
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);

    string player_type, aux_player_positions, stat_name, age_related, match_related;
    uint16_t nr_stats, stat_ratio;

    //Filling stats and ratios at the same time
    while (fin.peek() != EOF){
        fin >> player_type >> nr_stats;
        fin.ignore();

        //Readding all ratio_types at once for specific position_type
        getline(fin, aux_player_positions);
        vector<string> player_positions = split(aux_player_positions);

        //Iterating over the stats and setting the ratios
        while (nr_stats-- >= 1){
            fin >> stat_name >> age_related >> match_related;
            stats[player_type].push_back(stat_name);

            if (age_related == "age_related")
                age_stats[player_type].push_back(stat_name);

            for (const auto& p_poz : player_positions){
                fin >> stat_ratio;
                stats_ratios[p_poz].push_back(make_pair(stat_name, stat_ratio));
            }
        }
        fin.ignore();
    }
}

void Constants :: initTeamNames(const string& file_name){
    
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);
    
    uint16_t nr_names;
    string t_name;
    fin >> nr_names;
    fin.ignore();
    team_names.reserve(nr_names);

    while (getline(fin, t_name))
        team_names.push_back(t_name);
}

void Constants :: initFormations(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);
    
    string formation_name, positions;

    //Comment at the top of the file to be ignored
    getline(fin, positions);
    //REMOVE THIS
    // string constants_path = (filesystem :: current_path().parent_path() / "resources" / "constants" / "").string();
    // ofstream fout(constants_path + "match_pos_coords.txt");

    while (fin.peek() != EOF){
        fin >> formation_name;
        fin.ignore();
        formations[formation_name] = Formation();

        //Reading positions and setting them
        getline(fin, positions);
        formations[formation_name].positions = split(positions);

        //Reading link matrix for chemestry
        formations[formation_name].readLinkMatrix(fin);

       //formations[formation_name].initPlayersCoords(fout);
        fin.ignore();
    }

    // for (const auto& p : p_coords){
    //     fout << p.first << ' ' << p.second.x << ' ' << p.second.y << '\n';
    // }
}

void Constants :: initSubsLayout(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);

    string det_p_type;
    uint16_t nr_players;
    //Reading the number of players for each detailed player type
    while (fin >> det_p_type >> nr_players){
        subs_layout[det_p_type] = nr_players;
    }
}

void Constants :: Formation :: readLinkMatrix(ifstream& fin){
    string pos1, pos2;
    uint16_t nr_links = Constants :: getVal("NR_LINKS");

    while (nr_links--){
        fin >> pos1 >> pos2 >> matrix[pos1][pos2].y >> matrix[pos1][pos2].x;
        this->matrix[pos2][pos1] = -matrix[pos1][pos2];
    }
}


void Constants :: initMPosCoords(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);

    string pos;
    //Reading the player positions and their coordinates
    while (fin >> pos)
        fin >> p_coords[pos];
}

/*
void Constants :: Formation :: initPlayersCoords(ofstream& fout){
    this->coords["GK"] = Coordinates(0, -Constants :: getVal("TOUCHLINE_LENGTH") / 2);
    queue<string> p_pos;
    p_pos.push("GK");
    while (!p_pos.empty()){
        string pos = p_pos.front();
        p_coords[pos] = this->coords[pos];
        p_pos.pop();
        //Going through all the positions that are linked to pos
        for (const auto& p : matrix[pos])
            //If the position is not already in the coords map, add it
            if (this->coords.find(p.first) == coords.end()){
                //Adding the offset coordinates to the current position coordinates
                //That will be the new position coordinates
                this->coords[p.first] = this->coords[pos] + this->matrix[pos][p.first];
                p_pos.push(p.first);
            }
    }
}
*/
/*
void Constants :: Formation :: normalizeCoords(){
    float startx = -1.0;
    float starty = 1.0;

    float unity_x = 1.0 / (Constants :: getVal("GOAL_LINE_LENGTH") / 2);
    float unity_y = 1.0 / (Constants :: getVal("TOUCHLINE_LENGTH") / 2);

    for (auto& p : this->coords)
        p.second = Coordinates(startx + p.second.x * unity_x, starty - p.second.y * unity_y);

}

*/
void Constants :: initPositionEquivalence(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);

    string match_pos, normal_pos;
    while (fin >> match_pos >> normal_pos)
        pos_equivalence[match_pos] = normal_pos;

}


void Constants :: initVertices(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);

    string const_name;
    uint16_t nr_positions;

    while (fin >> const_name >> nr_positions){
        vertices[const_name] = unique_ptr<GLfloat>(new GLfloat[nr_positions]);
        for (int i = 0; i < nr_positions; ++i)
            fin >> vertices[const_name].get()[i];
    }
}

void Constants :: initVertexIndices(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);

    string shape;
    uint16_t nr_indices;

    while (fin >> shape >> nr_indices){
        vertex_indices[shape] = unique_ptr<GLuint>(new GLuint[nr_indices]);
        for (int i = 0; i < nr_indices; ++i)
            fin >> vertex_indices[shape].get()[i];
    }
}


void Constants :: initEntityTypes(const string& file_name){
    ifstream fin(file_name);
    if (!fin.is_open())
        throw FileOpenException(__func__, file_name);

    string entity_type;
    uint16_t entity_nr;

    while (fin >> entity_type >> entity_nr)
        entity_types[entity_type] = entity_nr;
}


uint16_t Constants ::  getVal(const string& const_name){
    try{
        return values.at(const_name);
    } catch(out_of_range& e){
        throw InvalidConstName(__func__, const_name);
    }
}


unordered_map<string, uint16_t> Constants ::  getAllAgeInfo(const string& age_type){
    try{
        return age_info.at(age_type);
    } catch(out_of_range& e){
        throw InvalidAgeType(__func__, age_type);
    }
}


uint16_t Constants :: getAgeInfo(const string& age_type, const string& const_name){
    try{
        return getAllAgeInfo(age_type).at(const_name);
    }
    catch(out_of_range& e){
        throw InvalidConstName(__func__, const_name);
    }
}


vector<string> Constants :: getPositions(const string& p_type){
    try{
        return positions.at(p_type);
    }catch(out_of_range& e){
        throw InvalidPlayerType(__func__, p_type);
    }
}


vector<string> Constants :: getPositions(){
    return getKeys(stats_ratios);
}


vector<string> Constants :: getStats(const string& p_type){
    try{
        return stats.at(p_type);
    }catch(out_of_range& e){
        throw InvalidPlayerType(__func__, p_type);
    }
}


vector<string> Constants :: getAgeRelatedStats(const string& p_type){
    try{
        return age_stats.at(p_type);
    }catch(out_of_range& e){
        throw InvalidPlayerType(__func__, p_type);
    }
    
}


vector<pair<string, uint16_t>> Constants :: getStatsRatios(const string& p_pos){
    try{
        return stats_ratios.at(p_pos);
    }catch(out_of_range& e){
        throw InvalidPosition(__func__, p_pos);
    }
    
}


vector<string> Constants :: getPlayerTypes(){
        return getKeys(stats);
}


vector<string> Constants :: getDetailedPTypes(){
    return getKeys(positions);
}


vector<string> Constants :: getSameDetType(const string& pos){
    for (auto& p_t : positions)
        if (find(p_t.second.begin(), p_t.second.end(), pos) != p_t.second.end())
            return p_t.second;

    throw InvalidPosition(__func__, pos);
}


vector<string> Constants :: getAgeTypes(){
    return getKeys(age_info);
}


const vector<string>& Constants :: getTeamNames(){
    return team_names;
}


vector<string> Constants :: getFormationsNames(){
    return getKeys(formations);
}


const link_matrix& Constants :: getLinkMatrix(const string& formation_name){
    try{
        return Constants :: formations.at(formation_name).matrix;
    } catch(out_of_range& e){
        throw InvalidFormation(__func__, formation_name);
    }
}


const vector<string>& Constants :: getFormationPositions(const string& formation_name){
    try{
        return Constants :: formations.at(formation_name).positions;
    } catch(out_of_range& e){
        throw InvalidFormation(__func__, formation_name);
    }
}


const Coordinates& Constants :: getMPosCoords(const string& m_pos){
    try{
        return p_coords.at(m_pos);
    } catch(out_of_range& e){
        throw InvalidMatchPosition(__func__, m_pos);
    }
}


/*
const players_coords& Constants :: getPlayersCoords(const string& formation_name){
    try{
        return Constants :: formations.at(formation_name).coords;
    } catch(out_of_range& e){
        throw InvalidFormation(__func__, formation_name);
    }
}
*/


const string& Constants :: getPosEquivalence(const string& m_pos){
    try{
        return pos_equivalence.at(m_pos);
    } catch(out_of_range& e){
        throw InvalidMatchPosition(__func__, m_pos);
    }
}


const unordered_map<string, uint16_t>& Constants :: getSubsLayout(){
    return subs_layout;
}


GLfloat* Constants :: getVertices(const string& const_name){
    try{
        return vertices.at(const_name).get();
    } catch(out_of_range& e){
        throw InvalidConstName(__func__, const_name);
    }
}


GLuint* Constants :: getVertexIndices(const string& const_name){
    try{
        return vertex_indices.at(const_name).get();
    } catch(out_of_range& e){
        throw InvalidConstName(__func__, const_name);
    }
}


uint16_t Constants :: getEntityNumber(const string& entity_type){
    try{
        return entity_types.at(entity_type);
    } catch(out_of_range& e){
        throw InvalidEntityType(__func__, entity_type);
    }
}