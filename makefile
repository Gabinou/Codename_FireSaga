
BUILD_PATH = build\
!if "$(SOURCE_PATH)" == ""
SOURCE_PATH = ..
!endif
!if "$(BUILD_PATH)" == ""
BUILD_PATH = ..
!endif

CXX	= g++
LINK = link
CFLAGS_BARE = -I$(BUILD_PATH)\include \
              -I$(BUILD_PATH)\build \
              -I$(SOURCE_PATH)\build \
              -I$(SOURCE_PATH)\include
              
CXXFLAGS_BARE = $(CFLAGS_BARE)
build/main.exe:src/main.c
    $(CXX) src/main.c  -o build/main.exe -I include/