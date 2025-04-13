
#include "mace.h"

#ifndef CC
    #define CC "tcc"
#endif
#ifndef AR
    #define AR "tcc -ar"
#endif
// TODO sanitize woth-fsanitize=undefined,address
struct Config debug         = {
    .flags = "-g -gdwarf -rdynamic -O0 -DSDL_ASSERT_LEVEL=2"
};

struct Config tcc_bounds    = {
    .flags = "-g -gdwarf -b -rdynamic -O0 -DSDL_ASSERT_LEVEL=2"
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
    .base_dir  = "second_party/noursmath",
    .flags     = "-std=iso9899:1999",
    .sources   = ".",
    .link_flags = "-whole-archive",
    .kind      = MACE_STATIC_LIBRARY,
};

struct Target parg      = {
    .base_dir  = "second_party/parg",
    .flags     = "-std=iso9899:1999",
    .sources   = ".",
    .link_flags = "-whole-archive",
    .kind      = MACE_STATIC_LIBRARY,
};

struct Target noursclock      = {
    .base_dir  = "second_party/noursclock",
    .flags     = "-std=iso9899:1999",
    .sources   = ".",
    .link_flags = "-whole-archive",
    .kind      = MACE_STATIC_LIBRARY,
};

struct Target tnecs     = {
    .base_dir  = "second_party/tnecs",
    .flags     = "-std=iso9899:1999",
    .sources   = ".",
    .link_flags = "-whole-archive",
    .kind      = MACE_STATIC_LIBRARY,
};

/* - third_party - */
struct Target cjson     = {
    .base_dir  = "third_party/cJSON",
    .flags     = "-std=iso9899:1999",
    .sources   = ".",
    .link_flags = "-whole-archive,-rpath=/home/gabinours/firesaga/build",
    .kind      = MACE_STATIC_LIBRARY,
};

struct Target physfs    = {
    .sources   = ".",
    /* Disable all archives except .zip */
    /* Note: tcc can't compile 7z -> __cpuid missing */
    /* Cannot be compiled with -std=iso9899:1999/-std=c99 */
    .flags     = "-DPHYSFS_SUPPORTS_7Z=0,"
                 "-DPHYSFS_SUPPORTS_GRP=0,"
                 "-DPHYSFS_SUPPORTS_WAD=0,"
                 "-DPHYSFS_SUPPORTS_CSM=0,"
                 "-DPHYSFS_SUPPORTS_HOG=0,"
                 "-DPHYSFS_SUPPORTS_MVL=0,"
                 "-DPHYSFS_SUPPORTS_QPAK=0,"
                 "-DPHYSFS_SUPPORTS_ISO9660=0,"
                 "-DPHYSFS_SUPPORTS_SLB=0,"
                 "-DPHYSFS_SUPPORTS_VDF=0",
    .base_dir  = "third_party/physfs",
    .link_flags = "-whole-archive",
    .allatonce = false,
    .kind      = MACE_STATIC_LIBRARY,
};

/* --- SotA --- */
/* -- Native Windows -- */
/* TODO: Test native windows target */
struct Target win_sota = {
    .includes = ".,include,include/bars,include/menu,"
                "include/popup,include/unit,"
                "include/systems,names,names/popup,names/menu,"
                "second_party/noursmath,second_party/tnecs,"
                "second_party/parg,second_party/nourstest,"
                "second_party/noursclock,"
                "third_party/physfs," 
                "third_party/stb,third_party/cJSON,"
                "/usr/include/SDL2",
    .sources  = "src,src/bars/,src/menu/,src/popup/,"
                "src/systems/,src/game/,src/map/,src/unit/,"
                "src/controller/",
    .links    = "SDL2,SDL2_image,SDL2_mixer,m,cjson,noursclock,"
                "noursmath,physfs,tnecs,parg",
                /* TODO: Remove flags given by sdl2-config */
    .flags    = "-lmingw32,-lSDL2,-lSDL2main,-std=iso9899:1999,"
                "-fno-strict-overflow,-fno-strict-aliasing,"
                "-fwrapv,-fno-delete-null-pointer-checks,"
                "$(sdl2-config --cflags)",
    .cmd_pre  = "astyle --options=utils/style.txt --verbose --recursive"
                " src/*.c include/*.h test/*.c test/*.h names/*.h",
    .kind     = MACE_EXECUTABLE,
};

/* -- Native Linux -- */
struct Target sota = {
    .includes = ".,include,include/bars,include/menu,"
                "include/popup,include/unit,"
                "include/systems,names,names/popup,names/menu,"
                "second_party/noursmath,second_party/tnecs,"
                "second_party/parg,second_party/nourstest,"
                "second_party/noursclock,"
                "third_party/physfs,"
                "third_party/stb,third_party/cJSON,"
                "/usr/include/SDL2",
    .sources  = "src,src/bars/,src/menu/,src/popup/,"
                "src/systems/,src/game/,src/map/,src/unit/,"
                "src/controller/",
    .links    = "SDL2,SDL2_image,SDL2_mixer,m,GLEW,noursclock,"
                "cjson,noursmath,physfs,tnecs,parg",
    .flags    = "-fno-strict-overflow,-fno-strict-aliasing,"
                "-fwrapv,-fno-delete-null-pointer-checks,"
                "-DSDL_DISABLE_IMMINTRIN_H,-std=iso9899:1999,"
                "$(sdl2-config --cflags)",
    .cmd_pre  = "astyle --options=utils/style.txt --verbose --recursive"
                " src/*.c include/*.h test/*.c test/*.h names/*.h",
    .kind     = MACE_EXECUTABLE,
};

/* Main loop for hot reloading */
struct Target sota_main = {
    .includes = ".,include,include/bars,include/menu,"
                "include/popup,include/unit,"
                "include/systems,names,names/popup,names/menu,"
                "second_party/noursmath,second_party/tnecs,"
                "second_party/parg,second_party/nourstest,"
                "second_party/noursclock,"
                "third_party/physfs," 
                "third_party/stb,third_party/cJSON,"
                "/usr/include/SDL2",
    .sources  = "src/main.c",
    .links    = "SDL2,parg",
    .link_flags = "-rpath=./",
    .flags    = "-fno-strict-overflow,-fno-strict-aliasing,"
                "-fwrapv,-fno-delete-null-pointer-checks,"
                "-DSDL_DISABLE_IMMINTRIN_H,-std=iso9899:1999,"
                "$(sdl2-config --cflags)",
    .cmd_pre  = "astyle --options=utils/style.txt --verbose --recursive"
                " src/*.c include/*.h test/*.c test/*.h names/*.h",
    .kind     = MACE_EXECUTABLE,
};

struct Target sota_dll = {
    .includes =  ".,include,include/bars,include/menu,"
                 "include/popup,include/unit,"
                 "include/systems,names,names/popup,names/menu,"
                 "second_party/noursmath,second_party/tnecs,"
                 "second_party/parg,second_party/nourstest,"
                "second_party/noursclock,"
                 "third_party/physfs," 
                 "third_party/stb,third_party/cJSON,"
                 "/usr/include/SDL2",
    .sources    = "src,src/bars/,src/menu/,src/popup/,"
                  "src/systems/,src/game/,src/map/,src/unit/,"
                  "src/controller/",
    .excludes   = "src/main.c",
    .link_flags = "-rpath=./,-whole-archive,",
    .links      = "SDL2,SDL2_image,SDL2_mixer,m,GLEW,noursclock,"
                  "cjson,noursmath,physfs,tnecs,parg",
    .flags      = "-Lbuild,-fno-strict-overflow,-fno-strict-aliasing,"
                  "-fwrapv,-fno-delete-null-pointer-checks,"
                  "-DSDL_DISABLE_IMMINTRIN_H,-std=iso9899:1999"
                  "$(sdl2-config --cflags)",
    .cmd_pre  = "astyle --options=utils/style.txt --verbose --recursive"
                " src/*.c include/*.h test/*.c test/*.h names/*.h",
    .kind       = MACE_SHARED_LIBRARY, /* Check with "file" cmd */
};


/* -- Linux to Windows cross compilation -- */
struct Target l2w_sota = {
    .includes = ".,include,include/bars,include/menu,"
                "include/popup,include/unit,"
                "include/systems,names,names/popup,names/menu,"
                "second_party/noursmath,second_party/tnecs,"
                "second_party/parg,second_party/nourstest,"
                "second_party/noursclock,"
                "third_party/physfs," 
                "third_party/stb,third_party/cJSON,"
                "/usr/local/x86_64-w64-mingw32/include",
    .sources  = "src,src/bars/,src/menu/,src/popup/,"
                "src/systems/,src/game/,src/map/,src/unit/,"
                "src/controller/",
    .links    = "mingw32,SDL2main,SDL2,SDL2_image,SDL2_mixer,"
                "cjson,noursmath,physfs,tnecs,parg,noursclock",
    .flags    = "-L/usr/local/x86_64-w64-mingw32/lib,-B/usr/local/lib/tcc/win32"
                "-fno-strict-overflow,-fno-strict-aliasing,"
                "-fwrapv,-fno-delete-null-pointer-checks,"
                "-DSDL_DISABLE_IMMINTRIN_H,-std=iso9899:1999,"
                "$(sdl2-config --cflags)",
    .cmd_pre  = "astyle --options=utils/style.txt --verbose --recursive"
                " src/*.c include/*.h test/*.c test/*.h names/*.h",
    .kind     = MACE_EXECUTABLE,
};

/* -- Testing -- */
struct Target test = {
    .includes = ".,include,include/bars,include/menu,"
                "include/popup,include/unit,include/systems,"
                "names,names/popup,names/menu,"
                "second_party/noursmath,second_party/tnecs,"
                "second_party/parg,second_party/nourstest,"
                "second_party/noursclock,"
                "third_party/physfs,"
                "third_party/stb,third_party/cJSON,"
                "/usr/include/SDL2,"
                "test,test/menu,test/popup",
    .sources  = "test/*.c,src/*.c,src/bars/,src/menu/,src/unit,"
                "src/popup/,src/systems/,src/game/,src/map,"
                "src/controller",
    .excludes = "src/main.c",
    .links    = "SDL2,SDL2_image,SDL2_mixer,m,GLEW,noursclock,"
                "cjson,noursmath,physfs,tnecs,parg",
    .flags    = "-fno-strict-overflow,-fno-strict-aliasing,"
                "-fwrapv,-fno-delete-null-pointer-checks,"
                "-DSDL_DISABLE_IMMINTRIN_H,-std=iso9899:1999,"
                "$(sdl2-config --cflags)",
    .cmd_pre  = "astyle --options=utils/style.txt --verbose --recursive"
                " src/*.c include/*.h test/*.c test/*.h names/*.h",
    .kind     = MACE_EXECUTABLE,
};

struct Target bench = {
    .includes = ".,include,include/bars,include/menu,"
                "include/popup,include/unit,include/systems,"
                "names,names/popup,names/menu,"
                "second_party/noursmath,second_party/tnecs,"
                "second_party/parg,second_party/nourstest,"
                "second_party/noursclock,"
                "third_party/physfs,"
                "third_party/stb,third_party/cJSON,"
                "/usr/include/SDL2,"
                "test,test/menu,test/popup",
    .sources  = "bench/*.c,src/*.c,src/bars/,src/menu/,src/unit,"
                "src/popup/,src/systems/,src/game/,src/map,"
                "src/controller",
    .excludes = "src/main.c",
    .links    = "SDL2,SDL2_image,SDL2_mixer,m,GLEW,noursclock,"
                "cjson,noursmath,physfs,tnecs,parg",
    .flags    = "-L/usr/lib,-fno-strict-overflow,"
                "-fno-strict-aliasing,-std=iso9899:1999,"
                "-fwrapv,-fno-delete-null-pointer-checks,"
                "-DSDL_DISABLE_IMMINTRIN_H,"
                "$(sdl2-config --cflags)",
    .cmd_pre  = "astyle --options=utils/style.txt --verbose --recursive"
                " src/*.c include/*.h test/*.c test/*.h names/*.h",
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
