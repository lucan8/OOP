cmake_minimum_required(VERSION 3.25)
project(FSIM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 10.1.1
)


FetchContent_MakeAvailable(fmt)

add_executable(fsim3    app/app.cpp 
                        classes/constants/Constants.cpp 
                        random_generated/player_generation/generate_player.cpp
                        random_generated/team_generation/generate_team.cpp
                        functions/functions.cpp
                        classes/human/Human.cpp
                        classes/players/player/Player.cpp
                        classes/players/outfield/OutFieldPlayer.cpp
                        classes/players/goalkeeper/Goalkeeper.cpp
                        classes/teams/team/Team.cpp
                        classes/first_eleven/FirstEleven.cpp
                        )

target_link_libraries(fsim3 PRIVATE fmt)
