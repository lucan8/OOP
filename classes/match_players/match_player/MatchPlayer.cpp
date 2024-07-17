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
    uint16_t player_vert_count = Constants :: getVal("TRIANGLE_VERTICES") * Constants :: getVal("NR_COORDS_VERTEX");
    unique_ptr<float> player_vert = this->getTrianglePositions(half);

    //Creating the VBO and VAO and binding them
    VBO player_vbo(player_vert.get(), player_vert_count * sizeof(float), GL_STATIC_DRAW);
    VAO player_vao;
    player_vao.addBuffer(player_vbo, player_layout);

    //Player vert indices for IBO
    GLuint player_indices[3] = {
        0, 1, 2,
    };

    //Creating the IBO and drawing the player
    IBO player_ibo(player_indices, 3);
    Renderer :: draw(player_vao, player_ibo, p_shader);
    
}
unique_ptr<float> MatchPlayer :: getTrianglePositions(pitch_half half) const{
    const float triangle_offset = 0.02f;
    //triangle if the player is in second half(grows upwards)
    if (half == pitch_half :: second)
        return unique_ptr<float>(new float[6]{
            coords.x, coords.y - triangle_offset,
            coords.x - triangle_offset, coords.y,
            coords.x + triangle_offset, coords.y
        });
    //triangle if the player is in first half(grows downwards)
    return unique_ptr<float>(new float[6]{
        coords.x, coords.y + triangle_offset,
        coords.x - triangle_offset, coords.y,
        coords.x + triangle_offset, coords.y
    });
}


bool MatchPlayer :: operator <(const MatchPlayer& other) const{
    return this->OVR < other.OVR;
}