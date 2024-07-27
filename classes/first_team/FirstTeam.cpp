#include "FirstTeam.h"


void FirstTeam :: changeSide(){
    for (auto& p : this->first_eleven)
        p->changeSide();
}


void FirstTeam :: changeSide1(){
    for (auto& p : this->first_eleven)
        p->changeSide1();
}


double FirstTeam :: getElevenTotalStats()const{
    return this->getElevenOVR() + this->getChemestry();
}


double FirstTeam :: getElevenOVR() const{
    double eleven_ovr = 0;
    for (const auto& p : this->first_eleven)
        eleven_ovr += p->getOVR();
    
    return eleven_ovr / Constants :: getVal("MATCH_TEAM_SIZE");
}


double FirstTeam :: getSubsOVR()const{
     double subs_ovr = 0;
    for (const auto& p : this->subs)
        subs_ovr += p->getOVR();
    
    return subs_ovr / Constants :: getVal("NR_PLAYERS_BENCH");
}


//TO DO
uint16_t FirstTeam :: getChemestry() const{
    return 0;
}


void FirstTeam :: drawPlayers(MatchPlayer :: pitch_half half, Shader& p_shader, const IBO& player_ibo,
                              const VertexBufferLayout& player_layout, 
                              const VertexBufferLayout& player_aura_layout)const {
    for (const auto& p : this->first_eleven)
        p->draw(half, p_shader, player_ibo, player_layout, player_aura_layout);
}


void FirstTeam :: setAdjacencyMatrix(){
    for (uint16_t i = 0 ; i < first_eleven.size(); ++i)
            for (uint16_t j = i + 1; j < first_eleven.size(); ++j){
                double dist = distance(first_eleven[i]->getCoords(), first_eleven[j]->getCoords());
                adjacency_matrix[first_eleven[i]][first_eleven[j]] = dist;
                adjacency_matrix[first_eleven[j]][first_eleven[i]] = dist;
            }
}


pair<uint16_t, shared_m_player> FirstTeam :: getOpponentIntersections(const shared_m_player& player) const{
    //Will be needed in case there is only one intersection
    shared_m_player opponent;
    uint16_t nr_intersections = 0;

    //We go through the team's first eleven and check if the player intersects with any of them
    for (const auto& p : this->first_eleven){
        if (player->intersects(*p)){
            ++nr_intersections;
            opponent = p;
        }

        //Passed two intersections the player should pass the ball so the opponent becomes nullptr
        if (nr_intersections == 2)
            return {nr_intersections, nullptr};
    }
    
    return {nr_intersections, opponent};
}


void FirstTeam :: movePlayer(uint16_t index, uint16_t intersections,
                             const shared_m_player& opponent, const Coordinates& opp_gk_coords){
    shared_m_player& player = this->first_eleven.at(index);
    vector<MatchPlayer :: PassingInfo> passing_options = this->getPassingOptions(player, opp_gk_coords);

    player->decide(intersections, passing_options, opponent);
}


vector<MatchPlayer :: PassingInfo> FirstTeam :: getPassingOptions(const shared_m_player& player,
                                                                const Coordinates& opp_gk_coords) const{
    vector<MatchPlayer :: PassingInfo> passing_options;
    passing_options.reserve(this->first_eleven.size() - 1);

    //Going through all the players except the one passed as argument
    //Pushing the player and the distance to him in the passing_options vector 
    for (const auto& team_mate : this->first_eleven)
        if (team_mate != player)
            passing_options.emplace_back(team_mate,
                                         player->getPassChance(adjacency_matrix.at(player).at(team_mate)), 
                                         distance(team_mate->getCoords(), opp_gk_coords));

    return passing_options;
}
// void FirstTeam :: movePlayers(const unordered_map<float, unordered_map<float, bool>>& players_positions){
//     for (auto& p : this->first_eleven)
//         p->p_move(players_positions);
// }


// void FirstTeam :: initPlayersPositions(unordered_map<float, unordered_map<float, bool>>& players_positions, 
//                                        bool team) const{
//     for (const auto& p : this->first_eleven){
//         Coordinates player_coords = p->getCoords();
//         players_positions[player_coords.x][player_coords.y] = team;
//     }
// }