#pragma once
#include "../teams/first_team/FirstTeam.h"
#include "../textures/Textures.h"
#include "../VAO/VAO.h"
#include "../font/Font.h"

//Ignoring player match stamina for the sake of simplicity
//Ignoring player match form for the sake of simplicity
//t1 is in the first half of the pitch, t2 is in the second half
class Match{
public:
    unique_first_team team1, team2;
    glm :: vec2 ball_coords;
    std :: unordered_map<std :: string, Textures> textures;
    unique_shader entity_shader;
    unique_font font;
    std :: pair<uint16_t, uint16_t> score = {0, 0};
    //shared_m_matrix pitch_matrix;
    

    void drawPlayers(Shader& player_shader, const IBO& ibo);
    void drawField(Shader& pitch_shader, const IBO& ibo);
    void drawBall(Shader& ball_shader, const IBO& ibo);
    void drawScore(Shader& score_shader, const IBO& ibo);
    void drawTeamCrests(Shader& team_shader, const IBO& ibo);
    void drawSubs(Shader& subs_shader, const IBO& ibo);
    //Binds the texture, updates the vbo using getScoreVertices and draws the team_crest
    void drawTeamCrest(Shader& score_shader, const IBO& score_ibo, VBO& score_vbo, VAO& score_vao,
                              const Textures& texture, const glm :: vec2& center);
    //Returns the pitch vertices(contains the position and texture coords)
    glm :: mat4 getPitchVertices() const;
    //Returns the ball vertices(contains the position and texture coords)
    glm :: mat4 getBallVertices() const;
    //Returns the canvas vertices with the center at the argument and the radius as team_crest radius
    glm :: mat4 getTeamCrestVertices(const glm :: vec2& center) const;

    void loadTextures();
    void loadShaders();
    void loadFont();
    //Players move to their initial positions and the ball is placed in the center
    void resetGameState();
    //Chosses a random player to act, returns true if a goal was scored
    bool usePlayer(std :: vector<uint16_t>& p_indices, FirstTeam& acting_team, FirstTeam& opp_team);
public:
    //Draws the pitch and players
    void draw();
    
    Match(shared_team team1, shared_team team2);
    ~Match(){};

    //Moves players and ball
    void play();
};