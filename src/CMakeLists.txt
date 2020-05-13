
add_executable(${PROJECT_NAME} ${FSM_SRC})

ADD_LIBRARY(physfs-static STATIC ${PFS_SRC})
ADD_LIBRARY(LodePNG-static STATIC ${LODE_SRC})
ADD_LIBRARY(tinyMT-static STATIC ${tinyMT_SRC})
ADD_LIBRARY(tinyXML2-static STATIC ${tinyXML2_SRC})
ADD_LIBRARY(entityx-static STATIC ${ENTITYX_SRC})
ADD_LIBRARY(cjson-static STATIC ${CJSON_SRC})
ADD_LIBRARY(fortest-static STATIC ${FSM_SRC})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} LodePNG-static physfs-static tinyMT-static tinyXML2-static entityx-static cjson-static -O2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)
# target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} LodePNG-static physfs-static tinyMT-static tinyXML2-static entityx-static -O2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -mwindows)