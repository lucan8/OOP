#include "Match.h"
#include "../textures/Textures.h"
#include "../FirstTeamBuilder/FirstTeamBuilder.h"

Match :: Match(shared_team team1, shared_team team2)
        : team1(FirstTeamBuilder :: buildFirstTeam(team1)), team2(FirstTeamBuilder :: buildFirstTeam(team2)), ball_coords(0, 0){
    this->team2->changeSide();

    //From vertical pitch to horizontal pitch(and vice versa)
    this->team1->changeSide1();
    this->team2->changeSide1();

    loadShaders();
    loadTextures();

    //Setting the aura colors for the teams
    this->team1->getTeam()->setAuraColor(textures.at("TEAM1").getAverageColor());
    this->team2->getTeam()->setAuraColor(textures.at("TEAM2").getAverageColor());

    //this->setPitchMatrix();
}


void Match :: loadTextures(){
    string texture_path = (filesystem::current_path().parent_path() / "resources" / "textures" / "").string();
    for (const auto& entry : filesystem::directory_iterator(texture_path)){
        const filesystem :: path& file_path = entry.path();
        textures.emplace(file_path.stem().string(), file_path.string());
    }
    for (auto& texture : textures)
        texture.second.bind();
}


void Match :: loadShaders(){
    //Setting the shaders paths
    filesystem :: path shader_path = filesystem::current_path().parent_path() / "resources" / "shaders";
    //Loading the shaders
    entity_shader = Shader((shader_path / "vertex" / "entity.glsl").string(),
                           (shader_path / "fragment" / "entity.glsl").string());
}


void Match :: draw(){
    this->entity_shader.bind();
    this->setConstUniforms();

    //Getting the indices for the square
    IBO square_ibo(Constants :: getVertexIndices("SQUARE"), Constants :: getVal("NR_SQUARE_INDICES"));

    this->drawField(entity_shader, square_ibo);
    this->drawPlayers(entity_shader, square_ibo);
    this->drawBall(entity_shader, square_ibo);
    this->drawScore(entity_shader, square_ibo);
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

    player_shader.setUniform1i("u_Texture", this->textures.at("TEAM1").getSlot());
    team1->drawPlayers(MatchPlayer :: pitch_half :: first, player_shader, player_ibo, player_layout, player_aura_layout);

    //Drawing the second team's players
    player_shader.setUniform1i("u_Texture", this->textures.at("TEAM2").getSlot());
    team2->drawPlayers(MatchPlayer :: pitch_half :: second, player_shader, player_ibo, player_layout, player_aura_layout);
}


void Match :: drawField(Shader& pitch_shader, const IBO& pitch_ibo){
    //Binding the shader
    pitch_shader.bind();

    //Setting the entity type
    pitch_shader.setUniform1i("u_entity_type", Constants :: getEntityNumber("PITCH"));

    //Setting the texture to slot 0
    pitch_shader.setUniform1i("u_Texture", this->textures.at("PITCH").getSlot());

    //Getting the number of values for the pitch vertices
    uint16_t pitch_vert_val_count = Constants :: getVal("SQUARE_VERTICES") * Constants :: getVal("NR_COORDS_VERTEX")
                                    * Constants :: getVal("NR_COORDS_TEXTURE");
    //Setting the pitch vertices
    VBO pitch_vbo(&getPitchVertices()[0][0], pitch_vert_val_count * sizeof(float), GL_STATIC_DRAW);

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
    ball_shader.setUniform1i("u_Texture", this->textures.at("BALL").getSlot());

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


void Match :: drawScore(Shader& score_shader, const IBO& score_ibo){
    //Binding the shader and setting the uniforms
    score_shader.bind();
    score_shader.setUniform1i("u_entity_type", Constants :: getEntityNumber("SCORE"));
    score_shader.setUniform1i("u_Texture", this->textures[to_string(this->score.first)].getSlot());
    
    //Setting the score's vertices
    glm :: mat4 score_vertices = this->getScoreVertices();
    VBO score_vbo(&getScoreVertices()[0][0], sizeof(score_vertices), GL_STATIC_DRAW);

    //Setting layout for VAO
    VertexBufferLayout score_layout;
    score_layout.addAttribute<float>(2);
    score_layout.addAttribute<float>(2);

    //Binding the layout and VBO to the VAO
    VAO score_vao;
    score_vao.addBuffer(score_vbo, score_layout);

    Renderer :: draw(score_vao, score_ibo, score_shader);
}

/*
void Match :: setPitchMatrix(){
    team1->setPitchMatrix(pitch_matrix);
    team2->setPitchMatrix(pitch_matrix);
}
*/


glm :: mat4 Match :: getPitchVertices() const{
    //Getting the pitch's max x and y values
    float p_max_x = Constants :: getVal("TOUCHLINE_LENGTH") / 2,
          p_max_y = Constants :: getVal("GOAL_LINE_LENGTH") / 2,
          aspect_ratio = p_max_x / p_max_y,
          padding_y = Constants :: getVal("PITCH_OUT_PADDING"),
          padding_x = padding_y * aspect_ratio;

    glm :: mat4 pitch_vertices = toMat4(getCanvasPositions(glm :: vec2(0, 0),
                                        p_max_x + padding_x, p_max_y + padding_y));
    setTextureCoords(pitch_vertices);
    return pitch_vertices;
}


glm :: mat4 Match :: getBallVertices() const{
    glm :: mat4 ball_vertices = toMat4(getCanvasPositions(ball_coords, Constants :: getVal("PLAYER_RADIUS") / 2.0));
    setTextureCoords(ball_vertices);

    return ball_vertices;
}


glm :: mat4 Match :: getScoreVertices() const{
    float max_y = Constants :: getVal("GOAL_LINE_LENGTH") / 2,
          padding = Constants :: getVal("PITCH_PADDING") - Constants :: getVal("PITCH_OUT_PADDING");
    //Pure center for x, y is above the pitch(and it's padding)
    glm :: vec2 center(0, max_y + padding);
    glm :: mat4 ball_vertices = toMat4(getCanvasPositions(center, Constants :: getVal("SCORE_RADIUS")));
    setTextureCoords(ball_vertices);

    return ball_vertices;
}


void Match :: setTextureCoords(glm :: mat4& entity_vertices) const{
    //Getting the texture coordinates
    float max_x = Constants :: getVal("TEXTURE_MAX_X"), max_y = Constants :: getVal("TEXTURE_MAX_Y");
    float min_x = Constants :: getVal("TEXTURE_MIN_X"), min_y = Constants :: getVal("TEXTURE_MIN_Y");

    //Setting the texture coordinates
    entity_vertices[0][2] = min_x, entity_vertices[0][3] = min_y,
    entity_vertices[1][2] = min_x, entity_vertices[1][3] = max_y,
    entity_vertices[2][2] = max_x, entity_vertices[2][3] = max_y,
    entity_vertices[3][2] = max_x, entity_vertices[3][3] = min_y;
}


void Match :: play(){
    if (team1->inPossesion())
        movePlayers(*team1, *team2);
    else if (team2->inPossesion())
        movePlayers(*team2, *team1);
    else{
        //Moving both teams to get possesion
        team1->getPossesion(ball_coords, team2->getGKCoords());
        team2->getPossesion(ball_coords, team1->getGKCoords());
        setPosession();
    }
}


void Match :: movePlayers(FirstTeam& poss_team, FirstTeam& opp_team){
    poss_team.attack(ball_coords, opp_team);
    opp_team.defend(ball_coords, poss_team);
}


void Match :: setPosession(){
    //Getting the closest two players to the ball
    shared_m_player closest1 = team1->getClosestPlayer(ball_coords, team1->getFirstEleven());
    shared_m_player closest2 = team2->getClosestPlayer(ball_coords, team2->getFirstEleven());

    if (closest1->isNearBall(ball_coords))
        if (closest2->isNearBall(ball_coords))
            //Player with the highest pace gets the ball
            if (closest1->getPlayer()->getStat("PAC") > closest2->getPlayer()->getStat("PAC"))
                closest1->setPossesion(true);
            else
                closest2->setPossesion(true);
        else
            closest1->setPossesion(true);
    else if (closest2->isNearBall(ball_coords))
        closest2->setPossesion(true);
}


void Match :: setConstUniforms(){
    //Getting the aspect ratio of the pitch and setting the padding accordingly
    float aspect_ratio = Constants :: getVal("TOUCHLINE_LENGTH") / Constants :: getVal("GOAL_LINE_LENGTH"),
          pitch_padding_y = Constants :: getVal("PITCH_PADDING"),
          pitch_padding_x = pitch_padding_y * aspect_ratio;
    //Setting the window resolution and the pitch resolution and the units for the window relative to the pitch
    const glm :: vec2 halfed_window_res(Constants :: getVal("WINDOW_WIDTH") / 2,
                                        Constants :: getVal("WINDOW_HEIGHT") / 2),
                      halfed_pitch_res (Constants :: getVal("TOUCHLINE_LENGTH") / 2 + pitch_padding_x,
                                        Constants :: getVal("GOAL_LINE_LENGTH") / 2 + pitch_padding_y),
                      res_units(halfed_window_res / halfed_pitch_res);

    //Setting the projection matrix                       
    glm :: mat4 proj = glm :: ortho(-halfed_pitch_res.x, halfed_pitch_res.x,
                                    -halfed_pitch_res.y, halfed_pitch_res.y);
    this->entity_shader.setUniformMat4f("u_projection", proj);

    //Setting the uniforms for determining the entity's coordinates according to the window resolution
    this->entity_shader.setUniform2f("u_screen_units", res_units);
    this->entity_shader.setUniform2f("u_screen_center", halfed_window_res);
}

Match :: ~Match(){
    this->entity_shader.deleteProgram();
}