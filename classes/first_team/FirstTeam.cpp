#include "FirstTeam.h"
#include "../exceptions/MyRuntimeException.h"
void FirstTeam :: changeSide(){
    for (auto& p : this->first_eleven)
        p->changeSide();
}


void FirstTeam :: changeSide1(){
    for (auto& p : this->first_eleven)
        p->changeSide1();
}


float FirstTeam :: getElevenTotalStats()const{
    return this->getElevenOVR() + this->getChemestry();
}


float FirstTeam :: getElevenOVR() const{
    float eleven_ovr = 0;
    for (const auto& p : this->first_eleven)
        eleven_ovr += p->getOVR();
    
    return eleven_ovr / Constants :: getVal("MATCH_TEAM_SIZE");
}


float FirstTeam :: getSubsOVR()const{
     float subs_ovr = 0;
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


pair<shared_m_player, shared_m_player> FirstTeam :: getClosest2Players(const glm :: vec2& target) const{
    uint16_t closest1 = 0, closest2 = 0;
    float min_dist1 = numeric_limits<float>::max(), min_dist2 = numeric_limits<float>::max();

    for (uint16_t i = 0; i < this->first_eleven.size(); ++i){
        const shared_m_player& curr_player = this->first_eleven[i];
        //Using it as a vec2 for convenience
        glm :: vec2 player_coords = curr_player->getCoords();

        //If the player is the target we ignore him
        if (player_coords == target)
            continue;

        float dist = glm :: distance(curr_player->getCoords(), target);

        if (dist < min_dist1){
            min_dist2 = min_dist1;
            closest2 = closest1;

            min_dist1 = dist;
            closest1 = i;
        }
        else if (dist < min_dist2){
            min_dist2 = dist;
            closest2 = i;
        }
    }

    return {this->first_eleven[closest1], this->first_eleven[closest2]};
}


shared_m_player FirstTeam :: getClosestPlayer(const glm :: vec2& target) const{
    uint16_t closest = 0;
    float min_dist = numeric_limits<float>::max();

    for (uint16_t i = 0; i < this->first_eleven.size(); ++i){
        const shared_m_player& curr_player = this->first_eleven[i];
        //Using it as a vec2 for convenience
        glm :: vec2 player_coords = curr_player->getCoords();
        //If the player is the target we ignore him
        if (player_coords == target)
            continue;

        float dist = glm :: distance(player_coords, target);

        if (dist < min_dist){
            min_dist = dist;
            closest = i;
        }
    }

    return this->first_eleven[closest];
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


void FirstTeam :: movePlayerWithBall(glm :: vec2& ball_coords, shared_m_player& player , uint16_t intersections,
                                     const shared_m_player& opponent, const glm :: vec2& opp_gk_coords){
    //Checking if the player is in the team
    if (find(this->first_eleven.begin(), this->first_eleven.end(), player) == this->first_eleven.end())
        throw MyRuntimeException(__FILE__, __func__, __LINE__, "Player not found in the team");

    vector<MatchPlayer :: PassingInfo> passing_options = this->getPassingOptions(player, opp_gk_coords);
    player->decide(ball_coords, intersections, passing_options, opponent, opp_gk_coords, getClosestPlayer(player->getCoords()));
}


void FirstTeam :: movePlayersWithoutBall(const shared_m_player& player_with_ball, const glm :: vec2& opp_gk_coords){
    for (auto& player : this->first_eleven){
        //Ignoring the player with the ball
        if (player == player_with_ball)
            continue;
        //Getting the player's and the player with the ball's coordinates
        glm :: vec2 p_coords = player->getCoords(),
                    p_ball_coords = player_with_ball->getCoords();

        //Getting the player's succesful pass chance
        float pass_chance = player_with_ball->getPassChance(glm :: distance(p_coords, p_ball_coords));

        //If not within player's passing range we move him towards the him
        if (pass_chance < Constants :: getVal("PASS_CHANCE_THRESHOLD"))
            player->p_move(p_ball_coords, getClosestPlayer(p_coords));
        else //If within passing range we move him towards the opponent's goal
            player->p_move(opp_gk_coords, getClosestPlayer(p_coords));
    }
        
}


void FirstTeam :: movePlayersWithoutBall(glm :: vec2 ball_coords, const glm :: vec2& opp_gk_coords){
    //Moving the closest two players to the ball towards it
    shared_m_player closest_player = getClosestPlayer(ball_coords);
    closest_player->p_move(ball_coords, getClosestPlayer(closest_player->getCoords()));

    //Moving the rest of the players towards the opponent's goal
    for (const auto&  player : this->first_eleven){
        if (player != closest_player)
            player->p_move(opp_gk_coords, getClosestPlayer(player->getCoords()));
    }
}


vector<MatchPlayer :: PassingInfo> FirstTeam :: getPassingOptions(const shared_m_player& player,
                                                                  const glm :: vec2& opp_gk_coords) const{
    vector<MatchPlayer :: PassingInfo> passing_options;
    passing_options.reserve(this->first_eleven.size() - 2);

    //Going through all the players except the one passed as argument
    //Pushing the player and the distance to him in the passing_options vector 
    for (const auto& team_mate : this->first_eleven)
        //Ignoring the player passed as argument and the goalkeeper
        if (team_mate != player && team_mate->getPosition() != "GK"){
            float dist_team_mate = glm :: distance(player->getCoords(), team_mate->getCoords());
            float dist_goal = glm :: distance(team_mate->getCoords(), opp_gk_coords);
            passing_options.emplace_back(team_mate, player->getPassChance(dist_team_mate), dist_goal);
        }

    return passing_options;
}