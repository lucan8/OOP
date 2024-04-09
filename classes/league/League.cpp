#include "League.h"

//Copies all teams from last season to new season except relegated ones
void League :: newSeason(){
    const vector<const Team*>& Teams = ActiveSeason->getTeams();

    //Be careful with the last season
    EndedSeasons.push_back(ActiveSeason);
    ActiveSeason = new Season(Teams.begin(), Teams.begin() + Teams.size() - Constants :: getVal("RELEGATED_NR").value());
    ActiveSeason->resetSeason();
}


League :: ~League(){
    for (auto& s : EndedSeasons)
        delete s;
    EndedSeasons.clear();
    EndedSeasons.shrink_to_fit();

}

ostream& operator <<(ostream& op, const League& L){
    for (int i = 0; i < L.EndedSeasons.size(); ++i)
        op << "Season " << i << ": \n" 
        << L.EndedSeasons[i] << "\n\n";
    
    return op;
}
