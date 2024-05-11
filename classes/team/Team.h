#pragma once
#include "../players/outfield/OutfieldPlayer.h"
#include "../players/goalkeeper/Goalkeeper.h"

//Add comparator for players and way to retrieve e a sorted vector of players
class Team : public Printable, public Readable{
private:
    string name;
    double budget;
    vector<unique_ptr<Player>> Players;
    //Index is stage_nr, value is ptr to the oponent and goals scored against them
    vector<pair<shared_ptr<Team>, uint16_t>> Oponents;
    unsigned short points = 0;
    
    //Copies only the obj pointed at by the pointers
    void clonePlayers(const vector<unique_ptr<Player>>& Players);
public:
    Team(const string& name = "", vector<unique_ptr<Player>> Players = {}, double budget = 0) : 
    name(name), Players(move(Players)), budget(budget){}

    Team(const Team& other) : name(other.name), budget(other.budget){
        clonePlayers(other.Players);
    }
    ~Team(){};
    unsigned short getChemestry() const;//Simplified for the moment
    double getBudget() const{return budget;} 
    const string& getName() const{return name;}
    unsigned short getPoints() const{return points;}

    const vector<unique_ptr<Player>>& getPlayers() const{return Players;}

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
