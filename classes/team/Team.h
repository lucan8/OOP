#pragma once
#include "../players/player/Player.h"
#include "../players/attacker/Attacker.h"
#include "../players/midfielder/Midfielder.h"
#include "../players/defender/Defender.h"
#include "../players/goalkeeper/Goalkeeper.h"
class Team{
private:
    string name;
    double budget;
    vector<Player*> Players;
    unsigned short points = 0;
    
    //Not implemented
    friend void generateGoalkeepers(Team*);
    friend void generateDefenders(Team*);
    friend void generateMidfielders(Team*);
    friend void generateAttackers(Team*);
    friend void generateBudget(Team*);
    //Copies only the obj pointed at by the pointers
    void copyPlayers(const vector<Player*>& Players);
public:
    Team(const string& name) : name(name){
        generateGoalkeepers(this);
        generateDefenders(this);
        generateMidfielders(this);
        generateAttackers(this);
        generateBudget(this);
    }
    //Only for testing
    Team(unsigned short T_size);
    Team(const string& name, const vector<Player*>& Players, double budget) : name(name), budget(budget){
        copyPlayers(Players);
    }
    Team(const Team& other){*this = other;}//Initializer list
    ~Team();
    void operator =(const Team& other);
    unsigned short getChemestry() const;//Simplified for the moment
    double getBudget() const{return budget;} 
    const string& getName() const{return name;}
    unsigned short getPoints() const{return points;}

    vector<const Player*> getPlayers() const{return vector<const Player*>(Players.cbegin(), Players.cend());}

    void setBudget(double budget){this->budget = budget;}

    void playMatch(Team* other); //Simplified for tests
    void buyPlayer(Player*);//Not implemented yet
    void sellPlayer(Player*);//Not implemented yet

    void trainPlayers();
    void restPlayers();

    void resetSeasonStats();
    //void addPoints(M_POINTS r);
    void sortByOVR(){sort(Players.begin(), Players.end(), compareOVR);}
    vector<const Player*> sortedByOVR() const;

    friend ostream& operator <<(ostream& op, const Team&);
    friend istream& operator >>(istream&, Team&);

    friend bool compareOVR(const Player* P1, const Player* P2);
};
