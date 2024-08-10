#pragma once
#include "../../human/Human.h"
//Also think about updating the form
//TO DO: Dynamic potential

class Player;
typedef Player* player_ptr;
typedef shared_ptr<Player> shared_player;
typedef unique_ptr<Player> unique_player;
typedef vector<shared_player> shared_squad;
//vector of shared_player represented as map(for easier removal)
typedef unordered_map<uint16_t, shared_player> shared_squad_map;
//Usually split by player_type(or detailed player type)
typedef unordered_map<string, shared_squad> shared_squad_split;
typedef vector<unique_player> unique_squad;
typedef vector<player_ptr> squad_ptr;

class Player : public Human{
protected:
    uint16_t s_yellow_cards = 0, s_red_cards = 0, form = 0;
    uint16_t shirt_nr, remaining_sessions = Constants :: getVal("MAX_TRAIN_SESSIONS");
    float stamina = (float)Constants::getVal("MAX_STAMINA"), train_nerf, potential_OVR;
    string position;
    unordered_map<string, float> stats;

    bool transfer_eligible = true, red_carded = false;

    void readStats(istream&);
    virtual void initStats() = 0;

    void eliminateMaxes(vector<pair<string, uint16_t>>& weights) const;

    float getTrainPlus() const;
    void upgradeStat(const string& stat_name, float stat_plus);
public:
    void train();
    void setTrainNerf();
    void rest();

    //Players can play outside position as well
    float calculateOVR(const string& pos) const;
    float getPrice() const; //TO DO

    uint16_t getYCard() const{return s_yellow_cards;}
    uint16_t getRCard() const{return s_red_cards;}
    uint16_t getForm() const{return form;}
    uint16_t getShirt() const{return shirt_nr;}
    float getPotential() const{return potential_OVR;}

    float getStamina() const{return stamina;}
    const string& getPosition() const{return position;}

    const unordered_map<string, float>& getStats() const{return stats;}
    float getAttackingOVR() const;
    float getStat(const string& stat_name) const;

    bool verifTransferEligible() const{return transfer_eligible;}
    bool verifRedCarded() const{return red_carded;}

    void setShirt(uint16_t shirt_nr){this->shirt_nr = shirt_nr;}
    void setStamina(float stamina){this->stamina = stamina;}
    void setPosition(const string& pos){this->position = pos;}

    void setStats(unordered_map<string, float> Stats);
    void setStat(const string& stat_name, float stat_val);

    void setPotential(float potential){this->potential_OVR = potential;}

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
        return p1->calculateOVR(p1->position) < p2->calculateOVR(p2->position);
    }

    virtual ~Player(){}
};