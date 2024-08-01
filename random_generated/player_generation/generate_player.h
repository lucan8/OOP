#pragma once
#include "../../classes/players/outfield/OutFieldPlayer.h"
#include "../../classes/players/goalkeeper/Goalkeeper.h"
#include "../../dasmig-src/dasmig/namegen.hpp"
//p_types: GK, OUTFIELD
//age types: YOUNG, MATURE, OLD

player_ptr generatePlayer(const string& p_type, const string& age_type);

uint16_t generateAge(const string& age_type);
float generatePotential(const string& age_type, float player_ovr);

string determinePreferedPos(const string& p_type, const player_ptr player);
unordered_map<string, float> generateStats(const string& p_type, const string& age_type);
void loadNamegenResources();
player_ptr createPlayer(const string& p_type);
