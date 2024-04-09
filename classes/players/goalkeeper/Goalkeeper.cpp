#include "Goalkeeper.h"

double Goalkeeper :: calculateOVR() const{
        double OVR = 0;
        for (auto& x : Constants :: getPositions("GK_STATS").value()){
            OVR += this->stats.at(x);
        }
        return OVR / Constants :: getPositions("GK_STATS").value().size();
    }

//Each stat gets a plus between 0 and 1
void Goalkeeper :: train(){
    for (auto& x : Constants :: getPositions("GK_STATS").value()){
        this->stats[x] += rand() / RAND_MAX;
    }
}