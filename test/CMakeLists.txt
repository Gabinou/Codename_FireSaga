include_directories (${TEST_SOURCE_DIR}/src)
add_executable (Test test.cpp)

target_link_libraries(Test fortest-static ${SDL2_LIBRARY} LodePNG-static physfs-static tinyMT-static tinyXML2-static entityx-static -O2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)
