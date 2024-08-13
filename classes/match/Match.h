#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../renderer/Renderer.h"
#include "../teams/first_team/FirstTeam.h"
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
    shared_m_matrix pitch_matrix;

    void drawPlayers(Shader& player_shader, const IBO& player_ibo);
    void drawField(Shader& pitch_shader, const IBO& pitch_ibo);
    void drawBall(Shader& ball_shader, const IBO& ball_ibo);

    //Returns the ball vertices(contains the position and texture coords)
    glm :: mat4 getBallVertices() const;

    //Loads and binds the textures needed(pitch, ball, teams)
    void loadTextures();

    void setPitchMatrix();
    //Arg1 team in possesion attacks, arg2 defends
    void movePlayers(FirstTeam& poss_team, FirstTeam& opp_team);
    //Determines if any player gets possesion and sets the player with the ball
    void setPosession();
public:
    //Draws the pitch and players
    void draw();
    
    Match(shared_team team1, shared_team team2);
    Match(){}

    //Moves players and ball
    void play();
};