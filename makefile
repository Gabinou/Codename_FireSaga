

CC := gcc  # on Android gcc is alias for clang
$(info $$CC [$(CC)])

BUILD_TYPE :=Debug# Debug, Release
$(info $$BUILD_TYPE [$(BUILD_TYPE)])

DIR_INCLUDE = include
DIR_NAMES = names
DIR_SCRIPTS = scenes
DIR_CJSON = third_party/cJson
DIR_CWALK = third_party/cwalk
DIR_TNECS = third_party/tnecs
DIR_LODEPNG = third_party/LodePNG
DIR_PHYSFS = third_party/physfs
DIR_MINCTEST = third_party/minctest
DIR_STB = third_party/stb
DIR_THREAD = third_party/thread
DIR_TINYCTHREAD = third_party/TinyCThread
DIR_TINYMT = third_party/tinymt
DIR_UTF8 = third_party/utf8
DIR_BUILD = build
DIR_TEST = test

DIRS_THIRD = $(DIR_CJSON) $(DIR_CWALK) $(DIR_TNECS) $(DIR_LODEPNG) $(DIR_PHYSFS) $(DIR_MINCTEST) $(DIR_STB) $(DIR_THREAD) $(DIR_TINYCTHREAD) $(DIR_TINYMT) $(DIR_UTF8)
LIBS_THIRD = -l_flecs -l_tnecs -l_cjson -l_cwalk -l_lodepng -l_physfs -l_tinymt
# LIBS_THIRD = -l_flecs -l_cjson -l_cwalk -l_lodepng -l_physfs -l_tinymt
# LIBS_THIRD = -l$(DIR_BUILD)/lib_cjson.a -l$(DIR_BUILD)/lib_cwalk.a -l$(DIR_BUILD)/lib_flecs.a -l$(DIR_BUILD)/lib_lodepng.a -l$(DIR_BUILD)/lib_physfs.a -l$(DIR_BUILD)/lib_tinymt.a
# LIBS_THIRD = $(DIR_BUILD)/lib_cjson.a $(DIR_BUILD)/lib_cwalk.a $(DIR_BUILD)/lib_flecs.a $(DIR_BUILD)/lib_lodepng.a $(DIR_BUILD)/lib_physfs.a $(DIR_BUILD)/lib_tinymt.a
INCLUDE_ALL = -I. -I${DIR_INCLUDE} -I${DIR_NAMES} -I${DIR_SCRIPTS} -I${DIR_CJSON} -I${DIR_CWALK} -I${DIR_FLECS} -I${DIR_TNECS} -I${DIR_LODEPNG} -I${DIR_PHYSFS} -I${DIR_MINCTEST} -I${DIR_STB} -I${DIR_THREAD} -I${DIR_TINYCTHREAD} -I${DIR_TINYMT} -I${DIR_UTF8} -I${DIR_TEST} -L${DIR_BUILD}

# OS AND Processor detection 
ifeq ($(OS),Windows_NT)
    OS_FLAG := WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        PROCESSOR_FLAG = AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            PROCESSOR_FLAG := AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            PROCESSOR_FLAG := IA32
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        OS_FLAG := LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        PROCESSOR_FLAG := OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        PROCESSOR_FLAG := AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        PROCESSOR_FLAG := IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        PROCESSOR_FLAG := ARM
    endif
endif

$(info $$OS_FLAG is [${OS_FLAG}])
$(info $$PROCESSOR_FLAG is [${PROCESSOR_FLAG}])

ifeq ($(OS), Windows_NT)
	EXEC := CodenameFireSaga_make.exe
    TEST := Test.exe
	FLAGS_SDL := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
# 	FLAGS_SDL := -lSDL2 -lSDL2_image -lSDL2_ttf  -lgdi32 -ltcc1-64 -lfreetype-6 -ljpeg-9 -lpng16-16 -ltiff-5 -lwebp-7 -lzlib1 -lshell32 -lkernel32 -luser32 -ladvapi32 -lwin32k -lwin32spl -mscvrt # for tcc
else
	EXEC := CodenameFireSaga_make.bin
    TEST := Test.bin
	FLAGS_SDL := `sdl2-config --libs --cflags` -lSDL2 -lSDL2_image -lSDL2_ttf -lm
endif 
$(info $$EXEC [$(EXEC)])
$(info $$FLAGS_SDL [$(FLAGS_SDL)])

ifeq ($(BUILD_TYPE),Release)
	FLAGS_BUILD_TYPE := -O1 -D__FIRESAGA_RELEASE__ -DSDL_ASSERT_LEVEL=2
else ifeq ($(BUILD_TYPE),Debug)
	FLAGS_BUILD_TYPE := -O0 -g -D__FIRESAGA_DEBUG__ -DSDL_ASSERT_LEVEL=2
endif
$(info $$FLAGS_BUILD_TYPE [$(FLAGS_BUILD_TYPE)])

CFLAGS := ${INCLUDE_ALL} ${FLAGS_BUILD_TYPE} ${FLAGS_SDL} ${LIBS_THIRD}

SOURCES_THIRD :=  $(wildcard ${DIR_CJSON}/*.c) $(wildcard ${DIR_CWALK}/*.c) $(wildcard ${DIR_TNECS}/*.c) $(wildcard ${DIR_FLECS}/*.c) $(wildcard ${DIR_LODEPNG}/*.c) $(wildcard ${DIR_PHYSFS}/*.c) $(wildcard ${DIR_MINCTEST}/*.c) $(wildcard ${DIR_STB}/*.c) $(wildcard ${DIR_THREAD}/*.c) $(wildcard ${DIR_TINYMT}/*.c) $(wildcard ${DIR_UTF8}/*.c) 
SOURCES_FIRESAGA := $(wildcard src/*.c)
SOURCES_FIRESAGA_NOMAIN := $(filter-out src/main.c, ${SOURCES_FIRESAGA})
SOURCES_CJSON := $(wildcard ${DIR_CJSON}/*.c)
SOURCES_CWALK := $(wildcard ${DIR_CWALK}/*.c)
SOURCES_FLECS := $(wildcard ${DIR_FLECS}/*.c)
SOURCES_TEST := $(wildcard ${DIR_TEST}/*.c)
SOURCES_TNECS := $(wildcard ${DIR_TNECS}/*.c)
SOURCES_LODEPNG := $(wildcard ${DIR_LODEPNG}/*.c)
SOURCES_PHYSFS := $(wildcard ${DIR_PHYSFS}/*.c)
SOURCES_MINCTEST := $(wildcard ${DIR_MINCTEST}/*.c)
SOURCES_STB := $(wildcard ${DIR_STB}/*.c)
SOURCES_THREAD := $(wildcard ${DIR_THREAD}/*.c)
SOURCES_TINYMT := $(wildcard ${DIR_TINYMT}/*.c)
SOURCES_UTF8 := $(wildcard ${DIR_UTF8}/*.c)
SOURCES_ALL := $(SOURCES_THIRD) $(SOURCES_FIRESAGA)

TARGETS_THIRD := $(SOURCES_THIRD:.c=.o)
TARGETS_TEST := $(SOURCES_TEST:.c=.o)
TARGETS_CJSON := $(SOURCES_CJSON:.c=.o)
TARGETS_CWALK := $(SOURCES_CWALK:.c=.o)
TARGETS_FLECS := $(SOURCES_FLECS:.c=.o)
TARGETS_TNECS := $(SOURCES_TNECS:.c=.o)
TARGETS_LODEPNG := $(SOURCES_LODEPNG:.c=.o)
TARGETS_PHYSFS := $(SOURCES_PHYSFS:.c=.o)
TARGETS_TINYMT := $(SOURCES_TINYMT:.c=.o)
TARGETS_FIRESAGA := $(SOURCES_FIRESAGA:.c=.o)
TARGETS_FIRESAGA_NOMAIN := $(SOURCES_FIRESAGA_NOMAIN:.c=.o)
TARGETS_ALL := $(SOURCES_ALL:.c=.o)

.PHONY: all
all: astyle $(EXEC) $(TEST)

.PHONY: test
test: astyle $(TEST)

${DIR_CJSON}/%.o : ${DIR_CJSON}/%.c ; $(CC) $< -c -o $@ -I$(DIR_CJSON) ${FLAGS_BUILD_TYPE}
.PHONY: lib_cjson
lib_cjson: $(TARGETS_CJSON); ar rcs ${DIR_BUILD}/$@.a $(TARGETS_CJSON)

${DIR_CWALK}/%.o : ${DIR_CWALK}/%.c ; $(CC) $< -c -o $@ -I$(DIR_CWALK) ${FLAGS_BUILD_TYPE}
.PHONY: lib_cwalk
lib_cwalk: $(TARGETS_CWALK); ar rcs ${DIR_BUILD}/$@.a $(TARGETS_CWALK)

${DIR_TNECS}/%.o : ${DIR_TNECS}/%.c ; $(CC) $< -c -o $@ -I$(DIR_TNECS) ${FLAGS_BUILD_TYPE}
.PHONY: lib_tnecs
lib_tnecs: $(TARGETS_TNECS); ar rcs ${DIR_BUILD}/$@.a $(TARGETS_TNECS)

${DIR_LODEPNG}/%.o : ${DIR_LODEPNG}/%.c ; $(CC) $< -c -o $@ -I$(DIR_LODEPNG) ${FLAGS_BUILD_TYPE}
.PHONY: lib_lodepng
lib_lodepng: $(TARGETS_LODEPNG); ar rcs ${DIR_BUILD}/$@.a $(TARGETS_LODEPNG)

${DIR_PHYSFS}/%.o : ${DIR_PHYSFS}/%.c ; $(CC) $< -c -o $@ -I$(DIR_PHYSFS) ${FLAGS_BUILD_TYPE}
.PHONY: lib_physfs
lib_physfs: $(TARGETS_PHYSFS); ar rcs ${DIR_BUILD}/$@.a $(TARGETS_PHYSFS)

${DIR_TINYMT}/%.o : ${DIR_TINYMT}/%.c ; $(CC) $< -c -o $@ -I$(DIR_TINYMT) ${FLAGS_BUILD_TYPE}
.PHONY: lib_tinymt
lib_tinymt: $(TARGETS_TINYMT); ar rcs ${DIR_BUILD}/$@.a $(TARGETS_TINYMT)

.PHONY : astyle
astyle: ; astyle --style=java --indent=spaces=4 --indent-switches --pad-oper --pad-comma --pad-header --unpad-paren  --align-pointer=middle --align-reference=middle --add-braces --add-one-line-braces --attach-return-type --convert-tabs --suffix=none include/*.h src/*.c test/*.h test/*.c


src/%.o : src/%.c lib_cjson lib_cwalk lib_tnecs lib_lodepng lib_physfs lib_tinymt makefile ; $(CC) $< -c -o $@ $(INCLUDE_ALL)  ${FLAGS_BUILD_TYPE} ${FLAGS_ERROR}
test/%.o : test/%.c lib_cjson lib_cwalk lib_tnecs lib_lodepng lib_physfs lib_tinymt makefile ; $(CC) $< -c -o $@ $(INCLUDE_ALL)  ${FLAGS_BUILD_TYPE} ${FLAGS_ERROR}

$(EXEC): $(TARGETS_FIRESAGA) $(TARGETS_THIRD) lib_cjson lib_cwalk lib_tnecs lib_lodepng lib_physfs lib_tinymt makefile ; $(CC) $(TARGETS_FIRESAGA) -o $(DIR_BUILD)/$@ $(CFLAGS) 

$(TEST): $(TARGETS_FIRESAGA_NOMAIN) $(TARGETS_TEST) $(TARGETS_THIRD) lib_cjson lib_cwalk lib_tnecs lib_lodepng lib_physfs lib_tinymt makefile ; $(CC) $(TARGETS_FIRESAGA_NOMAIN) $(TARGETS_TEST) -o $(DIR_BUILD)/$@ $(CFLAGS) 

.PHONY: wclean
wclean: ; del /q /s *.o *.a *.exe build\\*.txt
.PHONY: clean
clean: ; rm -fr ${LIBS_THIRD}
