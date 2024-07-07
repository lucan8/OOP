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
        unique_first_team first_team(new FirstTeam(f_name,
                                                   this->getFirstEleven(f_name),
                                                   this->getSubstitutes(f_name)
                                                   ));
        if (best_first_team == nullptr || first_team->calculateTotalStats() > best_first_team->calculateTotalStats())
            best_first_team = move(first_team);
    }
    return best_first_team;
}
unique_m_squad Team :: getFirstEleven(const string& form_name) const{
    unique_m_squad first_eleven;
    first_eleven.reserve(Constants :: getVal("MATCH_TEAM_SIZE"));

    //Splitting team by player type
    shared_squad_split split_team  = Team :: splitTeamPType(this->Players);

    //Creating a map for the unsused players
    shared_squad_map unused_players = toUMap(split_team.at("OUTFIELD"));

    //Going through each match position for the formation
    for (const auto& m_pos : Constants :: getFormationPositions(form_name)){
        //Getting the best player for that position
        uint16_t best_index = Team :: getBestPlayerIndex(unused_players, Constants :: getPosEquivalence(m_pos));

        //Adding the player to the first eleven
        first_eleven.emplace_back(unused_players.at(best_index), m_pos);
        unused_players.erase(best_index);
    }

    //TO DO: Adding the goalkeeper
    return first_eleven;
}


uint16_t Team :: getBestPlayerIndex(const shared_squad_map& players, const string& pos){
    double best_OVR = 0;
    uint16_t best_p_index = 0;

    //Going through all players for that position
    for (uint16_t i = 0; i < players.size(); ++i){
        double p_ovr = players.at(i)->calculateOVR(pos);
         //Keeping track of the index of the best player
        if (p_ovr > best_OVR){
            best_OVR = p_ovr;
            best_p_index = i;
        }
    }

    return best_p_index; 
}

unordered_map<string, shared_squad> Team :: splitTeamPos(const shared_squad& team){
    unordered_map<string, shared_squad> split_team;
    initMap(split_team, Constants :: getPositions());

    for (const auto& p : team)
        split_team[p->getPosition()].push_back(p);
    
    return split_team;
}

unordered_map<string, shared_squad> Team :: splitTeamPType(const shared_squad& team){
    unordered_map<string, shared_squad> split_team;
    initMap(split_team, Constants :: getPlayerTypes());

    for (const auto& p : team)
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
    op << "Name: " << this->name << "\nChemestry: " << this->getChemestry()
    << "\nBudget: " << this->budget
    << "\nPoints: " << this->points << "\n";
}

unordered_map<string, double> Team :: getTeamStats() const{
    unordered_map<string, double> stats;
    
    for (const auto& p : this->Players)
        for (const auto& stat : p->getStats())
            stats[stat.first] += stat.second;
    
    for (auto& stat : stats)
        stat.second = (stat.second) / this->Players.size();
    
    return stats;
}