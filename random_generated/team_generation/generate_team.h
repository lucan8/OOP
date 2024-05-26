#pragma once
#include "../player_generation/generate_player.h"
#include "../../classes/teams/team/Team.h"

shared_ptr<Team> generateTeam(vector<string>& available_names);
string generateTeamName(vector<string>& available_names);
vector<shared_ptr<Player>> generateOutfields();
vector<shared_ptr<Player>> generateGoalkeepers();
double generateBudget();
