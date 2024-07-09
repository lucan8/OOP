#pragma once
#include "../../first_team/FirstTeam.h"
#include "../../../functions/functions.h"
//Add comparator for players and way to retrieve e a sorted vector of players

class Team;
typedef Team* team_ptr;
typedef shared_ptr<Team> shared_team;
typedef unique_ptr<Team> unique_team;
typedef vector<shared_ptr<Team>> shared_teams;

class Team : public Printable, public Readable{
private:
    string name;
    double budget;
    shared_squad Players;
    unsigned short points = 0;

    //Should only be used before getSubstitutes
    //Returns the best first eleven for a formation from unused players
    //Players that are part of the starting 11 are removed from the unused players
    static unique_m_squad getFirstEleven(const string& form_name, shared_squad_map& unused_outfields,
                                         shared_squad_map& unused_goalkeepers);

    //Helper function for getFirstEleven, returns the best outfield players for a formation
    static unique_m_squad getFirstTeamOutfields(const string& form_name, 
                                                shared_squad_map& unused_outfields);
                                                
    //Returns the best goalkeeper in the form of a match player
    static unique_m_player getBestGoalkeeper(shared_squad_map& unused_goalkeepers);

    //Should only be used after getFirstEleven
    //Returns the best substitutes from unused players
    //Players that are part of the substitutes are removed from the unused players                                     
    static unique_m_squad getSubstitutes(shared_squad_map& unused_outfields, shared_squad_map& unused_goalkeepers);

    //Helper function for getSubstitutes, returns the best outfield substitutes
    static unique_m_squad_map getSubsOutfields(shared_squad_map& unused_outfields, const string& det_p_type);

    //Helper function for getSubsOutfields, returns the best two match players indexes
    static pair<uint16_t, uint16_t> getMax2PlayersIndexes(const unique_m_squad_map& players);
    
    //Splits team by player type(OUTFIELD, GK)
    shared_squad_split splitTeamPType() const;
public:
    shared_squad_split splitTeamPos() const;
    Team(const string& name = "", shared_squad Players = {}, double budget = 0) : 
    name(name), Players(move(Players)), budget(budget){}

    Team(const Team& other) : name(other.name), budget(other.budget),
     Players(clonePtrVector(other.Players)){}
    ~Team(){};

    double getBudget() const{return budget;} 
    const string& getName() const{return name;}

    unsigned short getPoints() const{return points;}
    const shared_squad& getPlayers() const{return Players;}

    //Returns a map of averages calculated from the players stats
    unordered_map<string, double> getTeamStats() const;
    unique_first_team getFirstTeam() const;          
    //Returns the index of the best player for a position                    
    static uint16_t getBestPlayerIndex(const shared_squad_map& players, const string& pos);
    team_ptr clone() const; 

    void setBudget(double budget){this->budget = budget;}

    void buyPlayer(Player*);//Not implemented yet
    void sellPlayer(Player*);//Not implemented yet

    void trainPlayers();
    void restPlayers();

    void resetSeasonStats();
    void addPoints(const unsigned char p);

    void print(ostream&) const override;
    void read(istream&) override;
};
