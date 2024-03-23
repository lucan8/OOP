#include <algorithm>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <vector>
using namespace std;

enum M_RESULT{WIN, DRAW, LOSS};
enum M_POINTS{W_POINTS, D_POINTS, L_POINTS};
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

const unordered_map<string, int> DEF_STAT_RATIO = {{}};
const unordered_map<string, int> MID_STAT_RATIO = {{}};
const unordered_map<string, int> ATT_STAT_RATIO = {{}};

template <typename T>
ostream& operator <<(ostream& op, vector<T*> v);

istream& operator >>(istream&, unordered_map<string, double>&);
const string boolToString(bool);

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
    Player();
    Player(const string& name, unsigned short age, double wage, const string& nationality, const unordered_map<string, double>& stats, unsigned short shirt_nr, unsigned short potential_OVR, const string& position) 
    : Human(name, age, wage, nationality), stats(stats), shirt_nr(shirt_nr), 
      position(position), potential_OVR(potential_OVR){}
    //Player(const Player& other){*this = other; cout << "Copied" << endl;}
    ~Player(){cout << "Player Destroyed" << '\n';}

    void train();
    void rest(){this->stamina = max((double)MAX_STAMINA, this->stamina + REST_STAMINA_PLUS);};
    void score(){ s_goals ++;};
    void assist(){ s_assists ++;};
    void resetSeasonStats();
    double getOVR() const;
    double getPrice() const;

    unsigned short getGoals() const{return s_goals;}
    unsigned short getAssists() const{return s_assists;}
    unsigned short getYCard() const{return s_yellow_cards;}
    unsigned short getRCard() const{return s_red_cards;}
    unsigned short getForm() const{return form;}
    unsigned short getShirt() const{return shirt_nr;}
    unsigned short getPotential() const{return potential_OVR;}

    double getStamina() const{return stamina;}

    const unordered_map<string, double>& getStats() const{return stats;}

    bool verifTransferEligible() const{return transfer_eligible;}
    bool verifRedCarded() const{return red_carded;}

    void setShirt(unsigned short shirt_nr){this->shirt_nr = shirt_nr;}
    void setStamina(unsigned short stamina){this->stamina = stamina;}
    void changeTranferEligible(){this->transfer_eligible = !this->transfer_eligible;}
    void changeRedCarded(){this->red_carded = !this->red_carded;}

    friend ostream& operator <<(ostream&, const Player&);
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
    Team(const Team& other){*this = other;}
    ~Team();
    void operator =(const Team& other);
    unsigned short getChemestry() const;//Simplified for the moment
    double getBudget() const{return budget;} 
    const string& getName() const{return name;}
    //Read only vector
    const vector<const Player*> getPlayers() const{return vector<const Player*>(Players.cbegin(), Players.cend());}

    void setBudget(double budget){this->budget = budget;}

    void playMatch(Team* other); //Simplified for tests
    void buyPlayer(Player*);//Not implemented yet
    void sellPlayer(Player*);//Not implemented yet

    void resetSeasonStats();
    void addPoints(M_POINTS r);
    void sortByOVR(){sort(Players.begin(), Players.end());}

    friend ostream& operator <<(ostream& op, const Team&);
    friend istream& operator >>(istream&, Team&);
    friend bool operator <(const Team&, const Team&);
    friend bool operator >(const Team&, const Team&);
    friend bool operator ==(const Team&, const Team&);
    friend bool operator !=(const Team&, const Team&);
};

class Season{
private:
    vector<Team*> Teams;
    unsigned short stage = 1;
    bool tranfer_window = false;
public:
    Season(); //Generates teams, first season, don't use this yet
    Season(vector<const Team*> :: const_iterator start, vector<const Team*> :: const_iterator end);
    ~Season();

    unsigned short getStage(){return stage;}
    //Read only vector
    const vector<const Team*> getTeams(){return vector<const Team*>(Teams.cbegin(), Teams.cend());}
    bool verifTWindowActive(){return tranfer_window;}

    void setStage(unsigned short stage){this->stage = stage;}
    void changeTranferWindow(){this->tranfer_window = !this->tranfer_window;}

    void simulateStage();
    //Reset stages, points, season_stats
    void resetSeason();
    void sortByPoints(){sort(Teams.begin(), Teams.end());}

    friend ostream& operator <<(ostream& op, const Season&);
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

    //Only for testing, not usefull for app
    League(vector<Season*>& Seasons) : Seasons(Seasons){}

    ~League();
    void newSeason();
    friend ostream& operator <<(ostream& op, const League&);
};

int main(){
    ifstream fin("input_aux.txt");
    ofstream fout("output_aux.txt");

    vector<Team*> Teams;
    Teams.reserve(3);
    for (int i = 0; i < 3; i++){
        Teams.push_back(new Team(5));
        fin >> *(Teams[i]);
        Teams[i]->getPlayers()[0]->getOVR();
    }
    vector<const Team*> Teams1(Teams.cbegin(), Teams.cend());
    Season S(Teams1.cbegin(), Teams1.cbegin() + 2);
    /*
    S.getTeams().back()->addPoints(WIN);
    Team2.addPoints(LOSS);

    Team2.sortByOVR();

    fout << Team1 << '\n' << Team2 << '\n';
    for (auto& p : Team2.getPlayers())
        fout << *p << endl;
    */

}

//Copies all teams from last season to new season except relegated ones
void League :: newSeason(){
    Seasons.back()->sortByPoints();

    //Avoiding copying the returned vector
    const vector<const Team*>& Teams = Seasons.back()->getTeams();

    Seasons.push_back(new Season(Teams.begin(), Teams.begin() + Teams.size() - RELEGATED_NR));
    Seasons.back()->resetSeason();
}

Season :: Season(vector<const Team*> :: const_iterator start, vector<const Team*> :: const_iterator end){
    Teams.reserve(end - start);
    
    //Creates new pointers to the same teams
    while (start <= end){
        Teams.push_back(new Team(**start));
        start++;
    }
}

//Simplified for tests
double Player :: getOVR() const{
    double OVR = 0;
    for (auto& x : stats){
        OVR += x.second;
    }
    return OVR / stats.size();

}

//To be implemented
double Player :: getPrice() const{
    return getOVR() * 10000;
}
//Simplied for tests
void Player :: train(){
    for (auto& x : stats){
        this->stats[x.first] += rand() / RAND_MAX;
    }
}
//Creates team of T_size with empty players
Team :: Team(unsigned short T_size){
    T_size = min(T_size, (unsigned short)MAX_TEAM_LENGTH);
    Players.reserve(T_size);

    for (int i = 0; i < T_size; i++)
        Players.push_back(new Player);
    name = "";
    budget = 0;
}

void Team :: operator=(const Team& other){
    name = other.name, budget = other.budget, points = other.points;
    copyPlayers(other.Players);
}

void Team :: copyPlayers(const vector<Player*>& Players){
    this->Players.reserve(Players.size());
    for (auto& p : Players)
        this->Players.push_back(new Player(*p));
}

//To be implemented
unsigned short Team :: getChemestry() const{
    return MAX_CHEM;
}

//Simplified for tests
void Season :: simulateStage(){
    for (int i = 0; i < Teams.size() / 2; i++)
        Teams[i]->playMatch(Teams[Teams.size() - i - 1]);
};

//Simplified for tests
void Team :: playMatch(Team* other){
    M_RESULT result = M_RESULT(rand() % 3);
    switch (result)
    {
        case WIN:
            this->addPoints(W_POINTS);
            break;
        case DRAW:
            this->addPoints(D_POINTS);
            other->addPoints(D_POINTS);
            break;
        case LOSS:
            other->addPoints(W_POINTS);
            break;
        default:
            break; //Throw error
    }
}

void Team :: addPoints(M_POINTS p){
    this->points += p;
}

Player :: Player(){
    for (auto& s : OUTFIELD_STATS)
        stats[s] = 0;

    position = "";
    shirt_nr = 0;
    potential_OVR = 0;

    cout << "Player Created!" << endl;
}


void Season :: resetSeason(){
    this->stage = 1;
    for (auto& t : Teams)
        t->resetSeasonStats();
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
        delete t;
    
    Teams.clear();
    Teams.shrink_to_fit();
}

Team :: ~Team(){
    for (auto& p :Players)
        delete p;

    this->Players.clear();
    Players.shrink_to_fit();
}


ostream& operator <<(ostream& op, const League& L){
    for (int i = 0; i < L.Seasons.size(); ++i)
        op << "Season " << i << ": \n" 
        << L.Seasons[i] << "\n\n";
    
    return op;
}

ostream& operator <<(ostream& op, const Season& S){
    op << "Stage:" << S.stage <<"\nTranfer window active: " 
    << boolToString(S.tranfer_window) << "\nTeams:\n";
    op << S.Teams;
    return op;
}

ostream& operator <<(ostream& op, const Team& T){
    op << "Name: " << T.name << "\nChemestry: " << T.getChemestry()
    << "\nBudget: " << T.budget
    << "\nPoints: " << T.points << "\n";
    
    return op;
}

//Not using const Player& because I can't access member functions with that 
ostream& operator <<(ostream& op, const Player& P){
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

template <typename T>
ostream& operator <<(ostream& op, vector<T*> v){
    for (auto& x : v)
        op << *x << '\n';
    return op;
}


istream& operator >>(istream& op, Team& T){
    op >> T.name >> T.budget;
    for (auto& p : T.Players)
        op >> *p;
    return op;
}

istream& operator >>(istream& op, Player& P){
    op >> (Human&)P >> P.position >> P.shirt_nr >> P.potential_OVR >> P.stats;

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

