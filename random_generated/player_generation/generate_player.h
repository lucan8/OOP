#pragma once
#include "../../classes/players/outfield/OutFieldPlayer.h"
#include "../../classes/players/goalkeeper/Goalkeeper.h"
#include "../../dasmig-src/dasmig/namegen.hpp"
//p_types: GK, OUTFIELD
//age types: YOUNG, MATURE, OLD

player_ptr generatePlayer(const std :: string& p_type, const std :: string& age_type);

uint16_t generateAge(const std :: string& age_type);
float generatePotential(const std :: string& age_type, float player_ovr);

std :: string determinePreferedPos(const std :: string& p_type, const player_ptr player);
std :: unordered_map<std :: string, float> generateStats(const std :: string& p_type, const std :: string& age_type);
void loadNamegenResources();
player_ptr createPlayer(const std :: string& p_type);
