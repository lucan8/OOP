#include "MatchPlayer.h"

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
    p_shader.setUniform2f("u_player_coords", glm :: vec2(coords.x, coords.y));

    drawAura(half, p_shader, player_aura_layout, player_ibo, player_radius * 2);
    Renderer :: draw(player_vao, player_ibo, p_shader);   
}


void MatchPlayer :: drawAura(pitch_half half, Shader& p_shader,
                            const VertexBufferLayout& layout, const IBO& player_ibo, float radius) const{     
    //Getting the vertex positions for the player's aura canvas
    glm :: mat4x2 player_vert_big = this->getCanvasPositions(radius);

    //Setting and linking and VBO and VAO
    VBO player_vbo_big(&player_vert_big[0][0], sizeof(player_vert_big), GL_STATIC_DRAW);
    VAO player_vao_big;
    player_vao_big.addBuffer(player_vbo_big, layout);

    //Drawing the player's aura
    Renderer :: draw(player_vao_big, player_ibo, p_shader);
}


glm :: mat4x2 MatchPlayer :: getCanvasPositions(float radius) const{
    return glm :: mat4x2(
        coords.x - radius, coords.y - radius, 
        coords.x - radius, coords.y + radius,
        coords.x + radius, coords.y + radius, 
        coords.x + radius, coords.y - radius

    );
}


glm :: mat4 MatchPlayer :: getPlayerVertices(pitch_half half, float radius) const{
    //Getting the player's canvas vertex positions coordinates
    glm :: mat4x4 player_vertices = toMat4(getCanvasPositions(radius));
    
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


void MatchPlayer :: p_move(){
    

}


bool MatchPlayer :: operator <(const MatchPlayer& other) const{
    return this->OVR < other.OVR;
}