#include "generate_team.h"
#include "../../classes/constants/Constants.h"
#include "../../functions/functions.h"
#include <unordered_set>
#include <array>

using std :: string, std :: vector;
team_ptr generateTeam(vector<string>& available_names){
    squad_ptr gks = generateGoalkeepers(), outfields = generateOutfields(); 
    //Moving the gks at the end of the outfields vector
    outfields.insert(
                        outfields.end(),
                        std :: make_move_iterator(gks.begin()), 
                        std :: make_move_iterator(gks.end())
                    );
    return new Team(
                    generateTeamName(available_names),
                    std :: move(toSharedVector(outfields)),
                    generateBudget()
                    );
}

string generateTeamName(vector<string>& available_names){
    uint16_t name_index = Constants :: generateNaturalNumber(0, available_names.size() - 1);
    string name = available_names[name_index];
    
    available_names.erase(available_names.begin() + name_index);
    return name;
}


squad_ptr generateOutfields(){  
    squad_ptr outfields;

    uint16_t start_shirt = 2,
             end_shirt = 99;
    //Allthe numbers except 1 and 12(used for gks)
    vector<uint16_t> shirt_numbers(end_shirt - start_shirt + 1);
    std :: iota(shirt_numbers.begin(), shirt_numbers.end(), start_shirt);
    shirt_numbers.erase(shirt_numbers.begin() + 10);

    for (const auto& age_type : Constants :: getAgeTypes()){
        uint16_t nr_outfields = Constants :: generateNaturalNumber(Constants :: getAgeInfo(age_type, "MIN_NR"),
                                                                   Constants :: getAgeInfo(age_type, "MAX_NR"));
        for (uint16_t i = 0; i < nr_outfields; ++i){
            //Generating the player
            outfields.push_back(generatePlayer("OUTFIELD", age_type));
            //Choosing a random shirt number
            uint16_t shirt_index = Constants :: generateNaturalNumber(0, shirt_numbers.size() - 1);
            outfields.back()->setShirt(shirt_numbers[shirt_index]);
            //Removing the chosen shirt number
            shirt_numbers.erase(shirt_numbers.begin() + shirt_index);
        }
    }
    return outfields;                           
}


squad_ptr generateGoalkeepers(){
    squad_ptr gks;
    vector<string> age_types = Constants :: getAgeTypes();

    //Chooses two of the age types and generates gks
    for (uint16_t i = 0; i < Constants :: getVal("DEFAULT_GK_NR"); ++i){
        uint16_t chosen_index = Constants :: generateNaturalNumber(0, age_types.size() - 1);

        gks.push_back(generatePlayer("GK", age_types[chosen_index]));
        //Make sure the same age_type is not chosen again
        age_types.erase(age_types.begin() + chosen_index);
    }
    //Setting the shirts for the gks
    gks[0]->setShirt(1);
    gks[1]->setShirt(12);
    return gks;
}

//TO DO
float generateBudget(){
    return 0;
}