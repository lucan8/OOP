#pragma once
#include "../../human/Human.h"
//Also think about updating the form
//TO DO: Dynamic potential
class Player : public Human{
protected:
    uint16_t s_yellow_cards = 0, s_red_cards = 0, form = 0;
    uint16_t shirt_nr, potential_OVR, remaining_sessions = Constants :: getVal("MAX_TRAIN_SESSIONS");
    double stamina = (double)Constants::getVal("MAX_STAMINA"), train_nerf;
    string position;
    unordered_map<string, double> stats;

    bool transfer_eligible = true, red_carded = false;

    void readStats(istream&);
    virtual void initStats() = 0;

    void eliminateMaxes(vector<pair<string, uint16_t>>& weights) const;

    void setTrainNerf();
    double getTrainPlus() const;

    void upgradeStat(const string& stat_name, double stat_plus);
public:
    Player() { initStats();}
    Player(const Player& p)
    : Human(p), shirt_nr(p.shirt_nr), potential_OVR(p.potential_OVR), 
      position(p.position), stats(p.stats), remaining_sessions(p.remaining_sessions), 
      train_nerf(p.train_nerf){}

    void train();
    void rest();

    
    void Age();

    double getOVR() const;
    double getPrice() const; //TO DO

    uint16_t getYCard() const{return s_yellow_cards;}
    uint16_t getRCard() const{return s_red_cards;}
    uint16_t getForm() const{return form;}
    uint16_t getShirt() const{return shirt_nr;}
    uint16_t getPotential() const{return potential_OVR;}

    double getStamina() const{return stamina;}

    const unordered_map<string, double>& getStats() const{return stats;}

    bool verifTransferEligible() const{return transfer_eligible;}
    bool verifRedCarded() const{return red_carded;}

    void setShirt(uint16_t shirt_nr){this->shirt_nr = shirt_nr;}
    void setStamina(uint16_t stamina){this->stamina = stamina;}
    void changeTranferEligible(){this->transfer_eligible = !this->transfer_eligible;}
    void changeRedCarded(){this->red_carded = !this->red_carded;}

    virtual unique_ptr<Player> clone() const = 0;

    void print(ostream&) const override;
    virtual void printSeasonStats(ostream&) const;
    void printStats(ostream&) const;
    void printBasicInfo(ostream&) const;

    void read(istream&) override;

    virtual ~Player() = 0;
};