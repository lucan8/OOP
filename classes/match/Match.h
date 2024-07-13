#pragma once
#include "../teams/match_team/MatchTeam.h"
#include <random>

//Ignoring player match stamina for the sake of simplicity
//Ignoring player match form for the sake of simplicity
//t1 is in the first half of the pitch, t2 is in the second half
class Match{
private:
    //Team, nr_goals
    unique_first_team t1, t2;

    //Buffer for pitch related positions(middle_line, pitch, goals...)
    unique_ptr<const float> pitch_positions;
    //Buffer for player positions and ball position
    unique_ptr<float> player_positions, ball_position;
    //Size of the player_positions buffer
    uint16_t positions_size = 0;

    void addTeamPositions(const unique_first_team& team, bool second_half){
        for (const auto& player : team->getFirstEleven()){
            float* triangle_positions = player->getTrianglePositions(second_half);
            memcpy(this->player_positions.get() + this->positions_size, triangle_positions, 6 * sizeof(float));
            positions_size += 6;
            delete[] triangle_positions;
        }
    }
public:
    Match(unique_first_team t1, unique_first_team t2): t1(move(t1)), t2(move(t2)){
        //Pitch positions and ball positions shall be in constants
        this->pitch_positions = Constants :: getPitchPositions();
        addTeamPositions(this->t1, false);
        addTeamPositions(this->t2, true);
        //Learn how to actually draw circles
        this->ball_position = Constants :: getBallPositions();
    }
    
};