#include "MatchPlayer.h"
#include "../../exceptions/MyRuntimeException.h"
#include "../../constants/Constants.h"
#include "../../../functions/functions.h"
#include "../../renderer/Renderer.h"
#include <random>
#include <algorithm>
#include <cstring>
using glm :: vec2, glm :: mat4, glm :: mat4x2;
using std :: make_pair, std :: to_string, std :: vector;

void MatchPlayer :: addYellowCard(){
    ++yellow_cards;
    player->addYellowCard();

    if(yellow_cards == 2){
        player->changeRedCarded();
        player->addRedCard();
    }
}


void MatchPlayer :: changeSide(){
    this->coords = -this->coords;
}


void MatchPlayer :: changeSide1(){
    float temp = this->coords.x;
    this->coords.x = this->coords.y;
    this->coords.y = -temp;
}


void MatchPlayer :: drawPlaying(pitch_half half, Shader& p_shader, const IBO& player_ibo,
                                   const VertexBufferLayout& player_layout, 
                                   const VertexBufferLayout& player_aura_layout) const{
    //Getting the player's radius in pitch units                    
    float player_radius = Constants :: getVal("PLAYER_RADIUS");        
    //Getting the player's vertices  
    glm :: mat4 player_vert = this->getPlayerVertices(half, player_radius);

    //Setting and linking the VBO and VAO
    VBO player_vbo(sizeof(player_vert), &player_vert[0][0], GL_STATIC_DRAW);
    VAO player_vao;
    player_vao.addBuffer(player_vbo, player_layout);

    //Setting the player's coordinates in pixel units
    p_shader.setUniform2f("u_entity_coords", convertCoords(this->coords,
                                                           Constants :: getPitchProj(),
                                                           Constants :: getPixelFragProj()));

    //Setting the radius for the aura circle in pixel units
    p_shader.setUniform1f("u_entity_radius", changeUnit(player_radius) * 2);
    drawAura(half, p_shader, player_aura_layout, player_ibo, player_radius * 2);

    //Setting the radius for the player's circle in pixel units
    p_shader.setUniform1f("u_entity_radius", changeUnit(player_radius));
    Renderer :: draw(player_vao, player_ibo, p_shader);
}


void MatchPlayer :: drawAura(pitch_half half, Shader& p_shader,
                            const VertexBufferLayout& layout, const IBO& player_ibo, float radius) const{     
    //Getting the vertex positions for the player's aura canvas
    mat4x2 player_vert_big = getCanvasPositions(this->coords, radius);

    //Setting and linking and VBO and VAO
    VBO player_vbo_big(sizeof(player_vert_big), &player_vert_big[0][0], GL_STATIC_DRAW);
    VAO player_vao_big;
    player_vao_big.addBuffer(player_vbo_big, layout);

    //Drawing the player's aura
    Renderer :: draw(player_vao_big, player_ibo, p_shader);
}


void MatchPlayer :: drawUnplaying(Shader& shader, const IBO& ibo, const Font& font, glm :: vec2 s_text_pos,
                                  pitch_half side) const{
    float font_scale = 0.6;
    for (const auto& info : this->getUnplayingInfo(side, font)){
        Renderer :: drawText(shader, ibo, info.first, s_text_pos, font, font_scale);
        s_text_pos.x += font_scale * info.second;
    }
}


std :: array<std :: pair<std :: string, uint16_t>, 4> MatchPlayer :: getUnplayingInfo(pitch_half side, const Font& font) const{
    //The order of the info is different depending on the side
    if (side == pitch_half :: second)
        return {make_pair(to_string((int)this->OVR), 3 * font.getMaxDigitWidth()),
                make_pair(toUpper(player->getSurname()), 10 * font.getMaxLetterWidth()),
                make_pair(player->getPosition(), 8 * font.getMaxLetterWidth()),
                make_pair(to_string(player->getShirt()), 3 * font.getMaxDigitWidth())};
    
    return {make_pair(to_string(player->getShirt()), 3 * font.getMaxDigitWidth()),
            make_pair(player->getPosition(), 8 * font.getMaxLetterWidth()), 
            make_pair(toUpper(player->getSurname()), 10 * font.getMaxLetterWidth()),
            make_pair(to_string((int)this->OVR), 3 * font.getMaxDigitWidth())};
}


glm :: mat4 MatchPlayer :: getPlayerVertices(pitch_half half, float radius) const{
    //Getting the player's canvas vertex positions coordinates
    mat4 player_vertices = toMat4(getCanvasPositions(this->coords, radius));
    
    //Getting the texture coordinates
    float max_x = Constants :: getVal("TEXTURE_MAX_X"), max_y = Constants :: getVal("TEXTURE_MAX_Y");
    float min_x = Constants :: getVal("TEXTURE_MIN_X"), min_y = Constants :: getVal("TEXTURE_MIN_Y");

    //Setting the texture coordinates depending on the half
    //Default for the second half
    player_vertices[0][2] =  max_x, player_vertices[0][3] = max_y,
    player_vertices[1][2] =  min_x, player_vertices[1][3]  = max_y,
    player_vertices[2][2] = min_x, player_vertices[2][3] = min_y,
    player_vertices[3][2] = max_x, player_vertices[3][3] = min_y;

    //For the first half we just swap the texture coordinates
    if (half == pitch_half :: first)
        std ::swap(player_vertices[3][2], player_vertices[0][2]),
        std ::swap(player_vertices[3][3], player_vertices[0][3]),
        std ::swap(player_vertices[2][2], player_vertices[1][2]),
        std ::swap(player_vertices[2][3], player_vertices[1][3]);


    return player_vertices;
}


bool MatchPlayer :: decide(vec2& ball_coords, const OpponentIntersections& opp_intersections,
                           const passing_options& pass_options, MatchPlayer& opp_gk,
                           const MatchPlayer& closest_team_mate){
    //Getting the scoring chance for this player and the team mate who is the best passing option
    PassingInfo best_passing_option = *std :: max_element(pass_options.begin(), pass_options.end(),
                                                          [this](const PassingInfo& p1, const PassingInfo& p2){
                                                            return this->getFinalPassChance(p1) < this->getFinalPassChance(p2);
                                                          });
    float scoring_chance = this->getScoringChance(opp_gk),
          team_mate_scoring_chance = best_passing_option.team_mate->getScoringChance(opp_gk);
    
    //If the scoring chance is greater than the team mate's scoring chance, the player will shoot
    if (scoring_chance > team_mate_scoring_chance)
        return this->shoot(ball_coords, opp_gk);

    switch (opp_intersections.nr_intersections){
        case 0:
            this->p_move(opp_gk.getCoords(), closest_team_mate);
            ball_coords = this->coords;
            break;
        case 1:
            this->decidePassDribble(ball_coords, *opp_intersections.opponent, best_passing_option, opp_gk.getCoords());
            break;
        default:
            this->pass(ball_coords, best_passing_option);
    }
    return false;
}


void MatchPlayer :: p_move(const vec2& target, const MatchPlayer& closest_team_mate){
    //Weights for the distances to the target and the team mate
    float target_dist_weight = 2, team_mate_dist_weight = 1;

    //Default option, the player stays in place
    vec2 best_option_coords = this->coords;
    float best_option_dist = glm :: distance(best_option_coords, target) * target_dist_weight - 
                             glm :: distance(best_option_coords, closest_team_mate.getCoords()) * team_mate_dist_weight;

    //Choosing the best valid move option based on the distance to the target and the team mate                              
    for (const auto& opt_coords : this->getMoveOptions())
        if (!isOutsidePitch(opt_coords)){
            //Calculating the final distance
            float final_dist = glm :: distance(opt_coords, target) * target_dist_weight - 
                               glm :: distance(opt_coords, closest_team_mate.getCoords()) * team_mate_dist_weight;

            //If the final distance is greater than this is a better option
            if (final_dist < best_option_dist){
                best_option_dist = final_dist;
                best_option_coords = opt_coords;
            }
        }
    
    this->coords = best_option_coords;
}


void MatchPlayer :: p_move(const MatchPlayer& player_with_ball, const MatchPlayer& closest_team_mate,
                           const shared_m_squad& opponents){
    //Weights for the distances to the target and the team mate
    float pass_chance_weight = 2, team_mate_dist_weight = 1;

    //Default option, the player stays in place
    vec2 best_option_coords = this->coords;
    float best_option_dist = player_with_ball.getPassChance(*this) * pass_chance_weight - 
                             glm :: distance(best_option_coords, closest_team_mate.coords) * team_mate_dist_weight;

    //Choosing the best valid move option based on the passing chance and the distance to the team mate                              
    for (const auto& opt_coords : this->getMoveOptions())
        if (!isOutsidePitch(opt_coords)){
            this->coords = opt_coords;

            float pass_chance = player_with_ball.getPassChance(*this),
                  team_mate_dist = glm :: distance(opt_coords, closest_team_mate.coords),
                  final_dist = pass_chance * pass_chance_weight - team_mate_dist * team_mate_dist_weight;

            //If the final distance is greater than this is a better option
            if (final_dist < best_option_dist){
                best_option_dist = final_dist;
                best_option_coords = opt_coords;
            }
        }
    
    this->coords = best_option_coords;
}


void MatchPlayer :: moveTowards(const vec2& target, const vec2& move_values){   
    if (this->coords.x < target.x)
        this->coords.x += move_values.x;
    else
        this->coords.x -= move_values.x;
    
    if (this->coords.y < target.y)
        this->coords.y += move_values.y;
    else
        this->coords.y -= move_values.y;
}


void MatchPlayer :: decidePassDribble(vec2& ball_coords, const MatchPlayer& opponent, 
                                      const PassingInfo& best_pass_option,const vec2& opp_gk_coords){
    float dribble_chance = 100 - this->getTackleChance(opponent);
    
    if (dribble_chance < best_pass_option.pass_success_chance)
        this->pass(ball_coords, best_pass_option);
    else 
        this->dribble(ball_coords, opponent, opp_gk_coords, dribble_chance);

}


void MatchPlayer :: pass(vec2& ball_coords, const PassingInfo& chosen_pass_option){
    vec2 team_mate_coords = chosen_pass_option.team_mate->getCoords();
    //Trying to pass to the chosen team mate
    if (Constants :: generateRealNumber(0, 100) < chosen_pass_option.pass_success_chance){
        ball_coords = team_mate_coords;
        chosen_pass_option.team_mate->setPossesion(true);
    }
    else{//If the pass is not successful, the ball is placed randomly around the team mate
        //Getting the error radius for the ball placement
        float error_radius = Constants :: getVal("PASS_ERROR_RADIUS");
        //Getting the error bounds
        float min_error_x = team_mate_coords.x - error_radius, max_error_x = team_mate_coords.x + error_radius,
              min_error_y = team_mate_coords.y - error_radius, max_error_y = team_mate_coords.y + error_radius;
        
        //Placing the ball randomly around the team mate
        ball_coords = team_mate_coords + vec2(Constants :: generateRealNumber(min_error_x, max_error_x),
                                                     Constants :: generateRealNumber(min_error_y, max_error_y));
    }
    //The player no longer has the ball
    this->has_ball = false;
    
}


float MatchPlayer :: getPassingRange() const{
    //Coordinates of the top right corner of the pitch
    vec2 max_pitch_coords(Constants :: getVal("TOUCHLINE_LENGTH") / 2,
                                 Constants :: getVal("GOAL_LINE_LENGTH") / 2);
    //Max distance betweeen to players                             
    float max_distance = glm :: distance(-max_pitch_coords, max_pitch_coords);

    //Nerfing the pass distance multiplier by deviding it by 2
    const float pass_distance_multiplier = max_distance / Constants :: getVal("MAX_STATS") / 2;
    return this->player->getStat("PAS") * pass_distance_multiplier;
}


float MatchPlayer :: getPassChance(const MatchPlayer& team_mate) const{
    float pass_distance = glm :: distance(this->coords, team_mate.getCoords());

    const float max_chance = 100;
    const float pass_secure_radius = getPassingRange();
    //With each meter past "pass_distance" the probability of a successful pass decreases by 5%
    const uint16_t meter_pass_chance_decrease = 5;

    //If within that distance chance is 100%(max_chance), else it decreases by 5% for each meter
    return std :: min(max_chance, max_chance - (pass_distance - pass_secure_radius) * meter_pass_chance_decrease);
}


float MatchPlayer :: getFinalPassChance(const PassingInfo& pass_info) const{
    if (pass_info.opp_intersections.nr_intersections == 2)
        return 0;

    float tackle_chance = 0;

    if (pass_info.opp_intersections.nr_intersections == 1)
        tackle_chance = this->getTackleChance(*pass_info.opp_intersections.opponent);
        
    const uint16_t pass_chance_weight = 3,
                   tackle_chance_weight = 2,
                   distance_from_goal_weight = 2,
                   team_mate_OVR_weight = 1;

    return pass_info.pass_success_chance * pass_chance_weight -
           tackle_chance * tackle_chance_weight -
           pass_info.distance_from_goal * distance_from_goal_weight +
           pass_info.team_mate->getPlayer()->getAttackingOVR() * team_mate_OVR_weight;
}


void MatchPlayer :: dribble(vec2& ball_coords, const MatchPlayer& opponent, const vec2& opp_gk_coords,
                            float dribble_chance){
    //Moving the player towards the opponent's goal and away from the opponent
    if (Constants :: generateRealNumber(0, 100) < dribble_chance)
        this->moveTowards(opp_gk_coords + vec2(0, -opponent.coords.y),
                          vec2(Constants :: generateRealNumber(2, 3), Constants :: generateRealNumber(2, 3)));
    else //Moving the player towards the opponent because the dribble was unsuccessful
        this->moveTowards(opponent.coords,
                         vec2(Constants :: generateRealNumber(0.1, 1), Constants :: generateRealNumber(0.1, 1)));
    //Moving the ball with the player                     
    ball_coords = this->coords;
}


void MatchPlayer :: tackle(vec2& ball_coords, MatchPlayer& opponent){
    float tackle_chance = this->getTackleChance(opponent);

    if (Constants :: generateRealNumber(0, 100) < tackle_chance){
        opponent.has_ball = false;
        this->has_ball = true;
        ball_coords = this->coords;
    }
}


bool MatchPlayer :: shoot(vec2& ball_coords, MatchPlayer& opp_gk){
    float scoring_chance = this->getScoringChance(opp_gk);

    if (Constants :: generateRealNumber(0, 100) < scoring_chance){
        ball_coords = opp_gk.getCoords();
        opp_gk.has_ball = true;
        return false;
    }
    this->has_ball = false;
    return true;
}


float MatchPlayer :: getScoringChance(const MatchPlayer& opp_gk) const{
    float gk_avg = (opp_gk.getPlayer()->getStat("REF") + opp_gk.getPlayer()->getStat("DIV") + 
                    opp_gk.getPlayer()->getStat("POS")) / 3;
    float player_avg = this->player->getStat("SHO");
    
    float dist = glm :: distance(this->coords, opp_gk.getCoords()),
          half_to_goal_dist = Constants :: getVal("TOUCHLINE_LENGTH") / 2,
          max_sho = Constants :: getVal("MAX_STATS"),
          chance = 100 - dist * (max_sho / half_to_goal_dist) + player_avg - gk_avg;
    return chance;
}


bool MatchPlayer :: intersects(const vec2& target, radius_type r_type) const{
    float dist = glm :: distance(this->coords, target);
    switch (r_type){
        case radius_type :: player:
            return dist <= Constants :: getVal("PLAYER_RADIUS") * 2;
        case radius_type :: aura://The aura's radius is 2 times the player's
            return dist <= Constants :: getVal("PLAYER_RADIUS") * 4;
        case radius_type :: ball: //The ball's radius is 1/2 of the player's
            return dist <= Constants :: getVal("PLAYER_RADIUS") * 1.5;
        default:
            throw MyRuntimeException(__FILE__, __func__, __LINE__, "Invalid radius type");
    }
}


bool MatchPlayer :: isOutsidePitch(const vec2& coords){
    float max_x = Constants :: getVal("TOUCHLINE_LENGTH") / 2,
          max_y = Constants :: getVal("GOAL_LINE_LENGTH") / 2;

    return !isBetween(coords.x, -max_x, max_x) || !isBetween(coords.y, -max_y, max_y);
}


std :: vector<vec2> MatchPlayer :: getMoveOptions() const{
    //Getting random move values
    float move_x = Constants :: generateRealNumber(0.01, 1), move_y = Constants :: generateRealNumber(0.01, 1);
    //Getting all possible new coordinates
    return {
            (this->coords + vec2(move_x, 0)),
            (this->coords + vec2(-move_x, 0)),
            (this->coords + vec2(0, move_y)),
            (this->coords + vec2(0, -move_y)),
            (this->coords + vec2(move_x, move_y)),
            (this->coords + vec2(move_x, -move_y)),
            (this->coords + vec2(-move_x, move_y)),
            (this->coords + vec2(-move_x, -move_y)),
            };
}

/*
vec2 MatchPlayer :: getPitchMatrixcoords() const{
    //Getting the player's coordinates relative to pitch width and height
    vec2 temp = this->coords + vec2(Constants :: getVal("TOUCHLINE_LENGTH") / 2,
                                      Constants :: getVal("GOAL_LINE_LENGTH") / 2);
    
    //Pitch matrix coordinates will be reversed
    return vec2(temp.y, temp.x);
}
*/


bool MatchPlayer :: operator <(const MatchPlayer& other) const{
    return this->OVR < other.OVR;
}


bool MatchPlayer :: inTackleRange(const MatchPlayer& opponent) const{
    return this->intersects(opponent.coords, radius_type :: player);
}


bool MatchPlayer :: isNearBall(const vec2& ball_coords) const{
    return this->intersects(ball_coords, radius_type :: ball);
}


bool MatchPlayer :: verifDetPType(const std :: string& det_p_type) const{
    const std :: vector<std :: string>& positions = Constants :: getPositions(det_p_type);
    return std :: find(positions.begin(), positions.end(), this->position) != positions.end();
}


bool MatchPlayer :: isPassedMidline(pitch_half half) const{
    if (half == pitch_half :: first)
        return this->coords.x > 0;
    return this->coords.x < 0;
}


MatchPlayer :: OpponentIntersections MatchPlayer :: getMarkingIntersections(const shared_m_squad& opponents) const{
    return getOpponentIntersections(opponents, radius_type :: aura);
}


MatchPlayer :: OpponentIntersections MatchPlayer :: getTacklingIntersections(const shared_m_squad& opponents) const{
    return getOpponentIntersections(opponents, radius_type :: player);
}


MatchPlayer :: OpponentIntersections MatchPlayer :: getOpponentIntersections(const shared_m_squad& opponents, 
                                                                             radius_type r_type) const{
    //Will be needed in case there is only one intersection
    shared_m_player opponent;
    uint16_t nr_intersections = 0;

    //We go through the team's first eleven and check if the player intersects with any of them
    for (const auto& opp : opponents){
        if (this->intersects(opp->coords, r_type)){
            ++nr_intersections;
            opponent = opp;
        }

        //Passed two intersections the player should pass the ball so the opponent becomes nullptr
        if (nr_intersections == 2)
            return {nr_intersections, nullptr};
    }
    
    return {nr_intersections, opponent};
}


float MatchPlayer :: getTackleChance(const MatchPlayer& opponent) const{
    float player_stat_sum = this->player->getStat("DEF") + this->player->getStat("PHY") + 
                            this->player->getStat("AGG") + this->player->getStat("PAC");

    float opponent_stat_sum = opponent.player->getStat("DRI") + opponent.player->getStat("PHY") + 
                              opponent.player->getStat("AGG") + opponent.player->getStat("PAC");

    return (player_stat_sum / (player_stat_sum + opponent_stat_sum)) * 100;
}