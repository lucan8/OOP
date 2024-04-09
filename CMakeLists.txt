cmake_minimum_required(VERSION 3.25)
project(FSIM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 10.2.1
)

FetchContent_MakeAvailable(fmt)

add_executable(fsim2 classes/constants/Constants.cpp app/app.cpp )

target_link_libraries(fsim2 PRIVATE fmt::fmt)
