#include "Match.h"


void Match :: draw(){
    //Loading the shaders
    const string shader_path = (filesystem::current_path().parent_path() / "resources" / "shaders" / "").string();
    Shader shader((shader_path + "vertex_shader.glsl"), (shader_path + "fragment_shader.glsl"));
    
    this->drawField(shader);
    this->drawPlayers(shader);
}

void Match :: drawPlayers(Shader& player_shader){
    player_shader.bind();
    //Retrieving position of color uniform
    GLint u_location = player_shader.getUniformLocation("u_Color");

    //Layout for player's triangles
    VertexBufferLayout player_layout;
    player_layout.addAttribute<float>(2);

    //Red color for the first team
    glUniform4f(u_location, 1.0f, 0.0f, 0.0f, 1.0f);
    t1->drawPlayers(MatchPlayer :: pitch_half :: first, player_shader, player_layout);

    //Blue color for the second team
    glUniform4f(u_location, 0.0f, 0.0f, 1.0f, 1.0f);
    t2->drawPlayers(MatchPlayer :: pitch_half :: second, player_shader, player_layout);
}


void Match :: drawField(Shader& pitch_shader){
    pitch_shader.bind();
    uint16_t pitch_vert_count = Constants :: getVal("PITCH_VERTICES") * Constants :: getVal("NR_COORDS_VERTEX");
    //Already bound on creation
    VBO pitch_vbo(Constants :: getVertexPositions("PITCH"), pitch_vert_count * sizeof(float), GL_STATIC_DRAW);

    //Retrieving position of color uniform
    GLint u_location = pitch_shader.getUniformLocation("u_Color");
    //Setting color of pitch and drawing it
    glUniform4f(u_location, 0.2f, 1.0f, 0.1f, 1.0f);

    //Layout for pitch
    VertexBufferLayout pitch_layout;
    pitch_layout.addAttribute<float>(2);

    //Binding the pitch VAO and VBO
    VAO pitch_vao;
    pitch_vao.addBuffer(pitch_vbo, pitch_layout);

    GLuint pitch_indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    IBO pitch_ibo(pitch_indices, 6);

    Renderer :: draw(pitch_vao, pitch_ibo, pitch_shader);
}