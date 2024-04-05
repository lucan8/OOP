#pragma once
#include "../human/human.h"

class Player : public Human{
protected:
    unsigned short s_goals = 0, s_assists = 0, s_yellow_cards = 0, s_red_cards = 0, form = 0;
    unsigned short shirt_nr, potential_OVR;
    double stamina = (double)Constants::getVal("MAX_STAMINA");
    string position;
    unordered_map<string, double> stats;
    bool transfer_eligible = true;
    bool red_carded = false;

public:
    Player();
    Player(const string& name, unsigned short age, double wage, const string& nationality, const unordered_map<string, double>& stats, unsigned short shirt_nr, unsigned short potential_OVR, const string& position) 
    : Human(name, age, wage, nationality), stats(stats), shirt_nr(shirt_nr), 
      position(position), potential_OVR(potential_OVR){}
    ~Player(){cout << "Player Destroyed" << '\n';}

    virtual void train();
    void rest(){this->stamina = max((double)Constants::getVal("MAX_STAMINA"), this->stamina + Constants::getVal("REST_STAMINA_PLUS"));}
    virtual void score(){ s_goals ++;};
    virtual void assist(){ s_assists ++;};
    void resetSeasonStats();
    virtual double getOVR() const;
    virtual double getPrice() const;

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
};