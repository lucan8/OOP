#include "Match.h"
#include "../textures/Textures.h"

void Match :: draw(){
    //Setting the shaders paths
    const string vert_shader_path = (filesystem::current_path().parent_path() / "resources" / "shaders" / "vertex" / "").string(),
                 frag_shader_path = (filesystem::current_path().parent_path() / "resources" / "shaders" / "fragment" / "").string();

    //Loading the shaders
    Shader pitch_shader((vert_shader_path + "player.glsl"), (frag_shader_path + "pitch.glsl")),
           player_shader((vert_shader_path + "player.glsl"), (frag_shader_path + "player.glsl"));
    
    Textures pitch_texture((filesystem::current_path().parent_path() / "resources" / "textures" / "pitch2.png").string()),
             team1_texture((filesystem::current_path().parent_path() / "resources" / "textures" / "arsenal.png").string()),
             team2_texture((filesystem::current_path().parent_path() / "resources" / "textures" / "real_madrid.png").string());

    //Binding the textures
    pitch_texture.bind(0);
    team1_texture.bind(1);
    team2_texture.bind(2);

    //Setting the projection matrix
    float pitch_max_x = Constants :: getVal("TOUCHLINE_LENGTH") / 2 + Constants :: getVal("PITCH_OUT_PADDING"),
          pitch_max_y = Constants :: getVal("GOAL_LINE_LENGTH") / 2 + Constants :: getVal("PITCH_OUT_PADDING");

    glm :: mat4 proj = glm :: ortho(-pitch_max_x , pitch_max_x ,
                                    -pitch_max_y , pitch_max_y, -1.0f, 1.0f);

    this->drawField(pitch_shader, proj);
    this->drawPlayers(player_shader, proj);
}

void Match :: drawPlayers(Shader& player_shader, const glm :: mat4& proj){
    float pitch_padding = Constants :: getVal("PITCH_OUT_PADDING");

    const glm :: vec2 halfed_window_res(Constants :: getVal("WINDOW_WIDTH") / 2,
                                        Constants :: getVal("WINDOW_HEIGHT") / 2),
                      halfed_pitch_res(Constants :: getVal("TOUCHLINE_LENGTH") / 2 + pitch_padding,
                                       Constants :: getVal("GOAL_LINE_LENGTH") / 2 + pitch_padding),
                      res_units(halfed_window_res.x / halfed_pitch_res.x,
                                halfed_window_res.y / halfed_pitch_res.y);

    //Binding the shader and setting the projection matrix
    player_shader.bind();
    player_shader.setUniformMat4f("u_MVP", proj);

    //Setting the unfiforms for determining the player's coordinates according to the window resolution
    //The player's coord uniform will be set at the player's draw function
    player_shader.setUniform2f("u_units", res_units);
    player_shader.setUniform2f("u_center", halfed_window_res);
    player_shader.setUniform1f("u_player_radius", res_units.x * Constants :: getVal("PLAYER_RADIUS"));

    //Layout for player's triangles(2 coords for position, 2 coords for texture)
    VertexBufferLayout player_layout;
    player_layout.addAttribute<float>(2);
    player_layout.addAttribute<float>(2);

    player_shader.setUniform1i("u_Texture", 1);
    t1->drawPlayers(MatchPlayer :: pitch_half :: first, player_shader, player_layout);

    //Drawing the second team's players
    player_shader.setUniform1i("u_Texture", 2);
    t2->drawPlayers(MatchPlayer :: pitch_half :: second, player_shader, player_layout);
}


void Match :: drawField(Shader& pitch_shader, const glm :: mat4& proj){
    //Binding the shader and setting the projection matrix
    pitch_shader.bind();
    pitch_shader.setUniformMat4f("u_MVP", proj);
    
    //Getting the number of values for the pitch vertices
    uint16_t pitch_vert_val_count = Constants :: getVal("SQUARE_VERTICES") * Constants :: getVal("NR_COORDS_VERTEX")
                                    * Constants :: getVal("NR_COORDS_TEXTURE");
    //Already bound on creation
    VBO pitch_vbo(Constants :: getVertexPositions("PITCH"), pitch_vert_val_count * sizeof(float), GL_STATIC_DRAW);

    pitch_shader.setUniform1i("u_Texture", 0);

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


void Match :: movePlayers(){
    //srand(time(0));
    t1->movePlayers();
    t2->movePlayers();
}