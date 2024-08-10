#include "Match.h"
#include "../textures/Textures.h"

Match :: Match(unique_first_team team1, unique_first_team team2)
        : team1(move(team1)), team2(move(team2)), ball_coords(0, 0),
          pitch_matrix(Constants :: getVal("GOAL_LINE_LENGTH") + 1, vector<shared_m_player>(Constants :: getVal("TOUCHLINE_LENGTH") + 1, nullptr)){
    this->team2->changeSide();

    //From vertical pitch to horizontal pitch(and vice versa)
    this->team1->changeSide1();
    this->team2->changeSide1();

    loadTextures();
    //Setting the aura colors for the teams
    this->team1->setAuraColor(textures.at("TEAM1").getAverageColor());
    this->team2->setAuraColor(textures.at("TEAM2").getAverageColor());

    this->setPitchMatrix();
}


void Match :: loadTextures(){
    const string texture_path = (filesystem::current_path().parent_path() / "resources" / "textures" / "").string();
    //Loading the textures
    textures.emplace("PITCH", texture_path + "pitch2.png"),
    textures.emplace("BALL", texture_path + "ball.png"),
    textures.emplace("TEAM1", texture_path +  "arsenal.png"),
    textures.emplace("TEAM2", texture_path + "real_madrid.png");
    
    //Binding the textures
    textures["PITCH"].bind(0);
    textures["BALL"].bind(1);
    textures["TEAM1"].bind(2);
    textures["TEAM2"].bind(3);
}


void Match :: draw(){
    //Setting the shaders paths
    const string vert_shader_path = (filesystem::current_path().parent_path() / "resources" / "shaders" / "vertex" / "").string(),
                 frag_shader_path = (filesystem::current_path().parent_path() / "resources" / "shaders" / "fragment" / "").string();

    //Loading the shaders
    Shader entity_shader((vert_shader_path + "entity.glsl"), (frag_shader_path + "entity.glsl"));
    entity_shader.bind();
    
    float pitch_padding = Constants :: getVal("PITCH_OUT_PADDING");
    //Setting the window resolution and the pitch resolution and the units for the window relative to the pitch
    const glm :: vec2 halfed_window_res(Constants :: getVal("WINDOW_WIDTH") / 2,
                                        Constants :: getVal("WINDOW_HEIGHT") / 2),
                      halfed_pitch_res (Constants :: getVal("TOUCHLINE_LENGTH") / 2 + pitch_padding,
                                        Constants :: getVal("GOAL_LINE_LENGTH") / 2 + pitch_padding),
                      res_units(halfed_window_res / halfed_pitch_res);

    //Setting the projection matrix                       
    glm :: mat4 proj = glm :: ortho(-halfed_pitch_res.x, halfed_pitch_res.x,
                                    -halfed_pitch_res.y, halfed_pitch_res.y);

    entity_shader.setUniformMat4f("u_projection", proj);

    //Getting the indices for the square
    IBO square_ibo(Constants :: getVertexIndices("SQUARE"), 6);

    this->drawField(entity_shader, square_ibo);

    //Setting the uniforms for determining the entity's coordinates according to the window resolution
    entity_shader.setUniform2f("u_screen_units", res_units);
    entity_shader.setUniform2f("u_screen_center", halfed_window_res);

    this->drawPlayers(entity_shader, square_ibo);
    this->drawBall(entity_shader, square_ibo);
}


void Match :: drawPlayers(Shader& player_shader, const IBO& player_ibo){
    //Binding the shader
    player_shader.bind();

    //Setting the entity type
    player_shader.setUniform1i("u_entity_type", Constants :: getEntityNumber("PLAYER"));

    //Layout for player's triangles(2 coords for position, 2 coords for texture)
    VertexBufferLayout player_layout, player_aura_layout;
    //Coords for vertex positions and texture
    player_layout.addAttribute<float>(2);
    player_layout.addAttribute<float>(2);

    //Coords for vertex positions
    player_aura_layout.addAttribute<float>(2);

    player_shader.setUniform1i("u_Texture", 2);
    team1->drawPlayers(MatchPlayer :: pitch_half :: first, player_shader, player_ibo, player_layout, player_aura_layout);

    //Drawing the second team's players
    player_shader.setUniform1i("u_Texture", 3);
    team2->drawPlayers(MatchPlayer :: pitch_half :: second, player_shader, player_ibo, player_layout, player_aura_layout);
}


void Match :: drawField(Shader& pitch_shader, const IBO& pitch_ibo){
    //Binding the shader
    pitch_shader.bind();

    //Setting the entity type
    pitch_shader.setUniform1i("u_entity_type", Constants :: getEntityNumber("PITCH"));

    //Setting the texture to slot 0
    pitch_shader.setUniform1i("u_Texture", 0);

    //Getting the number of values for the pitch vertices
    uint16_t pitch_vert_val_count = Constants :: getVal("SQUARE_VERTICES") * Constants :: getVal("NR_COORDS_VERTEX")
                                    * Constants :: getVal("NR_COORDS_TEXTURE");
    //Setting the pitch vertices
    VBO pitch_vbo(Constants :: getVertices("PITCH"), pitch_vert_val_count * sizeof(float), GL_STATIC_DRAW);

    //Layout for pitch
    VertexBufferLayout pitch_layout;
    pitch_layout.addAttribute<float>(2);
    pitch_layout.addAttribute<float>(2);

    //Binding the pitch VAO and VBO
    VAO pitch_vao;
    pitch_vao.addBuffer(pitch_vbo, pitch_layout);

    Renderer :: draw(pitch_vao, pitch_ibo, pitch_shader);
}


void Match :: drawBall(Shader& ball_shader, const IBO& ball_ibo){
    //Binding the shader
    ball_shader.bind();

    //Setting the entity type
    ball_shader.setUniform1i("u_entity_type", Constants :: getEntityNumber("BALL"));

    //Setting the ball's coordinates
    ball_shader.setUniform2f("u_entity_coords", glm :: vec2(ball_coords.x, ball_coords.y));

    //Setting the ball's radius
    ball_shader.setUniform1f("u_entity_radius", Constants :: getVal("PLAYER_RADIUS") / 2.0);

    //Setting the texture to slot 1
    ball_shader.setUniform1i("u_Texture", 1);

    //Getting the ball canvas vertices
    glm :: mat4 ball_vertices = this->getBallVertices();
    
    //Creating the VBO for the ball canvas
    VBO ball_vbo(&ball_vertices[0][0], sizeof(ball_vertices), GL_DYNAMIC_DRAW);

    //Layout for ball canvas
    VertexBufferLayout ball_layout;
    ball_layout.addAttribute<float>(2);
    ball_layout.addAttribute<float>(2);

    //Binding the ball VAO and VBO
    VAO ball_vao;
    ball_vao.addBuffer(ball_vbo, ball_layout);

    Renderer :: draw(ball_vao, ball_ibo, ball_shader);
}


void Match :: setPitchMatrix(){
    team1->setPitchMatrix(pitch_matrix);
    team2->setPitchMatrix(pitch_matrix);
}


glm :: mat4 Match :: getBallVertices() const{
    glm :: mat4 ball_vertices = toMat4(getCanvasPositions(ball_coords, Constants :: getVal("PLAYER_RADIUS") / 2.0));

    //Getting the texture coordinates
    float max_x = Constants :: getVal("TEXTURE_MAX_X"), max_y = Constants :: getVal("TEXTURE_MAX_Y");
    float min_x = Constants :: getVal("TEXTURE_MIN_X"), min_y = Constants :: getVal("TEXTURE_MIN_Y");

    //Setting the texture coordinates
    ball_vertices[0][2] = min_x, ball_vertices[0][3] = min_y,
    ball_vertices[1][2] = min_x, ball_vertices[1][3] = max_y,
    ball_vertices[2][2] = max_x, ball_vertices[2][3] = max_y,
    ball_vertices[3][2] = max_x, ball_vertices[3][3] = min_y;

    return ball_vertices;
}


bool Match :: hasBall(const shared_m_player& player) const{
    float player_radius = Constants :: getVal("PLAYER_RADIUS");
    float ball_radius = player_radius / 2.0;

    return glm :: distance(player->getCoords(), ball_coords) < player_radius + ball_radius;
}


void Match :: movePlayers(){
    //Getting the player with the ball
    shared_m_player player_with_ball = getPlayerWithBall();

    intersections_map t1_intersections = team1->getOpponentIntersections(team2),
                      t2_intersections = team2->getOpponentIntersections(team1);

    if (player_with_ball){
        //Moving the ball with the player
        ball_coords = player_with_ball->getCoords();
        //Getting the number of intersections and the opponent player(if there is one)
        //Moving the player with the ball
        team1->movePlayerWithBall(ball_coords, player_with_ball, t1_intersections, team2->getGKCoords());
        //Moving the players without the ball
        team1->movePlayersWithoutBall(player_with_ball, team2->getGKCoords(), t1_intersections);
    }
    else
        team1->movePlayersWithoutBall(ball_coords, team2->getGKCoords());
    
    for (const auto& player : team2->getFirstEleven())
        if (player->getPosition() != "GK")
            player->p_move(ball_coords, team2->getClosestPlayer(player->getCoords()));
    
}

shared_m_player Match :: getPlayerWithBall() const{
    //Searching for the player with the ball in the first team
    for (const auto& player : team1->getFirstEleven())
        if (hasBall(player))
            return player;
    
    //Searching for the player with the ball in the second team
    for (const auto& player : team2->getFirstEleven())
        if (hasBall(player))
            return player;

    //No player has the ball
    return nullptr;
}


