#pragma once
#include "../../../functions/functions.h"
#include "../../players/outfield/OutFieldPlayer.h"
#include "../../players/goalkeeper/Goalkeeper.h"
#include <glm/glm.hpp>
//Add comparator for players and way to retrieve e a sorted vector of players

class Team;
typedef Team* team_ptr;
typedef shared_ptr<Team> shared_team;
typedef unique_ptr<Team> unique_team;
typedef vector<shared_ptr<Team>> shared_teams;

class Team : public Printable, public Readable{
private:
    string name;
    float budget;
    shared_squad Players;
    uint16_t points = 0;
    glm :: vec4 aura_color;
    
public:
    shared_squad_split splitTeamPos() const;
    Team(const string& name = "", shared_squad Players = {}, float budget = 0) : 
    name(name), Players(move(Players)), budget(budget){}

    Team(const Team& other) : name(other.name), budget(other.budget),
     Players(clonePtrVector(other.Players)){}
    ~Team(){};

    float getBudget() const{return budget;} 
    const string& getName() const{return name;}

    unsigned short getPoints() const{return points;}
    const shared_squad& getPlayers() const{return Players;}

    const glm :: vec4& getAuraColor() const{return aura_color;}
    //Sets the aura color for the players, the color is normalized, alpha is 0.5
    void setAuraColor(const glm :: vec4& color);

    //Returns a map of averages calculated from the players stats
    unordered_map<string, float> getTeamStats() const;
    team_ptr clone() const; 

    void setBudget(float budget){this->budget = budget;}

    void buyPlayer(Player*);//Not implemented yet
    void sellPlayer(Player*);//Not implemented yet

    void trainPlayers();
    void restPlayers();

    void resetSeasonStats();
    void addPoints(const unsigned char p);

    void print(ostream&) const override;
    void read(istream&) override;
};
