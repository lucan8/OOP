#pragma once
#include "../player_generation/generate_player.h"
#include "../../classes/teams/team/Team.h"

team_ptr generateTeam(vector<string>& available_names);
string generateTeamName(vector<string>& available_names);
squad_ptr generateOutfields();
squad_ptr generateGoalkeepers();
double generateBudget();
