#include <iostream>
#include <string>
#include <fmt/core.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <fstream>
#include "../classes/constants/Constants.h"
#include "../classes/teams/team/Team.h"
#include "../random_generated/player_generation/generate_player.h"
#include "../random_generated/team_generation/generate_team.h"
#include "../classes/shader/Shader.h"
#include "../classes/VBO/VBO.h"
#include "../classes/VAO/VAO.h"
#include "../classes/IBO/IBO.h"
#include "../classes/match/Match.h"
#include <chrono>

//Undefined reff to VAO

//Unfinished, main only for testing purposes
//External library Dasmig(see forked directory)
//If reading from file add invalidCountry, invalidAge, invalidPoz
//For the moment only using random_generated players and teams

//Assuming we are running from app directory
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                const GLchar* message, const void* userParam) {
    cout << "GL ERROR CALLBACK: " << "type: " << type << ", severity: " << severity << "\nmessage: " << message << '\n';
}

int main(){


    Constants :: init();

    //Initializing glfw
    if (!glfwInit()){
        cout << "Failed to initialize GLFW\n";
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    //Initializing window
    GLFWwindow* window = glfwCreateWindow(900, 600, "Hello World", NULL, NULL);
    if (!window){
        cout << "Failed to create window\n";
        glfwTerminate();
    }
    //Creating context for window
    glfwMakeContextCurrent(window);
    
    //Initializing glew
    if (glewInit() != GLEW_OK){
        cout << "Failed to initialize GLEW\n";
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    
    vector<string> team_names = Constants :: getTeamNames();
    unique_team t1(generateTeam(team_names)), t2(generateTeam(team_names));

    Match match(t1->getFirstTeam(), t2->getFirstTeam());
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        match.createField();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    
    // Constants :: init();
    // vector<string> team_names = Constants :: getTeamNames();
    // vector<unique_team> teams;
    // teams.reserve(100);
    // auto a = chrono :: high_resolution_clock::now();

    // for (int i = 0; i < 20; ++i)
    //     teams.push_back(unique_team(generateTeam(team_names)));
    
    // //test this
    // for (const auto& t : teams){
    //     shared_squad_split split_team = t->splitTeamPos();
    //     printSplitTeam(split_team);
    //     unique_first_team first_team = t->getFirstTeam();
    //     cout << first_team->getFormation() << ", "
    //          << first_team->getElevenTotalStats() << ", "
    //          << first_team->getSubsOVR() << '\n';
    //     cout << endl << endl;
    // }
    // auto b = chrono :: high_resolution_clock::now();

    // cout << "took " << duration_cast<chrono :: seconds>(b - a).count() << " seconds" <<  endl;

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

double getAvgOVR(const shared_squad& squad){
    double ovr = 0;
    for (const auto& p : squad)
        ovr += p->calculateOVR(p->getPosition());
    return ovr / squad.size();
}
void printSplitTeam(const shared_squad_split& split_team){
    for (const auto& p : split_team)
        cout << p.first << "(" << p.second.size() << "): " << getAvgOVR(p.second) << "\n";
}

void printStats(const unordered_map<string, double>& stats){
    vector<string> outfield_stats = Constants :: getStats("OUTFIELD");
    for (const auto& s : stats)
        if (find(outfield_stats.begin(), outfield_stats.end(), s.first )
            != outfield_stats.end())
        cout << s.first << ": " << s.second << '\n';
}
*/


