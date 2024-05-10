#include "../player_generation/generate_player.h"
#include "../../classes/team/Team.h"
unique_ptr<Team> generateTeam(vector<string>& available_names);
string generateTeamName(vector<string>& available_names);
vector<unique_ptr<Player>> generateOutfields();
vector<unique_ptr<Player>> generateGoalkeepers();
double generateBudget();
