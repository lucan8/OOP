#include "FirstTeam.h"
#include "../../exceptions/MyRuntimeException.h"
#include "../../constants/Constants.h"
#include "../../../functions/functions.h"
#include <limits>
#include <algorithm>

using glm :: vec2, glm :: distance;
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


void FirstTeam :: drawPlaying(MatchPlayer :: pitch_half half, Shader& p_shader, const IBO& player_ibo,
                              const VertexBufferLayout& player_layout, 
                              const VertexBufferLayout& player_aura_layout)const {
    p_shader.setUniform4f("u_aura_color", this->team->getAuraColor());
    for (const auto& p : this->first_eleven)
        p->drawPlaying(half, p_shader, player_ibo, player_layout, player_aura_layout);
}


void FirstTeam :: drawUnplaying(Shader& shader, const IBO& ibo, const Font& font) const{
    glm :: vec2 pos;
    //Converting the pitch coordinates to the pixel coordinate system
    pos = convertCoords(getSubsTextStartPos(), Constants :: getPitchProj(), Constants :: getPixelProj());
    for (const auto& p : this->subs){
        p->drawUnplaying(shader, ibo, font, pos, this->side);
        //Moving the text position downwards for the next sub
        pos.y -= Constants :: getVal("FONT_SIZE");
    }
}


glm :: vec2 FirstTeam :: getSubsTextStartPos() const{
    float max_pitch_x = Constants :: getVal("TOUCHLINE_LENGTH") / 2,
          max_pitch_y = Constants :: getVal("GOAL_LINE_LENGTH") / 2,
          aspect_ratio = max_pitch_x / max_pitch_y,
          padding_x = Constants :: getVal("PITCH_PADDING") * aspect_ratio,
          padding_x_out = Constants :: getVal("PITCH_OUT_PADDING") * aspect_ratio;
    
    //Setting the position of the first sub depending on the side of the pitch
    if (this->side == MatchPlayer :: pitch_half :: first)
        return vec2(-max_pitch_x - padding_x, max_pitch_y);
    //Starting at the right side of the pitch
    return vec2(max_pitch_x + padding_x_out, max_pitch_y);
}


shared_m_player FirstTeam :: getClosestPlayer(const vec2& target, const shared_m_squad& players){
    return *std :: min_element(players.begin(), players.end(), [target](const shared_m_player& p1, const shared_m_player& p2){
                               return p1->getCoords() != target && distance(p1->getCoords(), target) < distance(p2->getCoords(), target);
                              });
}


MatchPlayer :: passing_options FirstTeam :: getPassingOptions(const MatchPlayer& player, const FirstTeam& opp_team) const{
    MatchPlayer :: passing_options passing_options;
    passing_options.reserve(this->first_eleven.size() - 2);

    const shared_m_squad& opponents = opp_team.getFirstEleven();
    vec2 opp_gk_coords = opp_team.getGK().getCoords();

    //Going through all the players except the one passed as argument
    for (const auto& team_mate : this->first_eleven)
        //Ignoring the player passed as argument and the goalkeeper
        if (team_mate->getPlayer() != player.getPlayer() && team_mate->getPosition() != "GK"){
            MatchPlayer :: OpponentIntersections curr_intersections = team_mate->getMarkingIntersections(opponents);
            //Getting the distance to the opponent's goal and the pass chance
            float dist_goal = distance(team_mate->getCoords(), opp_gk_coords),
                  pass_chance = player.getPassChance(*team_mate);
            passing_options.emplace_back(team_mate, pass_chance, dist_goal, curr_intersections);
        }

    return passing_options;
}


bool FirstTeam :: operator<(const FirstTeam& other) const{
    float this_eleven_total_stats = getElevenTotalStats(),
            other_eleven_total_stats = other.getElevenTotalStats();
    
    if (this_eleven_total_stats == other_eleven_total_stats)
        return getSubsOVR() < other.getSubsOVR();
    
    return this_eleven_total_stats < other_eleven_total_stats;
}


std :: vector<shared_m_player> FirstTeam :: getUnmarkedPlayers(const FirstTeam& opp_team) const{
    std :: vector<shared_m_player> unmarked_players;
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


void FirstTeam :: resetPlayers(){
    for (const auto& p : first_eleven)
        p->setCoords(Constants :: getMPosCoords(p->getPosition()));
}


bool FirstTeam :: usePlayer(uint16_t index, glm :: vec2& ball_coords, const FirstTeam& opp_team){
    if (index >= first_eleven.size())
        throw MyRuntimeException(__FILE__, __func__, __LINE__, "Player index out of range: " + std :: to_string(index));

    shared_m_player player = first_eleven[index],
                    closest_ball = getClosestPlayer(ball_coords, this->first_eleven);

    //case1: team is not in possesion
    if (!closest_ball->hasBall()){
        shared_m_player player_with_ball = opp_team.getPlayerWithBall();
        //case 1.0: no team is in possesion so we get try to get possesion
        if (player_with_ball == nullptr)
            if (player != closest_ball) //player is not closest to the ball so he moves towards the goal
                player->p_move(opp_team.getGK().getCoords(), *getClosestPlayer(player->getCoords(),
                               this->first_eleven));
            else
                getPossesion(ball_coords, *player, *getClosestPlayer(ball_coords, opp_team.first_eleven));
            
        //case 1.1: player is the closest to the ball so he tries to tackle the opponent with the ball
        else if (player == closest_ball)
            tryTackle(ball_coords, *player, *player_with_ball);
        //case 1.2: player is not closest to the ball so he marks the closest unmarked opponent
        else
            markPlayer(*player, opp_team);
    }
        
    //case2: team is in possesion
    else if (closest_ball == player) //case 2.1 player has the ball
        return closest_ball->decide(ball_coords, closest_ball->getTacklingIntersections(opp_team.first_eleven),
                                    getPassingOptions(*closest_ball, opp_team), opp_team.getGK(),
                                    *getClosestPlayer(closest_ball->getCoords(), this->first_eleven));
    //case 2.2 player doesn't have the ball so he tries to support the player with the ball
    else
        movePlayerWithoutBall(*player, *closest_ball, opp_team);
    
    return false;
}


void FirstTeam :: tryTackle(glm :: vec2 ball_coords, MatchPlayer& tackling_player, MatchPlayer& player_with_ball){
    if(player_with_ball.inTackleRange(tackling_player))
        tackling_player.tackle(ball_coords, player_with_ball);
    else
        tackling_player.p_move(ball_coords, *getClosestPlayer(tackling_player.getCoords(), this->first_eleven));
}


void FirstTeam :: markPlayer(MatchPlayer& marking_player, const FirstTeam& opp_team){
    MatchPlayer :: OpponentIntersections player_markers = marking_player.getMarkingIntersections(opp_team.first_eleven);
    const MatchPlayer& closest_team_mate = *getClosestPlayer(marking_player.getCoords(), this->first_eleven);

    //Checking if the player is marking someone, and if that someone is only marked by him
    //If so, he moves towards that player
    if (player_markers.nr_intersections == 1 &&
        player_markers.opponent->getMarkingIntersections(this->first_eleven).nr_intersections == 1)
        marking_player.p_move(player_markers.opponent->getCoords(), closest_team_mate);

    else{ //Else he moves towards the closest unmarked player
        shared_m_squad unmarked_players = opp_team.getUnmarkedPlayers(*this);
        marking_player.p_move(getClosestPlayer(marking_player.getCoords(), unmarked_players)->getCoords(),
                              closest_team_mate);
    }
}


void FirstTeam :: movePlayerWithoutBall(MatchPlayer& player, const MatchPlayer& player_with_ball,
                                        const FirstTeam& opp_team){
    //If not within player's passing range we move him where the pass_chance is highest
    if (player_with_ball.getPassChance(player) < Constants :: getVal("PASS_CHANCE_THRESHOLD"))
        player.p_move(player_with_ball, *getClosestPlayer(player.getCoords(), this->first_eleven), opp_team.first_eleven);
    else //If within passing range we move him towards the opponent's goal
        player.p_move(opp_team.getGK().getCoords(), *getClosestPlayer(player.getCoords(), this->first_eleven));
}


void FirstTeam :: getPossesion(glm :: vec2 ball_coords, MatchPlayer& player, const MatchPlayer& opponent){
    //If the player is near the ball he tries to get possesion
    if (player.isNearBall(ball_coords) )
        player.setPossesion((!opponent.isNearBall(ball_coords) ||
                             player.getPlayer()->getStat("PAC") > opponent.getPlayer()->getStat("PAC")));
    else //Else he moves towards the ball
        player.p_move(ball_coords, *getClosestPlayer(player.getCoords(), this->first_eleven));
}