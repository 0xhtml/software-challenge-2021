cmake_minimum_required(VERSION 3.16)
project(software_challenge_2021)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -flto -march=native")

add_executable(main src/main.cpp src/Algorithm.h src/Algorithm.cpp src/Constants.h src/Evaluation.h src/Evaluation.cpp src/GameState.h src/GameState.cpp src/Hash.h src/Hash.cpp src/Network.h src/Network.cpp src/Pieces.h src/Strings.h src/Types.h)
add_subdirectory(pugixml)
target_link_libraries(main pugixml)

add_executable(test src/test.cpp src/Algorithm.h src/Algorithm.cpp src/Constants.h src/Evaluation.h src/Evaluation.cpp src/GameState.h src/GameState.cpp src/Hash.h src/Hash.cpp src/Pieces.h src/Types.h)
