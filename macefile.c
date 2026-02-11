
#ifndef MACE_CC
    #define MACE_CC tcc
#endif

#define PAGE_SIZE 4096
#define MACE_MEM PAGE_SIZE * 20

#include "mace.h"

#include "names/game.h"
#include "names/dir.h"

/* Note: tcc doesn't support c89 */
#define C_STANDARD "-std=iso9899:1999"

#define FLAGS_WARNING "-Wno-format -Wno-unused-value "\
    "-Wno-char-subscripts -Wno-unused-function "\
    "-Wno-pointer-sign -Wno-enum-compare -Wmissing-braces "\
    "-Wunused-but-set-variable -Wduplicate-decl-specifier "\
    "-Wunused-but-set-variable -Wunused-variable "\
    "-Wno-unused-command-line-argument "\
    "-Wno-format-contains-nul -Wno-unknown-warning-option"

#define FLAGS_DEBUG "-g -gdwarf -rdynamic -O0 "\
    "-DSDL_ASSERT_LEVEL=2"

// -fwrapv defines signed integer overflow to wrap
// -fno-strict-overflow ensures no UB when 
///     signed integers overflow
#define FLAGS_SANE "-fno-delete-null-pointer-checks "\
    "-fno-strict-aliasing -fwrapv -fno-strict-overflow"

#define FLAGS_SANITIZE "-fsanitize=undefined "\
    "-fsanitize=address"

// FLAGS_SDL_LINUX: run "sdl2-config --cflags"
//   + add -DSDL_DISABLE_IMMINTRIN_H
#define FLAGS_SDL_LINUX "-DSDL_DISABLE_IMMINTRIN_H "\
"-I/usr/include/SDL2 -D_GNU_SOURCE=1 -D_REENTRANT"

#define FLAGS_WINDOWS "-lmingw32 -lSDL2 -lSDL2main"

#define INCLUDES ". include include/bars include/menu "\
    "include/popup include/register include/systems include/unit "\
    "names names/popup names/menu "\
    "second_party/noursmath second_party/tnecs "\
    "second_party/parg second_party/nourstest "\
    "second_party/noursclock "\
    "third_party/physfs "\
    "third_party/stb third_party/cJSON "\
    "/usr/include/SDL2"
#define INCLUDES_TEST "test test/menu test/popup"
#define INCLUDES_BENCH "bench"

#define SOURCES "src src/bars/ src/menu/ src/popup/ "\
    "src/systems/ src/game/ src/map/ src/unit/ "\
    "src/controller/"
#define SOURCES_TEST "test/*.c"
#define SOURCES_BENCH "bench/*.c"

#define ZIP "./utils/zip_assets.sh"

#define CLEAN "find " BUILD_DIR " -mindepth 1 -type d,l,f -delete && "\
    "find " OBJ_DIR " -mindepth 1 -type d,l,f -delete"

#define ASTYLE "astyle --options=utils/style.txt "\
    "--verbose --recursive src/*.c include/*.h test/*.c "\
    "test/*.h names/*.h"

#define LINKS "SDL2 SDL2_image SDL2_mixer m "\
    "nstr cjson noursclock noursmath physfs tnecs parg"

#define SAVES_LINK "rm -f build/"SAVES_DIR" && ln -s ../saves build/"SAVES_DIR

#define LINKS_L2W "mingw32 SDL2main SDL2 SDL2_image "\
    "SDL2_mixer cjson noursmath physfs tnecs parg "\
    "noursclock nstr"

#define GCC_WINDOWS "x86_64-w64-mingw32-gcc"
#define AR_WINDOWS "x86_64-w64-mingw32-ar"
#define TCC_WINDOWS "x86_64-win32-tcc"
#define TCC_AR_WINDOWS "x86_64-win32-tcc -ar"

struct Config debug         = {
    .flags = FLAGS_DEBUG
};

struct Config tcc_bounds    = {
    .flags = FLAGS_DEBUG" -b"
};

struct Config release       = {
    .flags = "-O2 -DSDL_ASSERT_LEVEL=1"
};

struct Config win_debug     = {
    .flags  = "-g -O0",
    .cc     = GCC_WINDOWS,
    .ar     = AR_WINDOWS,
};

struct Config win_release   = {
    .flags  = "-O2",                
    .cc     = GCC_WINDOWS,
    .ar     = AR_WINDOWS,
};

/* BORKED: tcc can't compile physfs
**          cause no userenv.h in tcc includes */
struct Config l2w_tcc_debug     = {
    .flags  = "-g,-O0",
    .cc     = TCC_WINDOWS,
    .ar     = TCC_AR_WINDOWS
};

struct Config l2w_tcc_release   = {
    .flags  = "-O2",                
    .cc     = TCC_WINDOWS,
    .ar     = TCC_AR_WINDOWS
};

struct Config l2w_gcc_debug     = {
    .flags  = "-g,-O0",
    .cc     = GCC_WINDOWS,
    .ar     = AR_WINDOWS,
};

struct Config l2w_gcc_release   = {
    .flags  = "-O2",                
    .cc     = GCC_WINDOWS,
    .ar     = AR_WINDOWS,
};

/* - second_party - */
struct Target noursmath = {
    .base_dir   = "second_party/noursmath",
    .flags      = C_STANDARD" "
                  FLAGS_WARNING,
    .sources    = ".",
    .link_flags = "-whole-archive",
    .kind       = MACE_STATIC_LIBRARY,
};

struct Target parg      = {
    .base_dir   = "second_party/parg",
    .flags      = C_STANDARD" "
                  FLAGS_WARNING,
    .sources    = ".",
    .link_flags = "-whole-archive",
    .kind       = MACE_STATIC_LIBRARY,
};

struct Target noursclock      = {
    .base_dir   = "second_party/noursclock",
    .flags      = C_STANDARD" "
                  FLAGS_WARNING,
    .sources    = ".",
    .link_flags = "-whole-archive",
    .kind       = MACE_STATIC_LIBRARY,
};

struct Target tnecs     = {
    .base_dir   = "second_party/tnecs",
    .flags      = C_STANDARD" "
                  FLAGS_WARNING" "
                  "-DNDEBUG",
    .sources    = ".",
    .link_flags = "-whole-archive",
    .kind       = MACE_STATIC_LIBRARY,
};

/* - third_party - */
struct Target cjson     = {
    .base_dir   = "third_party/cJSON",
    .flags      = C_STANDARD" "
                  FLAGS_WARNING,
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
    .flags      = "-DPHYSFS_SUPPORTS_7Z=0 "
                  "-DPHYSFS_SUPPORTS_GRP=0 "
                  "-DPHYSFS_SUPPORTS_WAD=0 "
                  "-DPHYSFS_SUPPORTS_CSM=0 "
                  "-DPHYSFS_SUPPORTS_HOG=0 "
                  "-DPHYSFS_SUPPORTS_MVL=0 "
                  "-DPHYSFS_SUPPORTS_QPAK=0 "
                  "-DPHYSFS_SUPPORTS_ISO9660=0 "
                  "-DPHYSFS_SUPPORTS_SLB=0 "
                  "-DPHYSFS_SUPPORTS_VDF=0 "
                  FLAGS_WARNING" "
                  FLAGS_SDL_LINUX,
    .base_dir   = "third_party/physfs",
    .link_flags = "-whole-archive",
    .allatonce  = false,
    .kind       = MACE_STATIC_LIBRARY,
};

/* --- SotA --- */
/* -- Native Windows -- */
/* TODO: Test native windows target */
struct Target win_sota = {
    .includes   = INCLUDES,
    .sources    = SOURCES,
    .links      = LINKS,
    .flags      = FLAGS_WINDOWS" "
                  C_STANDARD" "
                  FLAGS_SANE" "
                  FLAGS_WARNING" "
                  FLAGS_SDL_LINUX,
    .cmd_pre    = ASTYLE,
    .kind       = MACE_EXECUTABLE,
};

/* -- Native Linux -- */
struct Target GAME_TITLE_ABREV = {
    .includes   = INCLUDES,
    .sources    = SOURCES,
    .excludes   = "src/install.c src/nstr.c",
    .links      = LINKS,
    .flags      = C_STANDARD" "
                  FLAGS_SANE" "
                  FLAGS_WARNING" "
                  // FLAGS_SANITIZE" "
                  FLAGS_SDL_LINUX,
    .cmd_pre    = ASTYLE,
    .kind       = MACE_EXECUTABLE,
    .dependencies = "zip",
    .cmd_post   = SAVES_LINK,
};

/* TODO: Main loop for hot reloading */
struct Target sota_main = {
    .includes   = INCLUDES,
    .sources    = "src/main.c src/nstr.c",
    .links      = "SDL2,parg",
    .link_flags = "-rpath=./",
    .flags      = C_STANDARD" "
                  FLAGS_SANE" "
                  FLAGS_WARNING" "
                  FLAGS_SDL_LINUX,
    .cmd_pre    = ASTYLE,
    .kind       = MACE_EXECUTABLE,
};

struct Target nstr = {
    .includes   = ". include",
    .sources    = "src/nstr.c",
    .flags      = C_STANDARD" "
                  FLAGS_SANE" "
                  FLAGS_WARNING,
    .cmd_pre    = ASTYLE,
    .kind       = MACE_STATIC_LIBRARY,
};


/* TODO: Hot reloadable part of game */
struct Target sota_dll = {
    .includes   = INCLUDES,
    .sources    = SOURCES,
    .excludes   = "src/main.c src/nstr.c src/install.c",
    .link_flags = "-rpath=./,-whole-archive ",
    .links      = LINKS,
    .flags      = "-Lbuild "
                  C_STANDARD" "
                  FLAGS_SANE" "
                  FLAGS_WARNING" "
                  FLAGS_SDL_LINUX,
    .cmd_pre    = ASTYLE,
    .kind       = MACE_DYNAMIC_LIBRARY, /* Check with "file" cmd */
};

/* -- Linux to Windows cross compilation -- */
struct Target l2w_sota = {
    .includes   = INCLUDES
                  "/usr/local/x86_64-w64-mingw32/include",
    .sources    = SOURCES,
    .links      = LINKS_L2W,
    .flags      = "-L/usr/local/x86_64-w64-mingw32/lib "
                  "-B/usr/local/lib/tcc/win32 "
                  C_STANDARD" "
                  FLAGS_SANE" "
                  FLAGS_WARNING" "
                  FLAGS_SDL_LINUX,
    .cmd_pre    = ASTYLE,
    .kind       = MACE_EXECUTABLE,
};

/* -- Testing -- */
struct Target test = {
    .includes   = INCLUDES" "
                  INCLUDES_TEST,
    .sources    = SOURCES" "
                  SOURCES_TEST,
    .excludes   = "src/main.c src/install.c src/nstr.c",
    .links      = LINKS,
    .flags      = C_STANDARD" "
                  FLAGS_SANE" "
                  FLAGS_WARNING" "
                  // FLAGS_SANITIZE" "
                  FLAGS_SDL_LINUX,
    .cmd_pre    = ASTYLE,
    .dependencies = "zip",
    .kind       = MACE_EXECUTABLE,
    .cmd_post   = SAVES_LINK,
};

struct Target bench = {
    .includes   = INCLUDES" "
                  INCLUDES_TEST" "
                  INCLUDES_BENCH,
    .sources    = SOURCES" "
                  SOURCES_BENCH,
    .excludes   = "src/main.c src/nstr.c",
    .links      = LINKS,
    .flags      = "-L/usr/lib "
                  C_STANDARD" "
                  FLAGS_SANE" "
                  FLAGS_WARNING" "
                  FLAGS_SDL_LINUX,
    .cmd_pre    = ASTYLE,
    .kind       = MACE_EXECUTABLE,
};

struct Target install = {
    .kind           = MACE_EXECUTABLE,
    .includes       = ". include third_party/physfs",
    .sources        = "src/install.c",
    .links          = "nstr physfs",
    .dependencies   = "zip " STRINGIFY(GAME_TITLE_ABREV),
    .cmd_post       = "build/install",
};

struct Target zip = {
    .cmd_pre    = ZIP,
    .kind       = MACE_PHONY,
};

struct Target clean = {
    .cmd_pre    = CLEAN,
    .kind       = MACE_PHONY,
};

int mace(int argc, char *argv[]) {
    /* -- Setting compiler, directories -- */
    MACE_SET_COMPILER(CC);
    mace_set_build_dir(BUILD_DIR);
    mace_set_obj_dir(OBJ_DIR);

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
    MACE_ADD_TARGET(zip);
    MACE_ADD_TARGET(nstr);
    MACE_ADD_TARGET(clean);
    MACE_ADD_TARGET(install);
    MACE_ADD_TARGET(sota_dll);
    MACE_ADD_TARGET(win_sota);
    MACE_ADD_TARGET(l2w_sota);
    MACE_ADD_TARGET(sota_main);
    MACE_ADD_TARGET(GAME_TITLE_ABREV);
    MACE_SET_DEFAULT_TARGET(GAME_TITLE_ABREV);

    /* - Testing - */
    MACE_ADD_TARGET(test);
    MACE_ADD_TARGET(bench);

    /* - Target configs - */
    MACE_TARGET_CONFIG(l2w_sota, l2w_gcc_debug);
    MACE_TARGET_CONFIG(sota, debug);
    MACE_TARGET_CONFIG(test, debug);

    return(0);
}
