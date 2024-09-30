#include "Match.h"
#include "../renderer/Renderer.h"
#include "../textures/Textures.h"
#include "../FirstTeamBuilder/FirstTeamBuilder.h"
#include "../constants/Constants.h"
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>

using std :: string, std :: to_string;
using glm :: vec2, glm :: vec3, glm :: mat4;
namespace fs = std :: filesystem;

Match :: Match(shared_team team1, shared_team team2)
        : team1(FirstTeamBuilder :: buildFirstTeam(team1)), team2(FirstTeamBuilder :: buildFirstTeam(team2)), ball_coords(0, 0){
    this->team2->changeSide();

    //From vertical pitch to horizontal pitch(and vice versa)
    this->team1->changeSide1();
    this->team2->changeSide1();

    loadShaders();
    loadTextures();
    loadFont();

    //Setting the aura colors for the teams
    this->team1->getTeam()->setAuraColor(textures.at("TEAM1").getAverageColor());
    this->team2->getTeam()->setAuraColor(textures.at("TEAM2").getAverageColor());

    //this->setPitchMatrix();
}


void Match :: loadTextures(){
   
    string texture_path = (fs::current_path().parent_path() / "resources" / "textures" / "").string();
    for (const auto& entry : fs::directory_iterator(texture_path)){
        const fs :: path& file_path = entry.path();
        string f_name = file_path.stem().string(),
               f_path = file_path.string();
        textures.emplace(f_name, f_path);       
    }
}


void Match :: loadShaders(){
    //Setting the shaders paths
    fs :: path shader_path = fs::current_path().parent_path() / "resources" / "shaders";
    //Loading the shaders
    entity_shader = std :: make_unique<Shader>((shader_path / "vertex" / "entity.glsl").string(),
                                                (shader_path / "fragment" / "entity.glsl").string());
}


void Match :: loadFont(){
    fs :: path font_path = fs::current_path().parent_path() / "resources" / "fonts" / "arial.ttf";
    font = std :: make_unique<Font>(font_path.string());
}


void Match :: draw(){
    this->entity_shader->bind();

    //Getting the indices for the square
    IBO square_ibo(Constants :: getVertexIndices("SQUARE"), Constants :: getVal("NR_SQUARE_INDICES"));

    entity_shader->setUniformMat4f("u_projection", Constants :: getPitchProj());
    //entity_shader->setUniformMat4f("u_model", glm :: scale(glm :: mat4(1.0f), vec3(1.0f, 1.0f, 1.0f)));
    this->drawField(*entity_shader, square_ibo);
    this->drawPlayers(*entity_shader, square_ibo);
    this->drawBall(*entity_shader, square_ibo);
    this->drawTeamCrests(*entity_shader, square_ibo);
    //Uses different projection matrix
    font->setUniforms(*entity_shader, vec3(1.0f));
    font->bind(0);
    this->drawScore(*entity_shader, square_ibo);
    this->drawSubs(*entity_shader, square_ibo);
}


void Match :: drawPlayers(Shader& player_shader, const IBO& player_ibo){
    //Binding the shader
    player_shader.bind();

    //Setting the entity type
    player_shader.setUniform1i("u_entity_type", Constants :: getEntityNumber("PLAYER"));

    VertexBufferLayout player_layout, player_aura_layout;
    //Coords for vertex positions and team crest texture coords
    player_layout.addAttribute<float>(2);
    player_layout.addAttribute<float>(2);
    //Coords for vertex positions
    player_aura_layout.addAttribute<float>(2);

    player_shader.setUniform1i("u_texture", 0);
    //Drawing the players
    this->textures.at("TEAM1").bind(0);
    team1->drawPlaying(MatchPlayer :: pitch_half :: first, player_shader, player_ibo, player_layout,
                       player_aura_layout);

    this->textures.at("TEAM2").bind(0);
    team2->drawPlaying(MatchPlayer :: pitch_half :: second, player_shader, player_ibo, player_layout,
                       player_aura_layout);
}


void Match :: drawField(Shader& pitch_shader, const IBO& pitch_ibo){
    //Binding the shader
    pitch_shader.bind();

    //Setting the entity type
    pitch_shader.setUniform1i("u_entity_type", Constants :: getEntityNumber("PITCH"));

    //Setting the texture
    pitch_shader.setUniform1i("u_texture", 0);
    this->textures.at("PITCH").bind(0);

    glm :: mat4 pitch_vertices = this->getPitchVertices();
    //Setting the pitch vertices
    VBO pitch_vbo(sizeof(pitch_vertices), &pitch_vertices[0][0], GL_STATIC_DRAW);

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

    //Setting the ball's coordinates in the fragment pixel system
    ball_shader.setUniform2f("u_entity_coords",convertCoords(this->ball_coords,
                                                             Constants :: getPitchProj(),
                                                             Constants :: getPixelFragProj()));
    //Setting the ball's radius in pixel units
    ball_shader.setUniform1f("u_entity_radius", changeUnit(Constants :: getVal("PLAYER_RADIUS") / 2.0));

    //Setting the texture
    ball_shader.setUniform1i("u_texture", 0);
    this->textures.at("BALL").bind(0);

    //Getting the ball canvas vertices
    mat4 ball_vertices = this->getBallVertices();
    
    //Creating the VBO for the ball canvas
    VBO ball_vbo(sizeof(ball_vertices), &ball_vertices[0][0], GL_DYNAMIC_DRAW);

    //Layout for ball canvas
    VertexBufferLayout ball_layout;
    ball_layout.addAttribute<float>(2);
    ball_layout.addAttribute<float>(2);

    //Binding the ball VAO and VBO
    VAO ball_vao;
    ball_vao.addBuffer(ball_vbo, ball_layout);

    Renderer :: draw(ball_vao, ball_ibo, ball_shader);
}


void Match :: drawTeamCrests(Shader& crest_shader, const IBO& crest_ibo){
    //Binding the shader and setting the uniforms
    crest_shader.bind();
    crest_shader.setUniform1i("u_entity_type", Constants :: getEntityNumber("TEAM_CREST"));

    //Creating layout for VAO
    VertexBufferLayout crest_layout;
    crest_layout.addAttribute<float>(2);
    crest_layout.addAttribute<float>(2);

    //Creating an empty vbo for the team crests
    VBO crest_vbo(sizeof(glm :: mat4));

    //Binding the layout and VBO to the VAO
    VAO crest_vao;
    crest_vao.addBuffer(crest_vbo, crest_layout);

    //Getting the coordinates needed for the crest canvas center
    float max_y = Constants :: getVal("GOAL_LINE_LENGTH") / 2,
          padding = Constants :: getVal("PITCH_PADDING") - Constants :: getVal("PITCH_OUT_PADDING");
    //Drawing the team crests
    drawTeamCrest(crest_shader, crest_ibo, crest_vbo, crest_vao, textures.at("TEAM1"),
                  vec2(-Constants :: getVal("TOUCHLINE_LENGTH") / 2, max_y + padding));
    drawTeamCrest(crest_shader, crest_ibo, crest_vbo, crest_vao, textures.at("TEAM2"),
                  vec2(Constants :: getVal("TOUCHLINE_LENGTH") / 2, max_y + padding));
}


void Match :: drawScore(Shader& shader, const IBO& ibo){
    //Binding the shader and setting the entity type uniform
    shader.bind();

    //Getting the coordinates needed for the score line center
    float max_y = Constants :: getVal("GOAL_LINE_LENGTH") / 2,
          padding = Constants :: getVal("PITCH_PADDING") - Constants :: getVal("PITCH_OUT_PADDING");
    
    //getting from pitch to pixel coords
    glm :: vec2 center = convertCoords(glm :: vec2(0, max_y + padding),
                                       Constants :: getPitchProj(),
                                       Constants :: getPixelProj());
    
    Renderer :: drawText(shader, ibo, to_string(score.first) + " - " + to_string(score.second),
                         center, *font, 2.0f, true);
  
}


void Match :: drawSubs(Shader& shader, const IBO& ibo){
    //Binding the shader and setting the entity type uniform
    shader.bind();
    team1->drawUnplaying(shader, ibo, *font);
    team2->drawUnplaying(shader, ibo, *font);
}


void Match :: drawTeamCrest(Shader& shader, const IBO& ibo, VBO& vbo, VAO& vao, const Textures& texture,
                            const vec2& center){
    shader.setUniform1i("u_texture", 0);
    texture.bind(0);
    mat4 vertices = this->getTeamCrestVertices(center);
    vbo.update(&vertices[0][0], sizeof(vertices));
    Renderer :: draw(vao, ibo, shader);
}


mat4 Match :: getPitchVertices() const{
    //Getting the pitch's max x and y values
    float p_max_x = Constants :: getVal("TOUCHLINE_LENGTH") / 2,
          p_max_y = Constants :: getVal("GOAL_LINE_LENGTH") / 2,
          aspect_ratio = p_max_x / p_max_y,
          padding_y = Constants :: getVal("PITCH_OUT_PADDING"),
          padding_x = padding_y * aspect_ratio;

    mat4 pitch_vertices = toMat4(getCanvasPositions(vec2(0, 0),
                                        p_max_x + padding_x, p_max_y + padding_y));
    setTextureCoords(pitch_vertices);
    return pitch_vertices;
}


mat4 Match :: getBallVertices() const{
    mat4 ball_vertices = toMat4(getCanvasPositions(ball_coords, Constants :: getVal("PLAYER_RADIUS") / 2.0));
    setTextureCoords(ball_vertices);

    return ball_vertices;
}


mat4 Match :: getTeamCrestVertices(const vec2& center) const{
    mat4 vertices = toMat4(getCanvasPositions(center, Constants :: getVal("TEAM_CREST_RADIUS")));
    setTextureCoords(vertices);

    return vertices;
}


void Match :: play(){
    bool inteurrupted = false;
    //Indices of the players that have not moved yet
    std :: vector<uint16_t> remaining_players1(team1->getFirstEleven().size()),
                            remaining_players2(team2->getFirstEleven().size());
    std :: iota(remaining_players1.begin(), remaining_players1.end(), 0);
    std :: iota(remaining_players2.begin(), remaining_players2.end(), 0);
    
    while (!remaining_players1.empty() && !remaining_players2.empty() && !inteurrupted)
        //Flipping a coin to see which team moves a player 
        if (Constants :: generateNaturalNumber(0, 1) == 0){
            if (usePlayer(remaining_players1, *team1, *team2)){
                score.first++;
                inteurrupted = true;
            }
        }
        else if (usePlayer(remaining_players1, *team2, *team1)){
                score.second++;
                inteurrupted = true;
            }
    if (inteurrupted)
        resetGameState();
}


bool Match :: usePlayer(std :: vector<uint16_t>& p_indices, FirstTeam& acting_team, FirstTeam& opp_team){
    uint16_t index = Constants :: generateNaturalNumber(0, p_indices.size() - 1),
             p_index = p_indices[index];
             p_indices.erase(p_indices.begin() + index);
    return acting_team.usePlayer(p_index, ball_coords, opp_team);
}


void Match :: resetGameState(){
    team1->resetPlayers();
    team1->changeSide1();
    
    team2->resetPlayers();
    team2->changeSide1();
    team2->changeSide();

    ball_coords = glm :: vec2(0, 0);
}