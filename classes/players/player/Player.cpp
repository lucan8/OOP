#include "Player.h"

void Player :: resetSeasonStats(){
    s_goals = s_assists = s_yellow_cards = s_red_cards = form = 0;
    stamina = (double)Constants :: getVal("MAX_STAMINA").value();
    transfer_eligible = true;
    red_carded = false;
}

istream& operator >>(istream& op, Player& P){
    op >> (Human&)P >> P.position >> P.shirt_nr >> P.potential_OVR >> P.stats;

    return op;
}

ostream& operator <<(ostream& op, const Player& P){
    op << (Human&)P << "\nPosition: " << P.position << "\nShirt: "
    << P.shirt_nr << "\nGoals: " << P.s_goals 
    << "\nAssists: " << P.s_assists << "\nYeallow Cards: " << P.s_yellow_cards
    <<"\nRed Cards: " << P.s_red_cards << "\nRed Carded: " << boolToString(P.red_carded)
    << "\nForm: " << P.form << "/10\nOVR: " << P.getOVR()
    << "\nPrice: " << P.getPrice() << "\n";

    return op;

}
