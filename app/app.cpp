#include <iostream>
#include <string>
#include <fmt/core.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <fstream>
#include "../classes/constants/Constants.h"
#include "../classes/teams/first_team/FirstTeam.h"
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
    const uint16_t window_width = Constants :: getVal("WINDOW_WIDTH"),
                   window_height = Constants :: getVal("WINDOW_HEIGHT");
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Hello World", NULL, NULL);
    
    if (!window){
        cout << "Failed to create window\n";
        glfwTerminate();
    }
    //Creating context for window
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //Initializing glew
    if (glewInit() != GLEW_OK){
        cout << "Failed to initialize GLEW\n";
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    //Blending test
    /*
    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    float positions1[] = {
        -0.3f, -0.3f,
         0.7f, -0.3f,
         0.7f,  0.7f,
        -0.3f,  0.7f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    const string vert_shader_path = (filesystem::current_path().parent_path() / "resources" / "shaders" / "vertex" / "").string(),
                 frag_shader_path = (filesystem::current_path().parent_path() / "resources" / "shaders" / "fragment" / "").string();

    //Loading the shaders
    Shader pitch_shader((vert_shader_path + "pitch.glsl"), (frag_shader_path + "pitch.glsl"));
    pitch_shader.bind();

    GLint u_location = pitch_shader.getUniformLocation("u_Color");
    assert(u_location != -1);

    VBO vbo(positions, 4 * 2 * sizeof(float), GL_STATIC_DRAW),
        vbo1(positions1, 4 * 2 * sizeof(float), GL_STATIC_DRAW);

    VertexBufferLayout layout;
    layout.addAttribute<float>(2);

    VAO vao, vao1;
    vao.addBuffer(vbo, layout);
    vao1.addBuffer(vbo1, layout);

    IBO ibo(indices, 6);
    */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vector<string> team_names = Constants :: getTeamNames();
    shared_team t1(generateTeam(team_names)), t2(generateTeam(team_names));
    Match match(t1, t2);

    int nr = 0;
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        match.draw();
        if (nr % 50 == 0)
            match.play();
        nr++;

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


