#pragma once
#include "../../human/Human.h"
//Also think about updating the form
//TO DO: Dynamic potential

class Player;
typedef Player* player_ptr;
typedef shared_ptr<Player> shared_player;
typedef unique_ptr<Player> unique_player;
typedef vector<shared_player> shared_squad;
typedef vector<unique_player> unique_squad;

class Player : public Human{
protected:
    uint16_t s_yellow_cards = 0, s_red_cards = 0, form = 0;
    uint16_t shirt_nr, remaining_sessions = Constants :: getVal("MAX_TRAIN_SESSIONS");
    double stamina = (double)Constants::getVal("MAX_STAMINA"), train_nerf, potential_OVR;
    string position;
    unordered_map<string, double> stats;

    bool transfer_eligible = true, red_carded = false;

    void readStats(istream&);
    virtual void initStats() = 0;

    void eliminateMaxes(vector<pair<string, uint16_t>>& weights) const;

    double getTrainPlus() const;
    void upgradeStat(const string& stat_name, double stat_plus);
public:
    void train();
    void setTrainNerf();
    void rest();

    //Players can play outside position as well
    double getOVR(const string& pos) const;
    double getPrice() const; //TO DO

    uint16_t getYCard() const{return s_yellow_cards;}
    uint16_t getRCard() const{return s_red_cards;}
    uint16_t getForm() const{return form;}
    uint16_t getShirt() const{return shirt_nr;}
    double getPotential() const{return potential_OVR;}

    double getStamina() const{return stamina;}
    const string& getPosition() const{return position;}

    const unordered_map<string, double>& getStats() const{return stats;}

    bool verifTransferEligible() const{return transfer_eligible;}
    bool verifRedCarded() const{return red_carded;}

    void setShirt(uint16_t shirt_nr){this->shirt_nr = shirt_nr;}
    void setStamina(double stamina){this->stamina = stamina;}
    void setPosition(const string& pos){this->position = pos;}

    void setStats(unordered_map<string, double> Stats);
    void setStat(const string& stat_name, double stat_val);

    void setPotential(double potential){this->potential_OVR = potential;}

    void changeTranferEligible(){this->transfer_eligible = !this->transfer_eligible;}
    void addYellowCard(){++s_yellow_cards;}
    void addRedCard(){++s_red_cards;}
    void changeRedCarded(){this->red_carded = !this->red_carded;}

    virtual player_ptr clone() const = 0;

    void print(ostream&) const override;
    virtual void printSeasonStats(ostream&) const;
    void printStats(ostream&) const;
    void printBasicInfo(ostream&) const;

    void printEssentials(ostream&) const;

    void read(istream&) override;
    virtual void resetSeasonStats();

    pair<string, string> minStats2() const;

    static bool compOVR(const player_ptr p1, const player_ptr p2){
        return p1->getOVR(p1->position) < p2->getOVR(p2->position);
    }

    virtual ~Player(){cout << "Player destroyed!\n";}
};