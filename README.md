# FKHS - Football King of the Hill Simulator

## This app will generate a league starting with 21 teams and will end with only the one who manages to survive

### Reasoning and Inspiration

This is a project for the **Object Oriented Programming** class and is inspired from the **FIFA games carrer mode**, main differences being that teams are "randomly generated" and that this app only simulates matches.

### Project Structure

```

app: Contains the main app file that just runs the game
classes: Contains all the classes used for this project(ex: Player, Team, Season etc)
dasmig-src: Forked directory used to generate the data
functions: Contains common functions used throught the project
random_generated: Contains functions used to generate teams and players
resources: Contains files used for constants, fonts, shaders and textures
vendor: Contains other external libraries

```

### What the app should do

The app starts by generating the teams and players for the league, every week teams will play matches, train their players and rest them.
Mid season the **transfer window** emerges and teams can buy and sell players for a period of 4 weeks(4 matches). Mid season is also the time for **player potential** changes, the top half get potential boosts, the bottom half get nerfs. At the end of the season the last three teams are *relegated*(removed from the league) and so after a few seasons we will have our **KING TEAM**.

### What the app does

The app currently only plays a match between two "randomly" generated teams. There is no timer, no corners, no outs, the teams are only fighting each other endlessly. The players can run, pass, dribble, tackle and mark.

### App Features

- [x] generates players and teams "randomly"
- [ ] simulates matches between teams
- [ ] keeps the history of every season played so far
- [ ] eliminates after each season the last 3 teams in the league
- [ ] dynamic potential for the players
- [x] dynamic stats for players
- [ ] dynamic team determination before match