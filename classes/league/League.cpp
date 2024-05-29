#include "League.h"

//Copies all teams from last season to new season except relegated ones
void League :: newSeason(){
    this->ActiveSeason->sortByPoints();
    this->EndedSeasons.push_back(move(ActiveSeason));
    this->ActiveSeason = make_unique<Season>();
}
