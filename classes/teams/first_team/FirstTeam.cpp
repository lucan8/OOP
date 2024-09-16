#include "FirstTeam.h"
#include "../../exceptions/MyRuntimeException.h"

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


void FirstTeam :: changeSide(){
    this->side = !this->side;
    for (auto& p : this->first_eleven)
        p->changeSide();
}


void FirstTeam :: changeSide1(){
    for (auto& p : this->first_eleven)
        p->changeSide1();
}


void FirstTeam :: drawPlayers(MatchPlayer :: pitch_half half, Shader& p_shader, const IBO& player_ibo,
                              const VertexBufferLayout& player_layout, 
                              const VertexBufferLayout& player_aura_layout)const {
    p_shader.setUniform4f("u_aura_color", this->team->getAuraColor());
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


shared_m_player FirstTeam :: getClosestPlayer(const glm :: vec2& target, const shared_m_squad& players){
    uint16_t closest = 0;
    float min_dist = numeric_limits<float>::max();

    for (uint16_t i = 0; i < players.size(); ++i){
        const shared_m_player& curr_player = players[i];
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

    return players[closest];
}


void FirstTeam :: attack(glm :: vec2& ball_coords, const FirstTeam& opp_team){
    //Moving the player with the ball
    shared_m_player player_with_ball = this->getPlayerWithBall();
    glm :: vec2 opp_gk_coords = opp_team.getGKCoords();
    
    //Moving the rest of the players
    this->movePlayersWithoutBall(*player_with_ball, opp_team);
    MatchPlayer :: passing_options passing_options = this->getPassingOptions(*player_with_ball, opp_team);
    player_with_ball->decide(ball_coords, player_with_ball->getTacklingIntersections(opp_team.getFirstEleven()),
                             passing_options, opp_gk_coords, *getClosestPlayer(player_with_ball->getCoords(),
                             this->first_eleven));
}


void FirstTeam :: defend(glm :: vec2& ball_coords, const FirstTeam& opp_team){
    shared_m_player closest_ball = getClosestPlayer(ball_coords, this->first_eleven),
                    player_with_ball = opp_team.getPlayerWithBall();
    
    //If the player with the ball is in tackle range we tackle him
    if (player_with_ball->inTackleRange(*closest_ball))
        closest_ball->tackle(ball_coords, *player_with_ball);
    else //Else we move towards him
        closest_ball->p_move(ball_coords, *getClosestPlayer(closest_ball->getCoords(), this->first_eleven));

    for (const auto& player : this->first_eleven)
        //Ignoring the player with the ball and the GK
        if (player->getPosition() != "GK" && player != closest_ball){
            MatchPlayer :: OpponentIntersections
                player_markers = player->getMarkingIntersections(opp_team.getFirstEleven());
            
            //Checking if the player is marking someone, and if that someone is only marked by him
            //If so, he moves towards that player
            if (player_markers.nr_intersections == 1 &&
                player_markers.opponent->getMarkingIntersections(this->first_eleven).nr_intersections == 1)
                player->p_move(player_markers.opponent->getCoords(), *getClosestPlayer(player->getCoords(),
                               this->first_eleven));
   
            else{ //Else he moves towards the closest unmarked player
                shared_m_squad unmarked_players = opp_team.getUnmarkedPlayers(*this);
                player->p_move(FirstTeam :: getClosestPlayer(player->getCoords(), unmarked_players)->getCoords(),
                               *getClosestPlayer(player->getCoords(), this->first_eleven));  
            }
        }
}


void FirstTeam :: getPossesion(glm :: vec2 ball_coords, const glm :: vec2& opp_gk_coords){
    //Moving the closest player towards the ball
    shared_m_player closest_player = getClosestPlayer(ball_coords, this->first_eleven);
    closest_player->p_move(ball_coords, *getClosestPlayer(closest_player->getCoords(), this->first_eleven));

    //Moving the rest of the players towards the opponent's goal
    for (const auto&  player : this->first_eleven)
        if (player != closest_player && player->getPosition() != "GK")
            player->p_move(opp_gk_coords, *getClosestPlayer(player->getCoords(), this->first_eleven));
}


void FirstTeam :: movePlayersWithoutBall(const MatchPlayer& player_with_ball, const FirstTeam& opp_team){
    for (auto& player : this->first_eleven){
        //Ignoring the player with the ball, GK, and the CBs that passed the midline
        if (player->getPlayer() == player_with_ball.getPlayer() || player->getPosition() == "GK" ||
            (Constants :: getPosEquivalence(player->getPosition()) == "CB" && player->isPassedMidline(this->side)))
            continue;

        //Getting the player's and the player with the ball's coordinates
        glm :: vec2 p_coords = player->getCoords(),
                    p_ball_coords = player_with_ball.getCoords();
                    
        const shared_m_squad& opponents = opp_team.getFirstEleven();
        glm :: vec2 opp_gk_coords = opp_team.getGKCoords();

        //Getting the player's succesful pass chance
        float pass_chance = player_with_ball.getPassChance(*player);

        //If not within player's passing range we move him where the pass_chance is highest
        if (pass_chance < Constants :: getVal("PASS_CHANCE_THRESHOLD"))
            player->p_move(player_with_ball, *getClosestPlayer(p_coords, this->first_eleven), opponents);
        else //If within passing range we move him towards the opponent's goal
            player->p_move(opp_gk_coords, *getClosestPlayer(p_coords, this->first_eleven));
    }
        
}




MatchPlayer :: passing_options FirstTeam :: getPassingOptions(const MatchPlayer& player, const FirstTeam& opp_team) const{
    MatchPlayer :: passing_options passing_options;
    passing_options.reserve(this->first_eleven.size() - 2);

    const shared_m_squad& opponents = opp_team.getFirstEleven();
    glm :: vec2 opp_gk_coords = opp_team.getGKCoords();

    //Going through all the players except the one passed as argument
    for (const auto& team_mate : this->first_eleven)
        //Ignoring the player passed as argument and the goalkeeper
        if (team_mate->getPlayer() != player.getPlayer() && team_mate->getPosition() != "GK"){
            MatchPlayer :: OpponentIntersections curr_intersections = team_mate->getMarkingIntersections(opponents);
            //Getting the distance to the opponent's goal and the pass chance
            float dist_goal = glm :: distance(team_mate->getCoords(), opp_gk_coords),
                  pass_chance = player.getPassChance(*team_mate);
            passing_options.emplace_back(team_mate, pass_chance, dist_goal, curr_intersections);
        }

    return passing_options;
}

/*
void FirstTeam :: setPitchMatrix(shared_m_matrix& pitch_matrix) const{
    for (auto player : this->first_eleven){
        glm :: vec2 p_coords = player->getPitchMatrixcoords();
        //Setting the player in the pitch matrix(if coordinates are valid)
        if (p_coords.x < pitch_matrix.size() && p_coords.y < pitch_matrix.at(p_coords.x).size())
            pitch_matrix.at(p_coords.x).at(p_coords.y) = player;
        else
            throw MyRuntimeException(__FILE__, __func__, __LINE__, "Player's coordinates out of matrix range: " + 
                                    to_string(p_coords.x) + " " + to_string(p_coords.y));
    }
}
*/


bool FirstTeam :: operator<(const FirstTeam& other) const{
    float this_eleven_total_stats = getElevenTotalStats(),
            other_eleven_total_stats = other.getElevenTotalStats();
    
    if (this_eleven_total_stats == other_eleven_total_stats)
        return getSubsOVR() < other.getSubsOVR();
    
    return this_eleven_total_stats < other_eleven_total_stats;
}


 vector<shared_m_player> FirstTeam :: getUnmarkedPlayers(const FirstTeam& opp_team) const{
    vector<shared_m_player> unmarked_players;
    unmarked_players.reserve(this->first_eleven.size());
    const shared_m_squad& opponents = opp_team.getFirstEleven();
    
    for (const auto& player : this->first_eleven)
        if (player->getMarkingIntersections(opponents).nr_intersections == 0)
            unmarked_players.push_back(player);

    return unmarked_players;
}


shared_m_player FirstTeam :: getPlayerWithBall() const{
    for (const auto& player : this->first_eleven)
        if (player->hasBall())
            return player;
    return nullptr;
}