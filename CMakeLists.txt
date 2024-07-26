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
FetchContent_Declare (
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
    GIT_TAG glew-cmake-2.2.0
)


FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL  "")
set(GLFW_INSTALL OFF CACHE BOOL  "")

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(fmt)
FetchContent_GetProperties(glew)
FetchContent_MakeAvailable(glm)

if(NOT glew_POPULATED)
  FetchContent_Populate(glew)

  add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(fsim1    app/app.cpp 
                        classes/constants/Constants.cpp 
                        functions/functions.cpp
                        classes/human/Human.cpp
                        classes/players/player/Player.cpp
                        classes/players/outfield/OutfieldPlayer.cpp
                        classes/players/goalkeeper/Goalkeeper.cpp
                        classes/match_players/match_player/MatchPlayer.cpp
                        classes/match_players/match_outfield/MatchOutfield.cpp
                        classes/match_players/match_goalkeeper/MatchGoalkeeper.cpp
                        classes/first_team/FirstTeam.cpp
                        classes/Teams/Team/Team.cpp
                        random_generated/player_generation/generate_player.cpp
                        random_generated/team_generation/generate_team.cpp
                        classes/match/Match.cpp
                        classes/shader/Shader.cpp
                        classes/VBO/VBO.cpp
                        classes/IBO/IBO.cpp
                        classes/VAO/VAO.cpp
                        classes/VB_layout/VertexBufferLayout.cpp
                        classes/renderer/Renderer.cpp
                        classes/textures/Textures.cpp
                        vendor/stb_image.cpp
              )

target_link_libraries(fsim1 PRIVATE fmt)
target_link_libraries(fsim1 PRIVATE glfw)
target_link_libraries(fsim1 PRIVATE libglew_static)
target_link_libraries(fsim1 PRIVATE opengl32.lib)
target_link_libraries(fsim1 PRIVATE Gdi32.lib)
target_link_libraries(fsim1 PRIVATE User32.lib)
target_link_libraries(fsim1 PRIVATE glm::glm)