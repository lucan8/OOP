#pragma once
#include "../../first_eleven/FirstEleven.h"
//Add comparator for players and way to retrieve e a sorted vector of players
class Team : public Printable, public Readable{
private:
    string name;
    double budget;
    vector<shared_ptr<Player>> Players;
    //Index is stage_nr, value is ptr to the oponent and goals scored against them
    unsigned short points = 0;
    
    //Copies only the obj pointed at by the pointers
    void clonePlayers(const vector<shared_ptr<Player>>& Players);
    
public:
    Team(const string& name = "", vector<shared_ptr<Player>> Players = {}, double budget = 0) : 
    name(name), Players(move(Players)), budget(budget){}

    Team(const Team& other) : name(other.name), budget(other.budget){
        clonePlayers(other.Players);
    }
    ~Team(){};
    unsigned short getChemestry() const;//Simplified for the moment
    double getBudget() const{return budget;} 
    const string& getName() const{return name;}
    unsigned short getPoints() const{return points;}

    const vector<shared_ptr<Player>>& getPlayers() const{return Players;}
    //Returns a map of averages calculated from the players stats
    unordered_map<string, double> getTeamStats() const;

    unique_ptr<FirstEleven> getFirstTeamOutfields(const string& form_name, 
                                         vector<shared_ptr<Player>> unused_players) const;
    unique_ptr<FirstEleven> getBestFirstTeam() const;
    //Splits team by player position
    static unordered_map<string, vector<shared_ptr<Player>>> 
                                splitTeamPos(const vector<shared_ptr<Player>>& team);

    //Splits team by player type(OUTFIELD, GK)
    static unordered_map<string, vector<shared_ptr<Player>>> 
                                splitTeamPType(const vector<shared_ptr<Player>>& team);                                  
    
    static vector<shared_ptr<Player>> :: const_iterator 
                            getBestPlayerIt(const vector<shared_ptr<Player>>& players, const string& pos);

    void setBudget(double budget){this->budget = budget;}

    void playMatch(Team* other); //Simplified for tests
    void buyPlayer(Player*);//Not implemented yet
    void sellPlayer(Player*);//Not implemented yet

    void trainPlayers();
    void restPlayers();

    void resetSeasonStats();
    void addPoints(const unsigned char p);

    void print(ostream&) const override;
    void read(istream&) override;
};
