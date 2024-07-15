#include "MatchPlayer.h"

void MatchPlayer :: addYellowCard(){
    ++yellow_cards;
    player->addYellowCard();

    if(yellow_cards == 2){
        player->changeRedCarded();
        player->addRedCard();
    }
}


void MatchPlayer :: changeSide(){
    this->coords = -this->coords;
}


unique_ptr<float> MatchPlayer :: getTrianglePositions(pitch_half half) const{
    const float triangle_offset = 0.02f;
    //triangle if the player is in second half(grows upwards)
    if (half == pitch_half :: second)
        return unique_ptr<float>(new float[6]{
            coords.x, coords.y - triangle_offset,
            coords.x - triangle_offset, coords.y,
            coords.x + triangle_offset, coords.y
        });
    //triangle if the player is in first half(grows downwards)
    return unique_ptr<float>(new float[6]{
        coords.x, coords.y + triangle_offset,
        coords.x - triangle_offset, coords.y,
        coords.x + triangle_offset, coords.y
    });
}


bool MatchPlayer :: operator <(const MatchPlayer& other) const{
    return this->OVR < other.OVR;
}