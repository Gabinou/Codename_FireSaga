cmake_minimum_required (VERSION 2.9)

set(CMAKE_C_STANDARD 99)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
cmake_policy(SET CMP0079 NEW)

OPTION(ENABLE_WARNINGS "Enable compilation warnings" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

message("CMAKE_C_FLAGS_DEBUG is ${CMAKE_C_FLAGS_DEBUG}")
message("CMAKE_C_FLAGS_RELEASE is ${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_C_FLAGS_RELWITHDEBINFO is ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message("CMAKE_C_FLAGS_MINSIZEREL is ${CMAKE_C_FLAGS_MINSIZEREL}")

message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
message("CMAKE_CXX_FLAGS_RELWITHDEBINFO is ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message("CMAKE_CXX_FLAGS_MINSIZEREL is ${CMAKE_CXX_FLAGS_MINSIZEREL}")

project("CodenameFireSaga")

include_directories(
	.
	include
	names
	scenes
	third_party
	third_party/physfs
	third_party/LodePNG
	third_party/tinymt
	third_party/stb
	third_party/minctest
	third_party/cJson
	third_party/cwalk
	third_party/tnecs
	${SDL2_INCLUDE_DIR}
)

file(GLOB FSM_SRC "src/*.c")
file(GLOB CJSON_SRC "third_party/cJson/*.c")
file(GLOB CWALK_SRC "third_party/cwalk/*.c")
file(GLOB TNECS_SRC "third_party/tnecs/*.c")
file(GLOB LODE_SRC "third_party/LodePNG/*.c")
file(GLOB PFS_SRC "third_party/physfs/*.c")
file(GLOB tinyMT_SRC "third_party/tinymt/*.c")

ADD_LIBRARY(cJSON-static STATIC ${CJSON_SRC})
ADD_LIBRARY(CWALK-static STATIC ${CWALK_SRC})
ADD_LIBRARY(tnecs-static STATIC ${TNECS_SRC})
ADD_LIBRARY(lodepng-static STATIC ${LODE_SRC})
ADD_LIBRARY(physfs-static STATIC ${PFS_SRC})
ADD_LIBRARY(tinyMT-static STATIC ${tinyMT_SRC})

ADD_LIBRARY(fortest-static STATIC ${FSM_SRC})

set(LIBRARY_STATIC cJSON-static CWALK-static tnecs-static lodepng-static physfs-static tinyMT-static)

if ($(CMAKE_C_COMPILER_ID) STREQUAL "MSVC")
	set(FLAGS_ERRORS "") 
	# set(FLAGS_ERRORS "/W4 /WX") 
elseif (${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
	set(FLAGS_ERRORS "") 
	# set(FLAGS_ERRORS "-Wall -Wextra -pedantic -Werror") 
elseif (${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
	set(FLAGS_ERRORS "") 
	# set(FLAGS_ERRORS "-DNDEBUG") 
endif()

if (ANDROID) 
	set(LIBRARY_SDL "-lSDL2 -lSDL2_image -lSDL2_ttf")
elseif (WIN32)
	set(LIBRARY_SDL "-lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf")
	# set(LIBRARY_SDL "-lSDL2 -lSDL2_image -lSDL2_ttf")
elseif (UNIX)
	set(LIBRARY_SDL "-lSDL2 -lSDL2_image -lSDL2_ttf")
endif ()

message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
if(${CMAKE_BUILD_TYPE} MATCHES "Release")
message("MATCHES Release")
	add_compile_options(-O1 -D__FIRESAGA_RELEASE__ -DSDL_ASSERT_LEVEL=1)
elseif (${CMAKE_BUILD_TYPE} MATCHES Debug)
message("MATCHES Debug")
	add_compile_options(-O0 -g  -D__FIRESAGA_DEBUG__ -DSDL_ASSERT_LEVEL=2 -Wall)
endif()

set(FLAGS_ANDROID "`sdl2-config --libs --cflags`")
message("FLAGS_ANDROID is ${FLAGS_ANDROID}")


set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory (src)

enable_testing ()
add_subdirectory (test)
add_test (${PROJECT_NAME} MyTest COMMAND Test)