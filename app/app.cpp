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
#include <chrono>

//Undefined reff to VAO

//Unfinished, main only for testing purposes
//External library Dasmig(see forked directory)
//If reading from file add invalidCountry, invalidAge, invalidPoz
//For the moment only using random_generated players and teams

#define shader_path filesystem::current_path().parent_path() / "resources" / "shaders"
const float triangle_offset = 0.02f;

players_coords normalize(const players_coords& p_coords){
    players_coords normalized_coords;
    float startx = -1.0;
    float starty = 1.0;

    float unity_x = 1.0 / (Constants :: getVal("GOAL_LINE_LENGTH") / 2);
    float unity_y = 1.0 / (Constants :: getVal("TOUCHLINE_LENGTH") / 2);

    for (const auto& [pos, coords] : p_coords){
        Coordinates normalized(startx + coords.x * unity_x, starty - unity_y *coords.y);
        normalized_coords[pos] = normalized;
    }
    return normalized_coords;
}

players_coords changeSide(const players_coords& p_coords){
    players_coords new_coords;
    float max_x = Constants :: getVal("GOAL_LINE_LENGTH");
    float max_y = Constants :: getVal("TOUCHLINE_LENGTH");

    for (const auto& [pos, coords] : p_coords)
        new_coords[pos] = Coordinates(max_x - coords.x, max_y - coords.y);
    return new_coords;
}


//Make triangle from the player's coordinates depending on the half
float* getTriangle(const Coordinates& p_coords, bool second_half){
    //triangl if the player is in second half(grows upwards)
    if (second_half)
        return new float[6]{
            p_coords.x, p_coords.y,
            p_coords.x - triangle_offset, p_coords.y + triangle_offset,
            p_coords.x + triangle_offset, p_coords.y + triangle_offset
        };
    //triangle if the player is in first half(grows downwards)
    return new float[6]{
        p_coords.x, p_coords.y,
        p_coords.x - triangle_offset, p_coords.y - triangle_offset,
        p_coords.x + triangle_offset, p_coords.y - triangle_offset
    };
}


void addTeamPositions(float* positions, int& positions_size,
                      const players_coords& team_coords, bool second_half){
    for (const auto& [pos, coords] : team_coords){
        float* triangle = getTriangle(coords, second_half);
        memcpy(positions + positions_size, triangle, 6 * sizeof(float));
        positions_size += 6;
        delete[] triangle;
    }
}

int main(){
    Constants :: init();
    const vector<string> formations = Constants :: getFormationsNames();
    const players_coords& team1 = Constants :: getPlayersCoords("4-4-2");
    players_coords normalized_team1 = normalize(team1), normalized_team2 = normalize(changeSide(team1));

    //Initializing glfw
    if (!glfwInit()){
        cout << "Failed to initialize GLFW\n";
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Initializing window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
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

    //8 for pitch, 4 middle line,  6(triangle) * 22 for players
    float positions[8 + 4 + 6 * 22] = {
        //Green square
        -1.0f, -1.0f,
        -1.0f, 1.0f,
         1.0f, 1.0f,
         1.0f, -1.0f,

         //Middle line
         -1.0f, 0.0f,
          1.0f, 0.0f,
    };
    int positions_size = 12;
    addTeamPositions(positions, positions_size, normalized_team1, true);
    addTeamPositions(positions, positions_size, normalized_team2, false);

    GLuint indices[6] = {
        //For square(and triangle)
        0, 1, 2,
        //Used only for square
        2, 3, 0,
    };
    VAO vao;
    vao.bind();
    //Already bound on creation
    VBO vbo(positions, sizeof(positions));
    IBO ibo(indices, 6 * sizeof(GLuint));

    Shader pitch_shader((shader_path / "vertex_shader.glsl").string(),
                    (shader_path / "fragment_shader.glsl").string()),
           middle_line_shader((shader_path / "vertex_shader.glsl").string(),
                    (shader_path / "fragment_shader2.glsl").string()),
           player_shader((shader_path / "vertex_shader.glsl").string(),
                    (shader_path / "fragment_shader1.glsl").string());
    
    while (!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        //Drawing the pitch
        pitch_shader.use();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        //Drawing the middle line
        middle_line_shader.use();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(8 * sizeof(float)));
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr);
        

        //Drawing the players
        player_shader.use();
        for (int i = 0; i < Constants :: getVal("MATCH_TEAM_SIZE") * 2; ++i){
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)((8 + 4 + i * 6) * sizeof(float)));
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        }

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


