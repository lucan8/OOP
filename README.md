# FKHS - Football King of the Hill Simulator

## Reasoning and Inspiration

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

### What I managed to make

- Create "fair" rules for player and team generation.
- Make a simple algorithm for selecting the first eleven and the bench
- Draw the pitch(texture), players(as circles with their team texture) and ball (circle with a ball texture)
- Draw the substitutes names and numbers(with stb_truetype)
- Implemnt moving, passing, marking, tackling and dribbling for players(shooting is also...there), each player trying to make the best decision given their opponents, teammates and their positions, whether they have the ball or not, the distance from goal and so on (no it does not use AI, it's a "home-made" algorithm I tought about myself)
- Implement simple algorithm for training players

### How I made it

I made a small framework over OpenGL to abstract away stuff like loading shaders, fonts, textures(and using them) drawing players, and overall drawing stuff

I also used external libraries for loading textures(stb_image), drawing characters(stb_truetype) and generating players(dasmig, which I also added changes too)