
#include "mace.h"

#ifndef CC
    #define CC "gcc"
#endif
#ifndef AR
    #define AR "ar"
#endif

struct Config debug         = {.flags = "-g -rdynamic -O0"};
struct Config release       = {.flags = "-O2"};

struct Config win_debug     = {.flags = "-g -O0",              .target = "win_sota",
                               .cc    = "x86_64-w64-mingw32-gcc",  .ar = "x86_64-w64-mingw32-ar"};
struct Config win_release   = {.flags = "-O2",                 .target = "win_sota", 
                               .cc    = "x86_64-w64-mingw32-gcc",  .ar = "x86_64-w64-mingw32-ar"};

/* - second_party - */
struct Target noursmath = {
    .sources            = ".",
    .base_dir           = "second_party/noursmath",
    .kind               = MACE_STATIC_LIBRARY,
};

struct Target nstr = {
    .sources            = ".",
    .base_dir           = "second_party/nstr",
    .kind               = MACE_STATIC_LIBRARY,
};

struct Target parg = {
    .sources            = ".",
    .base_dir           = "second_party/parg",
    .kind               = MACE_STATIC_LIBRARY,
};

struct Target tnecs = {
    .sources            = ".",
    .base_dir           = "second_party/tnecs",
    .kind               = MACE_STATIC_LIBRARY,
};

/* - third_party - */
struct Target cjson = {
    .sources            = ".",
    .base_dir           = "third_party/cJson",
    .kind               = MACE_STATIC_LIBRARY,
};

struct Target physfs = {
    .sources            = ".",
    .base_dir           = "third_party/physfs",
    .allatonce          = false,
    .kind               = MACE_STATIC_LIBRARY,
};

struct Target tinymt = {
    .sources            = ".",
    .base_dir           = "third_party/tinymt",
    .kind               = MACE_STATIC_LIBRARY,
};

/* - SotA - */
struct Target win_sota = {
    .includes           = ".,include,include/bars,include/menu,include/popup,"
                          "include/systems,names,names/popup,names/menu,"
                          "second_party/nstr,second_party/noursmath,second_party/tnecs,"
                          "second_party/parg,second_party/nourstest,"
                          "third_party/physfs,third_party/tinymt,third_party/stb,"
                          "third_party/cJson",
    .sources            = "src/*.c,src/bars/,src/menu/,src/popup/,src/systems/,src/game/,src/map,"
                          "src/controller",
    .links              = "SDL2,SDL2_image,SDL2_ttf,m,cjson,noursmath,physfs,"
                          "tinymt,tnecs,nstr,parg",
    .flags              = "-lmingw32,-lSDL2main,-fwrapv,-fno-strict-overflow,-fno-strict-aliasing",
    .command_pre_build  = "astyle --options=utils/style.txt --verbose "
                          "--recursive src/* include/* test/* names/*",
    .kind               = MACE_EXECUTABLE,
};

struct Target sota = {
    .includes           = ".,include,include/bars,include/menu,include/popup,"
                          "include/systems,names,names/popup,names/menu,"
                          "include/systems,names,names/popup,names/menu,"
                          "second_party/nstr,second_party/noursmath,second_party/tnecs,"
                          "second_party/parg,second_party/nourstest,"
                          "third_party/physfs,third_party/tinymt,third_party/stb,"
                          "third_party/cJson",
    .sources            = "src/*.c,src/bars/,src/menu/,src/popup/,src/systems/,src/game/,src/map,"
                          "src/controller",
    .links              = "SDL2,SDL2_image,SDL2_ttf,m,GLEW,cjson,noursmath,physfs,"
                          "tinymt,tnecs,nstr,parg",
    .flags              = "-fwrapv,-fno-strict-overflow,-fno-strict-aliasing",
    .command_pre_build  = "astyle --options=utils/style.txt --verbose "
                          "--recursive src/* include/* test/* names/*",
    .kind               = MACE_EXECUTABLE,
};

/* - Testing - */
struct Target test = {
    .includes           = ".,include,include/bars,include/menu,include/popup,"
                          "include/systems,names,names/popup,names/menu,"
                          "second_party/nstr,second_party/noursmath,second_party/tnecs,"
                          "second_party/parg,second_party/nourstest,"
                          "third_party/physfs,third_party/tinymt,third_party/stb,"
                          "third_party/cJson",
                          "test,test/menu,test/popup",
    .sources            = "test/*.c,src/*.c,src/bars/,src/menu/,src/popup/,"
                          "src/systems/,src/game/,src/map,src/controller",
    .excludes           = "src/main.c",
    .flags              = "-fwrapv,-fno-strict-overflow,-fno-strict-aliasing",
    .links              = "SDL2,SDL2_image,SDL2_ttf,m,GLEW,cjson,noursmath,physfs,"
                          "tinymt,tnecs,nstr,parg",
    .command_pre_build  = "astyle --options=utils/style.txt --verbose "
                          "--recursive src/* include/* test/* names/*",
    .kind               = MACE_EXECUTABLE,
};

int mace(int argc, char *argv[]) {
    /* MACE_SET_COMPILER(CC); */
    mace_set_compiler(CC);
    mace_set_archiver(AR);
    mace_set_build_dir("build");
    mace_set_obj_dir("obj");

    /* -- Configs -- */
    MACE_ADD_CONFIG(debug);
    MACE_ADD_CONFIG(release);
    MACE_ADD_CONFIG(win_debug);
    MACE_ADD_CONFIG(win_release);
    
    /* -- Targets -- */
    /* - second_party - */
    MACE_ADD_TARGET(noursmath);
    MACE_ADD_TARGET(nstr);
    MACE_ADD_TARGET(parg); 
    MACE_ADD_TARGET(tnecs);

    /* - third_party - */
    MACE_ADD_TARGET(cjson);
    MACE_ADD_TARGET(physfs);
    MACE_ADD_TARGET(tinymt);

    /* - SotA - */
    MACE_ADD_TARGET(sota);
    MACE_ADD_TARGET(win_sota);
    MACE_SET_DEFAULT_TARGET(sota);

    /* - Testing - */
    MACE_ADD_TARGET(test);
    
    return(0);
}
