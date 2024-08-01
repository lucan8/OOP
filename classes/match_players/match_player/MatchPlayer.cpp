#include "MatchPlayer.h"
#include <random>
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


void MatchPlayer :: draw(pitch_half half, Shader& p_shader, const IBO& player_ibo,
                         const VertexBufferLayout& player_layout, 
                         const VertexBufferLayout& player_aura_layout) const{

    const float player_radius = Constants :: getVal("PLAYER_RADIUS");        
    //Getting the player's vertices  
    glm :: mat4 player_vert = this->getPlayerVertices(half, player_radius);

    //Setting and linking the VBO and VAO
    VBO player_vbo(&player_vert[0][0], sizeof(player_vert), GL_STATIC_DRAW);
    VAO player_vao;
    player_vao.addBuffer(player_vbo, player_layout);

    //Setting the player's coordinates
    p_shader.setUniform2f("u_entity_coords", glm :: vec2(coords.x, coords.y));

    //Setting the radius for the aura circle
    p_shader.setUniform1f("u_entity_radius", player_radius * 2);
    drawAura(half, p_shader, player_aura_layout, player_ibo, player_radius * 2);

    //Setting the radius for the player circle
    p_shader.setUniform1f("u_entity_radius", player_radius);
    Renderer :: draw(player_vao, player_ibo, p_shader);
}


void MatchPlayer :: drawAura(pitch_half half, Shader& p_shader,
                            const VertexBufferLayout& layout, const IBO& player_ibo, float radius) const{     
    //Getting the vertex positions for the player's aura canvas
    glm :: mat4x2 player_vert_big = getCanvasPositions(this->coords, radius);

    //Setting and linking and VBO and VAO
    VBO player_vbo_big(&player_vert_big[0][0], sizeof(player_vert_big), GL_STATIC_DRAW);
    VAO player_vao_big;
    player_vao_big.addBuffer(player_vbo_big, layout);

    //Drawing the player's aura
    Renderer :: draw(player_vao_big, player_ibo, p_shader);
}


glm :: mat4 MatchPlayer :: getPlayerVertices(pitch_half half, float radius) const{
    //Getting the player's canvas vertex positions coordinates
    glm :: mat4x4 player_vertices = toMat4(getCanvasPositions(this->coords, radius));
    
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
        swap(player_vertices[3][2], player_vertices[0][2]),
        swap(player_vertices[3][3], player_vertices[0][3]),
        swap(player_vertices[2][2], player_vertices[1][2]),
        swap(player_vertices[2][3], player_vertices[1][3]);
            
    
    return player_vertices;
}


void MatchPlayer :: decide(glm :: vec2& ball_coords, uint16_t nr_intersections,
                           const vector<PassingInfo>& passing_options, const shared_m_player& opponent,
                           const glm :: vec2& opp_gk_coords, const shared_m_player& closest_team_mate){
    switch (nr_intersections){
        case 0:
            this->p_move(opp_gk_coords, closest_team_mate);
            break;
        case 1:
            this->decidePassDribble(ball_coords, opponent, passing_options);
            break;
        default:
            this->pass(ball_coords, passing_options);
    }
}


void MatchPlayer :: p_move(const glm :: vec2& target, const shared_m_player& closest_team_mate){
    //Weights for the distances to the target and the team mate
    float target_dist_weight = 2, team_mate_dist_weight = 1;

    glm :: vec2 best_option_coords;
    float best_option_dist = numeric_limits<float>::max();

    //Choosing the best valid move option based on the distance to the target and the team mate                              
    for (const auto& opt_coords : this->getMoveOptions())
        if (!isOutsidePitch(opt_coords)){
            //Calculating the final distance
            float final_dist = glm :: distance(opt_coords, closest_team_mate->getCoords()) * team_mate_dist_weight
                               - glm :: distance(opt_coords, target) * target_dist_weight;

            //If the final distance is greater than this is a better option
            if (best_option_dist < final_dist){
                best_option_dist = final_dist;
                best_option_coords = opt_coords;
            }
        }
    
    this->coords = best_option_coords;
}


void MatchPlayer :: decidePassDribble(glm :: vec2& ball_coords, const shared_m_player& opponent, 
                                      const vector<PassingInfo>& passing_options){
    //Make this a function in player

    //Dribbling related stats(add weights for these)
    float p_stat_sum = this->player->getStat("PAC") + this->player->getStat("DRI") + 
                        this->player->getStat("PHY") + this->player->getStat("AGG");

    //Defending related stats(add weights for these)
    float o_stat_sum = opponent->player->getStat("PAC") + opponent->player->getStat("DEF") + 
                        opponent->player->getStat("PHY") + opponent->player->getStat("AGG");

    //if (p_stat_sum < o_stat_sum)
        this->pass(ball_coords, passing_options);
    //else
        //this->dribble();
}

float MatchPlayer :: getPassingRange() const{
    //Each pass stat is equivalent to 0.36 meters(should be part of the constants)
    const float pass_distance_multiplier = 0.36;
    return this->player->getStat("PAS") * pass_distance_multiplier;
}


float MatchPlayer :: getPassChance(float pass_distance) const{
    const float max_chance = 100;
    const float pass_secure_radius = getPassingRange();
    //With each meter past "pass_distance" the probability of a successful pass decreases by 5%
    const uint16_t meter_pass_chance_decrease = 5;

    //If within that distance chance is 100%(max_chance), else it decreases by 5% for each meter
    return min(max_chance, max_chance - (pass_distance - pass_secure_radius) * meter_pass_chance_decrease);
}


float MatchPlayer :: getFinalPassChance(const PassingInfo& pass_info) const{
    const uint16_t pass_chance_weight = 4,
                   distance_from_goal_weight = 2,
                   team_mate_OVR_weight = 1;

    return pass_info.pass_success_chance * pass_chance_weight +
           pass_info.distance_from_goal * distance_from_goal_weight +
           pass_info.team_mate->getOVR() * team_mate_OVR_weight;
}


void MatchPlayer :: pass(glm :: vec2& ball_coords, const vector<PassingInfo>& passing_options){
    PassingInfo chosen_pass_option = passing_options.at(0);
    float max_final_chance = this->getFinalPassChance(chosen_pass_option);

    //Choosing the best passing option based on the final pass chance
    for (uint16_t i = 1; i < passing_options.size(); ++i){
        const PassingInfo& pass_option = passing_options[i];
        float final_pass_chance = this->getFinalPassChance(pass_option);

        if (max_final_chance < final_pass_chance){
            max_final_chance = final_pass_chance;
            chosen_pass_option = pass_option;
        }
    }

    glm :: vec2 team_mate_coords = chosen_pass_option.team_mate->getCoords();
    //Trying to pass to the chosen team mate
    if (Constants :: generateRealNumber(0, 100) < chosen_pass_option.pass_success_chance)
        ball_coords = team_mate_coords;
    else{//If the pass is not successful, the ball is placed randomly around the team mate
        //Getting the error radius for the ball placement
        float error_radius = Constants :: getVal("PASS_ERROR_RADIUS");
        //Getting the error bounds
        float min_error_x = team_mate_coords.x - error_radius, max_error_x = team_mate_coords.x + error_radius,
              min_error_y = team_mate_coords.y - error_radius, max_error_y = team_mate_coords.y + error_radius;
        
        //Placing the ball randomly around the team mate
        ball_coords = team_mate_coords + glm :: vec2(Constants :: generateRealNumber(min_error_x, max_error_x),
                                                     Constants :: generateRealNumber(min_error_y, max_error_y));
    }
    
}


void MatchPlayer :: dribble(){
    
}


bool MatchPlayer :: intersects(const MatchPlayer& other) const{
    float dist = glm :: distance(this->coords, other.coords);
    float aura_radius = Constants :: getVal("PLAYER_RADIUS") * 2;

    //If the distance is less than the sum of the two players auras then they intersect
    return dist <= aura_radius * 2;
}


bool MatchPlayer :: isOutsidePitch(const glm :: vec2& coords){
    float max_x = Constants :: getVal("TOUCHLINE_LENGTH") / 2,
          max_y = Constants :: getVal("GOAL_LINE_LENGTH") / 2;

    return !isBetween(coords.x, -max_x, max_x) || !isBetween(coords.y, -max_y, max_y);
}


vector<glm :: vec2> MatchPlayer :: getMoveOptions() const{
    //Getting random move values
    float move_x = Constants :: generateRealNumber(0, 1), move_y = Constants :: generateRealNumber(0, 1);
    //Getting all possible new coordinates
    return {
            (this->coords + glm :: vec2(move_x, move_y)),
            (this->coords + glm :: vec2(move_x, -move_y)),
            (this->coords + glm :: vec2(-move_x, move_y)),
            (this->coords + glm :: vec2(-move_x, -move_y))};
}


bool MatchPlayer :: operator <(const MatchPlayer& other) const{
    return this->OVR < other.OVR;
}

