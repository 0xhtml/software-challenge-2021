cmake_minimum_required(VERSION 3.16)
project(software_challenge_2021)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -flto -march=native")

add_executable(software_challenge_2021 src/main.cpp src/GameState.h src/GameState.cpp src/Constants.h src/Algorithm.cpp src/Algorithm.h src/Evaluation.cpp src/Evaluation.h src/Hash.cpp src/Hash.h src/Pieces.h src/Types.h src/Network.cpp src/Network.h src/Strings.h)

add_subdirectory(pugixml)
target_link_libraries(software_challenge_2021 pugixml)
