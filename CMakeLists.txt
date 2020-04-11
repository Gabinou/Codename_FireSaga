cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

OPTION(ENABLE_WARNINGS "Enable compilation warnings" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

project("CodenameFireSaga")

find_package(SDL2 REQUIRED)

include_directories(
	include
	third_party
	third_party/physfs
	third_party/LodePNG
	third_party/tinymt
	third_party/tinyxml2
	third_party/stb
	third_party/redxdev
	third_party/entityx
	third_party/entityx/tags
	third_party/entityx/help
	third_party/entityx/deps
	third_party/entityx/3rdparty
	${SDL2_INCLUDE_DIR}
)

file(GLOB FSM_SRC "src/*.cpp")
file(GLOB PFS_SRC "third_party/physfs/*.c")
file(GLOB tinyMT_SRC "third_party/tinymt/*.c")
file(GLOB tinyXML2_SRC "third_party/tinyxml2/*.cpp")
file(GLOB LODE_SRC "third_party/LodePNG/*.cpp")
file(GLOB_RECURSE ENTITYX_SRC "third_party/entityx/*.cc")

add_executable(${PROJECT_NAME} ${FSM_SRC})

ADD_LIBRARY(physfs-static STATIC ${PFS_SRC})
ADD_LIBRARY(LodePNG-static STATIC ${LODE_SRC})
ADD_LIBRARY(tinyMT-static STATIC ${tinyMT_SRC})
ADD_LIBRARY(tinyXML2-static STATIC ${tinyXML2_SRC})
ADD_LIBRARY(entityx-static STATIC ${ENTITYX_SRC})

target_link_libraries(${PROJECT_NAME}  ${SDL2_LIBRARY} LodePNG-static physfs-static tinyMT-static tinyXML2-static entityx-static -O2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)