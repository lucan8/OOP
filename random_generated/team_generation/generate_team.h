#pragma once
#include "../player_generation/generate_player.h"
#include "../../classes/teams/team/Team.h"

team_ptr generateTeam(std :: vector<std :: string>& available_names);
//Generates a team name and removes it from the available names
std :: string generateTeamName(std :: vector<std :: string>& available_names);
squad_ptr generateOutfields();
squad_ptr generateGoalkeepers();
float generateBudget();
