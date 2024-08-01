#include "Team.h"

void Team :: addPoints(const unsigned char p){
    this->points += p;
}

team_ptr Team :: clone() const{
    return new Team(*this);
}

unique_first_team Team :: getFirstTeam() const{
    unique_first_team best_first_team;
    for (const auto& f_name : Constants :: getFormationsNames()){
        
        //Splitting the team by player type
        shared_squad_split split_team  = splitTeamPType();
        //Creating a map for the unsused outfield players
        shared_squad_map unused_outfields = toUMap(split_team.at("OUTFIELD"));
        //Creating a map for the unsused goalkeepers
        shared_squad_map unused_goalkeepers = toUMap(split_team.at("GK"));
        
        //This needs to execute before getSubstitutes
        //because it removes players from the unused players
        shared_m_squad first_eleven = this->getFirstEleven(f_name, unused_outfields, unused_goalkeepers);
        unique_first_team first_team(new FirstTeam(f_name,
                                                   move(first_eleven),
                                                   this->getSubstitutes(unused_outfields, unused_goalkeepers)
                                                   ));
        if (best_first_team == nullptr || first_team->getElevenTotalStats() > best_first_team->getElevenTotalStats())
            best_first_team = move(first_team);
    }
    return best_first_team;
}

shared_m_squad Team :: getFirstEleven(const string& form_name, shared_squad_map& unused_outfields,
                                      shared_squad_map& unused_goalkeepers){
    shared_m_squad first_eleven = getFirstTeamOutfields(form_name, unused_outfields);
    first_eleven.emplace_back(getBestGoalkeeper(unused_goalkeepers));

    return first_eleven;
}


shared_m_squad Team :: getFirstTeamOutfields(const string& form_name, shared_squad_map& unused_outfields){
    shared_m_squad first_eleven;
    first_eleven.reserve(Constants :: getVal("MATCH_TEAM_SIZE"));

    for (const auto& m_pos : Constants :: getFormationPositions(form_name)){
        //Getting the match position equivalence(we can't calculate OVR without it)
        const string& pos = Constants :: getPosEquivalence(m_pos);
        //Getting the best player for that position
        uint16_t best_index = Team :: getBestPlayerIndex(unused_outfields, pos);
        shared_player& best_player = unused_outfields.at(best_index);
        
        //Adding the player to the first eleven
        first_eleven.emplace_back(new MatchOutfield(best_player, m_pos, best_player->calculateOVR(pos),
                                                    Constants :: getMPosCoords(m_pos)));
        //removing the player from the unused players
        unused_outfields.erase(best_index);
    }
    return first_eleven;
}


m_player_ptr Team :: getBestGoalkeeper(shared_squad_map& unused_goalkeepers){
    uint16_t best_gk_index = Team :: getBestPlayerIndex(unused_goalkeepers, "GK");
    shared_player& chosen_gk = unused_goalkeepers.at(best_gk_index);

    m_player_ptr best_gk = new MatchGoalkeeper(chosen_gk, "GK", chosen_gk->calculateOVR("GK"), 
                                                Constants :: getMPosCoords("GK"));
    unused_goalkeepers.erase(best_gk_index);

    return best_gk;
}


uint16_t Team :: getBestPlayerIndex(const shared_squad_map& players, const string& pos){
    float best_OVR = 0;
    uint16_t best_p_index = 0;

    //Going through all players for that position(first: index, second: player)
    for (const auto& player : players){
        float p_ovr = player.second->calculateOVR(pos);
         //Keeping track of the index of the best player
        if (p_ovr > best_OVR){
            best_OVR = p_ovr;
            best_p_index = player.first;
        }
    }

    return best_p_index; 
}


unique_m_squad Team :: getSubstitutes(shared_squad_map& unused_outfields, shared_squad_map& unused_goalkeepers){
    unique_m_squad substitutes(Constants :: getVal("NR_PLAYERS_BENCH"));
    substitutes[0] = unique_m_player(getBestGoalkeeper(unused_goalkeepers));
    uint16_t subs_index = 1;

    //Going through all the p_types for the substitutes
    for (const auto& ptype_pnr : Constants :: getSubsLayout()){
        //Skipping the GK p_type
        if (ptype_pnr.first == "GK")
            continue;
        //The same players, but now in the form of match players with the best positions
        //related to the detailed player type
        unique_m_squad_map players = getSubsOutfields(unused_outfields, ptype_pnr.first);
        //Getting the best two players for that p_type
        pair<uint16_t, uint16_t> best_players = getMax2PlayersIndexes(players);

        //Adding the players to the substitutes
        substitutes[subs_index++] = move(players.at(best_players.first));
        substitutes[subs_index++] = move(players.at(best_players.second));

        //Removing the players from the unused players
        unused_outfields.erase(best_players.first);
        unused_outfields.erase(best_players.second);
    }
    return substitutes;
}

//TO DO:Name differently
unique_m_squad_map Team :: getSubsOutfields(shared_squad_map& unused_outfields, const string& det_p_type){
    unique_m_squad_map possible_substitutes;

    possible_substitutes.reserve(unused_outfields.size());

    //Going through all the outfield players(player.first is index, player.second is the actual player)
    for (const auto& player : unused_outfields){
        float best_OVR = 0;
        string best_pos;
        //Going through all the positions related to the detailed player type for that player
        for (const auto& pos : Constants :: getPositions(det_p_type)){
            float p_ovr = player.second->calculateOVR(pos);
            //Choosing the best position for that player
            if (p_ovr > best_OVR){
                best_OVR = p_ovr;
                best_pos = pos;
            }
        }
        possible_substitutes.insert(make_pair(player.first, unique_m_player(new MatchOutfield(player.second, best_pos, best_OVR))));
    }

    return possible_substitutes;
}
pair<uint16_t, uint16_t> Team :: getMax2PlayersIndexes(const unique_m_squad_map& players){
    //first is OVR, second is index
    pair<float, uint16_t> best_player1(0, 0), best_player2(0, 0);

    for (const auto& player : players){
        float p_ovr = player.second->getOVR();
        if (p_ovr > best_player1.first){
            best_player2 = best_player1;
            best_player1 = make_pair(p_ovr, player.first);
        }
        else if (p_ovr > best_player2.first)
            best_player2 = make_pair(p_ovr, player.first);
        }
        
    return make_pair(best_player1.second, best_player2.second);
}

shared_squad_split Team :: splitTeamPos() const{
    unordered_map<string, shared_squad> split_team;
    initMap(split_team, Constants :: getPositions());

    for (const auto& p : this->Players)
        split_team[p->getPosition()].push_back(p);
    
    return split_team;
}


shared_squad_split Team :: splitTeamPType() const{
    shared_squad_split split_team;
    initMap(split_team, Constants :: getPlayerTypes());

    for (const auto& p : this->Players)
        if (p->getPosition() != "GK")
            split_team.at("OUTFIELD").push_back(p);
        else    
            split_team.at("GK").push_back(p);

    return split_team;
}


void Team :: resetSeasonStats(){
    this->points = 0;
    for (auto& p : this->Players)
        p->resetSeasonStats();
}

void Team :: restPlayers(){
    for (auto&p : Players)
        p->rest();
}


void Team :: trainPlayers(){
    for (auto& p : Players)
        p->train();
}


void Team :: read(istream& op){
    op >> this->name >> this->budget;
    for (auto& p : this->Players)
        op >> *p;
}

void Team :: print(ostream& op) const{
    op << "Name: " << this->name
    << "\nBudget: " << this->budget
    << "\nPoints: " << this->points << "\n";
}

unordered_map<string, float> Team :: getTeamStats() const{
    unordered_map<string, float> stats;
    
    for (const auto& p : this->Players)
        for (const auto& stat : p->getStats())
            stats[stat.first] += stat.second;
    
    for (auto& stat : stats)
        stat.second = (stat.second) / this->Players.size();
    
    return stats;
}