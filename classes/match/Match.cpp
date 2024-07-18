#include "Match.h"
#include "../textures/Textures.h"

void Match :: draw(){
    //Setting the shaders paths
    const string vert_shader_path = (filesystem::current_path().parent_path() / "resources" / "shaders" / "vertex" / "").string(),
                 frag_shader_path = (filesystem::current_path().parent_path() / "resources" / "shaders" / "fragment" / "").string();

    //Loading the shaders
    Shader pitch_shader((vert_shader_path + "pitch.glsl"), (frag_shader_path + "pitch.glsl")),
           player_shader((vert_shader_path + "player.glsl"), (frag_shader_path + "player.glsl"));
    
    Textures pitch_texture((filesystem::current_path().parent_path() / "resources" / "textures" / "pitch.jpg").string()),
             team1_texture((filesystem::current_path().parent_path() / "resources" / "textures" / "arsenal.png").string()),
             team2_texture((filesystem::current_path().parent_path() / "resources" / "textures" / "real_madrid.png").string());

    //Binding the textures
    pitch_texture.bind(0);
    team1_texture.bind(1);
    team2_texture.bind(2);

    //Using the player shader for both
    this->drawField(player_shader);
    this->drawPlayers(player_shader);
}

void Match :: drawPlayers(Shader& player_shader){
    player_shader.bind();

    //Layout for player's triangles(2 coords for position, 2 coords for texture)
    VertexBufferLayout player_layout;
    player_layout.addAttribute<float>(2);
    player_layout.addAttribute<float>(2);

    //Retrieving position of color uniform
    GLint u_location = player_shader.getUniformLocation("u_Texture");
    
    //Drawing the first team's players
    glUniform1i(u_location, 1);
    t1->drawPlayers(MatchPlayer :: pitch_half :: first, player_shader, player_layout);

    //Drawing the second team's players
    glUniform1i(u_location, 2);
    t2->drawPlayers(MatchPlayer :: pitch_half :: second, player_shader, player_layout);
}


void Match :: drawField(Shader& pitch_shader){
    pitch_shader.bind();
    uint16_t pitch_vert_count = Constants :: getVal("SQUARE_VERTICES") * Constants :: getVal("NR_COORDS_VERTEX")
                                * Constants :: getVal("NR_COORDS_TEXTURE");
    //Already bound on creation
    VBO pitch_vbo(Constants :: getVertexPositions("PITCH"), pitch_vert_count * sizeof(float), GL_STATIC_DRAW);

    //Retrieving position of color uniform
    GLint u_location = pitch_shader.getUniformLocation("u_Texture");
    //Setting color of pitch and drawing it
    glUniform1i(u_location, 0);

    //Layout for pitch
    VertexBufferLayout pitch_layout;
    pitch_layout.addAttribute<float>(2);
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