#include "generate_team.h"
team_ptr generateTeam(vector<string>& available_names){
    squad_ptr gks = generateGoalkeepers(), outfields = generateOutfields(); 
    //Moving the gks at the end of the outfields vector
    outfields.insert(
                        outfields.end(),
                        make_move_iterator(gks.begin()), 
                        make_move_iterator(gks.end())
                    );
    return new Team(
                    generateTeamName(available_names),
                    move(toSharedVector(outfields)),
                    generateBudget()
                    );
}

string generateTeamName(vector<string>& available_names){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> name_dist(0, available_names.size() - 1);

    uint16_t name_index = name_dist(gen);
    string name = available_names[name_index];
    
    available_names.erase(available_names.begin() + name_index);
    return name;
}
squad_ptr generateOutfields(){
    squad_ptr outfields;
    random_device rd;
    mt19937 gen(rd());

    for (const auto& age_type : Constants :: getAgeTypes()){
        uniform_int_distribution<> nr_outfields_dist(
                                            Constants :: getAgeInfo(age_type, "MIN_NR"),
                                            Constants :: getAgeInfo(age_type, "MAX_NR")
                                            );
        for (uint16_t i = 0; i < nr_outfields_dist(gen); ++i)
            outfields.push_back(generatePlayer("OUTFIELD", age_type));
    }
    return outfields;                           
}

squad_ptr generateGoalkeepers(){
    squad_ptr gks;

    random_device rd;
    mt19937 gen(rd());

    vector<string> age_types = Constants :: getAgeTypes();

    //Chooses two of the age types and generates gks
    for (uint16_t i = 0; i < Constants :: getVal("DEFAULT_GK_NR"); ++i){
        uniform_int_distribution<> age_index_dist(0, age_types.size() - 1);
        uint16_t chosen_index = age_index_dist(gen);

        gks.push_back(generatePlayer("GK", age_types[chosen_index]));
        //Make sure the same age_type is not chosen again
        age_types.erase(age_types.begin() + chosen_index);
    }
    return gks;
}

//TO DO
double generateBudget(){
    return 0;
}