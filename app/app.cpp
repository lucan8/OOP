#include <iostream>
#include <string>
#include <fmt/core.h>
#include <filesystem>
#include <fstream>
#include "../classes/teams/match_team/MatchTeam.h"
#include "../random_generated/team_generation/generate_team.h"
//External library Dasmig(see forked directory)
//If reading from file add invalidCountry, invalidAge, invalidPoz
//For the moment only using random_generated players and teams

int main(){
    Constants :: init();
    vector<string> available_names = Constants::getTeamNames();
    try{
        shared_ptr<Team> t1 = generateTeam(available_names), t2 = generateTeam(available_names);
        MatchTeam m_1(move(t1->getBestFirstTeam()), t1), m_2(move(t2->getBestFirstTeam()), t2);         
        cout << "sus" << endl;
    } catch(exception& e){
        cout << e.what() << endl;
    }
}






/**
void training50(const vector<unique_ptr<Player>>& Players){
    for (auto& p : Players)
        for (int i = 0; i < 50; i++)
            p->train();
}
void printPlayers(const vector<unique_ptr<Player>>& players){
    for (auto& p : players)
        p->printEssentials(cout);
    cout << "------------------------" << '\n';
}



vector<unique_ptr<Player>> generatePlayers(uint16_t nr_players){
    vector<unique_ptr<Player>> Players;
    vector<string> age_types = Constants :: getAgeTypes();
    vector<string> p_types = Constants :: getPlayerTypes();

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<>  age_dist(0, age_types.size() - 1), 
                                p_types_dist(0, p_types.size() - 1);

    for (int i = 0; i < nr_players; ++i){
        try{
            const string& chosen_age_type = age_types.at(age_dist(gen));
            const string& chosen_p_type = p_types.at(p_types_dist(gen));
            Players.push_back(generatePlayer(chosen_p_type, chosen_age_type).value());
        } catch(out_of_range& e){
            cerr << "Error(main): chosen_type is invalid\n";
        } catch(bad_optional_access& e){
            cerr << "Error(main): generatePlayer failed\n";
        }
    }
    return Players;
}
vector<shared_ptr<Team>> generateTeams(vector<string>& t_names){
    vector<shared_ptr<Team>> teams;
    for (int i = 0; i < Constants :: getVal("DEFAULT_TEAM_NR"); ++i)
        teams.push_back(generateTeam(t_names));
    return teams;
}

double getAvgOVR(const shared_ptr<Team>& team){
    double ovr = 0;
    for (const auto& p : team->getPlayers())
        ovr += p->getOVR(p->getPosition());
    
    return ovr / team->getPlayers().size();
}

double getAvgPot(const shared_ptr<Team>& team){
    double pot = 0;
    for (const auto& p : team->getPlayers())
        pot += p->getPotential();
    
    return pot / team->getPlayers().size();
}

double getMaxOVR(const shared_ptr<Team>& team){
    double max_ovr = 0;
    for (const auto& p : team->getPlayers())
        max_ovr = max(max_ovr, p->getOVR(p->getPosition()));
    return max_ovr;
}

double getMaxPot(const shared_ptr<Team>& team){
    double max_pot = 0;
    for (const auto& p : team->getPlayers())
        max_pot = max(max_pot, p->getPotential());
    return max_pot;
}

double getMinOVR(const shared_ptr<Team>& team){
    double min_ovr = 99;
    for (const auto& p : team->getPlayers())
        min_ovr = min(min_ovr, p->getOVR(p->getPosition()));
    return min_ovr;
}

double getMinPot(const shared_ptr<Team>& team){
    double min_pot = 99;
    for (const auto& p : team->getPlayers())
        min_pot = min(min_pot, p->getPotential());
    return min_pot;
}


void printTeams(const vector<shared_ptr<Team>>& Teams){
    for (const auto& t : Teams){
        cout << t->getName() << ' ' << getAvgOVR(t) << ' ' << getAvgPot(t) << ' '
        << getMaxOVR(t) << ' ' << getMaxPot(t) << ' '
        << getMinOVR(t) << ' ' << getMinPot(t) << ' ' 
        << t->getPlayers().size() << '\n';
    }
    cout << "-------------------------------------------\n";
}

void trainTeams50(const vector<shared_ptr<Team>>& Teams){
    for (auto& t : Teams)
        training50(t->getPlayers());
}
*/





