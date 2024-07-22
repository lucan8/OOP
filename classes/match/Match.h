#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../renderer/Renderer.h"
#include "../first_team/FirstTeam.h"
#include <assert.h>
#include <random>


//Ignoring player match stamina for the sake of simplicity
//Ignoring player match form for the sake of simplicity
//t1 is in the first half of the pitch, t2 is in the second half
class Match{
private:
    unique_first_team t1, t2;

    void drawPlayers(Shader& player_shader, const glm :: mat4& proj);
    void drawField(Shader& pitch_shader, const glm :: mat4& proj);
public:
    //Draws the pitch and players
    void draw();
    //Setting the player's triangles positions
    Match(unique_first_team t1, unique_first_team t2): t1(move(t1)), t2(move(t2)){
        this->t2->changeSide();

        //From vertical pitch to horizontal pitch(and vice versa)
        this->t1->changeSide1();
        this->t2->changeSide1();
    }

    void movePlayers();
};