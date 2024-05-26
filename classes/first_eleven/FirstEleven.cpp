#include "FirstEleven.h"

double FirstEleven :: getFirstElevenOVR(const unordered_map<string, 
                                        vector<shared_ptr<Player>>>& first_eleven) const{
    double team_ovr = 0;
    for (const auto& p_pos : first_eleven)
        for (const auto& p : p_pos.second)
            team_ovr += p->getOVR(p_pos.first);
    
    return team_ovr / Constants :: getVal("MATCH_TEAM_SIZE");
}