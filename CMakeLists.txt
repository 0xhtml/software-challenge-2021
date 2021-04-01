cmake_minimum_required(VERSION 3.16)
project(software_challenge_2021)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -flto -march=native")

add_executable(main src/main.cpp src/Algorithm.cpp src/Evaluation.cpp src/GameState.cpp src/Hash.cpp src/Network.cpp)
add_subdirectory(pugixml)
target_link_libraries(main pugixml)
target_link_libraries(main pthread)

add_executable(tests tests/tests.cpp tests/test-GameState.cpp src/GameState.cpp src/Hash.cpp)

add_executable(masstest utils/masstest.cpp src/Algorithm.cpp src/Evaluation.cpp src/GameState.cpp src/Hash.cpp)
target_link_libraries(masstest pthread)

add_executable(bench utils/bench.cpp src/GameState.cpp src/Hash.cpp)
target_link_libraries(bench benchmark)
