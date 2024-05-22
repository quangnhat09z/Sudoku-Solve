

cmake_minimum_required(VERSION 3.27.8)
project(myfirstapp)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_INCLUDE_DIR include)
set(SDL2_LIB_DIR lib/x64)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(myfirstapp main.cpp)
target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_mixer)


