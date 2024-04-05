#include "Player.h"

void Player :: resetSeasonStats(){
    s_goals = s_assists = s_yellow_cards = s_red_cards = form = 0;
    stamina = (double)Constants :: getVal("MAX_STAMINA");
    transfer_eligible = true;
    red_carded = false;
}