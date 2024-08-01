#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../renderer/Renderer.h"
#include "../first_team/FirstTeam.h"
#include "../textures/Textures.h"
#include <assert.h>
#include <random>


//Ignoring player match stamina for the sake of simplicity
//Ignoring player match form for the sake of simplicity
//t1 is in the first half of the pitch, t2 is in the second half
class Match{
private:
    unique_first_team team1, team2;
    glm :: vec2 ball_coords;
    unordered_map<string, Textures> textures;

    void drawPlayers(Shader& player_shader, const IBO& player_ibo);
    void drawField(Shader& pitch_shader, const IBO& pitch_ibo);
    void drawBall(Shader& ball_shader, const IBO& ball_ibo);

    //Returns the ball vertices(contains the position and texture coords)
    glm :: mat4 getBallVertices() const;

    //Loads and binds the textures needed(pitch, ball, teams)
    void loadTextures();

    bool hasBall(const shared_m_player& player) const;
    shared_m_player getPlayerWithBall() const;
public:
    //Draws the pitch and players
    void draw();
    
    Match(unique_first_team team1, unique_first_team team2);

    //Moves players and ball
    void movePlayers();
};