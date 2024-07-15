#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../shader/Shader.h"
#include "../VBO/VBO.h"
#include "../IBO/IBO.h"
#include "../VAO/VAO.h"
#include "../first_team/FirstTeam.h"
#include <random>

//Ignoring player match stamina for the sake of simplicity
//Ignoring player match form for the sake of simplicity
//t1 is in the first half of the pitch, t2 is in the second half
class Match{
private:
    //Team, nr_goals
    unique_first_team t1, t2;
    //Buffer for player positions and ball position
    unique_ptr<float> player_positions, ball_position;
public:
    void setPositions();
    void createField();
    //Setting the player's triangles positions
    Match(unique_first_team t1, unique_first_team t2): t1(move(t1)), t2(move(t2)){
        setPositions();
    }

};