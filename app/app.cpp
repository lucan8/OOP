#include <iostream>
#include "../classes/constants/Constants.h"
#include "../classes/league/League.h"
using namespace std;


template <typename T>
ostream& operator <<(ostream& op, vector<T*> v);

istream& operator >>(istream&, unordered_map<string, double>&);
const string boolToString(bool);


int main(){
    Constants :: init();  

}


ostream& operator <<(ostream& op, const League& L){
    for (int i = 0; i < L.EndedSeasons.size(); ++i)
        op << "Season " << i << ": \n" 
        << L.EndedSeasons[i] << "\n\n";
    
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
    for (auto& s : Constants :: getStats("OUTFIELD_STATS").value()){
       op >> stats[s];
    }
    return op;
}
bool comparePoints(const Team* T1, const Team* T2){
    return T1->getPoints() > T2->getPoints();
}

bool compareOVR(const Player* P1, const Player* P2){
    return P1->getOVR() > P2->getOVR();
}
const string boolToString(bool B){
    if (B)
        return "YES";
    else
        return "NO";
}

