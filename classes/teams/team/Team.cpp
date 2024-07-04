#include "Team.h"

void Team :: addPoints(const unsigned char p){
    this->points += p;
}

team_ptr Team :: clone() const{
    return new Team(*this);
}

void Team :: clonePlayers(const shared_squad& Players){
    this->Players.reserve(Players.size());
    for (auto& p : Players)
        this->Players.push_back(shared_player(p->clone()));
}


unique_ptr<FirstEleven> Team :: getBestFirstTeam() const{
    unique_ptr<FirstEleven> best_first_eleven;
    unordered_map<string, shared_squad> split_team  = Team :: splitTeamPType(this->Players);
    double best_ovr = 0;

    for (const auto& f : Constants :: getFormations()){
        unique_ptr<FirstEleven> first_team = this->getFirstTeamOutfields(f.first, split_team["OUTFIELD"]);
        double t_ovr = first_team->getFirstElevenOVR(first_team->getPlayers());

        if (t_ovr > best_ovr){
            best_ovr = t_ovr;
            best_first_eleven = move(first_team);
        }
    }
    best_first_eleven->setGoalkeeper(*getBestPlayerIt(split_team["GK"], "GK"));
    return best_first_eleven;
}
//PROBLEMS:     
//Biased towards the first positions chosen
//Inneficient as we calculate the same OVR multiple times when we don't need to

unique_ptr<FirstEleven> Team :: getFirstTeamOutfields(const string& form_name, 
                                             shared_squad unused_players) const{
    unordered_map<string, shared_squad> first_eleven;

    //Going through each position in the formation
    for (const auto& p_pos : Constants :: getFormation(form_name))
        //Getting all players needed for that position
        for (int i = 0; i < p_pos.second; ++i){
            shared_squad :: const_iterator best_it = Team :: getBestPlayerIt(unused_players, 
                                                                                p_pos.first);
            first_eleven[p_pos.first].push_back(*best_it);
            unused_players.erase(best_it);
        }
    
    return make_unique<FirstEleven>(form_name, first_eleven);
}


shared_squad :: const_iterator Team :: getBestPlayerIt( const shared_squad& players, 
                                                        const string& pos){
    uint16_t best_p_index;
    double best_OVR = 0;

    for (uint16_t i = 0; i < players.size(); ++i){
        double p_ovr = players[i]->getOVR(pos);
        if (p_ovr > best_OVR){
            best_OVR = p_ovr;
            best_p_index = i;
        }
    }
    return players.begin() + best_p_index;
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

//Not implemented
uint16_t Team :: getChemestry() const{
    return Constants :: getVal("MAX_CHEM");
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