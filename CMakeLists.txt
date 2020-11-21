project(software_challenge_2021)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -mtune=skylake")
add_executable(software_challenge_2021 src/main.cpp src/GameState.h src/GameState.cpp src/Constants.h src/Algorithm.cpp src/Algorithm.h)
