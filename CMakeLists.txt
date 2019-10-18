cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_STANDARD 11)
project("Txt_Parser")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})