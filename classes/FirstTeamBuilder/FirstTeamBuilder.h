#pragma once
#include "../teams/first_team/FirstTeam.h"

class FirstTeamBuilder{
public:
    static unique_first_team buildFirstTeam(shared_team team1);
private:
    //Should only be used before getSubstitutes
    //Returns the best first eleven for a formation from unused players
    //Players that are part of the starting 11 are removed from the unused players
    static shared_m_squad getFirstEleven(const std :: string& form_name, shared_squad_map& unused_outfields,
                                        shared_squad_map& unused_goalkeepers);
    //Returns the index of the best player for a position                    
    static uint16_t getBestPlayerIndex(const shared_squad_map& players, const std :: string& pos);

    //Helper function for getFirstEleven, returns the best outfield players for a formation
    static shared_m_squad getFirstTeamOutfields(const std :: string& form_name, 
                                                shared_squad_map& unused_outfields);
                                                
    //Returns the best goalkeeper in the form of a match player
    static m_player_ptr getBestGoalkeeper(shared_squad_map& unused_goalkeepers);

    //Should only be used after getFirstEleven
    //Returns the best substitutes from unused players
    //Players that are part of the substitutes are removed from the unused players                                     
    static unique_m_squad getSubstitutes(shared_squad_map& unused_outfields, shared_squad_map& unused_goalkeepers);

    //Helper function for getSubstitutes, returns the best outfield substitutes
    static unique_m_squad_map getSubsOutfields(shared_squad_map& unused_outfields, const std :: string& det_p_type);

    //Helper function for getSubsOutfields, returns the best two match players indexes
    static std :: pair<uint16_t, uint16_t> getMax2PlayersIndexes(const unique_m_squad_map& players);

    static shared_squad_split splitTeamPType(const shared_team& team);

    static float getElevenTotalStats(const shared_m_squad& first_eleven);
    static float getElevenOVR(const shared_m_squad& first_eleven);
    static uint16_t getElevenChemestry(const shared_m_squad& first_eleven);
    static float getSubsOVR(const unique_m_squad& subs);
};