#pragma once
#include "../../first_eleven/FirstEleven.h"
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
    //Index is stage_nr, value is ptr to the oponent and goals scored against them
    unsigned short points = 0;
    
    void clonePlayers(const shared_squad& Players);
    
public:
    Team(const string& name = "", shared_squad Players = {}, double budget = 0) : 
    name(name), Players(move(Players)), budget(budget){}

    Team(const Team& other) : name(other.name), budget(other.budget){
        clonePlayers(other.Players);
    }
    ~Team(){};
    unsigned short getChemestry() const;//Simplified for the moment
    double getBudget() const{return budget;} 
    const string& getName() const{return name;}
    unsigned short getPoints() const{return points;}

    team_ptr clone() const; 

    const shared_squad& getPlayers() const{return Players;}
    //Returns a map of averages calculated from the players stats
    unordered_map<string, double> getTeamStats() const;

    unique_ptr<FirstEleven> getFirstTeamOutfields(const string& form_name, 
                                         shared_squad unused_players) const;

    //Maybe make static class for all these functions(regarding choosing a first team)
    unique_ptr<FirstEleven> getBestFirstTeam() const;
    //Splits team by player position
    static unordered_map<string, shared_squad> 
                                splitTeamPos(const shared_squad& team);

    //Splits team by player type(OUTFIELD, GK)
    static unordered_map<string, shared_squad> 
                                splitTeamPType(const shared_squad& team);                                  
    
    static shared_squad :: const_iterator 
                            getBestPlayerIt(const shared_squad& players, const string& pos);

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
