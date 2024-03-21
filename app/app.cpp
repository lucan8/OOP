#include <algorithm>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <vector>
//TO DO: Using vectors of pointers to object
//Write function that compares not the pointers but the objects
//Overload < for object comparison
//For now eliminate the player classes and make Player a normal class
//All virtual functions will be simpler
using namespace std;

enum M_RESULT{WIN = 3, DRAW = 1, LOSS = 0};
const unsigned char MAX_FORM = 10;
const unsigned char MAX_CHEM = 11;

const unsigned char MAX_STATS = 99;
const unsigned char MIN_STATS = 10;

const unsigned char MIN_OVR = 40;
const unsigned char MAX_OVR = 99;

const unsigned char MAX_PLAYER_AGE = 40;
const unsigned char MIN_PLAYER_AGE = 16;
const unsigned char MAX_NR_POZ = 3;

const unsigned char MIN_TEAM_LENGTH = 16;
const unsigned char MAX_TEAM_LENGTH = 30;
const unsigned char DEFAULT_TEAM_LENGTH = 22;

const unsigned char DEFAULT_TEAM_NR = 21;
const unsigned char RELEGATED_NR = 3;

const unsigned char MAX_SHIRT_NUMBER = 99;
const unsigned char MAX_STAMINA = 100;
const unsigned char REST_STAMINA_PLUS = 20;

const unsigned char MAX_STAGES = 38;

const char GK_poz[3] = "GK";
const char DEF_POSITIONS[3][3] = {"CB", "LB", "RB"};
const char MID_POSITIONS[5][4] = {"CDM", "CM", "LM", "RM", "CAM"};
const char ATT_POSITIONS[4][3] = {"LW", "RW", "CF", "ST"};

const char NR_GK_STATS = 5;
const char GK_STATS[NR_GK_STATS][4] = {"REF", "DIV", "KIC", "STA", "HAN"};

const char NR_OUTFIELD_STATS = 8;
const char OUTFIELD_STATS[NR_OUTFIELD_STATS][4] = {"PAC", "SHO", "DEF", "PHY", "STA", "DRI", "PAS", "AGG"};
/*
const unordered_map<string, int> DEF_STAT_RATIO = {{}};
const unordered_map<string, int> MID_STAT_RATIO = {{}};
const unordered_map<string, int> ATT_STAT_RATIO = {{}};
*/
const string boolToString(bool);
istream& operator >>(istream&, unordered_map<string, double>&);
//Cand o echipa este generata jucatorii for fii tot generati automat random(nu chiar)
class Human{
protected:
    string name, nationality;
    unsigned short age;
    double wage;
public:
    void Age(){age ++;}
    Human(){
        name = nationality = "";
        age = wage = 0;
    }
    Human(const string& name, unsigned short age, double wage, const string& nationality) : 
        name(name), age(age), wage(wage), nationality(nationality){}

    unsigned short getAge(){return age;}
    double getWage(){return wage;}
    const string& getNationality(){return nationality;}
    const string& getName(){return name;}

    void setWage(double wage){this->wage = wage;}
    //For requirements
    void setAge(unsigned short age){this->age = age;}

    friend ostream& operator <<(ostream&, const Human&);
    friend istream& operator >>(istream& op, Human& H);
};

//Maybe use pointers to const objects to avoid operator= issues
//Will implement functions to compare goals, assists, yellowC, redC
class Player : public Human{
protected:
    unsigned short s_goals = 0, s_assists = 0, s_yellow_cards = 0, s_red_cards = 0, form = 0;
    unsigned short shirt_nr, potential_OVR;
    double stamina = (double)MAX_STAMINA;
    string position;
    unordered_map<string, double> stats;
    bool transfer_eligible = true;
    bool red_carded = false;

public:
    void train();
    void rest(){this->stamina = max((double)MAX_STAMINA, this->stamina + REST_STAMINA_PLUS);};
    void score(){ s_goals ++;};
    void assist(){ s_assists ++;};
    void resetSeasonStats();
    double getOVR();
    double getPrice();
    Player(){
        for (auto& s : OUTFIELD_STATS)
            stats[s] = 0;
        position = "";
        shirt_nr = 0;
        potential_OVR = 0;
    }
    Player(const string& name, unsigned short age, double wage, const string& nationality, const unordered_map<string, double>& stats, unsigned short shirt_nr, unsigned short potential_OVR, const string& position) 
    : Human(name, age, wage, nationality), stats(stats), shirt_nr(shirt_nr), 
      position(position), potential_OVR(potential_OVR){}

    unsigned short getGoals(){return s_goals;}
    unsigned short getAssists(){return s_assists;}
    unsigned short getYCard(){return s_yellow_cards;}
    unsigned short getRCard(){return s_red_cards;}
    unsigned short getForm(){return form;}
    unsigned short getShirt(){return shirt_nr;}
    unsigned short getPotential(){return potential_OVR;}

    double getStamina(){return stamina;}

    const unordered_map<string, double>& getStats(){return stats;}

    bool verifTransferEligible(){return transfer_eligible;}
    bool verifRedCarded(){return red_carded;}

    void setShirt(unsigned short shirt_nr){this->shirt_nr = shirt_nr;}
    void setStamina(unsigned short stamina){this->stamina = stamina;}
    void changeTranferEligible(){this->transfer_eligible = !this->transfer_eligible;}
    void changeRedCarded(){this->red_carded = !this->red_carded;}

    friend ostream& operator <<(ostream&, Player&);
    friend istream& operator >>(istream&, Player&);
    friend bool operator <(Player&, Player&);
    friend bool operator >(Player&, Player&);
    friend bool operator ==(Player&, Player&);
    friend bool operator !=(Player&, Player&);
};


/*
class Goalkeeper : public Player{
public:
    //Arithmetic sum of stats
    double calculateOVR(){
        double OVR = 0;
        for (auto& x : GK_STATS){
            OVR += this->stats[x];
        }
        return OVR / NR_GK_STATS;
    }

    //Each stat gets a plus between 0 and 1
    void train(){
        for (auto& x : GK_STATS){
            this->stats[x] += rand() / RAND_MAX;
        }
    }
};
class Defender : public Player{
    //Train pt fiecare pozitie
};

class Midfielder : public Player{
    //Train pt fiecare pozitie
};

class Attacker : public Player{
    //Train pt fiecare pozitie
};
*/
//Will implement functions to compare budget
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
public:
    Team(const string& name) : name(name){
        generateGoalkeepers(this);
        generateDefenders(this);
        generateMidfielders(this);
        generateAttackers(this);
        generateBudget(this);
    }
    Team(const string& name, const vector<Player*>& Players, double budget) : name(name), Players(Players), budget(budget){}
    Team(const Team& other){*this = other;}
    ~Team();
    void operator =(const Team& other){name = other.name, budget = other.budget, Players = other.Players, points = other.points;}

    unsigned short getChemestry();//Simplified for the moment
    double getBudget(){return budget;}
    const string& getName(){return name;}
    const vector<Player*>& getPlayers(){return Players;}

    void setBudget(double budget){this->budget = budget;}

    void buyPlayer(Player*);//Not implemented yet
    void sellPlayer(Player*);//Not implemented yet

    void resetSeasonStats();
    void addPoints(M_RESULT r);

    friend ostream& operator <<(ostream& op, Team&);
    friend bool operator <(const Team&, const Team&);
    friend bool operator >(const Team&, const Team&);
    friend bool operator ==(const Team&, const Team&);
    friend bool operator !=(const Team&, const Team&);
};

class Season{
private:
    vector<Team> Teams;
    unsigned short stage = 1;
    bool tranfer_window = false;
public:
    Season(); //Generates teams, first season
    Season(vector<Team> :: iterator start, vector<Team> :: iterator end);
    ~Season();

    //Using these three instead of getTeams()
    vector<Team> :: iterator getTeamsBegin(){return Teams.begin();};
    vector<Team> :: iterator getTeamsEnd(){return Teams.end();};
    unsigned short getTeamsNr(){return Teams.size();};

    unsigned short getStage(){return stage;}
    bool verifTWindowActive(){return tranfer_window;}

    void setStage(unsigned short stage){this->stage = stage;}
    //Only for requirements
    void setTeams(const vector<Team>& Teams){this->Teams = Teams;}
    void changeTranferWindow(){this->tranfer_window = !this->tranfer_window;}
    //Reset stages, points, season_stats
    void resetSeason();

    friend ostream& operator <<(ostream& op, Season&);
};

class League{
private:
    vector<Season*> Seasons;
public:
    //Also generates the first season
    League(){
        Seasons.reserve(DEFAULT_TEAM_NR / RELEGATED_NR);
        Seasons.push_back(new Season());
    }

    //Only for testing
    League(vector<Season*>& Seasons) : Seasons(Seasons){}

    ~League();
    void newSeason();
    friend ostream& operator <<(ostream& op, League&);
};

int main(){
    ifstream fin("input_aux.txt");
    ofstream fout("output_aux.txt");
    vector<Player*> AUX_TEAM;
    for (int i = 0; i < 5; i++){
        AUX_TEAM.push_back(new Player);
        fin >> *(AUX_TEAM[i]);
        fout << *(AUX_TEAM[i]) << endl;
    }

    Team Team1("team1", AUX_TEAM, 50000);
    fout << Team1 << endl;

}

void League :: newSeason(){
    //Creates new season with all teams except last 3(relegated)
    vector<Team> :: iterator T_start = Seasons.back()->getTeamsBegin();
    vector<Team> :: iterator T_end = Seasons.back()->getTeamsEnd();
    unsigned int T_nr = Seasons.back()->getTeamsNr();

    //Sorts teams by points and copies all teams except relegated ones
    sort(T_start, T_end);
    Seasons.push_back(new Season(T_start, T_start + T_nr - RELEGATED_NR));
    Seasons.back()->resetSeason();
}

//Simplified for tests
double Player :: getOVR(){
    double OVR = 0;
    for (auto& x : stats){
        OVR += x.second;
    }
    return OVR / stats.size();

}

//To be implemented
double Player :: getPrice(){
    return getOVR() * 10000;
}
//Simplied for tests
void Player :: train(){
    for (auto& x : stats){
        this->stats[x.first] += rand() / RAND_MAX;
    }
}
//To be implemented
unsigned short Team :: getChemestry(){
    return MAX_CHEM;
}

Season :: Season(vector<Team> :: iterator start, vector<Team> :: iterator end){
    Teams.reserve(end - start);

    while (start <= end){
        Teams.push_back(*start);
        start++;
    }
}

void Season :: resetSeason(){
    this->stage = 1;
    for (auto& t : Teams)
        t.resetSeasonStats();
}

void Team :: resetSeasonStats(){
    this->points = 0;
    for (auto& p : this->Players)
        p->resetSeasonStats();
}

void Player :: resetSeasonStats(){
    s_goals = s_assists = s_yellow_cards = s_red_cards = form = 0;
    stamina = (double)MAX_STAMINA;
    transfer_eligible = true;
    red_carded = false;
}

League :: ~League(){
    for (auto& s : Seasons)
        delete s;
    Seasons.clear();
    Seasons.shrink_to_fit();

}

Season :: ~Season(){
    for (auto& t : Teams)
        t.~Team();
    
    Teams.clear();
    Teams.shrink_to_fit();
}

Team :: ~Team(){
    for (auto& p :Players)
        delete p;

    this->Players.clear();
    Players.shrink_to_fit();
}

void Team :: addPoints(M_RESULT r){
    this->points += r;
}

ostream& operator <<(ostream& op, League& L){
    for (int i = 0; i < L.Seasons.size(); ++i)
        op << "Season " << i << ": \n" 
        << L.Seasons[i] << "\n\n";
    
    return op;
}

ostream& operator <<(ostream& op, Season& S){
    op << "Stage:" << S.stage <<"\nTranfer window active: " 
    
    << boolToString(S.tranfer_window) << "\nTeams:\n";

    sort(S.Teams.begin(), S.Teams.end());
    for (auto& t : S.Teams)
        op << t << endl << endl;
    
    return op;
}

ostream& operator <<(ostream& op, Team& T){
    op << "Name: " << T.name << "\nChemestry: " << T.getChemestry()
    << "\nBudget: " << T.budget
    << "\nPoints: " << T.points << "\n";
    
    return op;
}

//Not using const Player& because I can't access member functions with that 
ostream& operator <<(ostream& op, Player& P){
    op << (Human&)P << "\nPosition: " << P.position << "\nShirt: "
    << P.shirt_nr << "\nGoals: " << P.s_goals 
    << "\nAssists: " << P.s_assists << "\nYeallow Cards: " << P.s_yellow_cards
    <<"\nRed Cards: " << P.s_red_cards << "\nRed Carded: " << boolToString(P.red_carded)
    << "\nForm: " << P.form << "/10\nOVR: " << P.getOVR()
    << "\nPrice: " << P.getPrice() << "\n";

    return op;

}

ostream& operator <<(ostream& op, const Human& H){
    op << "Name: " << H.name << "\nAge: "
    << H.age <<"\nNationality: " << H.nationality
    << "\nWage: " << H.wage << "\n";
    
    return op;
}


istream& operator >>(istream& op, Player& P){
    op >> (Human&)P >> P.position >> P.shirt_nr >> P.potential_OVR >> P.stats;
    cout << P.name;

    return op;
}

istream& operator>>(istream& op, Human& H){
    op >> H.name >> H.age >> H.nationality >> H.wage;
    return op;
}

istream& operator >>(istream& op, unordered_map<string, double>& stats){
    for (auto& s : OUTFIELD_STATS){
       op >> stats[s];
    }
    return op;
}

bool operator <(const Team& T1, const Team& T2){
    return T1.points < T2.points;
}

bool operator >(const Team& T1, const Team& T2){
    return T1.points > T2.points;
}

bool operator ==(const Team& T1, const Team& T2){
    return T1.points == T2.points;
}

bool operator !=(const Team& T1, const Team& T2){
    return T1.points != T2.points;
}


bool operator <(Player& P1, Player& P2){
    return P1.getOVR() < P2.getOVR();
}

bool operator >(Player& P1, Player& P2){
    return P1.getOVR() > P2.getOVR();
}

bool operator ==(Player& P1, Player& P2){
    return P1.getOVR() == P2.getOVR();
}

bool operator !=(Player& P1, Player& P2){
    return P1.getOVR() != P2.getOVR();
}

const string boolToString(bool B){
    if (B)
        return "YES";
    else
        return "NO";
}

