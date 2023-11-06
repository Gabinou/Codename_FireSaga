
#include "mace.h"

#ifndef CC
    #define CC "tcc"
#endif
#ifndef AR
    #define AR "ar"
#endif

struct Config debug         = {.flags = "-g3 -rdynamic -O0 -DSDL_ASSERT_LEVEL=2"};
struct Config release       = {.flags = "-O2 -DSDL_ASSERT_LEVEL=1"};

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
    /* Disable all archives except .zip */
    /* Note: tcc can't compile 7z -> __cpuid missing */
    .flags              = "-DPHYSFS_SUPPORTS_7Z=0,"
                          "-DPHYSFS_SUPPORTS_GRP=0,"
                          "-DPHYSFS_SUPPORTS_WAD=0,"
                          "-DPHYSFS_SUPPORTS_CSM=0,"
                          "-DPHYSFS_SUPPORTS_HOG=0,"
                          "-DPHYSFS_SUPPORTS_MVL=0,"
                          "-DPHYSFS_SUPPORTS_QPAK=0,"
                          "-DPHYSFS_SUPPORTS_ISO9660=0,"
                          "-DPHYSFS_SUPPORTS_SLB=0,"
                          "-DPHYSFS_SUPPORTS_VDF=0",
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
/* TODO: Can remove win-sota target and use only sota? */
struct Target win_sota = {
    .includes           = ".,include,include/bars,include/menu,include/popup,"
                          "include/systems,names,names/popup,names/menu,"
                          "second_party/noursmath,second_party/tnecs,"
                          "second_party/parg,second_party/nourstest,"
                          "third_party/physfs,third_party/tinymt,third_party/stb,"
                          "third_party/cJson",
    .sources            = "src/*.c,src/bars/,src/menu/,src/popup/,src/systems/,"
                          "src/game/,src/map,src/controller",
    .links              = "SDL2,SDL2_image,SDL2_ttf,m,cjson,noursmath,physfs,"
                          "tinymt,tnecs,parg",
                          /* TODO: Remove flags given by sdl2-config */
    .flags              = "-lmingw32,-lSDL2main,-fwrapv,-fno-strict-overflow,"
                          "-fno-strict-aliasing,-fno-delete-null-pointer-checks",
                          "$(sdl2-config --cflags)",
    .command_pre_build  = "astyle --options=utils/style.txt --verbose "
                          "--recursive src/* include/* test/* names/*",
    .kind               = MACE_EXECUTABLE,
};

struct Target sota = {
    .includes           = ".,include,include/bars,include/menu,include/popup,"
                          "include/systems,names,names/popup,names/menu,"
                          "second_party/noursmath,second_party/tnecs,"
                          "second_party/parg,second_party/nourstest,"
                          "third_party/physfs,third_party/tinymt,third_party/stb,"
                          "third_party/cJson,/usr/include/SDL2",
    .sources            = "src/*.c,src/bars/,src/menu/,src/popup/,src/systems/,"
                          "src/game/,src/map,src/controller",
    .links              = "SDL2,SDL2_image,SDL2_ttf,m,GLEW,cjson,noursmath,physfs,"
                          "tinymt,tnecs,parg",
    .flags              = "-fno-strict-overflow,-fno-strict-aliasing,"
                          "-fwrapv,-fno-delete-null-pointer-checks,"
                          "-DSDL_DISABLE_IMMINTRIN_H,"
                          "$(sdl2-config --cflags)",
    .command_pre_build  = "astyle --options=utils/style.txt --verbose "
                          "--recursive src/* include/* test/* names/*",
    .kind               = MACE_EXECUTABLE,
};

/* - Testing - */
struct Target test = {
    .includes           = ".,include,include/bars,include/menu,include/popup,"
                          "include/systems,names,names/popup,names/menu,"
                          "second_party/noursmath,second_party/tnecs,"
                          "second_party/parg,second_party/nourstest,"
                          "third_party/physfs,third_party/tinymt,third_party/stb,"
                          "third_party/cJson,/usr/include/SDL2",
                          "test,test/menu,test/popup",
    .sources            = "test/*.c,src/*.c,src/bars/,src/menu/,src/popup/,"
                          "src/systems/,src/game/,src/map,src/controller",
    .excludes           = "src/main.c",
    .links              = "SDL2,SDL2_image,SDL2_ttf,m,GLEW,cjson,noursmath,physfs,"
                          "tinymt,tnecs,parg",
    .flags              = "-L/usr/lib,-fno-strict-overflow,-fno-strict-aliasing,"
                          "-fwrapv,-fno-delete-null-pointer-checks,"
                          "-DSDL_DISABLE_IMMINTRIN_H,"
                          "$(sdl2-config --cflags)",
    .command_pre_build  = "astyle --options=utils/style.txt --verbose "
                          "--recursive src/* include/* test/* names/*",
    .kind               = MACE_EXECUTABLE,
};

int mace(int argc, char *argv[]) {
    /* -- Setting compiler, directories -- */
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
