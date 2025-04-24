
#include "mace.h"

#ifndef CC
    #define CC "tcc"
#endif
#ifndef AR
    #define AR "tcc -ar"
#endif

#define IES_C_STANDARD "-std=iso9899:1999"

#define WARNING_FLAGS "-Wall -Wno-format -Wno-unused-value "\
    "-Wno-char-subscripts -Wno-unused-function "\
    "-Wno-pointer-sign -Wno-enum-compare -Wmissing-braces "\
    "-Wunused-but-set-variable -Wduplicate-decl-specifier "\
    "-Wunused-but-set-variable -Wunused-variable "\
    "-Wno-unused-command-line-argument "\
    "-Wno-format-contains-nul -Wno-unknown-warning-option"

#define DEBUG_FLAGS "-g -gdwarf -rdynamic -O0"\
    "-DSDL_ASSERT_LEVEL=2"

#define IES_SANE_FLAGS "-fno-delete-null-pointer-checks "\
    "-fno-strict-aliasing -fwrapv -fno-strict-overflow"\

#define IES_SANITIZE_FLAGS "-fsanitize=undefined,"\
    "-fsanitize=address"
#define IES_SDL_FLAGS "-DSDL_DISABLE_IMMINTRIN_H "\
    "$(sdl2-config --cflags)"
#define IES_WINDOWS_FLAGS "-lmingw32,-lSDL2,-lSDL2main"

#define IES_INCLUDES ".,include,include/bars,include/menu,"\
    "include/popup,include/unit,"\
    "include/systems,names,names/popup,names/menu,"\
    "second_party/noursmath,second_party/tnecs,"\
    "second_party/parg,second_party/nourstest,"\
    "second_party/noursclock,"\
    "third_party/physfs," \
    "third_party/stb,third_party/cJSON,"\
    "/usr/include/SDL2"
#define IES_TEST_INCLUDES "test,test/menu,test/popup"
#define IES_BENCH_INCLUDES "bench"

#define IES_SOURCES "src,src/bars/,src/menu/,src/popup/,"\
    "src/systems/,src/game/,src/map/,src/unit/,"\
    "src/controller/"
#define IES_TEST_SOURCES "test/*.c"
#define IES_BENCH_SOURCES "bench/*.c,"

#define IES_ASTYLE "astyle --options=utils/style.txt "\
    "--verbose --recursive src/*.c include/*.h test/*.c "\
    "test/*.h names/*.h"

#define IES_LINKS "SDL2,SDL2_image,SDL2_mixer,m,"\
    "cjson,noursclock,noursmath,physfs,tnecs,parg"

#define IES_LINKS_L2W "mingw32,SDL2main,SDL2,SDL2_image,"\
    "SDL2_mixer,cjson,noursmath,physfs,tnecs,parg,"\
    "noursclock"

struct Config debug         = {
    .flags = DEBUG_FLAGS
};

struct Config tcc_bounds    = {
    .flags = DEBUG_FLAGS "-b"
};

struct Config release       = {
    .flags = "-O2 -DSDL_ASSERT_LEVEL=1"
};

struct Config win_debug     = {
    .flags  = "-g -O0",
    .cc     = "x86_64-w64-mingw32-gcc",
    .ar     = "x86_64-w64-mingw32-ar"
};

struct Config win_release   = {
    .flags  = "-O2",                
    .cc     = "x86_64-w64-mingw32-gcc",
    .ar     = "x86_64-w64-mingw32-ar"
};

/* BORKED: physfs can't compile cause no userenv.h in tcc includes */
struct Config l2w_tcc_debug     = {
    .flags  = "-g -O0",
    .cc     = "x86_64-win32-tcc",
    .ar     = "x86_64-win32-tcc -ar"
};

struct Config l2w_tcc_release   = {
    .flags  = "-O2",                
    .cc     = "x86_64-win32-tcc",
    .ar     = "x86_64-win32-tcc -ar"
};

struct Config l2w_gcc_debug     = {
    .flags  = "-g -O0",
    .cc     = "x86_64-w64-mingw32-gcc",
    .ar     = "x86_64-w64-mingw32-ar"
};

struct Config l2w_gcc_release   = {
    .flags  = "-O2",                
    .cc     = "x86_64-w64-mingw32-gcc",
    .ar     = "x86_64-w64-mingw32-ar"
};

/* - second_party - */
struct Target noursmath = {
    .base_dir   = "second_party/noursmath",
    .flags      = IES_C_STANDARD" "
                  WARNING_FLAGS,
    .sources    = ".",
    .link_flags = "-whole-archive",
    .kind       = MACE_STATIC_LIBRARY,
};

struct Target parg      = {
    .base_dir   = "second_party/parg",
    .flags      = IES_C_STANDARD,
    .sources    = ".",
    .link_flags = "-whole-archive",
    .kind       = MACE_STATIC_LIBRARY,
};

struct Target noursclock      = {
    .base_dir   = "second_party/noursclock",
    .flags      = IES_C_STANDARD,
    .sources    = ".",
    .link_flags = "-whole-archive",
    .kind       = MACE_STATIC_LIBRARY,
};

struct Target tnecs     = {
    .base_dir   = "second_party/tnecs",
    .flags      = IES_C_STANDARD,
    .sources    = ".",
    .link_flags = "-whole-archive",
    .kind       = MACE_STATIC_LIBRARY,
};

/* - third_party - */
struct Target cjson     = {
    .base_dir   = "third_party/cJSON",
    .flags      = IES_C_STANDARD,
    .sources    = ".",
    .link_flags = "-rpath=/home/gabinours/firesaga/build",
                  "-whole-archive",
    .kind       = MACE_STATIC_LIBRARY,
};

struct Target physfs    = {
    .sources    = ".",
    /* Disable all archives except .zip */
    /* Note: tcc can't compile 7z -> __cpuid missing */
    /* Cannot be compiled with -std=iso9899:1999/-std=c99 */
    .flags      = "-DPHYSFS_SUPPORTS_7Z=0,"
                  "-DPHYSFS_SUPPORTS_GRP=0,"
                  "-DPHYSFS_SUPPORTS_WAD=0,"
                  "-DPHYSFS_SUPPORTS_CSM=0,"
                  "-DPHYSFS_SUPPORTS_HOG=0,"
                  "-DPHYSFS_SUPPORTS_MVL=0,"
                  "-DPHYSFS_SUPPORTS_QPAK=0,"
                  "-DPHYSFS_SUPPORTS_ISO9660=0,"
                  "-DPHYSFS_SUPPORTS_SLB=0,"
                  "-DPHYSFS_SUPPORTS_VDF=0,"
                  WARNING_FLAGS,
    .base_dir   = "third_party/physfs",
    .link_flags = "-whole-archive",
    .allatonce  = false,
    .kind       = MACE_STATIC_LIBRARY,
};

/* --- SotA --- */
/* -- Native Windows -- */
/* TODO: Test native windows target */
struct Target win_sota = {
    .includes   = IES_INCLUDES,
    .sources    = IES_SOURCES,
    .links      = IES_LINKS,
                /* TODO: Remove flags given by sdl2-config */
    .flags      = IES_WINDOWS_FLAGS" "
                  IES_C_STANDARD" "
                  IES_SANE_FLAGS" "
                  IES_SDL_FLAGS,
    .cmd_pre    = IES_ASTYLE,
    .kind       = MACE_EXECUTABLE,
};

/* -- Native Linux -- */
struct Target sota = {
    .includes   = IES_INCLUDES,
    .sources    = IES_SOURCES,
    .links      = IES_LINKS,
    .flags      = IES_C_STANDARD" "
                  IES_SANE_FLAGS" "
                  IES_SDL_FLAGS,
    .cmd_pre    = IES_ASTYLE,
    .kind       = MACE_EXECUTABLE,
};

/* Main loop for hot reloading */
struct Target sota_main = {
    .includes   = IES_INCLUDES,
    .sources    = "src/main.c",
    .links      = "SDL2,parg",
    .link_flags = "-rpath=./",
    .flags      = IES_C_STANDARD" "
                  IES_SANE_FLAGS" "
                  IES_SDL_FLAGS,
    .cmd_pre    = IES_ASTYLE,
    .kind       = MACE_EXECUTABLE,
};

struct Target sota_dll = {
    .includes   = IES_INCLUDES,
    .sources    = IES_SOURCES,
    .excludes   = "src/main.c",
    .link_flags = "-rpath=./,-whole-archive,",
    .links      = IES_LINKS,
    .flags      = "-Lbuild "
                  IES_C_STANDARD" "
                  IES_SANE_FLAGS" "
                  IES_SDL_FLAGS,
    .cmd_pre    = IES_ASTYLE,
    .kind       = MACE_SHARED_LIBRARY, /* Check with "file" cmd */
};

/* -- Linux to Windows cross compilation -- */
struct Target l2w_sota = {
    .includes = IES_INCLUDES
                "/usr/local/x86_64-w64-mingw32/include",
    .sources  = IES_SOURCES,
    .links    = IES_LINKS_L2W,
    .flags    = "-L/usr/local/x86_64-w64-mingw32/lib "
                "-B/usr/local/lib/tcc/win32 "
                IES_C_STANDARD" "
                IES_SANE_FLAGS" "
                IES_SDL_FLAGS,
    .cmd_pre  = IES_ASTYLE,
    .kind     = MACE_EXECUTABLE,
};

/* -- Testing -- */
struct Target test = {
    .includes = IES_INCLUDES","
                IES_TEST_INCLUDES,
    .sources  = IES_SOURCES","
                IES_TEST_SOURCES,
    .excludes = "src/main.c",
    .links    = IES_LINKS,
    .flags    = IES_C_STANDARD" "
                IES_SANE_FLAGS" "
                IES_SDL_FLAGS,
    .cmd_pre  = IES_ASTYLE,
    .kind     = MACE_EXECUTABLE,
};

struct Target bench = {
    .includes = IES_INCLUDES","
                IES_TEST_INCLUDES","
                IES_BENCH_INCLUDES,
    .sources  = IES_SOURCES","
                IES_BENCH_SOURCES,
    .excludes = "src/main.c",
    .links    = IES_LINKS,
    .flags    = "-L/usr/lib "
                IES_C_STANDARD" "
                IES_SANE_FLAGS" "
                IES_SDL_FLAGS,
    .cmd_pre  = IES_ASTYLE,
    .kind     = MACE_EXECUTABLE,
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
    MACE_ADD_CONFIG(l2w_tcc_debug);
    MACE_ADD_CONFIG(l2w_tcc_release);
    MACE_ADD_CONFIG(l2w_gcc_debug);
    MACE_ADD_CONFIG(l2w_gcc_release);
    MACE_ADD_CONFIG(tcc_bounds);
    
    /* -- Targets -- */
    /* - second_party - */
    MACE_ADD_TARGET(noursmath);
    MACE_ADD_TARGET(parg); 
    MACE_ADD_TARGET(tnecs);
    MACE_ADD_TARGET(noursclock);

    /* - third_party - */
    MACE_ADD_TARGET(cjson);
    MACE_ADD_TARGET(physfs);

    /* - SotA - */
    // MACE_ADD_TARGET(sota);
    MACE_ADD_TARGET(sota);
    MACE_ADD_TARGET(sota_main);
    MACE_ADD_TARGET(sota_dll);
    MACE_ADD_TARGET(win_sota);
    MACE_ADD_TARGET(l2w_sota);
    MACE_SET_DEFAULT_TARGET(sota);

    /* - Testing - */
    MACE_ADD_TARGET(test);
    MACE_ADD_TARGET(bench);

    /* - Target configs - */
    MACE_TARGET_CONFIG(l2w_sota, l2w_gcc_debug);
    MACE_TARGET_CONFIG(sota, debug);
    MACE_TARGET_CONFIG(test, debug);

    return(0);
}
