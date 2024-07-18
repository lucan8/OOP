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


void MatchPlayer :: drawTriangle(pitch_half half, const Shader& p_shader,
                                 const VertexBufferLayout& player_layout) const{
    //Getting the triangle vertices
    uint16_t player_vert_count = Constants :: getVal("SQUARE_VERTICES") * Constants :: getVal("NR_COORDS_VERTEX") 
                                 * Constants :: getVal("NR_COORDS_TEXTURE");
                                 
    unique_ptr<float> player_vert = this->getTrianglePositions(half);

    //Creating the VBO and VAO and binding them
    VBO player_vbo(player_vert.get(), player_vert_count * sizeof(float), GL_STATIC_DRAW);
    VAO player_vao;
    player_vao.addBuffer(player_vbo, player_layout);

    //Player vert indices for IBO
    GLuint player_indices[6] = {
        0, 1, 2,
        1, 2, 3
    };

    //Creating the IBO and drawing the player
    IBO player_ibo(player_indices, 6);
    Renderer :: draw(player_vao, player_ibo, p_shader);
    
}
unique_ptr<float> MatchPlayer :: getTrianglePositions(pitch_half half) const{
    const float triangle_offset = 0.05f;
    //Added set of coords represents the texture coords

    //triangle if the player is in second half(grows upwards)
    if (half == pitch_half :: second)
        return unique_ptr<float>(new float[16]{
            coords.x, coords.y, 0.5f, 0.0f,
            coords.x - triangle_offset, coords.y - triangle_offset , 0.0f, 0.5f,
            coords.x + triangle_offset, coords.y - triangle_offset, 1.0f, 0.5f,
            coords.x, coords.y - triangle_offset * 2, 0.5f, 1.0f

        });
    //triangle if the player is in first half(grows downwards)
    return unique_ptr<float>(new float[16]{
            coords.x, coords.y, 0.5f, 0.0f,
            coords.x - triangle_offset, coords.y + triangle_offset , 0.0f, 0.5f,
            coords.x + triangle_offset, coords.y + triangle_offset, 1.0f, 0.5f,
            coords.x, coords.y + triangle_offset * 2, 0.5f, 1.0f
    });
}


bool MatchPlayer :: operator <(const MatchPlayer& other) const{
    return this->OVR < other.OVR;
}