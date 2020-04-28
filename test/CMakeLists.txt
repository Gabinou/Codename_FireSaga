include_directories (${TEST_SOURCE_DIR}/src)
add_executable (Test test.cpp)

target_link_libraries(Test ${SDL2_LIBRARY} -O2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)
