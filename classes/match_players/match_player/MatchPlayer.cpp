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

    float player_radius = Constants :: getVal("PLAYER_RADIUS");        
    //Getting the player's vertices  
    glm :: mat4 player_vert = this->getPlayerVertices(half, player_radius);

    //Setting and linking the VBO and VAO
    VBO player_vbo(&player_vert[0][0], sizeof(player_vert), GL_STATIC_DRAW);
    VAO player_vao;
    player_vao.addBuffer(player_vbo, player_layout);

    //Setting the player's coordinates
    p_shader.setUniform2f("u_entity_coords", glm :: vec2(coords.x, coords.y));

    //Setting the radius for the aura circle
    p_shader.setUniform1f("u_entity_radius", Constants :: getVal("PLAYER_RADIUS") * 2);
    drawAura(half, p_shader, player_aura_layout, player_ibo, player_radius * 2);

    //Setting the radius for the player circle
    p_shader.setUniform1f("u_entity_radius", Constants :: getVal("PLAYER_RADIUS"));
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


void MatchPlayer :: decide(Coordinates& ball_coords, uint16_t nr_intersections,
                           const vector<PassingInfo>& passing_options, const shared_m_player& opponent,
                           const Coordinates& opp_gk_coords){
    switch (nr_intersections){
        case 0:
            this->moveTowards(ball_coords, opp_gk_coords);
            break;
        case 1:
            this->decidePassDribble(ball_coords, opponent, passing_options);
            break;
        default:
            this->pass(ball_coords, passing_options);
    }
}


void MatchPlayer :: moveTowards(Coordinates& ball_coords , const Coordinates& target){
    moveTowards(target);
    ball_coords = this->coords;
}


void MatchPlayer :: moveTowards(const Coordinates& target){
    if (this->coords.x < target.x)
        this->coords.x += 1;
    else if (this->coords.x > target.x)
        this->coords.x -= 1;
    
    if (this->coords.y < target.y)
        this->coords.y += 1;
    else if (this->coords.y > target.y)
        this->coords.y -= 1;
}


void MatchPlayer :: decidePassDribble(Coordinates& ball_coords, const shared_m_player& opponent, 
                                      const vector<PassingInfo>& passing_options){
    //Make this a function in player

    //Dribbling related stats(add weights for these)
    double p_stat_sum = this->player->getStat("PAC") + this->player->getStat("DRI") + 
                        this->player->getStat("PHY") + this->player->getStat("AGG");

    //Defending related stats(add weights for these)
    double o_stat_sum = opponent->player->getStat("PAC") + opponent->player->getStat("DEF") + 
                        opponent->player->getStat("PHY") + opponent->player->getStat("AGG");

    if (p_stat_sum < o_stat_sum)
        this->pass(ball_coords, passing_options);
    else
        this->dribble();
}

double MatchPlayer :: getPassingRange() const{
    //Each pass stat is equivalent to 0.36 meters(should be part of the constants)
    const double pass_distance_multiplier = 0.36;
    return this->player->getStat("PAS") * pass_distance_multiplier;
}


double MatchPlayer :: getPassChance(double pass_distance) const{
    const double max_chance = 100;
    const double pass_secure_radius = getPassingRange();
    //With each meter past "pass_distance" the probability of a successful pass decreases by 5%
    const uint16_t meter_pass_chance_decrease = 5;

    //If within that distance chance is 100%(max_chance), else it decreases by 5% for each meter
    return min(max_chance, max_chance - (pass_distance - pass_secure_radius) * meter_pass_chance_decrease);
}


double MatchPlayer :: getFinalPassChance(const PassingInfo& pass_info) const{
    uint16_t pass_chance_weight = 4,
             distance_from_goal_weight = 2,
             team_mate_OVR_weight = 1;

    return pass_info.pass_success_chance * pass_chance_weight +
           pass_info.distance_from_goal * distance_from_goal_weight +
           pass_info.team_mate->getOVR() * team_mate_OVR_weight;
}


void MatchPlayer :: pass(Coordinates& ball_coords, const vector<PassingInfo>& passing_options){
    PassingInfo chosen_pass_option = passing_options.at(0);
    double max_final_chance = this->getFinalPassChance(chosen_pass_option);

    //Choosing the best passing option based on the final pass chance
    for (uint16_t i = 1; i < passing_options.size(); ++i){
        const PassingInfo& pass_option = passing_options[i];
        double final_pass_chance = this->getFinalPassChance(pass_option);

        if (max_final_chance < final_pass_chance){
            max_final_chance = final_pass_chance;
            chosen_pass_option = pass_option;
        }
    }

    //Random number generator for the pass success
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0, 100);

    //Trying to pass to the chosen team mate
    if (dist(gen) < chosen_pass_option.pass_success_chance)
        ball_coords = chosen_pass_option.team_mate->getCoords();
    else{
        //uniform_real_distribution<> dist(chosen_pass_option.pass_success_chance, 100);
        ball_coords = chosen_pass_option.team_mate->getCoords();
    }
    
}


void MatchPlayer :: dribble(){
    
}
bool MatchPlayer :: intersects(const MatchPlayer& other) const{
    float dist = distance(this->coords, other.coords);
    float aura_radius = Constants :: getVal("PLAYER_RADIUS") * 2;

    //If the distance is less than the sum of the two players auras then they intersect
    return dist <= aura_radius * 2;
}


bool MatchPlayer :: operator <(const MatchPlayer& other) const{
    return this->OVR < other.OVR;
}