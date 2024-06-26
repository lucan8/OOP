#pragma once
#include "../../classes/players/outfield/OutFieldPlayer.h"
#include "../../classes/players/goalkeeper/Goalkeeper.h"
#include "../../classes/exceptions/InvalidPlayerType.h"
#include "../../dasmig-src/dasmig/namegen.hpp"
//types: GK, OUTFIELD
//age types: YOUNG, MATURE, OLD

shared_ptr<Player> generatePlayer(const string& p_type, const string& age_type);

uint16_t generateAge(uint16_t min_age, uint16_t max_age);
double generatePotential(double min_pot, double max_pot);

string determinePreferedPos(const string& p_type, const shared_ptr<Player>& player);
unordered_map<string, double> generateStats(const vector<string>& stats_names, double min_pot, double max_pot);
void loadNamegenResources();
shared_ptr<Player> createPlayer(const string& p_type);
