#include "Goalkeeper.h"

double Goalkeeper :: calculateOVR() const{
        double OVR = 0;
        for (auto& x : GK_STATS){
            OVR += this->stats[x];
        }
        return OVR / NR_GK_STATS;
    }

//Each stat gets a plus between 0 and 1
void Goalkeeper :: train(){
    for (auto& x : GK_STATS){
        this->stats[x] += rand() / RAND_MAX;
    }
}