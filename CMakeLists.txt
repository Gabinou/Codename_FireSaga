cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_STANDARD 11)
project("FireSagaMaker")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)

include_directories(include)
include_directories(ECS)
include_directories(${SDL2_INCLUDE_DIR})


file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}  ${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)