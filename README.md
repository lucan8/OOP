# FKHS - Football King of the Hill Simulator

## This app will generate a league starting with 21 teams and will end with only the one who manages to survive

### Reasoning and Inspiration

This is a project for the **Object Oriented Programming** class and is inspired from the **FIFA games carrer mode**, main differences being that teams are "randomly generated" and that this app only simulates matches.

### Introduction

The app starts by generating the teams and players for the league, every week teams will play matches, train their players and rest them.
Mid season the **transfer window** emerges and teams can buy and sell players for a period of 4 weeks(4 matches). Mid season is also the time for **player potential** changes, the top half get potential boosts, the bottom half get nerfs. At the end of the season the last three teams are *relegated*(removed from the league) and so after a few seasons we will have our **KING TEAM**.

### App Features

- [x] generates players and teams "randomly"
- [ ] simulates matches between teams
- [ ] keeps the history of every season played so far
- [ ] eliminates after each season the last 3 teams in the league
- [ ] dynamic potential for the players
- [x] dynamic stats for players
- [ ] dynamic team determination before match

#### Implementation
<!--
```plantuml
@startuml FKHS class diagram
interface Printable{
    + print()
}
interface Readable{
    + read()
}
static class Constants{
    - values
    - stats
    - positions
    ...
    + init()
}
exception FileOpenException{
    - file_name
    + what()
}
exception InvalidPlayerType{
    - p_type
    + what()
}
class Human{
    # name
    # nationality
    # age
    # wage
    + Age()
}
abstract class Player{
    # position
    # stats
    # potential
    ...
    + train()
    + rest()
    + getOVR()
    + clone() = 0
    + initStats() = 0
}
class OutFieldPlayer{
    - s_goals, s_assists, s_blocks
    + clone()
    + initStats()
    + score()
    + block()
    + assist()
}
class Goalkeeper{
    - s_saves
    + clone()
    + initStats()
    + save()
}
class Team{
    - Players
    - points
    - Oponents
    - name
    - budget
    + playMatch()
    + buyPlayer()
    + sellPlayer()
    + trainplayers()
}
class Season{
    - curr_stage
    - Teams
    - bool transfer_window

    + simulateStage()
    + trainTeams()
    + restTeams()
}
class League{
    - Seasons
    - curr_season
    + newSeason()

}
Human <|-- Player
Player <|-- OutFieldPlayer
Player <|-- Goalkeeper
Team *-- Player
Season *-- Team
League *-- Season
note top of Printable: "Extended by every class"
note top of Readable: "Extended by every class"
note top of Constants: "Used to keep constant data from files"
@enduml
```
-->
