/*
**  Copyright 2023-2026 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
**  Single header build system.
**  Use C to build C.
**
**  Two step build (single-header build):
**      1. Bootstrap: `gcc macefile.c -o builder`
**      2. Build: `./builder`
**
**  One step build (mace install)
**     -1. Bootstrap: `gcc installer_macefile.c -o installer`
**      0. Install `mace`: `./installer`
**      1. Build: `mace`
**          1. `mace` compiles the macefile
**          2. `mace` passes flags to builder
**          3. `mace` runs the builder
*/

#define _XOPEN_SOURCE 500 /* include POSIX 1995 */

/* -- libc -- */
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/* -- POSIX -- */
#include <ftw.h>
#include <glob.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
/* TODO: Windows  */

/*------------------------------------------*/
/*                PUBLIC API                */
/*------------------------------------------*/

/* -- User entry point -- */
/* Must be implement by user & add at
** least one target with MACE_ADD_TARGET. */
int mace(int argc, char *argv[]);

/* -- Types -- */
typedef signed      char    i8;
typedef unsigned    char    u8;
typedef signed      short   i16;
typedef unsigned    short   u16;
typedef signed      int     i32;
typedef unsigned    int     u32; /* 42u     */
typedef signed      long    i64; /* 42ll    */
typedef unsigned    long    u64; /* 42ull   */
typedef i32                 b32;
typedef char                byte;

/* -- Macro utils -- */
#define  STRINGIFY(x) _STRINGIFY(x)
#define _STRINGIFY(x) #x

/* -- Forward declarations -- */
struct Target;
struct Config;

/* -- Target -- */
#define MACE_ADD_TARGET(target) \
    mace_add_target(&target, STRINGIFY(target))
static void mace_add_target(struct Target *target,
                            const char *name);

/* When default target set by user, mace builds
** only default target and its dependencies.
** Default target is first one if not set. */
#define MACE_SET_DEFAULT_TARGET(target) \
    mace_set_default_target(STRINGIFY(target))
static void mace_set_default_target(const char *name);

/* -- Compiler -- */
/* Compiler setting priority:
**      a- defines  (e.g. -DCC=tcc)
**          builder manual compile
**      b- input argument (with -c,--cc)
**          builder run, convenience executable
**      c- config
**      d- macefile       (with MACE_SET_COMPILER) */
#define MACE_SET_COMPILER(compiler) \
    mace_set_compiler(STRINGIFY(compiler))
static void  mace_set_compiler(const char *cc);

/* -- Config -- */
/* Default config is first one if not set. */
#define MACE_SET_DEFAULT_CONFIG(target) \
    mace_set_default_config(STRINGIFY(target))
static void mace_set_default_config(const char *name);

#define MACE_ADD_CONFIG(config) \
    mace_add_config(&config, STRINGIFY(config))
static void mace_add_config(struct Config *config,
                            const char *name);

#define MACE_TARGET_CONFIG(target, config) \
    mace_target_config( STRINGIFY(target), \
                        STRINGIFY(config))
static void mace_target_config( const char *ntarget,
                                const char *nconfig);

/* --- Constants --- */
#ifndef MACE_BUILD_DIR
    #define MACE_BUILD_DIR "build"
#endif /* MACE_BUILD_DIR */
#ifndef MACE_OBJ_DIR
    #define MACE_OBJ_DIR "obj"
#endif /* MACE_OBJ_DIR */
#ifndef MACE_SEPARATOR
    #define MACE_SEPARATOR " "
#endif /* MACE_SEPARATOR */

#define LINUX_SEPARATOR "/"

enum MACE_CONSTANTS {
    MACE_CC_BUFFER      = 64,
    MACE_DEPFLAG_BUFFER =  8
};

enum MACE_TARGET_KIND { /* for target.kind */
    MACE_TARGET_NULL,
    MACE_EXECUTABLE,
    MACE_STATIC_LIBRARY,
    MACE_DYNAMIC_LIBRARY,
    MACE_PHONY,
    MACE_TARGET_KIND_NUM
};

/* --- struct definitions --- */

/* Why Macro'd struct definitions?
**  1- Public section before private for clarity
**  2- Private struct member, not struct ptr
**      - Private before public
**  Problem:
**      1 & 2 contradiction.
**  Solution:
**      Macro the definiton in public section,
**      call it in private section
*/

#define MACE_TARGET_DEFINITION \
    typedef struct Target { \
        const char *includes;   /* dirs     */ \
        /* Pattern-matching any string of zero
        ** or more characters: * wildcard (1 only)      */ \
        const char *sources;    /* files, dirs, glob*   */ \
        const char *excludes;   /* files                */ \
        const char *base_dir;   /* dir cd before build  */ \
        const char *flags;      /* passed as is         */ \
        const char *cmd_pre;    /* ran before build     */ \
        const char *cmd_post;   /* ran after  build     */ \
        const char *msg_pre;    /* printed before build */ \
        const char *msg_post;   /* printed after  build */ \
        \
        /* Links are targets or libraries.
        ** If target, it's built before self. */ \
        const char *links; \
        \
        /* Linker flags are passed to the linker as is &
        ** passed to compiler prepended \w "-Wl," */ \
        const char *link_flags; \
        \
        /* Dependencies are targets, built before self. */ \
        const char *dependencies;   /* targets          */ \
        \
        /* allatonce: Compile all .o with one call.
        ** Slightly faster.
        ** WARNING: DOES NOT WORK if multiple source
        ** files have the same filename. */ \
        b32 allatonce; \
        int kind; /* MACE_TARGET_KIND */ \
        \
        Target_Private pr; \
    } Target;

#define MACE_CONFIG_DEFINITION \
    typedef struct Config { \
        char cc[MACE_CC_BUFFER];    /* compiler     */ \
        char ar[MACE_CC_BUFFER];    /* archiver     */ \
        const char *flags;          /* passed as is */ \
        \
        Config_Private pr; \
    } Config;

/*------------------------------------------*/
/*                 PRIVATE                  */
/*------------------------------------------*/

/* Static_assert: checks at compile time */
#define STATIC_ASSERT(COND, MSG) typedef char static_assertion_##MSG[(!!(COND))*2-1]

#define COMPILE_TIME_ASSERT3(X,L) STATIC_ASSERT(X,at_line_##L)
#define COMPILE_TIME_ASSERT2(X,L) COMPILE_TIME_ASSERT3(X,L)
#define COMPILE_TIME_ASSERT(X)    COMPILE_TIME_ASSERT2(X,__LINE__)

COMPILE_TIME_ASSERT(sizeof(MACE_SEPARATOR) == 2);
COMPILE_TIME_ASSERT(sizeof(LINUX_SEPARATOR) == 2);

/* --- mace arena --- */
#ifndef PAGE_SIZE /* for Linux, on amdinours */
    #define PAGE_SIZE 4096
#endif /* PAGE_SIZE */
#ifndef MACE_MEM
    #define MACE_MEM PAGE_SIZE * 6
#endif /* MACE_MEM */

#ifndef MACE_GROW
    #define MACE_GROW 2 /* realloc growth factor */
#endif /* MACE_GROW */

#ifdef MACE_NO_HEAP
    /* no heap: memory is in data */
    static byte mem[MACE_MEM / 2];
    static byte stackmem[MACE_MEM / 2];
#endif /* MACE_NO_HEAP */

/* Smallest alloc'able memory block */
#ifndef MACE_ARENA_ALIGN
    #define MACE_ARENA_ALIGN 8 /* [bytes] */
#endif /* MACE_ARENA_ALIGN */

/* --- Mace_Arena --- */
/* Only the arena malloc/free.
** Everything else shares arenas' memory.
** Plus, easily enables MACE_NO_HEAP. */

/* Size, as multiple of basic arena block */
#define MACE_ALIGN_SIZE(size) \
    ( \
        (floor(size / MACE_ARENA_ALIGN) + 1) * \
        MACE_ARENA_ALIGN \
    )

typedef struct Mace_Arena {
    byte    *mem;
    size_t   fill; /* [bytes] mem used */
    size_t   size; /* [bytes] total mem */
} Mace_Arena;

Mace_Arena mace_arena_init(size_t size);
void mace_arena_free(   Mace_Arena *ar);
void mace_arena_clear(  Mace_Arena *ar);

void  mace_pop(     Mace_Arena *ar, size_t b);
void *mace_malloc(  size_t size);
void *mace_calloc(  size_t num, size_t size);
void *mace_realloc( void *p, size_t os, size_t ns);

/* --- Struct definitions --- */
typedef struct Target_Private {
    /* config order set from name user
    ** inputs in MACE_TARGET_CONFIG */
    int      _config; /* [order]            */
    /* target name                          */
    char    *_name;
    /* target name hash                     */
    u64      _hash;
    /* target order added by user           */
    int      _order;

    /* --- Compilation --- */
    /* argv buffer for commands             */
    char **_argv;
    /* number of arguments in argv          */
    int    _argc;
    /* alloced len of argv                  */
    int    _arg_len;
    /* tail of argv to free                 */
    int    _argc_tail;
    /* user includes, in argv form          */
    char **_argv_includes;
    /* number of args in _argv_includes     */
    int    _argc_includes;
    /* linker flags                         */
    char **_argv_link_flags;
    /* num of args in argv_links            */
    int    _argc_link_flags;
    /* linked libraries                     */
    char **_argv_links;
    /* num of args in argv_links            */
    int    _argc_links;
    /* user flags                           */
    char **_argv_flags;
    /* number of args in argv_flags         */
    int    _argc_flags;
    /* sources                              */
    char **_argv_sources;
    /* number of args in argv_sources       */
    int    _argc_sources;
    /* alloc len of args in argv_sources    */
    int    _len_sources;

    /* WARNING: _argv_objects_hash DOES NOT
    ** include objects with numbers to prevent
    ** collisions! */
    /* objects, in argv form        */
    u64      *_argv_objects_hash;
    /* num of args in argv_sources  */
    int       _argc_objects_hash;
    /* sources, in argv form        */
    char    **_argv_objects;
    /* sources num                  */
    int      *_argv_objects_cnt;
    /* Note: Includes objects with number to
    ** prevent collisions.          */
    u64      *_objects_hash_nocoll;
    int       _objects_hash_nocoll_num;
    int       _objects_hash_nocoll_len;

    /* -- Exclusions --  */
    /* hash of excluded source files      */
    u64 *_excludes;
    int  _excludes_num;
    int  _excludes_len;

    /* --- Dependencies ---  */
    /* -- Target dependencies --  */
    /* target or libs hashes               */
    u64     *_deps_links;
    /* target or libs hashes               */
    size_t   _deps_links_num;
    /* target or libs hashes               */
    size_t   _deps_links_len;
    /* dependency count, for build order   */
    size_t   _d_cnt;

    /* -- Object dependencies --  */
    u64     *_headers_checksum_hash;
    char   **_headers_checksum;
    /* # hdrs with same path   */
    int     *_headers_checksum_cnt;

    /* [hdr_order] filenames       */
    char   **_headers;
    /* Note: num _headers == _headers_checksum */
    /* [hdr_order] filename hashes */
    u64     *_headers_hash;
    /* len of headers              */
    int      _headers_num;
    /* number of headers           */
    int      _headers_len;
    /* [arg_src][dep_order] hdr_order  */
    int    **_deps_headers;
    /* len of object header deps   */
    int     *_deps_headers_num;
    /* num of object header deps   */
    int     *_deps_headers_len;

    /* --- Check for cwd in header dependencies ---  */
    b32 _checkcwd;

    /* --- Recompile switches ---  */
    /* [argc_source]    */
    b32 *_recompiles;
    /* [hdr_order]      */
    b32 *_hdrs_changed;

} Target_Private;

typedef struct Config_Private {
    /* order config was added by user */
    int      _order;

    char    *_name;
    u64      _hash;

    char   **_flags;
    int      _flag_num;
} Config_Private;

MACE_TARGET_DEFINITION

MACE_CONFIG_DEFINITION

typedef struct Mace_Args {
    char *user_target;
    char *macefile;
    char *user_config;
    char *dir;
    char *cc;
    char *ar;
    char *cflags;
    char *cc_depflag;
    u64   user_target_hash;
    u64   user_config_hash;
    int   jobs;
    b32   debug;
    b32   silent;
    b32   dry_run;
    b32   build_all;
} Mace_Args;

/***************** CONSTANTS ****************/
#define MACE_VER_MAJOR 6
#define MACE_VER_MINOR 0
#define MACE_VER_PATCH 0
#define MACE_VER_STRING \
    STRINGIFY(MACE_VER_MAJOR)"."\
    STRINGIFY(MACE_VER_MINOR)"."\
    STRINGIFY(MACE_VER_PATCH)

#define MACE_SHA1_EXT   ".sha1"
#define MACE_WILDCARD   '*'
#define MACE_SRC_EXT    ".c"
#define MACE_HDR_EXT    ".h"
#define MACE_DEP_EXT    'd'
#define MACE_HO_EXT     "ho"

enum MACE_PRIVATE_CONSTANTS {
    MACE_DEFAULT_TARGET_LEN =    8,
    MACE_SHA1_EXT_LEN       =   sizeof(MACE_SHA1_EXT) - 1,
    MACE_USAGE_MIDCOLW      =   12,
    /* SHA1DC_LEN is a magic number in sha1dc */
    SHA1DC_LEN              =   20
};
#ifndef MACE_JOBS
    #define MACE_JOBS 12
#endif /* MACE_JOBS */
#if MACE_JOBS <= 0
    #error MACE_JOBS should be > 0
#endif /* MACE_JOBS <= 0 */

#ifndef MACE_CWD_BUFFER
    #define MACE_CWD_BUFFER 256
#endif /* MACE_CWD_BUFFER */

#ifndef MACE_OBJDEP_BUFFER
    #define MACE_OBJDEP_BUFFER 4096
#endif /* MACE_OBJDEP_BUFFER */

#ifndef MACE_MAX_ITERATIONS
    #define MACE_MAX_ITERATIONS 1024
#endif /* MACE_MAX_ITERATIONS */

enum MACE_ARGV {
    /* single source compilation */
    MACE_ARGV_CC,
    MACE_ARGV_SOURCE,
    MACE_ARGV_OBJECT,
    MACE_ARGV_OTHER
};

enum MACE_CHECKSUM_MODE {
    MACE_CHECKSUM_MODE_NULL,
    MACE_CHECKSUM_MODE_SRC,
    MACE_CHECKSUM_MODE_INCLUDE
};

/****************** DECLARATIONS ******************/
/* --- mace --- */
static void mace_build(void);
static void mace_pre_build(void);

static int mace_target_order(u64 hash);
static int mace_config_order(u64 hash);

static void mace_pre_user(  const   Mace_Args *args);
static void mace_post_user( const   Mace_Args *args);
static void mace_post_build(        Mace_Args *args);

/* --- mace_args --- */
static Mace_Args mace_parse_args(int     argc,
                                 char   *argv[]);
static Mace_Args mace_parse_env(void);
static void mace_parse_cflags(void);
static Mace_Args mace_combine_args_env(Mace_Args args,
                                       Mace_Args env);
/* --- setters --- */

/* -- Archiver -- */
/* NOTE: Automatically set in mace_set_compiler */
/* Archiver setting priority:
**      a- defines (e.g. -DAR=ar)
**      b- input argument (with -a,--ar)
**      c- config
**      d- macefile       (with MACE_SET_ARCHIVER) */
#define MACE_SET_ARCHIVER(archiver) \
    mace_set_archiver(STRINGIFY(archiver))
static void  mace_set_archiver(const char *ar);

/* -- cc_depflag -- */
/* NOTE: Automatically set in mace_set_compiler */
/* Compiler flag to build .d dependency files
** Ex: gcc -MM ... */
#define MACE_SET_CC_DEPFLAG(cc_depflag) \
    mace_set_cc_depflag(STRINGIFY(cc_depflag))
static void  mace_set_cc_depflag(const char *depflag);

/* --- mace_utils --- */
static char *_mace_copy_str( Mace_Arena *arena,
                            const char *tocpy);
static char *mace_copy_str( const char *tocpy);

/* --- mace_criteria --- */
typedef struct Mace_Checksum {
    FILE            *file;
    const char      *file_path;
    const char      *checksum_path;
    u8               hash_current[SHA1DC_LEN];
    u8               hash_previous[SHA1DC_LEN];
} Mace_Checksum;

static void mace_checksum(          Mace_Checksum *chk);
static b32  mace_checksum_cmp(const Mace_Checksum *chk);

static b32  mace_file_changed(const char *checksum,
                              const char *header);
static void mace_checksum_w(Mace_Checksum *checksum);
static void mace_checksum_r(Mace_Checksum *checksum);

/* --- mace_hashing --- */
static u64 mace_hash(const char *str);

/* -- argv -- */
static char **mace_argv_flags(int            *len,
                              int            *argc,
                              char          **argv,
                              const char     *includes,
                              const char     *flag,
                              b32             path,
                              const char     *separator);

/* --- mace_setters --- */
static void  mace_set_cc_depflag(const char *depflag);
static void  mace_set_cflags(char *cflags);

/* -- Target struct OOP -- */
/* - Grow - */
static void mace_Target_Grow_Headers(       Target *t);
static void mace_Target_Grow_deps_headers(  Target *t,
                                            int oid);

/* - hash - */
static int  Target_hasObjectHash(const Target *target,
                                 u64 hash);
static void Target_Object_Hash_Add(Target *target,
                                   u64 hash);
static int  Target_hasObjectHash_nocoll(const Target *target,
                                        u64 hash);
static void Target_Object_Hash_Add_nocoll(Target *target,
                                          u64 hash);

/* - obj_deps - */
static char *mace_Target_Read_d(Target *target,
                                int source_i);
static void mace_Target_Read_ho(Target *target,
                                int source_i);
static void mace_Target_Read_Objdeps(Target *target,
                                     char *deps,
                                     int source_i);
static void mace_Target_Parse_Objdep(Target *target,
                                     int source_i);
static int  mace_Target_header_order(Target *target,
                                     u64 hash);
static void mace_Target_Parse_Objdeps(Target *target);

/* - target dependencies - */
static b32 mace_Target_hasDep(const Target *target,
                              u64 hash);
static void mace_Target_Deps_Add(Target *target,
                                 u64 hash);
static void mace_Target_Deps_Hash(Target *target);
static void mace_Target_Deps_Grow(Target *target);

/* - Adding Files - */
static b32  mace_Target_Source_Add(Target *target,
                                   char *token);
static b32  mace_Target_Object_Add(Target *target,
                                   char *token);
static u64  mace_Target_Header_Add(Target *target,
                                   char *header);
static void mace_Target_Objdep_Add(Target *target,
                                   int header_order,
                                   int obj_hash_id);
static void mace_Target_Header_Add_Objpath(Target *target,
                                           char *header);

/* - Checksums - */
static b32 mace_Source_Checksum(const Target *target,
                                const char *s, const char *o);
static void mace_Headers_Checksums(const Target *target);
static void mace_Headers_Checksums_Checks(Target *target);

/* - argv - */
static void mace_argv_add_config(char ** *argv,
                                 int *argc,
                                 int *arg_len);
static void mace_argv_add_cflags(char ** *argv,
                                 int *argc, int *arg_len);

static void mace_Target_argv_grow(Target  *t);
static void mace_Target_Parse_User(Target  *t);
static void mace_Target_argv_compile(Target  *t);
static void mace_Target_Parse_Source(Target  *t,
                                     char    *path,
                                     char    *src);
static void mace_Target_argv_allatonce(Target  *t);

static char **mace_argv_grow(char       **argv,
                             const int   *argc,
                             int         *arg_len);
/* - recompilation flag - */
static void mace_Target_Recompiles_Add(Target *target,
                                       b32 add);

/* - compilation - */
static void mace_Target_compile(            Target *t);
static void mace_Target_precompile(         Target *t);
static void mace_Target_compile_allatonce(  Target *t);

/* --- mace_glob --- */
static int      mace_globerr(   const char *path,
                                int eerrno);
static glob_t   mace_glob_sources(const char *path);

/* --- mace_exec --- */
static pid_t mace_exec(const char *exec,
                       char *const arguments[]);
static void  mace_wait_pid(int pid);
static void  mace_argv_print(char *const argv[],
                             size_t argc);

/* --- mace_build --- */
/* -- linking -- */
static void mace_link_executable(       Target *t);
static void mace_link_static_library(   Target *t);
static void mace_link_dynamic_library(  Target *t);

typedef void (*mace_link_t)(Target *);
mace_link_t mace_link[MACE_TARGET_KIND_NUM - 1] = {
    mace_link_executable,
    mace_link_static_library,
    mace_link_dynamic_library
};

/* -- compiling object files -> .o -- */
static void mace_compile_glob(Target        *target,
                              const char    *globsrc,
                              const char    *flags);
static void mace_build_target(Target        *target);
static void mace_run_commands(const char    *commands,
                              const char    *preorpost,
                              const char    *target_name);
static void mace_print_message(const char   *message);

/* -- build_order -- */
static b32 mace_in_build_order(size_t       order,
                               const int   *build_order,
                               int          num);
static void mace_user_target_set(u64     hash);
static void mace_user_config_set(u64     hash);
static void mace_config_resolve(const Target  *target);
static void mace_target_resolve(void);
static void mace_default_target_order(void);

/* -- configs -- */
static void mace_parse_configs(void);
static void mace_parse_config(Config *config);

/* - build order of all targets - */
static void mace_build_order(void);
static void mace_build_order_recursive(Target target,
                                       size_t *o_cnt);

/* --- mace_is --- */
static int mace_isDir(      const char *path);
static int mace_isSource(   const char *path);
static int mace_isWildcard( const char *str);

/* --- mace_filesystem --- */
static void  mace_mkdir(const char *path);
static void  mace_make_dirs(void);
static void  mace_object_path(  const char *source);
static char *mace_library_path( Mace_Arena *arena,
                                const char *name, int kind);
static char *mace_executable_path(  Mace_Arena *arena,
                                    const char *name);
static char *mace_checksum_filename(const char *file, int mode);

/* --- mace_pqueue --- */
static void  mace_pqueue_put(pid_t pid);
static pid_t mace_pqueue_pop(void);

/* --- mace utils --- */
static void mace_chdir(     const   char *path);
static void mace_getcwd(    void);
static b32  mace_fexists(   const   char *path);

/******************* GLOBALS ********************/
#define false 0
#define true 1

typedef struct Mace_Globals {
    Mace_Arena arena;     /* keep */
    /* use stackrena in 1 scope, not alloca/malloc
    **  1. alloca is bad: UB on stack overflow 
    **  2. malloc&free in 1 scope is wasteful */
    Mace_Arena stackrena; /* clear every scope */

    /* -- current working directory -- */
    char cwd[MACE_CWD_BUFFER];

    /* --- Processes --- */
    /* 1. Compile objects in parallel. */
    /* 2. Compile targets in series. */
    pid_t    pqueue[MACE_JOBS];
    int      pnum;

    /* -- runtime flags -- */
    b32 silent;
    b32 verbose;
    /* dry_run: Pre-compile, don't compile */
    b32 dry_run;
    /* build_all: Build all targets */
    b32 build_all;

    /* Default config (may be set by user) [order] */
    int mace_default_config;
    /* Config input by user */
    int mace_user_config;
    /* Config to use */
    int mace_config;

    /* Default target (may be set by user) [order] */
    int mace_default_target;
    /* Target input by user */
    int mace_user_target;
    /* Target to compile */
    int mace_target;

    /* -- hashes -- */
    u64 mace_default_target_hash;
    u64 mace_default_config_hash;

    /* -- buffer to write object -- */
    char     *object;
    size_t    object_len;

    /* -- build order for user target -- */
    int *build_order;
    int  build_order_num;

    /* -- list of targets added by user -- */
    /* [order] as added    */
    Target  *targets;
    size_t   target_num;
    size_t   target_len;

    /* -- list of configs added by user -- */
    /* [order] as added    */
    Config  *configs;
    size_t   config_num;
    size_t   config_len;

    /* cflags: passed to compiler */
    char  *cflags;
    char **cflags_sep;
    size_t cflags_num;
} Mace_Globals;
static Mace_Globals gl = {0};

typedef struct Mace_Global_Strings {
    /* -- separator -- */
    char mace_command_separator[3];

    /* -- Compiler -- */
    /* cc: gcc, clang or tcc */
    char cc[MACE_CC_BUFFER];
    /* ar: ar, llvm-ar or tcc -ar */
    char ar[MACE_CC_BUFFER];

    /* flag to create .d file */
    char cc_depflag[MACE_DEPFLAG_BUFFER];
} Mace_Global_Strings;

static Mace_Global_Strings glstr = {
    /* mace_command_separator   */  "&&",
    /* cc                       */  "gcc",
    /* ar                       */  "ar",
    /* cc_depflag               */  "-MM"
};

/* -- mace_globals control -- */
static void mace_object_grow(void);

/*************** SHA1DC DECLARATION ***************/

/***
* Modified in 2026 by Gabriel Taillon for mace
* Copyright 2017 Marc Stevens <marc@marc-stevens.nl>, Dan Shumow <danshu@microsoft.com>
* Distributed under the MIT Software License.
* See accompanying file LICENSE.txt or copy at
* https://opensource.org/licenses/MIT
***/

/* sha-1 compression function that takes an already expanded message, and additionally store intermediate states */
/* only stores states ii (the state between step ii-1 and step ii) when DOSTORESTATEii is defined in ubc_check.h */
void sha1_compression_states(u32[5], const u32[16], u32[80], u32[80][5]);

/* A callback function type that can be set to be called when a collision block has been found: */
/* void collision_block_callback(u64 byteoffset, const u32 ihvin1[5], const u32 ihvin2[5], const u32 m1[80], const u32 m2[80]) */
typedef void(*collision_block_callback)(u64, const u32 *, const u32 *,
                                        const u32 *, const u32 *);

/* The SHA-1 context. */
typedef struct {
    u64 total;
    u32 ihv[5];
    unsigned char buffer[64];
    int found_collision;
    int safe_hash;
    int detect_coll;
    int ubc_check;
    int reduced_round_coll;
    collision_block_callback callback;

    u32 ihv1[5];
    u32 ihv2[5];
    u32 m1[80];
    u32 m2[80];
    u32 states[80][5];
} SHA1_CTX;

/* Initialize SHA-1 context. */
void SHA1DCInit(SHA1_CTX *);

/* update SHA-1 context with buffer contents */
void SHA1DCUpdate(SHA1_CTX *, const char *, size_t);

/* obtain SHA-1 hash from SHA-1 context */
/* returns: 0 = no collision detected, otherwise = collision found => warn user for active attack */
int  SHA1DCFinal(unsigned char[SHA1DC_LEN], SHA1_CTX *);

/***
* Modified in 2026 by Gabriel Taillon for mace
* Copyright 2017 Marc Stevens <marc@marc-stevens.nl>, Dan Shumow <danshu@microsoft.com>
* Distributed under the MIT Software License.
* See accompanying file LICENSE.txt or copy at
* https://opensource.org/licenses/MIT
***/

/*
// this file was generated by the 'parse_bitrel' program in the tools section
// using the data files from directory 'tools/data/3565'
//
// sha1_dvs contains a list of SHA-1 Disturbance Vectors (DV) to check
// dvType, dvK and dvB define the DV: I(K,B) or II(K,B) (see the paper)
// dm[80] is the expanded message block XOR-difference defined by the DV
// testt is the step to do the recompression from for collision detection
// maski and maskb define the bit to check for each DV in the dvmask returned by ubc_check
//
// ubc_check takes as input an expanded message block and verifies the unavoidable bitconditions for all listed DVs
// it returns a dvmask where each bit belonging to a DV is set if all unavoidable bitconditions for that DV have been met
// thus one needs to do the recompression check for each DV that has its bit set
*/

#define DVMASKSIZE 1
typedef struct {
    int dvType;
    int dvK;
    int dvB;
    int testt;
    int maski;
    int maskb;
    u32 dm[80];
} dv_info_t;
dv_info_t sha1_dvs[];
void ubc_check(const u32 W[80], u32 dvmask[DVMASKSIZE]);

#define DOSTORESTATE58
#define DOSTORESTATE65

/************* SHA1DC DECLARATION END *************/

/**************** PARG DECLARATION ****************/
/*
** parg - parse argv
**
** Modified in 2023-2026 by Gabriel Taillon for mace
**
** The MIT No Attribution License (MIT-0)
**
** Copyright 2015-2023 Joergen Ibsen
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software"),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
** THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
*/

#define PARG_VER_MAJOR 1
#define PARG_VER_MINOR 0
#define PARG_VER_PATCH 2
#define PARG_VER_STRING \
    STRINGIFY(PARG_VER_MAJOR)"."\
    STRINGIFY(PARG_VER_MINOR)"."\
    STRINGIFY(PARG_VER_PATCH)

enum PARG_HAS_ARG {
    PARG_NOARG,
    PARG_REQARG, /* required */
    PARG_OPTARG  /* optional */
};

struct parg_state {
    const char *optarg;   /* option argument*/
    int optind;           /* next index */
    int optopt;           /* error option */
    const char *nextchar;
};
struct parg_state parg_state_default;

/* Long options for `parg_getopt_long()` */
struct parg_opt {
    const char *name;
    int has_arg;
    int *flag;
    int val;
    const char *arg;
    const char *doc;
};

/* - option matching - */
int match_long( struct parg_state *ps, int c,
                char *const v[], const char *o,
                const struct parg_opt *lo, int *li);
int match_short(struct parg_state *ps, int c,
                char *const v[], const char *os);

/* - utilities - */
int is_argv_end(const struct parg_state *ps,
                int c, char *const v[]);

/* - parg public API: getopt and getopt_long - */
int parg_getopt_long(struct parg_state *ps, int c,
                     char *const v[], const char *os,
                     const struct parg_opt *lo, int *li);

/*************** PARG DECLARATION END ***************/

/******************* SHA1DC SOURCE ******************/
#define SHA1DC_INIT_SAFE_HASH_DEFAULT 1

#if (defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || \
defined(i386) || defined(__i386) || defined(__i386__) || defined(__i486__)  || \
defined(__i586__) || defined(__i686__) || defined(_M_IX86) || defined(__X86__) || \
defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__) || \
defined(__386) || defined(_M_X64) || defined(_M_AMD64))
#define SHA1DC_ON_INTEL_LIKE_PROCESSOR
#endif

/*
   Because Little-Endian architectures are most common,
   we only set SHA1DC_BIGENDIAN if one of these conditions is met.
   Note that all MSFT platforms are little endian,
   so none of these will be defined under the MSC compiler.
   If you are compiling on a big endian platform and your compiler does not define one of these,
   you will have to add whatever macros your tool chain defines to indicate Big-Endianness.
 */

#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__)
/*
 * Should detect Big Endian under GCC since at least 4.6.0 (gcc svn
 * rev #165881). See
 * https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
 *
 * This also works under clang since 3.2, it copied the GCC-ism. See
 * clang.git's 3b198a97d2 ("Preprocessor: add __BYTE_ORDER__
 * predefined macro", 2012-07-27)
 */
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define SHA1DC_BIGENDIAN
#endif

/* Not under GCC-alike */
#elif defined(__BYTE_ORDER) && defined(__BIG_ENDIAN)
/*
 * Should detect Big Endian under glibc.git since 14245eb70e ("entered
 * into RCS", 1992-11-25). Defined in <endian.h> which will have been
 * brought in by standard headers. See glibc.git and
 * https://sourceforge.net/p/predef/wiki/Endianness/
 */
#if __BYTE_ORDER == __BIG_ENDIAN
    #define SHA1DC_BIGENDIAN
#endif

/* Not under GCC-alike or glibc */
#elif defined(_BYTE_ORDER) && defined(_BIG_ENDIAN) && defined(_LITTLE_ENDIAN)
/*
 * *BSD and newlib (embedded linux, cygwin, etc).
 * the defined(_BIG_ENDIAN) && defined(_LITTLE_ENDIAN) part prevents
 * this condition from matching with Solaris/sparc.
 * (Solaris defines only one endian macro)
 */
#if _BYTE_ORDER == _BIG_ENDIAN
    #define SHA1DC_BIGENDIAN
#endif

/* Not under GCC-alike or glibc or *BSD or newlib */
#elif (defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || \
defined(__MIPSEB__) || defined(__MIPSEB) || defined(_MIPSEB) || \
defined(__sparc))
/*
 * Should define Big Endian for a whitelist of known processors. See
 * https://sourceforge.net/p/predef/wiki/Endianness/ and
 * http://www.oracle.com/technetwork/server-storage/solaris/portingtosolaris-138514.html
 */
#define SHA1DC_BIGENDIAN

/* Not under GCC-alike or glibc or *BSD or newlib or <processor whitelist> */
#elif (defined(_AIX) || defined(__hpux))

/*
 * Defines Big Endian on a whitelist of OSs that are known to be Big
 * Endian-only. See
 * https://public-inbox.org/git/93056823-2740-d072-1ebd-46b440b33d7e@felt.demon.nl/
 */
#define SHA1DC_BIGENDIAN

/* Not under GCC-alike or glibc or *BSD or newlib or <processor whitelist> or <os whitelist> */
#elif defined(SHA1DC_ON_INTEL_LIKE_PROCESSOR)
/*
 * As a last resort before we do anything else we're not 100% sure
 * about below, we blacklist specific processors here. We could add
 * more, see e.g. https://wiki.debian.org/ArchitectureSpecificsMemo
 */
#else /* Not under GCC-alike or glibc or *BSD or newlib or <processor whitelist> or <os whitelist> or <processor blacklist> */

/* We do nothing more here for now */
/*#error "Uncomment this to see if you fall through all the detection"*/

#endif /* Big Endian detection */

#if (defined(SHA1DC_FORCE_LITTLEENDIAN) && defined(SHA1DC_BIGENDIAN))
    #undef SHA1DC_BIGENDIAN
#endif
#if (defined(SHA1DC_FORCE_BIGENDIAN) && !defined(SHA1DC_BIGENDIAN))
    #define SHA1DC_BIGENDIAN
#endif
/*ENDIANNESS SELECTION*/

#ifndef SHA1DC_FORCE_ALIGNED_ACCESS
    #if defined(SHA1DC_FORCE_UNALIGNED_ACCESS) || defined(SHA1DC_ON_INTEL_LIKE_PROCESSOR)
        #define SHA1DC_ALLOW_UNALIGNED_ACCESS
    #endif /*UNALIGNED ACCESS DETECTION*/
#endif /*FORCE ALIGNED ACCESS*/

#define rotate_right(x,n) (((x)>>(n))|((x)<<(32-(n))))
#define rotate_left(x,n)  (((x)<<(n))|((x)>>(32-(n))))

#define sha1_bswap32(x) \
    {x = ((x << 8) & 0xFF00FF00) | ((x >> 8) & 0xFF00FF); x = (x << 16) | (x >> 16);}

#define sha1_mix(W, t)  (rotate_left(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1))

#ifdef SHA1DC_BIGENDIAN
    #define sha1_load(m, t, temp)  { temp = m[t]; }
#else
    #define sha1_load(m, t, temp)  { temp = m[t]; sha1_bswap32(temp); }
#endif

#define sha1_store(W, t, x) *(volatile u32 *)&W[t] = x

#define sha1_f1(b,c,d) ((d)^((b)&((c)^(d))))
#define sha1_f2(b,c,d) ((b)^(c)^(d))
#define sha1_f3(b,c,d) (((b)&(c))+((d)&((b)^(c))))
#define sha1_f4(b,c,d) ((b)^(c)^(d))

#define HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, m, t) \
    { e += rotate_left(a, 5) + sha1_f1(b,c,d) + 0x5A827999 + m[t]; b = rotate_left(b, 30); }
#define HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, m, t) \
    { e += rotate_left(a, 5) + sha1_f2(b,c,d) + 0x6ED9EBA1 + m[t]; b = rotate_left(b, 30); }
#define HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, m, t) \
    { e += rotate_left(a, 5) + sha1_f3(b,c,d) + 0x8F1BBCDC + m[t]; b = rotate_left(b, 30); }
#define HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, m, t) \
    { e += rotate_left(a, 5) + sha1_f4(b,c,d) + 0xCA62C1D6 + m[t]; b = rotate_left(b, 30); }

#define HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(a, b, c, d, e, m, t) \
    { b = rotate_right(b, 30); e -= rotate_left(a, 5) + sha1_f1(b,c,d) + 0x5A827999 + m[t]; }
#define HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(a, b, c, d, e, m, t) \
    { b = rotate_right(b, 30); e -= rotate_left(a, 5) + sha1_f2(b,c,d) + 0x6ED9EBA1 + m[t]; }
#define HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(a, b, c, d, e, m, t) \
    { b = rotate_right(b, 30); e -= rotate_left(a, 5) + sha1_f3(b,c,d) + 0x8F1BBCDC + m[t]; }
#define HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(a, b, c, d, e, m, t) \
    { b = rotate_right(b, 30); e -= rotate_left(a, 5) + sha1_f4(b,c,d) + 0xCA62C1D6 + m[t]; }

#define SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(a, b, c, d, e, m, W, t, temp) \
    {sha1_load(m, t, temp); sha1_store(W, t, temp); e += temp + rotate_left(a, 5) + sha1_f1(b,c,d) + 0x5A827999; b = rotate_left(b, 30);}

#define SHA1COMPRESS_FULL_ROUND1_STEP_EXPAND(a, b, c, d, e, W, t, temp) \
    {temp = sha1_mix(W, t); sha1_store(W, t, temp); e += temp + rotate_left(a, 5) + sha1_f1(b,c,d) + 0x5A827999; b = rotate_left(b, 30); }

#define SHA1COMPRESS_FULL_ROUND2_STEP(a, b, c, d, e, W, t, temp) \
    {temp = sha1_mix(W, t); sha1_store(W, t, temp); e += temp + rotate_left(a, 5) + sha1_f2(b,c,d) + 0x6ED9EBA1; b = rotate_left(b, 30); }

#define SHA1COMPRESS_FULL_ROUND3_STEP(a, b, c, d, e, W, t, temp) \
    {temp = sha1_mix(W, t); sha1_store(W, t, temp); e += temp + rotate_left(a, 5) + sha1_f3(b,c,d) + 0x8F1BBCDC; b = rotate_left(b, 30); }

#define SHA1COMPRESS_FULL_ROUND4_STEP(a, b, c, d, e, W, t, temp) \
    {temp = sha1_mix(W, t); sha1_store(W, t, temp); e += temp + rotate_left(a, 5) + sha1_f4(b,c,d) + 0xCA62C1D6; b = rotate_left(b, 30); }


#define SHA1_STORE_STATE(i) states[i][0] = a; states[i][1] = b; states[i][2] = c; states[i][3] = d; states[i][4] = e;

static void sha1_compression_W(u32 ihv[5], const u32 W[80]) {
    u32 a = ihv[0], b = ihv[1], c = ihv[2], d = ihv[3], e = ihv[4];

    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 0);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 1);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 2);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 3);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 4);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 5);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 6);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 7);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 8);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 9);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 10);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 11);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 12);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 13);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 14);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 15);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 16);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 17);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 18);
    HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 19);

    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 20);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 21);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 22);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 23);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 24);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 25);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 26);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 27);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 28);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 29);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 30);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 31);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 32);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 33);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 34);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 35);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 36);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 37);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 38);
    HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 39);

    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 40);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 41);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 42);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 43);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 44);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 45);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 46);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 47);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 48);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 49);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 50);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 51);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 52);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 53);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 54);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 55);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 56);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 57);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 58);
    HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 59);

    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 60);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 61);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 62);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 63);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 64);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 65);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 66);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 67);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 68);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 69);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 70);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 71);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 72);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 73);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 74);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 75);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 76);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 77);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 78);
    HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 79);

    ihv[0] += a;
    ihv[1] += b;
    ihv[2] += c;
    ihv[3] += d;
    ihv[4] += e;
}

void sha1_compression_states(u32 ihv[5], const u32 m[16], u32 W[80],
                             u32 states[80][5]) {
    u32 a = ihv[0], b = ihv[1], c = ihv[2], d = ihv[3], e = ihv[4];
    u32 temp;

#ifdef DOSTORESTATE00
    SHA1_STORE_STATE(0)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(a, b, c, d, e, m, W, 0, temp);

#ifdef DOSTORESTATE01
    SHA1_STORE_STATE(1)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(e, a, b, c, d, m, W, 1, temp);

#ifdef DOSTORESTATE02
    SHA1_STORE_STATE(2)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(d, e, a, b, c, m, W, 2, temp);

#ifdef DOSTORESTATE03
    SHA1_STORE_STATE(3)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(c, d, e, a, b, m, W, 3, temp);

#ifdef DOSTORESTATE04
    SHA1_STORE_STATE(4)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(b, c, d, e, a, m, W, 4, temp);

#ifdef DOSTORESTATE05
    SHA1_STORE_STATE(5)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(a, b, c, d, e, m, W, 5, temp);

#ifdef DOSTORESTATE06
    SHA1_STORE_STATE(6)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(e, a, b, c, d, m, W, 6, temp);

#ifdef DOSTORESTATE07
    SHA1_STORE_STATE(7)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(d, e, a, b, c, m, W, 7, temp);

#ifdef DOSTORESTATE08
    SHA1_STORE_STATE(8)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(c, d, e, a, b, m, W, 8, temp);

#ifdef DOSTORESTATE09
    SHA1_STORE_STATE(9)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(b, c, d, e, a, m, W, 9, temp);

#ifdef DOSTORESTATE10
    SHA1_STORE_STATE(10)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(a, b, c, d, e, m, W, 10, temp);

#ifdef DOSTORESTATE11
    SHA1_STORE_STATE(11)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(e, a, b, c, d, m, W, 11, temp);

#ifdef DOSTORESTATE12
    SHA1_STORE_STATE(12)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(d, e, a, b, c, m, W, 12, temp);

#ifdef DOSTORESTATE13
    SHA1_STORE_STATE(13)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(c, d, e, a, b, m, W, 13, temp);

#ifdef DOSTORESTATE14
    SHA1_STORE_STATE(14)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(b, c, d, e, a, m, W, 14, temp);

#ifdef DOSTORESTATE15
    SHA1_STORE_STATE(15)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_LOAD(a, b, c, d, e, m, W, 15, temp);

#ifdef DOSTORESTATE16
    SHA1_STORE_STATE(16)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_EXPAND(e, a, b, c, d, W, 16, temp);

#ifdef DOSTORESTATE17
    SHA1_STORE_STATE(17)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_EXPAND(d, e, a, b, c, W, 17, temp);

#ifdef DOSTORESTATE18
    SHA1_STORE_STATE(18)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_EXPAND(c, d, e, a, b, W, 18, temp);

#ifdef DOSTORESTATE19
    SHA1_STORE_STATE(19)
#endif
    SHA1COMPRESS_FULL_ROUND1_STEP_EXPAND(b, c, d, e, a, W, 19, temp);

#ifdef DOSTORESTATE20
    SHA1_STORE_STATE(20)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(a, b, c, d, e, W, 20, temp);

#ifdef DOSTORESTATE21
    SHA1_STORE_STATE(21)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(e, a, b, c, d, W, 21, temp);

#ifdef DOSTORESTATE22
    SHA1_STORE_STATE(22)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(d, e, a, b, c, W, 22, temp);

#ifdef DOSTORESTATE23
    SHA1_STORE_STATE(23)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(c, d, e, a, b, W, 23, temp);

#ifdef DOSTORESTATE24
    SHA1_STORE_STATE(24)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(b, c, d, e, a, W, 24, temp);

#ifdef DOSTORESTATE25
    SHA1_STORE_STATE(25)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(a, b, c, d, e, W, 25, temp);

#ifdef DOSTORESTATE26
    SHA1_STORE_STATE(26)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(e, a, b, c, d, W, 26, temp);

#ifdef DOSTORESTATE27
    SHA1_STORE_STATE(27)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(d, e, a, b, c, W, 27, temp);

#ifdef DOSTORESTATE28
    SHA1_STORE_STATE(28)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(c, d, e, a, b, W, 28, temp);

#ifdef DOSTORESTATE29
    SHA1_STORE_STATE(29)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(b, c, d, e, a, W, 29, temp);

#ifdef DOSTORESTATE30
    SHA1_STORE_STATE(30)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(a, b, c, d, e, W, 30, temp);

#ifdef DOSTORESTATE31
    SHA1_STORE_STATE(31)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(e, a, b, c, d, W, 31, temp);

#ifdef DOSTORESTATE32
    SHA1_STORE_STATE(32)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(d, e, a, b, c, W, 32, temp);

#ifdef DOSTORESTATE33
    SHA1_STORE_STATE(33)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(c, d, e, a, b, W, 33, temp);

#ifdef DOSTORESTATE34
    SHA1_STORE_STATE(34)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(b, c, d, e, a, W, 34, temp);

#ifdef DOSTORESTATE35
    SHA1_STORE_STATE(35)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(a, b, c, d, e, W, 35, temp);

#ifdef DOSTORESTATE36
    SHA1_STORE_STATE(36)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(e, a, b, c, d, W, 36, temp);

#ifdef DOSTORESTATE37
    SHA1_STORE_STATE(37)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(d, e, a, b, c, W, 37, temp);

#ifdef DOSTORESTATE38
    SHA1_STORE_STATE(38)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(c, d, e, a, b, W, 38, temp);

#ifdef DOSTORESTATE39
    SHA1_STORE_STATE(39)
#endif
    SHA1COMPRESS_FULL_ROUND2_STEP(b, c, d, e, a, W, 39, temp);



#ifdef DOSTORESTATE40
    SHA1_STORE_STATE(40)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(a, b, c, d, e, W, 40, temp);

#ifdef DOSTORESTATE41
    SHA1_STORE_STATE(41)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(e, a, b, c, d, W, 41, temp);

#ifdef DOSTORESTATE42
    SHA1_STORE_STATE(42)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(d, e, a, b, c, W, 42, temp);

#ifdef DOSTORESTATE43
    SHA1_STORE_STATE(43)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(c, d, e, a, b, W, 43, temp);

#ifdef DOSTORESTATE44
    SHA1_STORE_STATE(44)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(b, c, d, e, a, W, 44, temp);

#ifdef DOSTORESTATE45
    SHA1_STORE_STATE(45)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(a, b, c, d, e, W, 45, temp);

#ifdef DOSTORESTATE46
    SHA1_STORE_STATE(46)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(e, a, b, c, d, W, 46, temp);

#ifdef DOSTORESTATE47
    SHA1_STORE_STATE(47)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(d, e, a, b, c, W, 47, temp);

#ifdef DOSTORESTATE48
    SHA1_STORE_STATE(48)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(c, d, e, a, b, W, 48, temp);

#ifdef DOSTORESTATE49
    SHA1_STORE_STATE(49)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(b, c, d, e, a, W, 49, temp);

#ifdef DOSTORESTATE50
    SHA1_STORE_STATE(50)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(a, b, c, d, e, W, 50, temp);

#ifdef DOSTORESTATE51
    SHA1_STORE_STATE(51)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(e, a, b, c, d, W, 51, temp);

#ifdef DOSTORESTATE52
    SHA1_STORE_STATE(52)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(d, e, a, b, c, W, 52, temp);

#ifdef DOSTORESTATE53
    SHA1_STORE_STATE(53)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(c, d, e, a, b, W, 53, temp);

#ifdef DOSTORESTATE54
    SHA1_STORE_STATE(54)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(b, c, d, e, a, W, 54, temp);

#ifdef DOSTORESTATE55
    SHA1_STORE_STATE(55)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(a, b, c, d, e, W, 55, temp);

#ifdef DOSTORESTATE56
    SHA1_STORE_STATE(56)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(e, a, b, c, d, W, 56, temp);

#ifdef DOSTORESTATE57
    SHA1_STORE_STATE(57)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(d, e, a, b, c, W, 57, temp);

#ifdef DOSTORESTATE58
    SHA1_STORE_STATE(58)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(c, d, e, a, b, W, 58, temp);

#ifdef DOSTORESTATE59
    SHA1_STORE_STATE(59)
#endif
    SHA1COMPRESS_FULL_ROUND3_STEP(b, c, d, e, a, W, 59, temp);




#ifdef DOSTORESTATE60
    SHA1_STORE_STATE(60)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(a, b, c, d, e, W, 60, temp);

#ifdef DOSTORESTATE61
    SHA1_STORE_STATE(61)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(e, a, b, c, d, W, 61, temp);

#ifdef DOSTORESTATE62
    SHA1_STORE_STATE(62)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(d, e, a, b, c, W, 62, temp);

#ifdef DOSTORESTATE63
    SHA1_STORE_STATE(63)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(c, d, e, a, b, W, 63, temp);

#ifdef DOSTORESTATE64
    SHA1_STORE_STATE(64)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(b, c, d, e, a, W, 64, temp);

#ifdef DOSTORESTATE65
    SHA1_STORE_STATE(65)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(a, b, c, d, e, W, 65, temp);

#ifdef DOSTORESTATE66
    SHA1_STORE_STATE(66)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(e, a, b, c, d, W, 66, temp);

#ifdef DOSTORESTATE67
    SHA1_STORE_STATE(67)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(d, e, a, b, c, W, 67, temp);

#ifdef DOSTORESTATE68
    SHA1_STORE_STATE(68)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(c, d, e, a, b, W, 68, temp);

#ifdef DOSTORESTATE69
    SHA1_STORE_STATE(69)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(b, c, d, e, a, W, 69, temp);

#ifdef DOSTORESTATE70
    SHA1_STORE_STATE(70)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(a, b, c, d, e, W, 70, temp);

#ifdef DOSTORESTATE71
    SHA1_STORE_STATE(71)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(e, a, b, c, d, W, 71, temp);

#ifdef DOSTORESTATE72
    SHA1_STORE_STATE(72)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(d, e, a, b, c, W, 72, temp);

#ifdef DOSTORESTATE73
    SHA1_STORE_STATE(73)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(c, d, e, a, b, W, 73, temp);

#ifdef DOSTORESTATE74
    SHA1_STORE_STATE(74)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(b, c, d, e, a, W, 74, temp);

#ifdef DOSTORESTATE75
    SHA1_STORE_STATE(75)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(a, b, c, d, e, W, 75, temp);

#ifdef DOSTORESTATE76
    SHA1_STORE_STATE(76)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(e, a, b, c, d, W, 76, temp);

#ifdef DOSTORESTATE77
    SHA1_STORE_STATE(77)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(d, e, a, b, c, W, 77, temp);

#ifdef DOSTORESTATE78
    SHA1_STORE_STATE(78)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(c, d, e, a, b, W, 78, temp);

#ifdef DOSTORESTATE79
    SHA1_STORE_STATE(79)
#endif
    SHA1COMPRESS_FULL_ROUND4_STEP(b, c, d, e, a, W, 79, temp);

    ihv[0] += a;
    ihv[1] += b;
    ihv[2] += c;
    ihv[3] += d;
    ihv[4] += e;
}

#define SHA1_RECOMPRESS(t) \
    static void sha1recompress_fast_ ## t (u32 ihvin[5], u32 ihvout[5], const u32 me2[80], const u32 state[5]) \
    { \
        u32 a = state[0], b = state[1], c = state[2], d = state[3], e = state[4]; \
        if (t > 79) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(b, c, d, e, a, me2, 79); \
        if (t > 78) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(c, d, e, a, b, me2, 78); \
        if (t > 77) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(d, e, a, b, c, me2, 77); \
        if (t > 76) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(e, a, b, c, d, me2, 76); \
        if (t > 75) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(a, b, c, d, e, me2, 75); \
        if (t > 74) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(b, c, d, e, a, me2, 74); \
        if (t > 73) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(c, d, e, a, b, me2, 73); \
        if (t > 72) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(d, e, a, b, c, me2, 72); \
        if (t > 71) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(e, a, b, c, d, me2, 71); \
        if (t > 70) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(a, b, c, d, e, me2, 70); \
        if (t > 69) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(b, c, d, e, a, me2, 69); \
        if (t > 68) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(c, d, e, a, b, me2, 68); \
        if (t > 67) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(d, e, a, b, c, me2, 67); \
        if (t > 66) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(e, a, b, c, d, me2, 66); \
        if (t > 65) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(a, b, c, d, e, me2, 65); \
        if (t > 64) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(b, c, d, e, a, me2, 64); \
        if (t > 63) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(c, d, e, a, b, me2, 63); \
        if (t > 62) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(d, e, a, b, c, me2, 62); \
        if (t > 61) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(e, a, b, c, d, me2, 61); \
        if (t > 60) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(a, b, c, d, e, me2, 60); \
        if (t > 59) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(b, c, d, e, a, me2, 59); \
        if (t > 58) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(c, d, e, a, b, me2, 58); \
        if (t > 57) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(d, e, a, b, c, me2, 57); \
        if (t > 56) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(e, a, b, c, d, me2, 56); \
        if (t > 55) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(a, b, c, d, e, me2, 55); \
        if (t > 54) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(b, c, d, e, a, me2, 54); \
        if (t > 53) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(c, d, e, a, b, me2, 53); \
        if (t > 52) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(d, e, a, b, c, me2, 52); \
        if (t > 51) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(e, a, b, c, d, me2, 51); \
        if (t > 50) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(a, b, c, d, e, me2, 50); \
        if (t > 49) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(b, c, d, e, a, me2, 49); \
        if (t > 48) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(c, d, e, a, b, me2, 48); \
        if (t > 47) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(d, e, a, b, c, me2, 47); \
        if (t > 46) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(e, a, b, c, d, me2, 46); \
        if (t > 45) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(a, b, c, d, e, me2, 45); \
        if (t > 44) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(b, c, d, e, a, me2, 44); \
        if (t > 43) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(c, d, e, a, b, me2, 43); \
        if (t > 42) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(d, e, a, b, c, me2, 42); \
        if (t > 41) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(e, a, b, c, d, me2, 41); \
        if (t > 40) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(a, b, c, d, e, me2, 40); \
        if (t > 39) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(b, c, d, e, a, me2, 39); \
        if (t > 38) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(c, d, e, a, b, me2, 38); \
        if (t > 37) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(d, e, a, b, c, me2, 37); \
        if (t > 36) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(e, a, b, c, d, me2, 36); \
        if (t > 35) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(a, b, c, d, e, me2, 35); \
        if (t > 34) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(b, c, d, e, a, me2, 34); \
        if (t > 33) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(c, d, e, a, b, me2, 33); \
        if (t > 32) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(d, e, a, b, c, me2, 32); \
        if (t > 31) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(e, a, b, c, d, me2, 31); \
        if (t > 30) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(a, b, c, d, e, me2, 30); \
        if (t > 29) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(b, c, d, e, a, me2, 29); \
        if (t > 28) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(c, d, e, a, b, me2, 28); \
        if (t > 27) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(d, e, a, b, c, me2, 27); \
        if (t > 26) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(e, a, b, c, d, me2, 26); \
        if (t > 25) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(a, b, c, d, e, me2, 25); \
        if (t > 24) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(b, c, d, e, a, me2, 24); \
        if (t > 23) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(c, d, e, a, b, me2, 23); \
        if (t > 22) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(d, e, a, b, c, me2, 22); \
        if (t > 21) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(e, a, b, c, d, me2, 21); \
        if (t > 20) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(a, b, c, d, e, me2, 20); \
        if (t > 19) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(b, c, d, e, a, me2, 19); \
        if (t > 18) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(c, d, e, a, b, me2, 18); \
        if (t > 17) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(d, e, a, b, c, me2, 17); \
        if (t > 16) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(e, a, b, c, d, me2, 16); \
        if (t > 15) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(a, b, c, d, e, me2, 15); \
        if (t > 14) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(b, c, d, e, a, me2, 14); \
        if (t > 13) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(c, d, e, a, b, me2, 13); \
        if (t > 12) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(d, e, a, b, c, me2, 12); \
        if (t > 11) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(e, a, b, c, d, me2, 11); \
        if (t > 10) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(a, b, c, d, e, me2, 10); \
        if (t > 9) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(b, c, d, e, a, me2, 9); \
        if (t > 8) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(c, d, e, a, b, me2, 8); \
        if (t > 7) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(d, e, a, b, c, me2, 7); \
        if (t > 6) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(e, a, b, c, d, me2, 6); \
        if (t > 5) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(a, b, c, d, e, me2, 5); \
        if (t > 4) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(b, c, d, e, a, me2, 4); \
        if (t > 3) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(c, d, e, a, b, me2, 3); \
        if (t > 2) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(d, e, a, b, c, me2, 2); \
        if (t > 1) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(e, a, b, c, d, me2, 1); \
        if (t > 0) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(a, b, c, d, e, me2, 0); \
        ihvin[0] = a; ihvin[1] = b; ihvin[2] = c; ihvin[3] = d; ihvin[4] = e; \
        a = state[0]; b = state[1]; c = state[2]; d = state[3]; e = state[4]; \
        if (t <= 0) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, me2, 0); \
        if (t <= 1) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, me2, 1); \
        if (t <= 2) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, me2, 2); \
        if (t <= 3) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, me2, 3); \
        if (t <= 4) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, me2, 4); \
        if (t <= 5) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, me2, 5); \
        if (t <= 6) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, me2, 6); \
        if (t <= 7) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, me2, 7); \
        if (t <= 8) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, me2, 8); \
        if (t <= 9) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, me2, 9); \
        if (t <= 10) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, me2, 10); \
        if (t <= 11) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, me2, 11); \
        if (t <= 12) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, me2, 12); \
        if (t <= 13) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, me2, 13); \
        if (t <= 14) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, me2, 14); \
        if (t <= 15) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, me2, 15); \
        if (t <= 16) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, me2, 16); \
        if (t <= 17) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, me2, 17); \
        if (t <= 18) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, me2, 18); \
        if (t <= 19) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, me2, 19); \
        if (t <= 20) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, me2, 20); \
        if (t <= 21) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, me2, 21); \
        if (t <= 22) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, me2, 22); \
        if (t <= 23) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, me2, 23); \
        if (t <= 24) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, me2, 24); \
        if (t <= 25) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, me2, 25); \
        if (t <= 26) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, me2, 26); \
        if (t <= 27) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, me2, 27); \
        if (t <= 28) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, me2, 28); \
        if (t <= 29) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, me2, 29); \
        if (t <= 30) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, me2, 30); \
        if (t <= 31) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, me2, 31); \
        if (t <= 32) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, me2, 32); \
        if (t <= 33) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, me2, 33); \
        if (t <= 34) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, me2, 34); \
        if (t <= 35) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, me2, 35); \
        if (t <= 36) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, me2, 36); \
        if (t <= 37) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, me2, 37); \
        if (t <= 38) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, me2, 38); \
        if (t <= 39) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, me2, 39); \
        if (t <= 40) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, me2, 40); \
        if (t <= 41) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, me2, 41); \
        if (t <= 42) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, me2, 42); \
        if (t <= 43) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, me2, 43); \
        if (t <= 44) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, me2, 44); \
        if (t <= 45) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, me2, 45); \
        if (t <= 46) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, me2, 46); \
        if (t <= 47) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, me2, 47); \
        if (t <= 48) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, me2, 48); \
        if (t <= 49) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, me2, 49); \
        if (t <= 50) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, me2, 50); \
        if (t <= 51) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, me2, 51); \
        if (t <= 52) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, me2, 52); \
        if (t <= 53) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, me2, 53); \
        if (t <= 54) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, me2, 54); \
        if (t <= 55) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, me2, 55); \
        if (t <= 56) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, me2, 56); \
        if (t <= 57) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, me2, 57); \
        if (t <= 58) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, me2, 58); \
        if (t <= 59) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, me2, 59); \
        if (t <= 60) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, me2, 60); \
        if (t <= 61) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, me2, 61); \
        if (t <= 62) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, me2, 62); \
        if (t <= 63) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, me2, 63); \
        if (t <= 64) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, me2, 64); \
        if (t <= 65) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, me2, 65); \
        if (t <= 66) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, me2, 66); \
        if (t <= 67) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, me2, 67); \
        if (t <= 68) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, me2, 68); \
        if (t <= 69) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, me2, 69); \
        if (t <= 70) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, me2, 70); \
        if (t <= 71) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, me2, 71); \
        if (t <= 72) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, me2, 72); \
        if (t <= 73) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, me2, 73); \
        if (t <= 74) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, me2, 74); \
        if (t <= 75) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, me2, 75); \
        if (t <= 76) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, me2, 76); \
        if (t <= 77) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, me2, 77); \
        if (t <= 78) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, me2, 78); \
        if (t <= 79) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, me2, 79); \
        ihvout[0] = ihvin[0] + a; ihvout[1] = ihvin[1] + b; ihvout[2] = ihvin[2] + c; ihvout[3] = ihvin[3] + d; ihvout[4] = ihvin[4] + e; \
    }

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable: 4127)  /* Compiler complains about the checks in the above macro being constant. */
#endif

#ifdef DOSTORESTATE0
    SHA1_RECOMPRESS(0)
#endif

#ifdef DOSTORESTATE1
    SHA1_RECOMPRESS(1)
#endif

#ifdef DOSTORESTATE2
    SHA1_RECOMPRESS(2)
#endif

#ifdef DOSTORESTATE3
    SHA1_RECOMPRESS(3)
#endif

#ifdef DOSTORESTATE4
    SHA1_RECOMPRESS(4)
#endif

#ifdef DOSTORESTATE5
    SHA1_RECOMPRESS(5)
#endif

#ifdef DOSTORESTATE6
    SHA1_RECOMPRESS(6)
#endif

#ifdef DOSTORESTATE7
    SHA1_RECOMPRESS(7)
#endif

#ifdef DOSTORESTATE8
    SHA1_RECOMPRESS(8)
#endif

#ifdef DOSTORESTATE9
    SHA1_RECOMPRESS(9)
#endif

#ifdef DOSTORESTATE10
    SHA1_RECOMPRESS(10)
#endif

#ifdef DOSTORESTATE11
    SHA1_RECOMPRESS(11)
#endif

#ifdef DOSTORESTATE12
    SHA1_RECOMPRESS(12)
#endif

#ifdef DOSTORESTATE13
    SHA1_RECOMPRESS(13)
#endif

#ifdef DOSTORESTATE14
    SHA1_RECOMPRESS(14)
#endif

#ifdef DOSTORESTATE15
    SHA1_RECOMPRESS(15)
#endif

#ifdef DOSTORESTATE16
    SHA1_RECOMPRESS(16)
#endif

#ifdef DOSTORESTATE17
    SHA1_RECOMPRESS(17)
#endif

#ifdef DOSTORESTATE18
    SHA1_RECOMPRESS(18)
#endif

#ifdef DOSTORESTATE19
    SHA1_RECOMPRESS(19)
#endif

#ifdef DOSTORESTATE20
    SHA1_RECOMPRESS(20)
#endif

#ifdef DOSTORESTATE21
    SHA1_RECOMPRESS(21)
#endif

#ifdef DOSTORESTATE22
    SHA1_RECOMPRESS(22)
#endif

#ifdef DOSTORESTATE23
    SHA1_RECOMPRESS(23)
#endif

#ifdef DOSTORESTATE24
    SHA1_RECOMPRESS(24)
#endif

#ifdef DOSTORESTATE25
    SHA1_RECOMPRESS(25)
#endif

#ifdef DOSTORESTATE26
    SHA1_RECOMPRESS(26)
#endif

#ifdef DOSTORESTATE27
    SHA1_RECOMPRESS(27)
#endif

#ifdef DOSTORESTATE28
    SHA1_RECOMPRESS(28)
#endif

#ifdef DOSTORESTATE29
    SHA1_RECOMPRESS(29)
#endif

#ifdef DOSTORESTATE30
    SHA1_RECOMPRESS(30)
#endif

#ifdef DOSTORESTATE31
    SHA1_RECOMPRESS(31)
#endif

#ifdef DOSTORESTATE32
    SHA1_RECOMPRESS(32)
#endif

#ifdef DOSTORESTATE33
    SHA1_RECOMPRESS(33)
#endif

#ifdef DOSTORESTATE34
    SHA1_RECOMPRESS(34)
#endif

#ifdef DOSTORESTATE35
    SHA1_RECOMPRESS(35)
#endif

#ifdef DOSTORESTATE36
    SHA1_RECOMPRESS(36)
#endif

#ifdef DOSTORESTATE37
    SHA1_RECOMPRESS(37)
#endif

#ifdef DOSTORESTATE38
    SHA1_RECOMPRESS(38)
#endif

#ifdef DOSTORESTATE39
    SHA1_RECOMPRESS(39)
#endif

#ifdef DOSTORESTATE40
    SHA1_RECOMPRESS(40)
#endif

#ifdef DOSTORESTATE41
    SHA1_RECOMPRESS(41)
#endif

#ifdef DOSTORESTATE42
    SHA1_RECOMPRESS(42)
#endif

#ifdef DOSTORESTATE43
    SHA1_RECOMPRESS(43)
#endif

#ifdef DOSTORESTATE44
    SHA1_RECOMPRESS(44)
#endif

#ifdef DOSTORESTATE45
    SHA1_RECOMPRESS(45)
#endif

#ifdef DOSTORESTATE46
    SHA1_RECOMPRESS(46)
#endif

#ifdef DOSTORESTATE47
    SHA1_RECOMPRESS(47)
#endif

#ifdef DOSTORESTATE48
    SHA1_RECOMPRESS(48)
#endif

#ifdef DOSTORESTATE49
    SHA1_RECOMPRESS(49)
#endif

#ifdef DOSTORESTATE50
    SHA1_RECOMPRESS(50)
#endif

#ifdef DOSTORESTATE51
    SHA1_RECOMPRESS(51)
#endif

#ifdef DOSTORESTATE52
    SHA1_RECOMPRESS(52)
#endif

#ifdef DOSTORESTATE53
    SHA1_RECOMPRESS(53)
#endif

#ifdef DOSTORESTATE54
    SHA1_RECOMPRESS(54)
#endif

#ifdef DOSTORESTATE55
    SHA1_RECOMPRESS(55)
#endif

#ifdef DOSTORESTATE56
    SHA1_RECOMPRESS(56)
#endif

#ifdef DOSTORESTATE57
    SHA1_RECOMPRESS(57)
#endif

#ifdef DOSTORESTATE58
    SHA1_RECOMPRESS(58)
#endif

#ifdef DOSTORESTATE59
    SHA1_RECOMPRESS(59)
#endif

#ifdef DOSTORESTATE60
    SHA1_RECOMPRESS(60)
#endif

#ifdef DOSTORESTATE61
    SHA1_RECOMPRESS(61)
#endif

#ifdef DOSTORESTATE62
    SHA1_RECOMPRESS(62)
#endif

#ifdef DOSTORESTATE63
    SHA1_RECOMPRESS(63)
#endif

#ifdef DOSTORESTATE64
    SHA1_RECOMPRESS(64)
#endif

#ifdef DOSTORESTATE65
    SHA1_RECOMPRESS(65)
#endif

#ifdef DOSTORESTATE66
    SHA1_RECOMPRESS(66)
#endif

#ifdef DOSTORESTATE67
    SHA1_RECOMPRESS(67)
#endif

#ifdef DOSTORESTATE68
    SHA1_RECOMPRESS(68)
#endif

#ifdef DOSTORESTATE69
    SHA1_RECOMPRESS(69)
#endif

#ifdef DOSTORESTATE70
    SHA1_RECOMPRESS(70)
#endif

#ifdef DOSTORESTATE71
    SHA1_RECOMPRESS(71)
#endif

#ifdef DOSTORESTATE72
    SHA1_RECOMPRESS(72)
#endif

#ifdef DOSTORESTATE73
    SHA1_RECOMPRESS(73)
#endif

#ifdef DOSTORESTATE74
    SHA1_RECOMPRESS(74)
#endif

#ifdef DOSTORESTATE75
    SHA1_RECOMPRESS(75)
#endif

#ifdef DOSTORESTATE76
    SHA1_RECOMPRESS(76)
#endif

#ifdef DOSTORESTATE77
    SHA1_RECOMPRESS(77)
#endif

#ifdef DOSTORESTATE78
    SHA1_RECOMPRESS(78)
#endif

#ifdef DOSTORESTATE79
    SHA1_RECOMPRESS(79)
#endif

#ifdef _MSC_VER
    #pragma warning(pop)
#endif

static void sha1_recompression_step(u32 step, u32 ihvin[5], u32 ihvout[5],
                                    const u32 me2[80], const u32 state[5]) {
    switch (step) {
#ifdef DOSTORESTATE0
        case 0:
            sha1recompress_fast_0(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE1
        case 1:
            sha1recompress_fast_1(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE2
        case 2:
            sha1recompress_fast_2(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE3
        case 3:
            sha1recompress_fast_3(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE4
        case 4:
            sha1recompress_fast_4(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE5
        case 5:
            sha1recompress_fast_5(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE6
        case 6:
            sha1recompress_fast_6(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE7
        case 7:
            sha1recompress_fast_7(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE8
        case 8:
            sha1recompress_fast_8(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE9
        case 9:
            sha1recompress_fast_9(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE10
        case 10:
            sha1recompress_fast_10(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE11
        case 11:
            sha1recompress_fast_11(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE12
        case 12:
            sha1recompress_fast_12(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE13
        case 13:
            sha1recompress_fast_13(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE14
        case 14:
            sha1recompress_fast_14(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE15
        case 15:
            sha1recompress_fast_15(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE16
        case 16:
            sha1recompress_fast_16(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE17
        case 17:
            sha1recompress_fast_17(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE18
        case 18:
            sha1recompress_fast_18(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE19
        case 19:
            sha1recompress_fast_19(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE20
        case 20:
            sha1recompress_fast_20(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE21
        case 21:
            sha1recompress_fast_21(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE22
        case 22:
            sha1recompress_fast_22(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE23
        case 23:
            sha1recompress_fast_23(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE24
        case 24:
            sha1recompress_fast_24(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE25
        case 25:
            sha1recompress_fast_25(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE26
        case 26:
            sha1recompress_fast_26(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE27
        case 27:
            sha1recompress_fast_27(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE28
        case 28:
            sha1recompress_fast_28(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE29
        case 29:
            sha1recompress_fast_29(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE30
        case 30:
            sha1recompress_fast_30(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE31
        case 31:
            sha1recompress_fast_31(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE32
        case 32:
            sha1recompress_fast_32(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE33
        case 33:
            sha1recompress_fast_33(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE34
        case 34:
            sha1recompress_fast_34(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE35
        case 35:
            sha1recompress_fast_35(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE36
        case 36:
            sha1recompress_fast_36(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE37
        case 37:
            sha1recompress_fast_37(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE38
        case 38:
            sha1recompress_fast_38(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE39
        case 39:
            sha1recompress_fast_39(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE40
        case 40:
            sha1recompress_fast_40(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE41
        case 41:
            sha1recompress_fast_41(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE42
        case 42:
            sha1recompress_fast_42(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE43
        case 43:
            sha1recompress_fast_43(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE44
        case 44:
            sha1recompress_fast_44(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE45
        case 45:
            sha1recompress_fast_45(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE46
        case 46:
            sha1recompress_fast_46(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE47
        case 47:
            sha1recompress_fast_47(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE48
        case 48:
            sha1recompress_fast_48(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE49
        case 49:
            sha1recompress_fast_49(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE50
        case 50:
            sha1recompress_fast_50(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE51
        case 51:
            sha1recompress_fast_51(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE52
        case 52:
            sha1recompress_fast_52(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE53
        case 53:
            sha1recompress_fast_53(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE54
        case 54:
            sha1recompress_fast_54(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE55
        case 55:
            sha1recompress_fast_55(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE56
        case 56:
            sha1recompress_fast_56(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE57
        case 57:
            sha1recompress_fast_57(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE58
        case 58:
            sha1recompress_fast_58(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE59
        case 59:
            sha1recompress_fast_59(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE60
        case 60:
            sha1recompress_fast_60(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE61
        case 61:
            sha1recompress_fast_61(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE62
        case 62:
            sha1recompress_fast_62(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE63
        case 63:
            sha1recompress_fast_63(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE64
        case 64:
            sha1recompress_fast_64(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE65
        case 65:
            sha1recompress_fast_65(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE66
        case 66:
            sha1recompress_fast_66(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE67
        case 67:
            sha1recompress_fast_67(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE68
        case 68:
            sha1recompress_fast_68(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE69
        case 69:
            sha1recompress_fast_69(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE70
        case 70:
            sha1recompress_fast_70(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE71
        case 71:
            sha1recompress_fast_71(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE72
        case 72:
            sha1recompress_fast_72(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE73
        case 73:
            sha1recompress_fast_73(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE74
        case 74:
            sha1recompress_fast_74(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE75
        case 75:
            sha1recompress_fast_75(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE76
        case 76:
            sha1recompress_fast_76(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE77
        case 77:
            sha1recompress_fast_77(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE78
        case 78:
            sha1recompress_fast_78(ihvin, ihvout, me2, state);
            break;
#endif
#ifdef DOSTORESTATE79
        case 79:
            sha1recompress_fast_79(ihvin, ihvout, me2, state);
            break;
#endif
        default:
            abort();
    }

}

static void sha1_process(SHA1_CTX *ctx, const u32 block[16]) {
    unsigned i, j;
    u32 ubc_dv_mask[DVMASKSIZE] = { 0xFFFFFFFF };
    u32 ihvtmp[5];

    ctx->ihv1[0] = ctx->ihv[0];
    ctx->ihv1[1] = ctx->ihv[1];
    ctx->ihv1[2] = ctx->ihv[2];
    ctx->ihv1[3] = ctx->ihv[3];
    ctx->ihv1[4] = ctx->ihv[4];

    sha1_compression_states(ctx->ihv, block, ctx->m1, ctx->states);

    if (ctx->detect_coll) {
        if (ctx->ubc_check) {
            ubc_check(ctx->m1, ubc_dv_mask);
        }

        if (ubc_dv_mask[0] != 0) {
            for (i = 0; sha1_dvs[i].dvType != 0; ++i) {
                if (ubc_dv_mask[0] & ((u32)(1) << sha1_dvs[i].maskb)) {
                    for (j = 0; j < 80; ++j)
                        ctx->m2[j] = ctx->m1[j] ^ sha1_dvs[i].dm[j];

                    sha1_recompression_step(sha1_dvs[i].testt, ctx->ihv2, ihvtmp, ctx->m2,
                                            ctx->states[sha1_dvs[i].testt]);

                    /* to verify SHA-1 collision detection code with collisions for reduced-step SHA-1 */
                    if ((0 == ((ihvtmp[0] ^ ctx->ihv[0]) | (ihvtmp[1] ^ ctx->ihv[1]) | (ihvtmp[2] ^ ctx->ihv[2]) |
                               (ihvtmp[3] ^ ctx->ihv[3]) | (ihvtmp[4] ^ ctx->ihv[4])))
                        || (ctx->reduced_round_coll
                            && 0 == ((ctx->ihv1[0] ^ ctx->ihv2[0]) | (ctx->ihv1[1] ^ ctx->ihv2[1]) |
                                     (ctx->ihv1[2] ^ ctx->ihv2[2]) | (ctx->ihv1[3] ^ ctx->ihv2[3]) | (ctx->ihv1[4] ^ ctx->ihv2[4])))) {
                        ctx->found_collision = 1;

                        if (ctx->safe_hash) {
                            sha1_compression_W(ctx->ihv, ctx->m1);
                            sha1_compression_W(ctx->ihv, ctx->m1);
                        }

                        break;
                    }
                }
            }
        }
    }
}

void SHA1DCInit(SHA1_CTX *ctx) {
    ctx->total = 0;
    ctx->ihv[0] = 0x67452301;
    ctx->ihv[1] = 0xEFCDAB89;
    ctx->ihv[2] = 0x98BADCFE;
    ctx->ihv[3] = 0x10325476;
    ctx->ihv[4] = 0xC3D2E1F0;
    ctx->found_collision = 0;
    ctx->safe_hash = SHA1DC_INIT_SAFE_HASH_DEFAULT;
    ctx->ubc_check = 1;
    ctx->detect_coll = 1;
    ctx->reduced_round_coll = 0;
    ctx->callback = NULL;
}

void SHA1DCUpdate(SHA1_CTX *ctx, const char *buf, size_t len) {
    unsigned left, fill;

    if (len == 0)
        return;

    left = ctx->total & 63;
    fill = 64 - left;

    if (left && len >= fill) {
        ctx->total += fill;
        memcpy(ctx->buffer + left, buf, fill);
        sha1_process(ctx, (u32 *)(ctx->buffer));
        buf += fill;
        len -= fill;
        left = 0;
    }
    while (len >= 64) {
        ctx->total += 64;

#if defined(SHA1DC_ALLOW_UNALIGNED_ACCESS)
        sha1_process(ctx, (u32 *)(buf));
#else
        memcpy(ctx->buffer, buf, 64);
        sha1_process(ctx, (u32 *)(ctx->buffer));
#endif /* defined(SHA1DC_ALLOW_UNALIGNED_ACCESS) */
        buf += 64;
        len -= 64;
    }
    if (len > 0) {
        ctx->total += len;
        memcpy(ctx->buffer + left, buf, len);
    }
}

static const unsigned char sha1_padding[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int SHA1DCFinal(unsigned char output[SHA1DC_LEN], SHA1_CTX *ctx) {
    u32 last = ctx->total & 63;
    u32 padn = (last < 56) ? (56 - last) : (120 - last);
    u64 total;
    SHA1DCUpdate(ctx, (const char *)(sha1_padding), padn);

    total = ctx->total - padn;
    total <<= 3;
    ctx->buffer[56] = (unsigned char)(total >> 56);
    ctx->buffer[57] = (unsigned char)(total >> 48);
    ctx->buffer[58] = (unsigned char)(total >> 40);
    ctx->buffer[59] = (unsigned char)(total >> 32);
    ctx->buffer[60] = (unsigned char)(total >> 24);
    ctx->buffer[61] = (unsigned char)(total >> 16);
    ctx->buffer[62] = (unsigned char)(total >> 8);
    ctx->buffer[63] = (unsigned char)(total);
    sha1_process(ctx, (u32 *)(ctx->buffer));
    output[0] = (unsigned char)(ctx->ihv[0] >> 24);
    output[1] = (unsigned char)(ctx->ihv[0] >> 16);
    output[2] = (unsigned char)(ctx->ihv[0] >> 8);
    output[3] = (unsigned char)(ctx->ihv[0]);
    output[4] = (unsigned char)(ctx->ihv[1] >> 24);
    output[5] = (unsigned char)(ctx->ihv[1] >> 16);
    output[6] = (unsigned char)(ctx->ihv[1] >> 8);
    output[7] = (unsigned char)(ctx->ihv[1]);
    output[8] = (unsigned char)(ctx->ihv[2] >> 24);
    output[9] = (unsigned char)(ctx->ihv[2] >> 16);
    output[10] = (unsigned char)(ctx->ihv[2] >> 8);
    output[11] = (unsigned char)(ctx->ihv[2]);
    output[12] = (unsigned char)(ctx->ihv[3] >> 24);
    output[13] = (unsigned char)(ctx->ihv[3] >> 16);
    output[14] = (unsigned char)(ctx->ihv[3] >> 8);
    output[15] = (unsigned char)(ctx->ihv[3]);
    output[16] = (unsigned char)(ctx->ihv[4] >> 24);
    output[17] = (unsigned char)(ctx->ihv[4] >> 16);
    output[18] = (unsigned char)(ctx->ihv[4] >> 8);
    output[19] = (unsigned char)(ctx->ihv[4]);
    return ctx->found_collision;
}

/***
* Copyright 2017 Marc Stevens <marc@marc-stevens.nl>, Dan Shumow <danshu@microsoft.com>
* Distributed under the MIT Software License.
* See accompanying file LICENSE.txt or copy at
* https://opensource.org/licenses/MIT
***/

/*
// this file was generated by the 'parse_bitrel' program in the tools section
// using the data files from directory 'tools/data/3565'
//
// sha1_dvs contains a list of SHA-1 Disturbance Vectors (DV) to check
// dvType, dvK and dvB define the DV: I(K,B) or II(K,B) (see the paper)
// dm[80] is the expanded message block XOR-difference defined by the DV
// testt is the step to do the recompression from for collision detection
// maski and maskb define the bit to check for each DV in the dvmask returned by ubc_check
//
// ubc_check takes as input an expanded message block and verifies the unavoidable bitconditions for all listed DVs
// it returns a dvmask where each bit belonging to a DV is set if all unavoidable bitconditions for that DV have been met
// thus one needs to do the recompression check for each DV that has its bit set
//
// ubc_check is programmatically generated and the unavoidable bitconditions have been hardcoded
// a directly verifiable version named ubc_check_verify can be found in ubc_check_verify.c
// ubc_check has been verified against ubc_check_verify using the 'ubc_check_test' program in the tools section
*/

static const u32 DV_I_43_0_bit     = (u32)(1) << 0;
static const u32 DV_I_44_0_bit     = (u32)(1) << 1;
static const u32 DV_I_45_0_bit     = (u32)(1) << 2;
static const u32 DV_I_46_0_bit     = (u32)(1) << 3;
static const u32 DV_I_46_2_bit     = (u32)(1) << 4;
static const u32 DV_I_47_0_bit     = (u32)(1) << 5;
static const u32 DV_I_47_2_bit     = (u32)(1) << 6;
static const u32 DV_I_48_0_bit     = (u32)(1) << 7;
static const u32 DV_I_48_2_bit     = (u32)(1) << 8;
static const u32 DV_I_49_0_bit     = (u32)(1) << 9;
static const u32 DV_I_49_2_bit     = (u32)(1) << 10;
static const u32 DV_I_50_0_bit     = (u32)(1) << 11;
static const u32 DV_I_50_2_bit     = (u32)(1) << 12;
static const u32 DV_I_51_0_bit     = (u32)(1) << 13;
static const u32 DV_I_51_2_bit     = (u32)(1) << 14;
static const u32 DV_I_52_0_bit     = (u32)(1) << 15;
static const u32 DV_II_45_0_bit    = (u32)(1) << 16;
static const u32 DV_II_46_0_bit    = (u32)(1) << 17;
static const u32 DV_II_46_2_bit    = (u32)(1) << 18;
static const u32 DV_II_47_0_bit    = (u32)(1) << 19;
static const u32 DV_II_48_0_bit    = (u32)(1) << 20;
static const u32 DV_II_49_0_bit    = (u32)(1) << 21;
static const u32 DV_II_49_2_bit    = (u32)(1) << 22;
static const u32 DV_II_50_0_bit    = (u32)(1) << 23;
static const u32 DV_II_50_2_bit    = (u32)(1) << 24;
static const u32 DV_II_51_0_bit    = (u32)(1) << 25;
static const u32 DV_II_51_2_bit    = (u32)(1) << 26;
static const u32 DV_II_52_0_bit    = (u32)(1) << 27;
static const u32 DV_II_53_0_bit    = (u32)(1) << 28;
static const u32 DV_II_54_0_bit    = (u32)(1) << 29;
static const u32 DV_II_55_0_bit    = (u32)(1) << 30;
static const u32 DV_II_56_0_bit    = (u32)(1) << 31;

dv_info_t sha1_dvs[] = {
    {1, 43, 0, 58, 0, 0, { 0x08000000, 0x9800000c, 0xd8000010, 0x08000010, 0xb8000010, 0x98000000, 0x60000000, 0x00000008, 0xc0000000, 0x90000014, 0x10000010, 0xb8000014, 0x28000000, 0x20000010, 0x48000000, 0x08000018, 0x60000000, 0x90000010, 0xf0000010, 0x90000008, 0xc0000000, 0x90000010, 0xf0000010, 0xb0000008, 0x40000000, 0x90000000, 0xf0000010, 0x90000018, 0x60000000, 0x90000010, 0x90000010, 0x90000000, 0x80000000, 0x00000010, 0xa0000000, 0x20000000, 0xa0000000, 0x20000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x20000000, 0x00000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000040, 0x40000002, 0x80000004, 0x80000080, 0x80000006, 0x00000049, 0x00000103, 0x80000009, 0x80000012, 0x80000202, 0x00000018, 0x00000164, 0x00000408, 0x800000e6, 0x8000004c, 0x00000803, 0x80000161, 0x80000599 } }
    , {1, 44, 0, 58, 0, 1, { 0xb4000008, 0x08000000, 0x9800000c, 0xd8000010, 0x08000010, 0xb8000010, 0x98000000, 0x60000000, 0x00000008, 0xc0000000, 0x90000014, 0x10000010, 0xb8000014, 0x28000000, 0x20000010, 0x48000000, 0x08000018, 0x60000000, 0x90000010, 0xf0000010, 0x90000008, 0xc0000000, 0x90000010, 0xf0000010, 0xb0000008, 0x40000000, 0x90000000, 0xf0000010, 0x90000018, 0x60000000, 0x90000010, 0x90000010, 0x90000000, 0x80000000, 0x00000010, 0xa0000000, 0x20000000, 0xa0000000, 0x20000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x20000000, 0x00000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000040, 0x40000002, 0x80000004, 0x80000080, 0x80000006, 0x00000049, 0x00000103, 0x80000009, 0x80000012, 0x80000202, 0x00000018, 0x00000164, 0x00000408, 0x800000e6, 0x8000004c, 0x00000803, 0x80000161 } }
    , {1, 45, 0, 58, 0, 2, { 0xf4000014, 0xb4000008, 0x08000000, 0x9800000c, 0xd8000010, 0x08000010, 0xb8000010, 0x98000000, 0x60000000, 0x00000008, 0xc0000000, 0x90000014, 0x10000010, 0xb8000014, 0x28000000, 0x20000010, 0x48000000, 0x08000018, 0x60000000, 0x90000010, 0xf0000010, 0x90000008, 0xc0000000, 0x90000010, 0xf0000010, 0xb0000008, 0x40000000, 0x90000000, 0xf0000010, 0x90000018, 0x60000000, 0x90000010, 0x90000010, 0x90000000, 0x80000000, 0x00000010, 0xa0000000, 0x20000000, 0xa0000000, 0x20000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x20000000, 0x00000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000040, 0x40000002, 0x80000004, 0x80000080, 0x80000006, 0x00000049, 0x00000103, 0x80000009, 0x80000012, 0x80000202, 0x00000018, 0x00000164, 0x00000408, 0x800000e6, 0x8000004c, 0x00000803 } }
    , {1, 46, 0, 58, 0, 3, { 0x2c000010, 0xf4000014, 0xb4000008, 0x08000000, 0x9800000c, 0xd8000010, 0x08000010, 0xb8000010, 0x98000000, 0x60000000, 0x00000008, 0xc0000000, 0x90000014, 0x10000010, 0xb8000014, 0x28000000, 0x20000010, 0x48000000, 0x08000018, 0x60000000, 0x90000010, 0xf0000010, 0x90000008, 0xc0000000, 0x90000010, 0xf0000010, 0xb0000008, 0x40000000, 0x90000000, 0xf0000010, 0x90000018, 0x60000000, 0x90000010, 0x90000010, 0x90000000, 0x80000000, 0x00000010, 0xa0000000, 0x20000000, 0xa0000000, 0x20000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x20000000, 0x00000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000040, 0x40000002, 0x80000004, 0x80000080, 0x80000006, 0x00000049, 0x00000103, 0x80000009, 0x80000012, 0x80000202, 0x00000018, 0x00000164, 0x00000408, 0x800000e6, 0x8000004c } }
    , {1, 46, 2, 58, 0, 4, { 0xb0000040, 0xd0000053, 0xd0000022, 0x20000000, 0x60000032, 0x60000043, 0x20000040, 0xe0000042, 0x60000002, 0x80000001, 0x00000020, 0x00000003, 0x40000052, 0x40000040, 0xe0000052, 0xa0000000, 0x80000040, 0x20000001, 0x20000060, 0x80000001, 0x40000042, 0xc0000043, 0x40000022, 0x00000003, 0x40000042, 0xc0000043, 0xc0000022, 0x00000001, 0x40000002, 0xc0000043, 0x40000062, 0x80000001, 0x40000042, 0x40000042, 0x40000002, 0x00000002, 0x00000040, 0x80000002, 0x80000000, 0x80000002, 0x80000040, 0x00000000, 0x80000040, 0x80000000, 0x00000040, 0x80000000, 0x00000040, 0x80000002, 0x00000000, 0x80000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000080, 0x00000004, 0x00000009, 0x00000101, 0x00000009, 0x00000012, 0x00000202, 0x0000001a, 0x00000124, 0x0000040c, 0x00000026, 0x0000004a, 0x0000080a, 0x00000060, 0x00000590, 0x00001020, 0x0000039a, 0x00000132 } }
    , {1, 47, 0, 58, 0, 5, { 0xc8000010, 0x2c000010, 0xf4000014, 0xb4000008, 0x08000000, 0x9800000c, 0xd8000010, 0x08000010, 0xb8000010, 0x98000000, 0x60000000, 0x00000008, 0xc0000000, 0x90000014, 0x10000010, 0xb8000014, 0x28000000, 0x20000010, 0x48000000, 0x08000018, 0x60000000, 0x90000010, 0xf0000010, 0x90000008, 0xc0000000, 0x90000010, 0xf0000010, 0xb0000008, 0x40000000, 0x90000000, 0xf0000010, 0x90000018, 0x60000000, 0x90000010, 0x90000010, 0x90000000, 0x80000000, 0x00000010, 0xa0000000, 0x20000000, 0xa0000000, 0x20000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x20000000, 0x00000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000040, 0x40000002, 0x80000004, 0x80000080, 0x80000006, 0x00000049, 0x00000103, 0x80000009, 0x80000012, 0x80000202, 0x00000018, 0x00000164, 0x00000408, 0x800000e6 } }
    , {1, 47, 2, 58, 0, 6, { 0x20000043, 0xb0000040, 0xd0000053, 0xd0000022, 0x20000000, 0x60000032, 0x60000043, 0x20000040, 0xe0000042, 0x60000002, 0x80000001, 0x00000020, 0x00000003, 0x40000052, 0x40000040, 0xe0000052, 0xa0000000, 0x80000040, 0x20000001, 0x20000060, 0x80000001, 0x40000042, 0xc0000043, 0x40000022, 0x00000003, 0x40000042, 0xc0000043, 0xc0000022, 0x00000001, 0x40000002, 0xc0000043, 0x40000062, 0x80000001, 0x40000042, 0x40000042, 0x40000002, 0x00000002, 0x00000040, 0x80000002, 0x80000000, 0x80000002, 0x80000040, 0x00000000, 0x80000040, 0x80000000, 0x00000040, 0x80000000, 0x00000040, 0x80000002, 0x00000000, 0x80000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000080, 0x00000004, 0x00000009, 0x00000101, 0x00000009, 0x00000012, 0x00000202, 0x0000001a, 0x00000124, 0x0000040c, 0x00000026, 0x0000004a, 0x0000080a, 0x00000060, 0x00000590, 0x00001020, 0x0000039a } }
    , {1, 48, 0, 58, 0, 7, { 0xb800000a, 0xc8000010, 0x2c000010, 0xf4000014, 0xb4000008, 0x08000000, 0x9800000c, 0xd8000010, 0x08000010, 0xb8000010, 0x98000000, 0x60000000, 0x00000008, 0xc0000000, 0x90000014, 0x10000010, 0xb8000014, 0x28000000, 0x20000010, 0x48000000, 0x08000018, 0x60000000, 0x90000010, 0xf0000010, 0x90000008, 0xc0000000, 0x90000010, 0xf0000010, 0xb0000008, 0x40000000, 0x90000000, 0xf0000010, 0x90000018, 0x60000000, 0x90000010, 0x90000010, 0x90000000, 0x80000000, 0x00000010, 0xa0000000, 0x20000000, 0xa0000000, 0x20000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x20000000, 0x00000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000040, 0x40000002, 0x80000004, 0x80000080, 0x80000006, 0x00000049, 0x00000103, 0x80000009, 0x80000012, 0x80000202, 0x00000018, 0x00000164, 0x00000408 } }
    , {1, 48, 2, 58, 0, 8, { 0xe000002a, 0x20000043, 0xb0000040, 0xd0000053, 0xd0000022, 0x20000000, 0x60000032, 0x60000043, 0x20000040, 0xe0000042, 0x60000002, 0x80000001, 0x00000020, 0x00000003, 0x40000052, 0x40000040, 0xe0000052, 0xa0000000, 0x80000040, 0x20000001, 0x20000060, 0x80000001, 0x40000042, 0xc0000043, 0x40000022, 0x00000003, 0x40000042, 0xc0000043, 0xc0000022, 0x00000001, 0x40000002, 0xc0000043, 0x40000062, 0x80000001, 0x40000042, 0x40000042, 0x40000002, 0x00000002, 0x00000040, 0x80000002, 0x80000000, 0x80000002, 0x80000040, 0x00000000, 0x80000040, 0x80000000, 0x00000040, 0x80000000, 0x00000040, 0x80000002, 0x00000000, 0x80000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000080, 0x00000004, 0x00000009, 0x00000101, 0x00000009, 0x00000012, 0x00000202, 0x0000001a, 0x00000124, 0x0000040c, 0x00000026, 0x0000004a, 0x0000080a, 0x00000060, 0x00000590, 0x00001020 } }
    , {1, 49, 0, 58, 0, 9, { 0x18000000, 0xb800000a, 0xc8000010, 0x2c000010, 0xf4000014, 0xb4000008, 0x08000000, 0x9800000c, 0xd8000010, 0x08000010, 0xb8000010, 0x98000000, 0x60000000, 0x00000008, 0xc0000000, 0x90000014, 0x10000010, 0xb8000014, 0x28000000, 0x20000010, 0x48000000, 0x08000018, 0x60000000, 0x90000010, 0xf0000010, 0x90000008, 0xc0000000, 0x90000010, 0xf0000010, 0xb0000008, 0x40000000, 0x90000000, 0xf0000010, 0x90000018, 0x60000000, 0x90000010, 0x90000010, 0x90000000, 0x80000000, 0x00000010, 0xa0000000, 0x20000000, 0xa0000000, 0x20000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x20000000, 0x00000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000040, 0x40000002, 0x80000004, 0x80000080, 0x80000006, 0x00000049, 0x00000103, 0x80000009, 0x80000012, 0x80000202, 0x00000018, 0x00000164 } }
    , {1, 49, 2, 58, 0, 10, { 0x60000000, 0xe000002a, 0x20000043, 0xb0000040, 0xd0000053, 0xd0000022, 0x20000000, 0x60000032, 0x60000043, 0x20000040, 0xe0000042, 0x60000002, 0x80000001, 0x00000020, 0x00000003, 0x40000052, 0x40000040, 0xe0000052, 0xa0000000, 0x80000040, 0x20000001, 0x20000060, 0x80000001, 0x40000042, 0xc0000043, 0x40000022, 0x00000003, 0x40000042, 0xc0000043, 0xc0000022, 0x00000001, 0x40000002, 0xc0000043, 0x40000062, 0x80000001, 0x40000042, 0x40000042, 0x40000002, 0x00000002, 0x00000040, 0x80000002, 0x80000000, 0x80000002, 0x80000040, 0x00000000, 0x80000040, 0x80000000, 0x00000040, 0x80000000, 0x00000040, 0x80000002, 0x00000000, 0x80000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000080, 0x00000004, 0x00000009, 0x00000101, 0x00000009, 0x00000012, 0x00000202, 0x0000001a, 0x00000124, 0x0000040c, 0x00000026, 0x0000004a, 0x0000080a, 0x00000060, 0x00000590 } }
    , {1, 50, 0, 65, 0, 11, { 0x0800000c, 0x18000000, 0xb800000a, 0xc8000010, 0x2c000010, 0xf4000014, 0xb4000008, 0x08000000, 0x9800000c, 0xd8000010, 0x08000010, 0xb8000010, 0x98000000, 0x60000000, 0x00000008, 0xc0000000, 0x90000014, 0x10000010, 0xb8000014, 0x28000000, 0x20000010, 0x48000000, 0x08000018, 0x60000000, 0x90000010, 0xf0000010, 0x90000008, 0xc0000000, 0x90000010, 0xf0000010, 0xb0000008, 0x40000000, 0x90000000, 0xf0000010, 0x90000018, 0x60000000, 0x90000010, 0x90000010, 0x90000000, 0x80000000, 0x00000010, 0xa0000000, 0x20000000, 0xa0000000, 0x20000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x20000000, 0x00000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000040, 0x40000002, 0x80000004, 0x80000080, 0x80000006, 0x00000049, 0x00000103, 0x80000009, 0x80000012, 0x80000202, 0x00000018 } }
    , {1, 50, 2, 65, 0, 12, { 0x20000030, 0x60000000, 0xe000002a, 0x20000043, 0xb0000040, 0xd0000053, 0xd0000022, 0x20000000, 0x60000032, 0x60000043, 0x20000040, 0xe0000042, 0x60000002, 0x80000001, 0x00000020, 0x00000003, 0x40000052, 0x40000040, 0xe0000052, 0xa0000000, 0x80000040, 0x20000001, 0x20000060, 0x80000001, 0x40000042, 0xc0000043, 0x40000022, 0x00000003, 0x40000042, 0xc0000043, 0xc0000022, 0x00000001, 0x40000002, 0xc0000043, 0x40000062, 0x80000001, 0x40000042, 0x40000042, 0x40000002, 0x00000002, 0x00000040, 0x80000002, 0x80000000, 0x80000002, 0x80000040, 0x00000000, 0x80000040, 0x80000000, 0x00000040, 0x80000000, 0x00000040, 0x80000002, 0x00000000, 0x80000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000080, 0x00000004, 0x00000009, 0x00000101, 0x00000009, 0x00000012, 0x00000202, 0x0000001a, 0x00000124, 0x0000040c, 0x00000026, 0x0000004a, 0x0000080a, 0x00000060 } }
    , {1, 51, 0, 65, 0, 13, { 0xe8000000, 0x0800000c, 0x18000000, 0xb800000a, 0xc8000010, 0x2c000010, 0xf4000014, 0xb4000008, 0x08000000, 0x9800000c, 0xd8000010, 0x08000010, 0xb8000010, 0x98000000, 0x60000000, 0x00000008, 0xc0000000, 0x90000014, 0x10000010, 0xb8000014, 0x28000000, 0x20000010, 0x48000000, 0x08000018, 0x60000000, 0x90000010, 0xf0000010, 0x90000008, 0xc0000000, 0x90000010, 0xf0000010, 0xb0000008, 0x40000000, 0x90000000, 0xf0000010, 0x90000018, 0x60000000, 0x90000010, 0x90000010, 0x90000000, 0x80000000, 0x00000010, 0xa0000000, 0x20000000, 0xa0000000, 0x20000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x20000000, 0x00000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000040, 0x40000002, 0x80000004, 0x80000080, 0x80000006, 0x00000049, 0x00000103, 0x80000009, 0x80000012, 0x80000202 } }
    , {1, 51, 2, 65, 0, 14, { 0xa0000003, 0x20000030, 0x60000000, 0xe000002a, 0x20000043, 0xb0000040, 0xd0000053, 0xd0000022, 0x20000000, 0x60000032, 0x60000043, 0x20000040, 0xe0000042, 0x60000002, 0x80000001, 0x00000020, 0x00000003, 0x40000052, 0x40000040, 0xe0000052, 0xa0000000, 0x80000040, 0x20000001, 0x20000060, 0x80000001, 0x40000042, 0xc0000043, 0x40000022, 0x00000003, 0x40000042, 0xc0000043, 0xc0000022, 0x00000001, 0x40000002, 0xc0000043, 0x40000062, 0x80000001, 0x40000042, 0x40000042, 0x40000002, 0x00000002, 0x00000040, 0x80000002, 0x80000000, 0x80000002, 0x80000040, 0x00000000, 0x80000040, 0x80000000, 0x00000040, 0x80000000, 0x00000040, 0x80000002, 0x00000000, 0x80000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000080, 0x00000004, 0x00000009, 0x00000101, 0x00000009, 0x00000012, 0x00000202, 0x0000001a, 0x00000124, 0x0000040c, 0x00000026, 0x0000004a, 0x0000080a } }
    , {1, 52, 0, 65, 0, 15, { 0x04000010, 0xe8000000, 0x0800000c, 0x18000000, 0xb800000a, 0xc8000010, 0x2c000010, 0xf4000014, 0xb4000008, 0x08000000, 0x9800000c, 0xd8000010, 0x08000010, 0xb8000010, 0x98000000, 0x60000000, 0x00000008, 0xc0000000, 0x90000014, 0x10000010, 0xb8000014, 0x28000000, 0x20000010, 0x48000000, 0x08000018, 0x60000000, 0x90000010, 0xf0000010, 0x90000008, 0xc0000000, 0x90000010, 0xf0000010, 0xb0000008, 0x40000000, 0x90000000, 0xf0000010, 0x90000018, 0x60000000, 0x90000010, 0x90000010, 0x90000000, 0x80000000, 0x00000010, 0xa0000000, 0x20000000, 0xa0000000, 0x20000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x20000000, 0x00000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000040, 0x40000002, 0x80000004, 0x80000080, 0x80000006, 0x00000049, 0x00000103, 0x80000009, 0x80000012 } }
    , {2, 45, 0, 58, 0, 16, { 0xec000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x2c000004, 0xbc000018, 0xb0000010, 0x0000000c, 0xb8000010, 0x08000018, 0x78000010, 0x08000014, 0x70000010, 0xb800001c, 0xe8000000, 0xb0000004, 0x58000010, 0xb000000c, 0x48000000, 0xb0000000, 0xb8000010, 0x98000010, 0xa0000000, 0x00000000, 0x00000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x60000000, 0x00000018, 0xe0000000, 0x90000000, 0x30000010, 0xb0000000, 0x20000000, 0x20000000, 0xa0000000, 0x00000010, 0x80000000, 0x20000000, 0x20000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000041, 0x40000022, 0x80000005, 0xc0000082, 0xc0000046, 0x4000004b, 0x80000107, 0x00000089, 0x00000014, 0x8000024b, 0x0000011b, 0x8000016d, 0x8000041a, 0x000002e4, 0x80000054, 0x00000967 } }
    , {2, 46, 0, 58, 0, 17, { 0x2400001c, 0xec000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x2c000004, 0xbc000018, 0xb0000010, 0x0000000c, 0xb8000010, 0x08000018, 0x78000010, 0x08000014, 0x70000010, 0xb800001c, 0xe8000000, 0xb0000004, 0x58000010, 0xb000000c, 0x48000000, 0xb0000000, 0xb8000010, 0x98000010, 0xa0000000, 0x00000000, 0x00000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x60000000, 0x00000018, 0xe0000000, 0x90000000, 0x30000010, 0xb0000000, 0x20000000, 0x20000000, 0xa0000000, 0x00000010, 0x80000000, 0x20000000, 0x20000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000041, 0x40000022, 0x80000005, 0xc0000082, 0xc0000046, 0x4000004b, 0x80000107, 0x00000089, 0x00000014, 0x8000024b, 0x0000011b, 0x8000016d, 0x8000041a, 0x000002e4, 0x80000054 } }
    , {2, 46, 2, 58, 0, 18, { 0x90000070, 0xb0000053, 0x30000008, 0x00000043, 0xd0000072, 0xb0000010, 0xf0000062, 0xc0000042, 0x00000030, 0xe0000042, 0x20000060, 0xe0000041, 0x20000050, 0xc0000041, 0xe0000072, 0xa0000003, 0xc0000012, 0x60000041, 0xc0000032, 0x20000001, 0xc0000002, 0xe0000042, 0x60000042, 0x80000002, 0x00000000, 0x00000000, 0x80000000, 0x00000002, 0x00000040, 0x00000000, 0x80000040, 0x80000000, 0x00000040, 0x80000001, 0x00000060, 0x80000003, 0x40000002, 0xc0000040, 0xc0000002, 0x80000000, 0x80000000, 0x80000002, 0x00000040, 0x00000002, 0x80000000, 0x80000000, 0x80000000, 0x00000002, 0x00000040, 0x00000000, 0x80000040, 0x80000002, 0x00000000, 0x80000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000080, 0x00000004, 0x00000009, 0x00000105, 0x00000089, 0x00000016, 0x0000020b, 0x0000011b, 0x0000012d, 0x0000041e, 0x00000224, 0x00000050, 0x0000092e, 0x0000046c, 0x000005b6, 0x0000106a, 0x00000b90, 0x00000152 } }
    , {2, 47, 0, 58, 0, 19, { 0x20000010, 0x2400001c, 0xec000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x2c000004, 0xbc000018, 0xb0000010, 0x0000000c, 0xb8000010, 0x08000018, 0x78000010, 0x08000014, 0x70000010, 0xb800001c, 0xe8000000, 0xb0000004, 0x58000010, 0xb000000c, 0x48000000, 0xb0000000, 0xb8000010, 0x98000010, 0xa0000000, 0x00000000, 0x00000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x60000000, 0x00000018, 0xe0000000, 0x90000000, 0x30000010, 0xb0000000, 0x20000000, 0x20000000, 0xa0000000, 0x00000010, 0x80000000, 0x20000000, 0x20000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000041, 0x40000022, 0x80000005, 0xc0000082, 0xc0000046, 0x4000004b, 0x80000107, 0x00000089, 0x00000014, 0x8000024b, 0x0000011b, 0x8000016d, 0x8000041a, 0x000002e4 } }
    , {2, 48, 0, 58, 0, 20, { 0xbc00001a, 0x20000010, 0x2400001c, 0xec000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x2c000004, 0xbc000018, 0xb0000010, 0x0000000c, 0xb8000010, 0x08000018, 0x78000010, 0x08000014, 0x70000010, 0xb800001c, 0xe8000000, 0xb0000004, 0x58000010, 0xb000000c, 0x48000000, 0xb0000000, 0xb8000010, 0x98000010, 0xa0000000, 0x00000000, 0x00000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x60000000, 0x00000018, 0xe0000000, 0x90000000, 0x30000010, 0xb0000000, 0x20000000, 0x20000000, 0xa0000000, 0x00000010, 0x80000000, 0x20000000, 0x20000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000041, 0x40000022, 0x80000005, 0xc0000082, 0xc0000046, 0x4000004b, 0x80000107, 0x00000089, 0x00000014, 0x8000024b, 0x0000011b, 0x8000016d, 0x8000041a } }
    , {2, 49, 0, 58, 0, 21, { 0x3c000004, 0xbc00001a, 0x20000010, 0x2400001c, 0xec000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x2c000004, 0xbc000018, 0xb0000010, 0x0000000c, 0xb8000010, 0x08000018, 0x78000010, 0x08000014, 0x70000010, 0xb800001c, 0xe8000000, 0xb0000004, 0x58000010, 0xb000000c, 0x48000000, 0xb0000000, 0xb8000010, 0x98000010, 0xa0000000, 0x00000000, 0x00000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x60000000, 0x00000018, 0xe0000000, 0x90000000, 0x30000010, 0xb0000000, 0x20000000, 0x20000000, 0xa0000000, 0x00000010, 0x80000000, 0x20000000, 0x20000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000041, 0x40000022, 0x80000005, 0xc0000082, 0xc0000046, 0x4000004b, 0x80000107, 0x00000089, 0x00000014, 0x8000024b, 0x0000011b, 0x8000016d } }
    , {2, 49, 2, 58, 0, 22, { 0xf0000010, 0xf000006a, 0x80000040, 0x90000070, 0xb0000053, 0x30000008, 0x00000043, 0xd0000072, 0xb0000010, 0xf0000062, 0xc0000042, 0x00000030, 0xe0000042, 0x20000060, 0xe0000041, 0x20000050, 0xc0000041, 0xe0000072, 0xa0000003, 0xc0000012, 0x60000041, 0xc0000032, 0x20000001, 0xc0000002, 0xe0000042, 0x60000042, 0x80000002, 0x00000000, 0x00000000, 0x80000000, 0x00000002, 0x00000040, 0x00000000, 0x80000040, 0x80000000, 0x00000040, 0x80000001, 0x00000060, 0x80000003, 0x40000002, 0xc0000040, 0xc0000002, 0x80000000, 0x80000000, 0x80000002, 0x00000040, 0x00000002, 0x80000000, 0x80000000, 0x80000000, 0x00000002, 0x00000040, 0x00000000, 0x80000040, 0x80000002, 0x00000000, 0x80000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000080, 0x00000004, 0x00000009, 0x00000105, 0x00000089, 0x00000016, 0x0000020b, 0x0000011b, 0x0000012d, 0x0000041e, 0x00000224, 0x00000050, 0x0000092e, 0x0000046c, 0x000005b6 } }
    , {2, 50, 0, 65, 0, 23, { 0xb400001c, 0x3c000004, 0xbc00001a, 0x20000010, 0x2400001c, 0xec000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x2c000004, 0xbc000018, 0xb0000010, 0x0000000c, 0xb8000010, 0x08000018, 0x78000010, 0x08000014, 0x70000010, 0xb800001c, 0xe8000000, 0xb0000004, 0x58000010, 0xb000000c, 0x48000000, 0xb0000000, 0xb8000010, 0x98000010, 0xa0000000, 0x00000000, 0x00000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x60000000, 0x00000018, 0xe0000000, 0x90000000, 0x30000010, 0xb0000000, 0x20000000, 0x20000000, 0xa0000000, 0x00000010, 0x80000000, 0x20000000, 0x20000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000041, 0x40000022, 0x80000005, 0xc0000082, 0xc0000046, 0x4000004b, 0x80000107, 0x00000089, 0x00000014, 0x8000024b, 0x0000011b } }
    , {2, 50, 2, 65, 0, 24, { 0xd0000072, 0xf0000010, 0xf000006a, 0x80000040, 0x90000070, 0xb0000053, 0x30000008, 0x00000043, 0xd0000072, 0xb0000010, 0xf0000062, 0xc0000042, 0x00000030, 0xe0000042, 0x20000060, 0xe0000041, 0x20000050, 0xc0000041, 0xe0000072, 0xa0000003, 0xc0000012, 0x60000041, 0xc0000032, 0x20000001, 0xc0000002, 0xe0000042, 0x60000042, 0x80000002, 0x00000000, 0x00000000, 0x80000000, 0x00000002, 0x00000040, 0x00000000, 0x80000040, 0x80000000, 0x00000040, 0x80000001, 0x00000060, 0x80000003, 0x40000002, 0xc0000040, 0xc0000002, 0x80000000, 0x80000000, 0x80000002, 0x00000040, 0x00000002, 0x80000000, 0x80000000, 0x80000000, 0x00000002, 0x00000040, 0x00000000, 0x80000040, 0x80000002, 0x00000000, 0x80000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000080, 0x00000004, 0x00000009, 0x00000105, 0x00000089, 0x00000016, 0x0000020b, 0x0000011b, 0x0000012d, 0x0000041e, 0x00000224, 0x00000050, 0x0000092e, 0x0000046c } }
    , {2, 51, 0, 65, 0, 25, { 0xc0000010, 0xb400001c, 0x3c000004, 0xbc00001a, 0x20000010, 0x2400001c, 0xec000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x2c000004, 0xbc000018, 0xb0000010, 0x0000000c, 0xb8000010, 0x08000018, 0x78000010, 0x08000014, 0x70000010, 0xb800001c, 0xe8000000, 0xb0000004, 0x58000010, 0xb000000c, 0x48000000, 0xb0000000, 0xb8000010, 0x98000010, 0xa0000000, 0x00000000, 0x00000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x60000000, 0x00000018, 0xe0000000, 0x90000000, 0x30000010, 0xb0000000, 0x20000000, 0x20000000, 0xa0000000, 0x00000010, 0x80000000, 0x20000000, 0x20000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000041, 0x40000022, 0x80000005, 0xc0000082, 0xc0000046, 0x4000004b, 0x80000107, 0x00000089, 0x00000014, 0x8000024b } }
    , {2, 51, 2, 65, 0, 26, { 0x00000043, 0xd0000072, 0xf0000010, 0xf000006a, 0x80000040, 0x90000070, 0xb0000053, 0x30000008, 0x00000043, 0xd0000072, 0xb0000010, 0xf0000062, 0xc0000042, 0x00000030, 0xe0000042, 0x20000060, 0xe0000041, 0x20000050, 0xc0000041, 0xe0000072, 0xa0000003, 0xc0000012, 0x60000041, 0xc0000032, 0x20000001, 0xc0000002, 0xe0000042, 0x60000042, 0x80000002, 0x00000000, 0x00000000, 0x80000000, 0x00000002, 0x00000040, 0x00000000, 0x80000040, 0x80000000, 0x00000040, 0x80000001, 0x00000060, 0x80000003, 0x40000002, 0xc0000040, 0xc0000002, 0x80000000, 0x80000000, 0x80000002, 0x00000040, 0x00000002, 0x80000000, 0x80000000, 0x80000000, 0x00000002, 0x00000040, 0x00000000, 0x80000040, 0x80000002, 0x00000000, 0x80000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000080, 0x00000004, 0x00000009, 0x00000105, 0x00000089, 0x00000016, 0x0000020b, 0x0000011b, 0x0000012d, 0x0000041e, 0x00000224, 0x00000050, 0x0000092e } }
    , {2, 52, 0, 65, 0, 27, { 0x0c000002, 0xc0000010, 0xb400001c, 0x3c000004, 0xbc00001a, 0x20000010, 0x2400001c, 0xec000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x2c000004, 0xbc000018, 0xb0000010, 0x0000000c, 0xb8000010, 0x08000018, 0x78000010, 0x08000014, 0x70000010, 0xb800001c, 0xe8000000, 0xb0000004, 0x58000010, 0xb000000c, 0x48000000, 0xb0000000, 0xb8000010, 0x98000010, 0xa0000000, 0x00000000, 0x00000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x60000000, 0x00000018, 0xe0000000, 0x90000000, 0x30000010, 0xb0000000, 0x20000000, 0x20000000, 0xa0000000, 0x00000010, 0x80000000, 0x20000000, 0x20000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000041, 0x40000022, 0x80000005, 0xc0000082, 0xc0000046, 0x4000004b, 0x80000107, 0x00000089, 0x00000014 } }
    , {2, 53, 0, 65, 0, 28, { 0xcc000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x3c000004, 0xbc00001a, 0x20000010, 0x2400001c, 0xec000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x2c000004, 0xbc000018, 0xb0000010, 0x0000000c, 0xb8000010, 0x08000018, 0x78000010, 0x08000014, 0x70000010, 0xb800001c, 0xe8000000, 0xb0000004, 0x58000010, 0xb000000c, 0x48000000, 0xb0000000, 0xb8000010, 0x98000010, 0xa0000000, 0x00000000, 0x00000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x60000000, 0x00000018, 0xe0000000, 0x90000000, 0x30000010, 0xb0000000, 0x20000000, 0x20000000, 0xa0000000, 0x00000010, 0x80000000, 0x20000000, 0x20000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000041, 0x40000022, 0x80000005, 0xc0000082, 0xc0000046, 0x4000004b, 0x80000107, 0x00000089 } }
    , {2, 54, 0, 65, 0, 29, { 0x0400001c, 0xcc000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x3c000004, 0xbc00001a, 0x20000010, 0x2400001c, 0xec000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x2c000004, 0xbc000018, 0xb0000010, 0x0000000c, 0xb8000010, 0x08000018, 0x78000010, 0x08000014, 0x70000010, 0xb800001c, 0xe8000000, 0xb0000004, 0x58000010, 0xb000000c, 0x48000000, 0xb0000000, 0xb8000010, 0x98000010, 0xa0000000, 0x00000000, 0x00000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x60000000, 0x00000018, 0xe0000000, 0x90000000, 0x30000010, 0xb0000000, 0x20000000, 0x20000000, 0xa0000000, 0x00000010, 0x80000000, 0x20000000, 0x20000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000041, 0x40000022, 0x80000005, 0xc0000082, 0xc0000046, 0x4000004b, 0x80000107 } }
    , {2, 55, 0, 65, 0, 30, { 0x00000010, 0x0400001c, 0xcc000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x3c000004, 0xbc00001a, 0x20000010, 0x2400001c, 0xec000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x2c000004, 0xbc000018, 0xb0000010, 0x0000000c, 0xb8000010, 0x08000018, 0x78000010, 0x08000014, 0x70000010, 0xb800001c, 0xe8000000, 0xb0000004, 0x58000010, 0xb000000c, 0x48000000, 0xb0000000, 0xb8000010, 0x98000010, 0xa0000000, 0x00000000, 0x00000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x60000000, 0x00000018, 0xe0000000, 0x90000000, 0x30000010, 0xb0000000, 0x20000000, 0x20000000, 0xa0000000, 0x00000010, 0x80000000, 0x20000000, 0x20000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000041, 0x40000022, 0x80000005, 0xc0000082, 0xc0000046, 0x4000004b } }
    , {2, 56, 0, 65, 0, 31, { 0x2600001a, 0x00000010, 0x0400001c, 0xcc000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x3c000004, 0xbc00001a, 0x20000010, 0x2400001c, 0xec000014, 0x0c000002, 0xc0000010, 0xb400001c, 0x2c000004, 0xbc000018, 0xb0000010, 0x0000000c, 0xb8000010, 0x08000018, 0x78000010, 0x08000014, 0x70000010, 0xb800001c, 0xe8000000, 0xb0000004, 0x58000010, 0xb000000c, 0x48000000, 0xb0000000, 0xb8000010, 0x98000010, 0xa0000000, 0x00000000, 0x00000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0x20000000, 0x00000010, 0x60000000, 0x00000018, 0xe0000000, 0x90000000, 0x30000010, 0xb0000000, 0x20000000, 0x20000000, 0xa0000000, 0x00000010, 0x80000000, 0x20000000, 0x20000000, 0x20000000, 0x80000000, 0x00000010, 0x00000000, 0x20000010, 0xa0000000, 0x00000000, 0x20000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000020, 0x00000001, 0x40000002, 0x40000041, 0x40000022, 0x80000005, 0xc0000082, 0xc0000046 } }
    , {0, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}
};
void ubc_check(const u32 W[80], u32 dvmask[1]) {
    u32 mask = ~((u32)(0));
    mask &= (((((W[44] ^ W[45]) >> 29) & 1) - 1) | ~(DV_I_48_0_bit | DV_I_51_0_bit | DV_I_52_0_bit |
                                                     DV_II_45_0_bit | DV_II_46_0_bit | DV_II_50_0_bit | DV_II_51_0_bit));
    mask &= (((((W[49] ^ W[50]) >> 29) & 1) - 1) | ~(DV_I_46_0_bit | DV_II_45_0_bit | DV_II_50_0_bit |
                                                     DV_II_51_0_bit | DV_II_55_0_bit | DV_II_56_0_bit));
    mask &= (((((W[48] ^ W[49]) >> 29) & 1) - 1) | ~(DV_I_45_0_bit | DV_I_52_0_bit | DV_II_49_0_bit |
                                                     DV_II_50_0_bit | DV_II_54_0_bit | DV_II_55_0_bit));
    mask &= ((((W[47] ^ (W[50] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_I_47_0_bit | DV_I_49_0_bit |
             DV_I_51_0_bit | DV_II_45_0_bit | DV_II_51_0_bit | DV_II_56_0_bit));
    mask &= (((((W[47] ^ W[48]) >> 29) & 1) - 1) | ~(DV_I_44_0_bit | DV_I_51_0_bit | DV_II_48_0_bit |
                                                     DV_II_49_0_bit | DV_II_53_0_bit | DV_II_54_0_bit));
    mask &= (((((W[46] >> 4) ^ (W[49] >> 29)) & 1) - 1) | ~(DV_I_46_0_bit | DV_I_48_0_bit |
                                                            DV_I_50_0_bit | DV_I_52_0_bit | DV_II_50_0_bit | DV_II_55_0_bit));
    mask &= (((((W[46] ^ W[47]) >> 29) & 1) - 1) | ~(DV_I_43_0_bit | DV_I_50_0_bit | DV_II_47_0_bit |
                                                     DV_II_48_0_bit | DV_II_52_0_bit | DV_II_53_0_bit));
    mask &= (((((W[45] >> 4) ^ (W[48] >> 29)) & 1) - 1) | ~(DV_I_45_0_bit | DV_I_47_0_bit |
                                                            DV_I_49_0_bit | DV_I_51_0_bit | DV_II_49_0_bit | DV_II_54_0_bit));
    mask &= (((((W[45] ^ W[46]) >> 29) & 1) - 1) | ~(DV_I_49_0_bit | DV_I_52_0_bit | DV_II_46_0_bit |
                                                     DV_II_47_0_bit | DV_II_51_0_bit | DV_II_52_0_bit));
    mask &= (((((W[44] >> 4) ^ (W[47] >> 29)) & 1) - 1) | ~(DV_I_44_0_bit | DV_I_46_0_bit |
                                                            DV_I_48_0_bit | DV_I_50_0_bit | DV_II_48_0_bit | DV_II_53_0_bit));
    mask &= (((((W[43] >> 4) ^ (W[46] >> 29)) & 1) - 1) | ~(DV_I_43_0_bit | DV_I_45_0_bit |
                                                            DV_I_47_0_bit | DV_I_49_0_bit | DV_II_47_0_bit | DV_II_52_0_bit));
    mask &= (((((W[43] ^ W[44]) >> 29) & 1) - 1) | ~(DV_I_47_0_bit | DV_I_50_0_bit | DV_I_51_0_bit |
                                                     DV_II_45_0_bit | DV_II_49_0_bit | DV_II_50_0_bit));
    mask &= (((((W[42] >> 4) ^ (W[45] >> 29)) & 1) - 1) | ~(DV_I_44_0_bit | DV_I_46_0_bit |
                                                            DV_I_48_0_bit | DV_I_52_0_bit | DV_II_46_0_bit | DV_II_51_0_bit));
    mask &= (((((W[41] >> 4) ^ (W[44] >> 29)) & 1) - 1) | ~(DV_I_43_0_bit | DV_I_45_0_bit |
                                                            DV_I_47_0_bit | DV_I_51_0_bit | DV_II_45_0_bit | DV_II_50_0_bit));
    mask &= (((((W[40] ^ W[41]) >> 29) & 1) - 1) | ~(DV_I_44_0_bit | DV_I_47_0_bit | DV_I_48_0_bit |
                                                     DV_II_46_0_bit | DV_II_47_0_bit | DV_II_56_0_bit));
    mask &= (((((W[54] ^ W[55]) >> 29) & 1) - 1) | ~(DV_I_51_0_bit | DV_II_47_0_bit | DV_II_50_0_bit |
                                                     DV_II_55_0_bit | DV_II_56_0_bit));
    mask &= (((((W[53] ^ W[54]) >> 29) & 1) - 1) | ~(DV_I_50_0_bit | DV_II_46_0_bit | DV_II_49_0_bit |
                                                     DV_II_54_0_bit | DV_II_55_0_bit));
    mask &= (((((W[52] ^ W[53]) >> 29) & 1) - 1) | ~(DV_I_49_0_bit | DV_II_45_0_bit | DV_II_48_0_bit |
                                                     DV_II_53_0_bit | DV_II_54_0_bit));
    mask &= ((((W[50] ^ (W[53] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_I_50_0_bit | DV_I_52_0_bit |
             DV_II_46_0_bit | DV_II_48_0_bit | DV_II_54_0_bit));
    mask &= (((((W[50] ^ W[51]) >> 29) & 1) - 1) | ~(DV_I_47_0_bit | DV_II_46_0_bit | DV_II_51_0_bit |
                                                     DV_II_52_0_bit | DV_II_56_0_bit));
    mask &= ((((W[49] ^ (W[52] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_I_49_0_bit | DV_I_51_0_bit |
             DV_II_45_0_bit | DV_II_47_0_bit | DV_II_53_0_bit));
    mask &= ((((W[48] ^ (W[51] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_I_48_0_bit | DV_I_50_0_bit |
             DV_I_52_0_bit | DV_II_46_0_bit | DV_II_52_0_bit));
    mask &= (((((W[42] ^ W[43]) >> 29) & 1) - 1) | ~(DV_I_46_0_bit | DV_I_49_0_bit | DV_I_50_0_bit |
                                                     DV_II_48_0_bit | DV_II_49_0_bit));
    mask &= (((((W[41] ^ W[42]) >> 29) & 1) - 1) | ~(DV_I_45_0_bit | DV_I_48_0_bit | DV_I_49_0_bit |
                                                     DV_II_47_0_bit | DV_II_48_0_bit));
    mask &= (((((W[40] >> 4) ^ (W[43] >> 29)) & 1) - 1) | ~(DV_I_44_0_bit | DV_I_46_0_bit |
                                                            DV_I_50_0_bit | DV_II_49_0_bit | DV_II_56_0_bit));
    mask &= (((((W[39] >> 4) ^ (W[42] >> 29)) & 1) - 1) | ~(DV_I_43_0_bit | DV_I_45_0_bit |
                                                            DV_I_49_0_bit | DV_II_48_0_bit | DV_II_55_0_bit));
    if (mask & (DV_I_44_0_bit | DV_I_48_0_bit | DV_II_47_0_bit | DV_II_54_0_bit | DV_II_56_0_bit))
        mask &= (((((W[38] >> 4) ^ (W[41] >> 29)) & 1) - 1) | ~(DV_I_44_0_bit | DV_I_48_0_bit |
                                                                DV_II_47_0_bit | DV_II_54_0_bit | DV_II_56_0_bit));
    mask &= (((((W[37] >> 4) ^ (W[40] >> 29)) & 1) - 1) | ~(DV_I_43_0_bit | DV_I_47_0_bit |
                                                            DV_II_46_0_bit | DV_II_53_0_bit | DV_II_55_0_bit));
    if (mask & (DV_I_52_0_bit | DV_II_48_0_bit | DV_II_51_0_bit | DV_II_56_0_bit))
        mask &= (((((W[55] ^ W[56]) >> 29) & 1) - 1) | ~(DV_I_52_0_bit | DV_II_48_0_bit | DV_II_51_0_bit |
                                                         DV_II_56_0_bit));
    if (mask & (DV_I_52_0_bit | DV_II_48_0_bit | DV_II_50_0_bit | DV_II_56_0_bit))
        mask &= ((((W[52] ^ (W[55] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_I_52_0_bit | DV_II_48_0_bit |
                 DV_II_50_0_bit | DV_II_56_0_bit));
    if (mask & (DV_I_51_0_bit | DV_II_47_0_bit | DV_II_49_0_bit | DV_II_55_0_bit))
        mask &= ((((W[51] ^ (W[54] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_I_51_0_bit | DV_II_47_0_bit |
                 DV_II_49_0_bit | DV_II_55_0_bit));
    if (mask & (DV_I_48_0_bit | DV_II_47_0_bit | DV_II_52_0_bit | DV_II_53_0_bit))
        mask &= (((((W[51] ^ W[52]) >> 29) & 1) - 1) | ~(DV_I_48_0_bit | DV_II_47_0_bit | DV_II_52_0_bit |
                                                         DV_II_53_0_bit));
    if (mask & (DV_I_46_0_bit | DV_I_49_0_bit | DV_II_45_0_bit | DV_II_48_0_bit))
        mask &= (((((W[36] >> 4) ^ (W[40] >> 29)) & 1) - 1) | ~(DV_I_46_0_bit | DV_I_49_0_bit |
                                                                DV_II_45_0_bit | DV_II_48_0_bit));
    if (mask & (DV_I_52_0_bit | DV_II_48_0_bit | DV_II_49_0_bit))
        mask &= ((0 - (((W[53] ^ W[56]) >> 29) & 1)) | ~(DV_I_52_0_bit | DV_II_48_0_bit | DV_II_49_0_bit));
    if (mask & (DV_I_50_0_bit | DV_II_46_0_bit | DV_II_47_0_bit))
        mask &= ((0 - (((W[51] ^ W[54]) >> 29) & 1)) | ~(DV_I_50_0_bit | DV_II_46_0_bit | DV_II_47_0_bit));
    if (mask & (DV_I_49_0_bit | DV_I_51_0_bit | DV_II_45_0_bit))
        mask &= ((0 - (((W[50] ^ W[52]) >> 29) & 1)) | ~(DV_I_49_0_bit | DV_I_51_0_bit | DV_II_45_0_bit));
    if (mask & (DV_I_48_0_bit | DV_I_50_0_bit | DV_I_52_0_bit))
        mask &= ((0 - (((W[49] ^ W[51]) >> 29) & 1)) | ~(DV_I_48_0_bit | DV_I_50_0_bit | DV_I_52_0_bit));
    if (mask & (DV_I_47_0_bit | DV_I_49_0_bit | DV_I_51_0_bit))
        mask &= ((0 - (((W[48] ^ W[50]) >> 29) & 1)) | ~(DV_I_47_0_bit | DV_I_49_0_bit | DV_I_51_0_bit));
    if (mask & (DV_I_46_0_bit | DV_I_48_0_bit | DV_I_50_0_bit))
        mask &= ((0 - (((W[47] ^ W[49]) >> 29) & 1)) | ~(DV_I_46_0_bit | DV_I_48_0_bit | DV_I_50_0_bit));
    if (mask & (DV_I_45_0_bit | DV_I_47_0_bit | DV_I_49_0_bit))
        mask &= ((0 - (((W[46] ^ W[48]) >> 29) & 1)) | ~(DV_I_45_0_bit | DV_I_47_0_bit | DV_I_49_0_bit));
    mask &= ((((W[45] ^ W[47]) & (1 << 6)) - (1 << 6)) | ~(DV_I_47_2_bit | DV_I_49_2_bit |
                                                           DV_I_51_2_bit));
    if (mask & (DV_I_44_0_bit | DV_I_46_0_bit | DV_I_48_0_bit))
        mask &= ((0 - (((W[45] ^ W[47]) >> 29) & 1)) | ~(DV_I_44_0_bit | DV_I_46_0_bit | DV_I_48_0_bit));
    mask &= (((((W[44] ^ W[46]) >> 6) & 1) - 1) | ~(DV_I_46_2_bit | DV_I_48_2_bit | DV_I_50_2_bit));
    if (mask & (DV_I_43_0_bit | DV_I_45_0_bit | DV_I_47_0_bit))
        mask &= ((0 - (((W[44] ^ W[46]) >> 29) & 1)) | ~(DV_I_43_0_bit | DV_I_45_0_bit | DV_I_47_0_bit));
    mask &= ((0 - ((W[41] ^ (W[42] >> 5)) & (1 << 1))) | ~(DV_I_48_2_bit | DV_II_46_2_bit |
                                                           DV_II_51_2_bit));
    mask &= ((0 - ((W[40] ^ (W[41] >> 5)) & (1 << 1))) | ~(DV_I_47_2_bit | DV_I_51_2_bit |
                                                           DV_II_50_2_bit));
    if (mask & (DV_I_44_0_bit | DV_I_46_0_bit | DV_II_56_0_bit))
        mask &= ((0 - (((W[40] ^ W[42]) >> 4) & 1)) | ~(DV_I_44_0_bit | DV_I_46_0_bit | DV_II_56_0_bit));
    mask &= ((0 - ((W[39] ^ (W[40] >> 5)) & (1 << 1))) | ~(DV_I_46_2_bit | DV_I_50_2_bit |
                                                           DV_II_49_2_bit));
    if (mask & (DV_I_43_0_bit | DV_I_45_0_bit | DV_II_55_0_bit))
        mask &= ((0 - (((W[39] ^ W[41]) >> 4) & 1)) | ~(DV_I_43_0_bit | DV_I_45_0_bit | DV_II_55_0_bit));
    if (mask & (DV_I_44_0_bit | DV_II_54_0_bit | DV_II_56_0_bit))
        mask &= ((0 - (((W[38] ^ W[40]) >> 4) & 1)) | ~(DV_I_44_0_bit | DV_II_54_0_bit | DV_II_56_0_bit));
    if (mask & (DV_I_43_0_bit | DV_II_53_0_bit | DV_II_55_0_bit))
        mask &= ((0 - (((W[37] ^ W[39]) >> 4) & 1)) | ~(DV_I_43_0_bit | DV_II_53_0_bit | DV_II_55_0_bit));
    mask &= ((0 - ((W[36] ^ (W[37] >> 5)) & (1 << 1))) | ~(DV_I_47_2_bit | DV_I_50_2_bit |
                                                           DV_II_46_2_bit));
    if (mask & (DV_I_45_0_bit | DV_I_48_0_bit | DV_II_47_0_bit))
        mask &= (((((W[35] >> 4) ^ (W[39] >> 29)) & 1) - 1) | ~(DV_I_45_0_bit | DV_I_48_0_bit |
                                                                DV_II_47_0_bit));
    if (mask & (DV_I_48_0_bit | DV_II_48_0_bit))
        mask &= ((0 - ((W[63] ^ (W[64] >> 5)) & (1 << 0))) | ~(DV_I_48_0_bit | DV_II_48_0_bit));
    if (mask & (DV_I_45_0_bit | DV_II_45_0_bit))
        mask &= ((0 - ((W[63] ^ (W[64] >> 5)) & (1 << 1))) | ~(DV_I_45_0_bit | DV_II_45_0_bit));
    if (mask & (DV_I_47_0_bit | DV_II_47_0_bit))
        mask &= ((0 - ((W[62] ^ (W[63] >> 5)) & (1 << 0))) | ~(DV_I_47_0_bit | DV_II_47_0_bit));
    if (mask & (DV_I_46_0_bit | DV_II_46_0_bit))
        mask &= ((0 - ((W[61] ^ (W[62] >> 5)) & (1 << 0))) | ~(DV_I_46_0_bit | DV_II_46_0_bit));
    mask &= ((0 - ((W[61] ^ (W[62] >> 5)) & (1 << 2))) | ~(DV_I_46_2_bit | DV_II_46_2_bit));
    if (mask & (DV_I_45_0_bit | DV_II_45_0_bit))
        mask &= ((0 - ((W[60] ^ (W[61] >> 5)) & (1 << 0))) | ~(DV_I_45_0_bit | DV_II_45_0_bit));
    if (mask & (DV_II_51_0_bit | DV_II_54_0_bit))
        mask &= (((((W[58] ^ W[59]) >> 29) & 1) - 1) | ~(DV_II_51_0_bit | DV_II_54_0_bit));
    if (mask & (DV_II_50_0_bit | DV_II_53_0_bit))
        mask &= (((((W[57] ^ W[58]) >> 29) & 1) - 1) | ~(DV_II_50_0_bit | DV_II_53_0_bit));
    if (mask & (DV_II_52_0_bit | DV_II_54_0_bit))
        mask &= ((((W[56] ^ (W[59] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_II_52_0_bit | DV_II_54_0_bit));
    if (mask & (DV_II_51_0_bit | DV_II_52_0_bit))
        mask &= ((0 - (((W[56] ^ W[59]) >> 29) & 1)) | ~(DV_II_51_0_bit | DV_II_52_0_bit));
    if (mask & (DV_II_49_0_bit | DV_II_52_0_bit))
        mask &= (((((W[56] ^ W[57]) >> 29) & 1) - 1) | ~(DV_II_49_0_bit | DV_II_52_0_bit));
    if (mask & (DV_II_51_0_bit | DV_II_53_0_bit))
        mask &= ((((W[55] ^ (W[58] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_II_51_0_bit | DV_II_53_0_bit));
    if (mask & (DV_II_50_0_bit | DV_II_52_0_bit))
        mask &= ((((W[54] ^ (W[57] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_II_50_0_bit | DV_II_52_0_bit));
    if (mask & (DV_II_49_0_bit | DV_II_51_0_bit))
        mask &= ((((W[53] ^ (W[56] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_II_49_0_bit | DV_II_51_0_bit));
    mask &= ((((W[51] ^ (W[50] >> 5)) & (1 << 1)) - (1 << 1)) | ~(DV_I_50_2_bit | DV_II_46_2_bit));
    mask &= ((((W[48] ^ W[50]) & (1 << 6)) - (1 << 6)) | ~(DV_I_50_2_bit | DV_II_46_2_bit));
    if (mask & (DV_I_51_0_bit | DV_I_52_0_bit))
        mask &= ((0 - (((W[48] ^ W[55]) >> 29) & 1)) | ~(DV_I_51_0_bit | DV_I_52_0_bit));
    mask &= ((((W[47] ^ W[49]) & (1 << 6)) - (1 << 6)) | ~(DV_I_49_2_bit | DV_I_51_2_bit));
    mask &= ((((W[48] ^ (W[47] >> 5)) & (1 << 1)) - (1 << 1)) | ~(DV_I_47_2_bit | DV_II_51_2_bit));
    mask &= ((((W[46] ^ W[48]) & (1 << 6)) - (1 << 6)) | ~(DV_I_48_2_bit | DV_I_50_2_bit));
    mask &= ((((W[47] ^ (W[46] >> 5)) & (1 << 1)) - (1 << 1)) | ~(DV_I_46_2_bit | DV_II_50_2_bit));
    mask &= ((0 - ((W[44] ^ (W[45] >> 5)) & (1 << 1))) | ~(DV_I_51_2_bit | DV_II_49_2_bit));
    mask &= ((((W[43] ^ W[45]) & (1 << 6)) - (1 << 6)) | ~(DV_I_47_2_bit | DV_I_49_2_bit));
    mask &= (((((W[42] ^ W[44]) >> 6) & 1) - 1) | ~(DV_I_46_2_bit | DV_I_48_2_bit));
    mask &= ((((W[43] ^ (W[42] >> 5)) & (1 << 1)) - (1 << 1)) | ~(DV_II_46_2_bit | DV_II_51_2_bit));
    mask &= ((((W[42] ^ (W[41] >> 5)) & (1 << 1)) - (1 << 1)) | ~(DV_I_51_2_bit | DV_II_50_2_bit));
    mask &= ((((W[41] ^ (W[40] >> 5)) & (1 << 1)) - (1 << 1)) | ~(DV_I_50_2_bit | DV_II_49_2_bit));
    if (mask & (DV_I_52_0_bit | DV_II_51_0_bit))
        mask &= ((((W[39] ^ (W[43] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_I_52_0_bit | DV_II_51_0_bit));
    if (mask & (DV_I_51_0_bit | DV_II_50_0_bit))
        mask &= ((((W[38] ^ (W[42] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_I_51_0_bit | DV_II_50_0_bit));
    if (mask & (DV_I_48_2_bit | DV_I_51_2_bit))
        mask &= ((0 - ((W[37] ^ (W[38] >> 5)) & (1 << 1))) | ~(DV_I_48_2_bit | DV_I_51_2_bit));
    if (mask & (DV_I_50_0_bit | DV_II_49_0_bit))
        mask &= ((((W[37] ^ (W[41] >> 25)) & (1 << 4)) - (1 << 4)) | ~(DV_I_50_0_bit | DV_II_49_0_bit));
    if (mask & (DV_II_52_0_bit | DV_II_54_0_bit))
        mask &= ((0 - ((W[36] ^ W[38]) & (1 << 4))) | ~(DV_II_52_0_bit | DV_II_54_0_bit));
    mask &= ((0 - ((W[35] ^ (W[36] >> 5)) & (1 << 1))) | ~(DV_I_46_2_bit | DV_I_49_2_bit));
    if (mask & (DV_I_51_0_bit | DV_II_47_0_bit))
        mask &= ((((W[35] ^ (W[39] >> 25)) & (1 << 3)) - (1 << 3)) | ~(DV_I_51_0_bit | DV_II_47_0_bit));
    if (mask) {

        if (mask & DV_I_43_0_bit)
            if (
                    !((W[61] ^ (W[62] >> 5)) & (1 << 1))
                    || !(!((W[59] ^ (W[63] >> 25)) & (1 << 5)))
                    || !((W[58] ^ (W[63] >> 30)) & (1 << 0))
            )
                mask &= ~DV_I_43_0_bit;
        if (mask & DV_I_44_0_bit)
            if (
                    !((W[62] ^ (W[63] >> 5)) & (1 << 1))
                    || !(!((W[60] ^ (W[64] >> 25)) & (1 << 5)))
                    || !((W[59] ^ (W[64] >> 30)) & (1 << 0))
            )
                mask &= ~DV_I_44_0_bit;
        if (mask & DV_I_46_2_bit)
            mask &= ((~((W[40] ^ W[42]) >> 2)) | ~DV_I_46_2_bit);
        if (mask & DV_I_47_2_bit)
            if (
                    !((W[62] ^ (W[63] >> 5)) & (1 << 2))
                    || !(!((W[41]^W[43]) & (1 << 6)))
            )
                mask &= ~DV_I_47_2_bit;
        if (mask & DV_I_48_2_bit)
            if (
                    !((W[63] ^ (W[64] >> 5)) & (1 << 2))
                    || !(!((W[48] ^ (W[49] << 5)) & (1 << 6)))
            )
                mask &= ~DV_I_48_2_bit;
        if (mask & DV_I_49_2_bit)
            if (
                    !(!((W[49] ^ (W[50] << 5)) & (1 << 6)))
                    || !((W[42]^W[50]) & (1 << 1))
                    || !(!((W[39] ^ (W[40] << 5)) & (1 << 6)))
                    || !((W[38]^W[40]) & (1 << 1))
            )
                mask &= ~DV_I_49_2_bit;
        if (mask & DV_I_50_0_bit)
            mask &= ((((W[36] ^ W[37]) << 7)) | ~DV_I_50_0_bit);
        if (mask & DV_I_50_2_bit)
            mask &= ((((W[43] ^ W[51]) << 11)) | ~DV_I_50_2_bit);
        if (mask & DV_I_51_0_bit)
            mask &= ((((W[37] ^ W[38]) << 9)) | ~DV_I_51_0_bit);
        if (mask & DV_I_51_2_bit)
            if (
                    !(!((W[51] ^ (W[52] << 5)) & (1 << 6)))
                    || !(!((W[49]^W[51]) & (1 << 6)))
                    || !(!((W[37] ^ (W[37] >> 5)) & (1 << 1)))
                    || !(!((W[35] ^ (W[39] >> 25)) & (1 << 5)))
            )
                mask &= ~DV_I_51_2_bit;
        if (mask & DV_I_52_0_bit)
            mask &= ((((W[38] ^ W[39]) << 11)) | ~DV_I_52_0_bit);
        if (mask & DV_II_46_2_bit)
            mask &= ((((W[47] ^ W[51]) << 17)) | ~DV_II_46_2_bit);
        if (mask & DV_II_48_0_bit)
            if (
                    !(!((W[36] ^ (W[40] >> 25)) & (1 << 3)))
                    || !((W[35] ^ (W[40] << 2)) & (1 << 30))
            )
                mask &= ~DV_II_48_0_bit;
        if (mask & DV_II_49_0_bit)
            if (
                    !(!((W[37] ^ (W[41] >> 25)) & (1 << 3)))
                    || !((W[36] ^ (W[41] << 2)) & (1 << 30))
            )
                mask &= ~DV_II_49_0_bit;
        if (mask & DV_II_49_2_bit)
            if (
                    !(!((W[53] ^ (W[54] << 5)) & (1 << 6)))
                    || !(!((W[51]^W[53]) & (1 << 6)))
                    || !((W[50]^W[54]) & (1 << 1))
                    || !(!((W[45] ^ (W[46] << 5)) & (1 << 6)))
                    || !(!((W[37] ^ (W[41] >> 25)) & (1 << 5)))
                    || !((W[36] ^ (W[41] >> 30)) & (1 << 0))
            )
                mask &= ~DV_II_49_2_bit;
        if (mask & DV_II_50_0_bit)
            if (
                    !((W[55]^W[58]) & (1 << 29))
                    || !(!((W[38] ^ (W[42] >> 25)) & (1 << 3)))
                    || !((W[37] ^ (W[42] << 2)) & (1 << 30))
            )
                mask &= ~DV_II_50_0_bit;
        if (mask & DV_II_50_2_bit)
            if (
                    !(!((W[54] ^ (W[55] << 5)) & (1 << 6)))
                    || !(!((W[52]^W[54]) & (1 << 6)))
                    || !((W[51]^W[55]) & (1 << 1))
                    || !((W[45]^W[47]) & (1 << 1))
                    || !(!((W[38] ^ (W[42] >> 25)) & (1 << 5)))
                    || !((W[37] ^ (W[42] >> 30)) & (1 << 0))
            )
                mask &= ~DV_II_50_2_bit;
        if (mask & DV_II_51_0_bit)
            if (
                    !(!((W[39] ^ (W[43] >> 25)) & (1 << 3)))
                    || !((W[38] ^ (W[43] << 2)) & (1 << 30))
            )
                mask &= ~DV_II_51_0_bit;
        if (mask & DV_II_51_2_bit)
            if (
                    !(!((W[55] ^ (W[56] << 5)) & (1 << 6)))
                    || !(!((W[53]^W[55]) & (1 << 6)))
                    || !((W[52]^W[56]) & (1 << 1))
                    || !((W[46]^W[48]) & (1 << 1))
                    || !(!((W[39] ^ (W[43] >> 25)) & (1 << 5)))
                    || !((W[38] ^ (W[43] >> 30)) & (1 << 0))
            )
                mask &= ~DV_II_51_2_bit;
        if (mask & DV_II_52_0_bit)
            if (
                    !(!((W[59]^W[60]) & (1 << 29)))
                    || !(!((W[40] ^ (W[44] >> 25)) & (1 << 3)))
                    || !(!((W[40] ^ (W[44] >> 25)) & (1 << 4)))
                    || !((W[39] ^ (W[44] << 2)) & (1 << 30))
            )
                mask &= ~DV_II_52_0_bit;
        if (mask & DV_II_53_0_bit)
            if (
                    !((W[58]^W[61]) & (1 << 29))
                    || !(!((W[57] ^ (W[61] >> 25)) & (1 << 4)))
                    || !(!((W[41] ^ (W[45] >> 25)) & (1 << 3)))
                    || !(!((W[41] ^ (W[45] >> 25)) & (1 << 4)))
            )
                mask &= ~DV_II_53_0_bit;
        if (mask & DV_II_54_0_bit)
            if (
                    !(!((W[58] ^ (W[62] >> 25)) & (1 << 4)))
                    || !(!((W[42] ^ (W[46] >> 25)) & (1 << 3)))
                    || !(!((W[42] ^ (W[46] >> 25)) & (1 << 4)))
            )
                mask &= ~DV_II_54_0_bit;
        if (mask & DV_II_55_0_bit)
            if (
                    !(!((W[59] ^ (W[63] >> 25)) & (1 << 4)))
                    || !(!((W[57] ^ (W[59] >> 25)) & (1 << 4)))
                    || !(!((W[43] ^ (W[47] >> 25)) & (1 << 3)))
                    || !(!((W[43] ^ (W[47] >> 25)) & (1 << 4)))
            )
                mask &= ~DV_II_55_0_bit;
        if (mask & DV_II_56_0_bit)
            if (
                    !(!((W[60] ^ (W[64] >> 25)) & (1 << 4)))
                    || !(!((W[44] ^ (W[48] >> 25)) & (1 << 3)))
                    || !(!((W[44] ^ (W[48] >> 25)) & (1 << 4)))
            )
                mask &= ~DV_II_56_0_bit;
    }

    dvmask[0] = mask;
}

/**************** SHA1DC SOURCE END ***************/

/******************* PARG SOURCE ******************/

struct parg_state parg_state_default = {
    /* .optarg =    */ NULL,
    /* .optind =    */ 1,
    /* .optopt =    */ '?',
    /* .nextchar =  */ NULL
};

/* Check if state is at end of argv */
int is_argv_end(const struct parg_state *ps, int argc,
                char *const argv[]) {
    return ps->optind >= argc || argv[ps->optind] == NULL;
}

/* *INDENT-OFF* */
/* Match string at nextchar against longopts. */
int match_long( struct parg_state *ps, int argc,
                char *const argv[], const char *optstring,
                const struct parg_opt *longopts,
                int *longindex) {
    int i, match = -1, num_match = 0;
    size_t len = strcspn(ps->nextchar, "=");

    for (i = 0; longopts[i].name; ++i) {
        if (memcmp(ps->nextchar, longopts[i].name, len) == 0) {
            match = i;
            num_match++;
            /* Take if exact match */
            if (longopts[i].name[len] == '\0') {
                num_match = 1;
                break;
            }
        }
    }

    /* Return '?' on no or ambiguous match */
    if (num_match != 1) {
        ps->optopt = 0;
        ps->nextchar = NULL;
        return '?';
    }

    assert(match != -1);

    if (longindex != NULL) {
        *longindex = match;
    }

    if (ps->nextchar[len] == '=') {
        /* Option argument present, check if extraneous */
        if (longopts[match].has_arg == PARG_NOARG) {
            ps->optopt = longopts[match].flag ? 0 : longopts[match].val;
            ps->nextchar = NULL;
            return optstring[0] == ':' ? ':' : '?';
        } else {
            ps->optarg = &ps->nextchar[len + 1];
        }
    } else if (longopts[match].has_arg == PARG_REQARG) {
        /* Option argument required, so return next argv element */
        if (is_argv_end(ps, argc, argv)) {
            ps->optopt = longopts[match].flag ? 0 : longopts[match].val;
            ps->nextchar = NULL;
            return optstring[0] == ':' ? ':' : '?';
        }

        ps->optarg = argv[ps->optind++];
    }

    ps->nextchar = NULL;

    if (longopts[match].flag != NULL) {
        *longopts[match].flag = longopts[match].val;
        return 0;
    }

    return longopts[match].val;
}
/* *INDENT-ON* */

/* Match nextchar against optstring */
int match_short(struct parg_state *ps, int argc, char *const argv[], const char *optstring) {
    const char *p = strchr(optstring, *ps->nextchar);

    if (p == NULL) {
        ps->optopt = *ps->nextchar++;
        return '?';
    }

    /* If no option argument, return option */
    if (p[1] != ':') {
        return *ps->nextchar++;
    }

    /* If more characters, return as option argument */
    if (ps->nextchar[1] != '\0') {
        ps->optarg = &ps->nextchar[1];
        ps->nextchar = NULL;
        return *p;
    }

    /* If option argument is optional, return option */
    if (p[2] == ':') {
        return *ps->nextchar++;
    }

    /* Option argument required, so return next argv element */
    if (is_argv_end(ps, argc, argv)) {
        ps->optopt = *ps->nextchar++;
        return optstring[0] == ':' ? ':' : '?';
    }

    ps->optarg = argv[ps->optind++];
    ps->nextchar = NULL;
    return *p;
}

/* Parse next long or short option in `argv`.
 * Check GNU getopt_long example for details:
 * https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
 */
/* *INDENT-OFF* */
int parg_getopt_long(struct parg_state *ps, int argc,
                char *const argv[], const char *optstring, 
                const struct parg_opt *longopts, int *longindex) {
    assert(ps != NULL);
    assert(argv != NULL);
    assert(optstring != NULL);

    ps->optarg = NULL;

    if (argc < 2) {
        return -1;
    }

    /* Advance to next element if needed */
    if (ps->nextchar == NULL || *ps->nextchar == '\0') {
        if (is_argv_end(ps, argc, argv)) {
            return -1;
        }

        ps->nextchar = argv[ps->optind++];

        /* Check for nonoption element (including '-') */
        if (ps->nextchar[0] != '-' || ps->nextchar[1] == '\0') {
            ps->optarg = ps->nextchar;
            ps->nextchar = NULL;
            return 1;
        }

        /* Check for '--' */
        if (ps->nextchar[1] == '-') {
            if (ps->nextchar[2] == '\0') {
                ps->nextchar = NULL;
                return -1;
            }

            if (longopts != NULL) {
                ps->nextchar += 2;
                return match_long(ps, argc, argv, optstring, longopts, longindex);
            }
        }

        ps->nextchar++;
    }

    /* Match nextchar */
    return match_short(ps, argc, argv, optstring);
}
/* *INDENT-ON* */

/**************** PARG SOURCE END ***************/

/*----------------------------------------------*/
/*                MACE SOURCE                   */
/*----------------------------------------------*/
/* Early return,
**  1. if cond fails (pseudo-assert)
**  2. with output or not
**      if ret is MACE_VOID     -> no value return
**  3. with assert or not
**      if ass is MACE_nASSERT  -> don't assert */
#define MACE_VOID
#define MACE_nASSERT(a) do {} while(0)
#define MACE_EARLY_RET(cond, ret, ass) do {\
        if (!(cond)) {\
            ass(0);\
            return ret;\
        }\
    } while(0)

/***************** MACE_ARENA *****************/
Mace_Arena mace_arena_init(size_t size) {
    Mace_Arena arena = {0};
    arena.mem = malloc(arena.size = size);
    if (arena.mem == NULL) {
        fprintf(stderr, "Could not malloc arena memory.\n");
        assert(0);
        exit(1);
    }
    return (arena);
}

void mace_arena_clear(Mace_Arena *arena) {
    MACE_EARLY_RET(arena, MACE_VOID, MACE_nASSERT);
    MACE_EARLY_RET(arena->mem, MACE_VOID, MACE_nASSERT);

    arena->fill = 0;
    memset(arena->mem, 0, arena->size);    
}

void mace_arena_free(Mace_Arena *arena) {
    MACE_EARLY_RET(arena, MACE_VOID, MACE_nASSERT);
    MACE_EARLY_RET(arena->mem, MACE_VOID, MACE_nASSERT);

    mace_arena_clear(arena);

#ifndef MACE_NO_HEAP
    free(arena->mem);
#else
    memset(arena->mem, 0, arena->size);
#endif /* MACE_NO_HEAP */
    arena->size = 0;
    arena->mem  = NULL;
}

void mace_pop(Mace_Arena *arn, size_t size) {
    size_t asize;
    MACE_EARLY_RET(arn, MACE_VOID, MACE_nASSERT);
    MACE_EARLY_RET(arn->mem, MACE_VOID, MACE_nASSERT);

    asize = MACE_ALIGN_SIZE(size);
    arn->fill -= arn->fill > asize ? asize : arn->fill; 
}

void *_mace_malloc(Mace_Arena *arena, size_t size) {
    /* Checking if out of memory */
    void *out;
    size_t align_size   = MACE_ALIGN_SIZE(size);
    size_t new_size     = align_size + arena->fill;
    assert(align_size > size);
    if (new_size > arena->size) {
        fprintf(stderr, "Out of memory. Allocate more with MACE_MEM.\n");
        printf("%ld %ld %ld.\n", size, arena->fill, arena->size);
        assert(0);
        exit(1);
    }
    out = arena->mem + arena->fill;
    arena->fill += align_size;
    assert((arena->fill % MACE_ARENA_ALIGN) == 0);
    return (out);
}

void *mace_malloc(size_t size) {
    return(_mace_malloc(&gl.arena, size));
}

void *_mace_calloc(Mace_Arena *arena, size_t num, size_t size) {
    void *ptr = _mace_malloc(arena, num * size);
    size_t align_size = MACE_ALIGN_SIZE((num * size));
    memset(ptr, 0, align_size);
    return (ptr);
}

void *mace_calloc(size_t num, size_t size) {
    return(_mace_calloc(&gl.arena, num, size));
}

void *_mace_realloc(Mace_Arena *arena, void *old,
                    size_t osize, size_t nsize) {
    void *new = _mace_calloc(arena, 1, nsize);
    memcpy(new, old, osize);
    return (new);
}

void *mace_realloc( void *old, size_t osize, 
                    size_t nsize) {
    return(_mace_realloc(&gl.arena, old, osize, nsize));
}

/***************** MACE_ADD_CONFIG *****************/
void mace_add_config(Config *config, const char *name) {
    size_t len;
    size_t bytesize;
    char *buffer;
    MACE_EARLY_RET(name,    MACE_VOID, assert);
    MACE_EARLY_RET(config,  MACE_VOID, assert);

    gl.configs[gl.config_num] = *config;

    len     = strlen(name);
    buffer  = mace_calloc(1, len + 1);
    memcpy(buffer, name, len);
    gl.configs[gl.config_num].pr._name   = buffer;
    gl.configs[gl.config_num].pr._hash   = mace_hash(name);
    gl.configs[gl.config_num].pr._order  = gl.target_num;

    if (++gl.config_num >= gl.config_len) {
        gl.config_len *= MACE_GROW;
        bytesize    = gl.config_len * sizeof(*gl.configs);
        gl.configs  = mace_realloc(gl.configs, bytesize / MACE_GROW, bytesize);
    }
    assert(gl.config_num < gl.config_len);
}

/**************** MACE_ADD_TARGET ***************/
void mace_add_target(Target *target, const char *name) {
    size_t len;
    size_t bytesize;
    char *buffer;
    MACE_EARLY_RET(name,    MACE_VOID, assert);
    MACE_EARLY_RET(target,  MACE_VOID, assert);

    gl.targets[gl.target_num] = *target;

    len     = strlen(name);
    buffer  = mace_calloc(1, len + 1);
    memcpy(buffer, name, len);
    gl.targets[gl.target_num].pr._name     = buffer;
    gl.targets[gl.target_num].pr._hash     = mace_hash(name);
    gl.targets[gl.target_num].pr._order    = gl.target_num;
    gl.targets[gl.target_num].pr._checkcwd = true;
    mace_Target_Deps_Hash(&gl.targets[gl.target_num]);
    mace_Target_Parse_User(&gl.targets[gl.target_num]);
    mace_Target_argv_compile(&gl.targets[gl.target_num]);
    if (++gl.target_num >= gl.target_len) {
        gl.target_len *= MACE_GROW;
        bytesize    = gl.target_len * sizeof(*gl.targets);
        gl.targets  = mace_realloc(gl.targets, bytesize, bytesize / MACE_GROW);
        memset(gl.targets + gl.target_len / MACE_GROW, 0, bytesize / MACE_GROW);
        bytesize    = gl.target_len * sizeof(*gl.build_order);
        gl.build_order = mace_realloc(gl.build_order, bytesize / MACE_GROW, bytesize);
    }
    assert(gl.target_num < gl.target_len);
}

/*  Set target built by default when */
/*         running mace without target */
void mace_set_default_target(const char *name) {
    MACE_EARLY_RET(name, MACE_VOID, assert);

    gl.mace_default_target_hash = mace_hash(name);
}

/*  Compute default target order from hash. */
/*         Called post-user.  */
void mace_default_target_order(void) {
    MACE_EARLY_RET(gl.mace_default_target_hash != 0ul, MACE_VOID, MACE_nASSERT);

    gl.mace_default_target = mace_target_order(gl.mace_default_target_hash);

    if (gl.mace_default_target < 0) {
        fprintf(stderr, "Default target not found. Exiting\n");
        exit(1);
    }
}

/*  Set config used to build targets by default */
void mace_set_default_config(const char *name) {
    MACE_EARLY_RET(name, MACE_VOID, assert);

    gl.mace_default_config_hash = mace_hash(name);
}

/*  Compute default target order from hash. */
/*         Called post-user. */
void mace_default_config_order(void) {
    MACE_EARLY_RET(gl.mace_default_config_hash != 0ul, MACE_VOID, MACE_nASSERT);
    gl.mace_default_config = mace_config_order(gl.mace_default_config_hash);

    if (gl.mace_default_config < 0) {
        fprintf(stderr, "Default config not found. Exiting\n");
        exit(1);
    }
}

/*  Find user config from its hash. */
void mace_user_config_set(u64 hash) {
    MACE_EARLY_RET(hash != 0ul, MACE_VOID, MACE_nASSERT);
    gl.mace_user_config = mace_config_order(hash);
    if (gl.mace_user_config < 0) {
        fprintf(stderr, "Warning: User config '%lu' not found\n", hash);
    }
}

/*  Decide if user target or default */
/*         target should be compiled. */
void mace_target_resolve(void) {
    /* Target priority: */
    /*  - user      target */
    /*  - default   target */

    if ((gl.mace_user_target > MACE_TARGET_NULL) &&
        (gl.mace_user_target < gl.target_num)) {
        /* Using user target */
        gl.mace_target = gl.mace_user_target;
        return;
    }

    /* Using default target (may be set by user) */
    gl.mace_target = gl.mace_default_target;
}

/*  Decide if user config or default */
/*         config should be used. */
void mace_config_resolve(const Target *target) {
    MACE_EARLY_RET(target, MACE_VOID, assert);
    /* Config priority: */
    /*  - user      config */
    /*  - target    config */
    /*  - default   config */

    if ((gl.mace_user_config >= 0) &&
        (gl.mace_user_config < gl.config_num)) {
        /* Using user config */
        gl.mace_config = gl.mace_user_config;
        return;
    }

    if ((target->pr._config >= 0) &&
        (target->pr._config < gl.config_num)) {
        /* Using target config */
        gl.mace_config = target->pr._config;
        return;
    }

    /* Using default config (may be set by user) */
    gl.mace_config = gl.mace_default_config;
}

/*  Set mace_user_target from input hash. */
void mace_user_target_set(u64 hash) {
    MACE_EARLY_RET(hash != 0ul, MACE_VOID, MACE_nASSERT);

    gl.mace_user_target = mace_target_order(hash);

    if (gl.mace_user_target < 0)
        fprintf(stderr, "Warning: User target '%lu' not found.\n", hash);
}

/*  Set default config for target. */
void mace_target_config(const char *target_name,
                        const char *config_name) {
    u64 target_hash;
    u64 config_hash;
    int config_order;
    int target_order;

    MACE_EARLY_RET(target_name, MACE_VOID, assert);
    MACE_EARLY_RET(config_name, MACE_VOID, assert);

    target_hash = mace_hash(target_name);
    config_hash = mace_hash(config_name);
    config_order = mace_config_order(config_hash);
    target_order = mace_target_order(target_hash);

    MACE_EARLY_RET(target_order >= 0, MACE_VOID, MACE_nASSERT);
    MACE_EARLY_RET(config_order >= 0, MACE_VOID, MACE_nASSERT);

    gl.targets[target_order].pr._config = config_order;
}

/************** hashing ***************/
u64 mace_hash(const char *str) {
    /* djb2 hashing algorithm by Dan Bernstein.
    * Description: This algorithm (k=33) was first reported by dan bernstein many
    * years ago in comp.lang.c. Another version of this algorithm (now favored by bernstein)
    * uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33
    * (why it works better than many other constants, prime or not) has never been adequately explained.
    * [1] https://stackoverflow.com/questions/7666509/hash-function-for-string
    * [2] http://www.cse.yorku.ca/~oz/hash.html */
    u64 hash;
    i32 str_char;
    MACE_EARLY_RET(str, 0ul, assert);
    hash = 5381ul;
    while ((str_char = *str++))
        hash = ((hash << 5ul) + hash) + str_char; /* hash * 33 + c */
    return (hash);
}

/***************** MACE_SETTERS *****************/

/*  Only place where cc_depflag is set. */
void mace_set_cc_depflag(const char *depflag) {
    size_t len;
    MACE_EARLY_RET(depflag, MACE_VOID, MACE_nASSERT);

    len = strlen(depflag);
    if (len >= MACE_DEPFLAG_BUFFER) {
        fprintf(stderr, "Depflag is too long, mace expects %d bytes max. \n", MACE_DEPFLAG_BUFFER);
        exit(1);
    }
    memcpy(glstr.cc_depflag, depflag, len);
}

/*  Only place where archiver ar is set. */
void mace_set_archiver(const char *archiver) {
    size_t len;
    MACE_EARLY_RET(archiver, MACE_VOID, MACE_nASSERT);

    len = strlen(archiver);
    if (len > MACE_CC_BUFFER) {
        fprintf(stderr, "Archiver is too long, mace expects %d bytes max. \n", MACE_CC_BUFFER);
        exit(1);
    }
    memcpy(glstr.ar, archiver, len);
}

/*  Only place where cflags is set. */
void mace_set_cflags(char *in_flags) {
    MACE_EARLY_RET(in_flags, MACE_VOID, MACE_nASSERT);

    gl.cflags = in_flags;
}

/*  Only place where compiler cc is set. */
void mace_set_compiler(const char *compiler) {
    size_t len;
    MACE_EARLY_RET(compiler, MACE_VOID, MACE_nASSERT);

    len = strlen(compiler);
    if (len > MACE_CC_BUFFER) {
        fprintf(stderr, "Compiler is too long, mace expects %d bytes max. \n", MACE_CC_BUFFER);
        exit(1);
    }
    memcpy(glstr.cc, compiler, len);

    if (strstr(glstr.cc, "gcc") != NULL) {
        mace_set_cc_depflag("-MM");
        mace_set_archiver("ar");
    } else if (strstr(glstr.cc, "tcc") != NULL) {
        mace_set_cc_depflag("-MD");
        mace_set_archiver("tcc -ar");
    } else if (strstr(glstr.cc, "clang") != NULL) {
        mace_set_cc_depflag("-MM");
        mace_set_archiver("llvm-ar");
    } else {
        fprintf(stderr, "unknown compiler '%s'. \n", compiler);
        exit(1);
    }
}

/******************* argv *******************/
/*  Build all argv flags from input user string.
**      - Split user_str using separator
**      - Copy each token into a new buffer,
**        put pointer in argv  */
char **mace_argv_flags(int         *len,   int *argc,
                       char       **argv,  const char *user_str,
                       const char  *flag,  b32 path,
                       const char  *separator) {
    size_t   i;
    size_t   flag_len;
    size_t   total_len;
    size_t   to_use_len;
    char    *arg;
    char    *token;
    char    *buffer;

    MACE_EARLY_RET(((len != NULL) && (*len) >= 0), NULL, MACE_nASSERT);
    MACE_EARLY_RET(argc != NULL, NULL, MACE_nASSERT);

    flag_len = (flag == NULL) ? 0 : strlen(flag);

    /* -- Copy user_str into modifiable buffer -- */
    buffer  = _mace_copy_str(&gl.stackrena, user_str);
    token   = strtok(buffer, MACE_SEPARATOR);
    while (token != NULL) {
        size_t rpathlen;
        char *to_use    = token;
        char *rpath     = NULL;
        argv = mace_argv_grow(argv, argc, len);

        if (path) {
            /* - Expand path - */
            rpath = _mace_calloc(&gl.stackrena, PATH_MAX, sizeof(*rpath));
            if (realpath(token, rpath) == NULL) {
                to_use = token;
            } else {
                rpathlen = strlen(rpath) + 1;
                rpath[rpathlen]     = '\0';
                to_use              = rpath;
            }
        }

        to_use_len   = strlen(to_use);
        total_len    = (to_use_len + flag_len + 1);
        i            = 0;
        arg = mace_calloc(total_len, sizeof(*arg));

        /* - Copy flag into arg - */
        if ((flag_len > 0) && (flag != NULL)) {
            memcpy(arg, flag, flag_len);
            i += flag_len;
        }

        /* - Copy token into arg - */
        memcpy(arg + i, to_use, to_use_len);
        argv[(*argc)++] = arg;
        /* pop rpath */
        mace_pop(&gl.stackrena, PATH_MAX * sizeof(*rpath));

        token = strtok(NULL, MACE_SEPARATOR);
    }
    mace_arena_clear(&gl.stackrena);
    return (argv);
}

/*  Parse user input target->excludes string.  */
/*      - Split excludes using mace_separator */
/*      - Hash file into _excludes list */
void mace_Target_excludes(Target *target) {
    char *buffer;
    char *token;
    size_t bytesize;

    MACE_EARLY_RET(target->excludes != NULL, MACE_VOID, MACE_nASSERT);

    target->pr._excludes_num = 0;
    target->pr._excludes_len = 8;
    target->pr._excludes = mace_calloc(target->pr._excludes_len,
                                    sizeof(*target->pr._excludes));
    /* -- Copy user_str into modifiable buffer -- */
    bytesize = strlen(target->excludes) + 1;
    buffer = _mace_calloc(&gl.stackrena, 1, bytesize);
    memcpy(buffer, target->excludes, bytesize - 1);

    /* --- Split sources into tokens --- */
    token = strtok(buffer, MACE_SEPARATOR);
    do {
        char    *arg;
        char    *rpath;
        size_t   rpathlen;
        size_t   token_len  = strlen(token);
        arg = _mace_calloc(&gl.stackrena, (token_len + 1), sizeof(*arg));
        memcpy(arg, token, token_len);

        rpath = _mace_calloc(&gl.stackrena, PATH_MAX, sizeof(*rpath));
        if (realpath(token, rpath) == NULL) {
            if (!gl.silent)
                printf("Warning! excluded source '%s' does not exist\n", arg);
        }
        rpathlen = (strlen(rpath) + 1) * sizeof(*rpath);
        rpath[rpathlen]    = '\0';

        if (mace_isDir(rpath)) {
            fprintf(stderr, "dir '%s' in excludes: files only!\n", rpath);
        } else {
            target->pr._excludes[target->pr._excludes_num++] = mace_hash(rpath);
        }
        
        mace_arena_clear(&gl.stackrena);
        token = strtok(NULL, MACE_SEPARATOR);
    } while (token != NULL);

    mace_arena_clear(&gl.stackrena);
}

/*  Makes flags for target includes,  */
/*         links libraries, and flags */
/*      - NOT sources: they can be folders,  */
/*        so need to be globbed */
void mace_Target_Parse_User(Target *target) {
    int len;

    /* -- Make _argv_includes to argv -- */
    if (target->includes != NULL) {
        len = 8;
        target->pr._argc_includes = 0;
        target->pr._argv_includes = mace_calloc(len, sizeof(*target->pr._argv_includes));
        target->pr._argv_includes = mace_argv_flags(&len,
                                                         &target->pr._argc_includes,
                                                         target->pr._argv_includes,
                                                         target->includes,
                                                         "-I",
                                                         true,
                                                         MACE_SEPARATOR);
    }

    /* -- Make _argv_linker_flags to argv -- */
    if (target->link_flags != NULL) {
        len = 8;
        target->pr._argc_link_flags = 0;
        target->pr._argv_link_flags = mace_calloc(len, sizeof(*target->pr._argv_link_flags));
        target->pr._argv_link_flags = mace_argv_flags(&len,
                                                           &target->pr._argc_link_flags,
                                                           target->pr._argv_link_flags,
                                                           target->link_flags,
                                                           "-Wl,",
                                                           true,
                                                           MACE_SEPARATOR);
    }

    /* -- Make _argv_links to argv -- */
    if (target->links != NULL) {
        len = 8;
        target->pr._argc_links = 0;
        target->pr._argv_links = mace_calloc(len, sizeof(*target->pr._argv_links));
        target->pr._argv_links = mace_argv_flags(&len,
                                                      &target->pr._argc_links,
                                                      target->pr._argv_links,
                                                      target->links,
                                                      "-l",
                                                      false,
                                                      MACE_SEPARATOR);
    }

    /* -- Make _argv_flags to argv -- */
    if (target->flags != NULL) {
        len = 8;
        target->pr._argc_flags = 0;
        target->pr._argv_flags = mace_calloc(len, sizeof(*target->pr._argv_flags));
        target->pr._argv_flags = mace_argv_flags(&len,
                                                      &target->pr._argc_flags,
                                                      target->pr._argv_flags,
                                                      target->flags,
                                                      NULL,
                                                      false,
                                                      MACE_SEPARATOR);
    }

    /* -- Exclusions -- */
    mace_Target_excludes(target);
}

/*  Realloc target->argv to bigger */
/*         if num close to len */
void mace_Target_argv_grow(Target *target) {
    target->pr._argv = mace_argv_grow(
        target->pr._argv,
        &target->pr._argc,
        &target->pr._arg_len
    );
}

/*  Alloc target sources stuff.  */
/*         Realloc to bigger if num close to len. */
void mace_Target_sources_grow(Target *target) {
    int     new_len;
    int     previous_len;
    size_t  bytesize;

    /* -- Alloc sources -- */
    if (target->pr._argv_sources == NULL) {
        target->pr._len_sources  = 8;
        target->pr._argv_sources = mace_calloc(target->pr._len_sources,
                                               sizeof(*target->pr._argv_sources));
    }

    /* -- Alloc deps_headers -- */
    if (target->pr._deps_headers == NULL) {
        target->pr._deps_headers  = mace_calloc(target->pr._len_sources,
                                                sizeof(*target->pr._deps_headers));
    }
    if (target->pr._deps_headers_num == NULL) {
        target->pr._deps_headers_num  = mace_calloc(target->pr._len_sources,
                                                    sizeof(*target->pr._deps_headers_num));
    }
    if (target->pr._deps_headers_len == NULL) {
        target->pr._deps_headers_len  = mace_calloc(target->pr._len_sources,
                                                    sizeof(*target->pr._deps_headers_len));
    }

    /* -- Alloc recompiles -- */
    if (target->pr._recompiles == NULL) {
        bytesize = target->pr._len_sources * sizeof(*target->pr._recompiles);
        target->pr._recompiles = mace_calloc(1, bytesize);
    }

    /* -- Alloc objects -- */
    if (target->pr._argv_objects == NULL) {
        bytesize = sizeof(*target->pr._argv_objects);
        target->pr._argv_objects       = mace_calloc(target->pr._len_sources, bytesize);
    }
    if (target->pr._argv_objects_cnt == NULL) {
        bytesize = sizeof(*target->pr._argv_objects_cnt);
        target->pr._argv_objects_cnt   = mace_calloc(target->pr._len_sources, bytesize);
    }
    if (target->pr._argv_objects_hash == NULL) {
        bytesize = sizeof(*target->pr._argv_objects_hash);
        target->pr._argv_objects_hash  = mace_calloc(target->pr._len_sources, bytesize);
    }
    /* -- Realloc sources -- */
    previous_len  = target->pr._len_sources;
    target->pr._argv_sources = mace_argv_grow(target->pr._argv_sources,
                                                   &target->pr._argc_sources,
                                                   &target->pr._len_sources);
    new_len  = target->pr._len_sources;

    /* -- Alloc object dependencies -- */
    if (previous_len != new_len) {
        /* -- Realloc recompiles -- */
        bytesize = target->pr._len_sources * sizeof(*target->pr._recompiles);
        target->pr._recompiles = mace_realloc(target->pr._recompiles, bytesize / MACE_GROW, bytesize);
        memset(target->pr._recompiles + target->pr._len_sources / MACE_GROW, 0, bytesize / MACE_GROW);

        /* -- Realloc objects -- */
        bytesize = target->pr._len_sources * sizeof(*target->pr._argv_objects);
        target->pr._argv_objects = mace_realloc(target->pr._argv_objects, bytesize / MACE_GROW, bytesize);
        memset(target->pr._argv_objects + target->pr._len_sources / MACE_GROW, 0, bytesize / MACE_GROW);
    }

    /* -- Realloc deps_headers -- */
    if (previous_len != new_len) {
        size_t bytesize = target->pr._len_sources * sizeof(*target->pr._deps_headers);
        target->pr._deps_headers = mace_realloc(target->pr._deps_headers, bytesize / MACE_GROW, bytesize);
        memset(target->pr._deps_headers + target->pr._len_sources / MACE_GROW, 0, bytesize / MACE_GROW);

        bytesize = target->pr._len_sources * sizeof(*target->pr._deps_headers_num);
        target->pr._deps_headers_num = mace_realloc(target->pr._deps_headers_num, bytesize / MACE_GROW, bytesize);
        memset(target->pr._deps_headers_num + target->pr._len_sources / MACE_GROW, 0, bytesize / MACE_GROW);

        bytesize = target->pr._len_sources * sizeof(*target->pr._deps_headers_len);
        target->pr._deps_headers_len = mace_realloc(target->pr._deps_headers_len, bytesize / MACE_GROW, bytesize);
        memset(target->pr._deps_headers_num + target->pr._len_sources / MACE_GROW, 0, bytesize / MACE_GROW);
    }

    /* -- Realloc objects -- */
    if (target->pr._len_sources >= target->pr._argc_objects_hash) {
        bytesize = target->pr._len_sources * sizeof(*target->pr._argv_objects_hash);
        target->pr._argv_objects_hash = mace_realloc(target->pr._argv_objects_hash, bytesize / MACE_GROW, bytesize);
        bytesize = target->pr._len_sources * sizeof(*target->pr._argv_objects_cnt);
        target->pr._argv_objects_cnt = mace_realloc(target->pr._argv_objects_cnt, bytesize / MACE_GROW, bytesize);
    }
}

/*  Realloc argv if argc close to arg_len. */
char **mace_argv_grow(char **argv, const int *argc,
                      int *arg_len) {
    if ((*argc + 1) >= *arg_len) {
        size_t new_len  = (*arg_len) * MACE_GROW;
        size_t bytesize = new_len * sizeof(*argv);
        argv = mace_realloc(argv, bytesize / MACE_GROW, bytesize);
        memset(argv + (*arg_len), 0, bytesize / MACE_GROW);
        (*arg_len) = new_len;
    }
    return (argv);
}

/*  Create argv, argc for compiling objects */
/*         all at once. Should be called after all */
/*         sources have been added. */
void mace_Target_argv_allatonce(Target *target) {
    int      i;
    char    *ldirflag;
    char    *compflag;
    size_t   build_dir_len;

    if (target->pr._argv == NULL) {
        target->pr._arg_len = 8;
        target->pr._argc = 0;
        target->pr._argv = mace_calloc(target->pr._arg_len, sizeof(*target->pr._argv));
    }
    target->pr._argv[MACE_ARGV_CC] = glstr.cc;
    target->pr._argc = MACE_ARGV_CC + 1;

    /* -- argv sources -- */
    if ((target->pr._argc_sources > 0) && (target->pr._argv_sources != NULL)) {
        for (i = 0; i < target->pr._argc_sources; i++) {
            mace_Target_argv_grow(target);
            target->pr._argv[target->pr._argc++] = target->pr._argv_sources[i];
        }
    }

    /* -- argv includes -- */
    if ((target->pr._argc_includes > 0) && (target->pr._argv_includes != NULL)) {
        for (i = 0; i < target->pr._argc_includes; i++) {
            mace_Target_argv_grow(target);
            target->pr._argv[target->pr._argc++] = target->pr._argv_includes[i];
        }
    }

    /* -- argv -L flag for build_dir -- */
    target->pr._argc_tail = target->pr._argc;
    mace_Target_argv_grow(target);
    MACE_EARLY_RET(MACE_BUILD_DIR != NULL, MACE_VOID, assert);
    build_dir_len = strlen(MACE_BUILD_DIR);
    ldirflag = mace_calloc(3 + build_dir_len, sizeof(*ldirflag));
    memcpy(ldirflag, "-L", 2);
    memcpy(ldirflag + 2, MACE_BUILD_DIR, build_dir_len);
    target->pr._argv[target->pr._argc++] = ldirflag;

    /* -- argv -c flag for libraries -- */
    mace_Target_argv_grow(target);
    compflag = mace_calloc(3, sizeof(*compflag));
    memcpy(compflag, "-c", 2);
    target->pr._argv[target->pr._argc++] = compflag;

    /* -- add config -- */
    mace_argv_add_config(   &target->pr._argv,
                            &target->pr._argc,
                            &target->pr._arg_len);
    mace_argv_add_cflags(   &target->pr._argv,
                            &target->pr._argc,
                            &target->pr._arg_len);
    target->pr._argv[target->pr._argc] = NULL;
}

/*  Create argv, argc for compiling objects */
/*         one at a time. Should be called after */
/*         mace_Target_Parse_User */
void mace_Target_argv_compile(Target *target) {
    int      i;
    char    *compflag;

    if (target->pr._argv == NULL) {
        target->pr._arg_len = 8;
        target->pr._argc = 0;
        target->pr._argv = mace_calloc(target->pr._arg_len, sizeof(*target->pr._argv));
    }

    /* --- Adding argvs common to all --- */
    target->pr._argc = MACE_ARGV_OTHER;
    /* -- argv user flags -- */
    if ((target->pr._argc_flags > 0) && (target->pr._argv_flags != NULL)) {
        for (i = 0; i < target->pr._argc_flags; i++) {
            mace_Target_argv_grow(target);
            target->pr._argv[target->pr._argc++] = target->pr._argv_flags[i];
        }
    }

    /* -- argv link_flags -- */
    if ((target->pr._argc_link_flags > 0) && (target->pr._argv_link_flags != NULL)) {
        for (i = 0; i < target->pr._argc_link_flags; i++) {
            mace_Target_argv_grow(target);
            target->pr._argv[target->pr._argc++] = target->pr._argv_link_flags[i];
        }
    }

    /* -- argv includes -- */
    if ((target->pr._argc_includes > 0) && (target->pr._argv_includes != NULL)) {
        for (i = 0; i < target->pr._argc_includes; i++) {
            mace_Target_argv_grow(target);
            target->pr._argv[target->pr._argc++] = target->pr._argv_includes[i];
        }
    }

    /* -- argv -c flag for objects -- */
    target->pr._argc_tail =    target->pr._argc;
    mace_Target_argv_grow(target);
    compflag = mace_calloc(3, sizeof(*compflag));
    memcpy(compflag, "-c", 2);
    target->pr._argv[target->pr._argc++] = compflag;

    /* -- argv -fPIC flag for objects -- */
    if (target->kind == MACE_DYNAMIC_LIBRARY) {
        char *fPICflag = mace_calloc(6, sizeof(*compflag));
        mace_Target_argv_grow(target);
        memcpy(fPICflag, "-fPIC", 5);
        target->pr._argv[target->pr._argc++] = fPICflag;
    }

    target->pr._argv[target->pr._argc] = NULL;
}

/*  Add config as flags to argv for compilation. */
void mace_argv_add_config(char ** *argv,
                          int *argc, int *arg_len) {
    int i;

    if (gl.config_num <= 0) {
        return;
    }

    for (i = 0; i < gl.configs[gl.mace_config].pr._flag_num; ++i) {
        size_t len  = strlen(gl.configs[gl.mace_config].pr._flags[i]) + 1;
        char *flag  = mace_calloc(len, sizeof(*flag));
        *argv       = mace_argv_grow(*argv, argc, arg_len);
        memcpy(flag, gl.configs[gl.mace_config].pr._flags[i],  len);
        (*argv)[(*argc)++] = flag;
    }
}

void mace_argv_add_cflags(char ** *argv,
                          int *argc, int *arg_len) {
    int i;

    if ((gl.cflags_sep == NULL) || (gl.cflags_num == 0))
        return;

    for (i = 0; i < gl.cflags_num; ++i) {
        size_t len  = strlen(gl.cflags_sep[i]) + 1;
        char *cflag = mace_calloc(len, sizeof(*cflag));
        *argv       = mace_argv_grow(*argv, argc, arg_len);
        memcpy(cflag, gl.cflags_sep[i],  len);
        (*argv)[(*argc)++] = cflag;
    }
}

/***************** mace_pqueue ******************/

pid_t mace_pqueue_pop(void) {
    MACE_EARLY_RET(gl.pnum > 0, 0, assert);

    return (gl.pqueue[--gl.pnum]);
}

void mace_pqueue_put(pid_t pid) {
    size_t bytes;
    if (gl.pnum >= MACE_JOBS) {
        mace_wait_pid(pid);
    }
    bytes = (MACE_JOBS - 1) * sizeof(*gl.pqueue);
    memmove(gl.pqueue + 1, gl.pqueue, bytes);
    gl.pqueue[0] = pid;
    gl.pnum++;
}

/***************** mace_glob_sources ****************/
/*  If source is a folder, get all .c files in it. */
glob_t mace_glob_sources(const char *path) {
    glob_t  globbed;
    int     flags   = 0;
    int     ret     = glob(path, flags, mace_globerr, &globbed);

    if (ret != 0) {
        fprintf(stderr, "problem with %s (%s), quitting\n", 
                path,
                (ret == GLOB_ABORTED ? "filesystem problem" :
                 ret == GLOB_NOMATCH ? "no match of pattern" :
                 ret == GLOB_NOSPACE ? "no dynamic memory" :
                 "unknown problem\n"));
        exit(ret);
    }
    return (globbed);
}

int mace_globerr(const char *path, int eerrno) {
    fprintf(stderr, "%s: %s\n", path, strerror(eerrno));
    return (eerrno);
}

/***************** mace_exec ******************/
/*  Print command to be run in forked process. */
void mace_argv_print(char *const argv[],
                     size_t argc) {
    int i;

    for (i = 0; i < argc; i++) {
        if (gl.verbose && !gl.silent)
            printf("%s ", argv[i]);
    }
    if (gl.verbose && !gl.silent)
        printf("%s", "\n");
}

/*  Execute command in a different fork */
/*         with execvp. */
pid_t mace_exec(const char *exec,
                char *const arguments[]) {
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "forking issue.\n");
        assert(0);
        exit(1);
    } else if (pid == 0) {
        execvp(exec, arguments);
        exit(0);
    }
    return (pid);
}

/*  Wait on process with pid to finish */
void mace_wait_pid(int pid) {
    int status;

    if (waitpid(pid, &status, 0) > 0) {
        if (WEXITSTATUS(status) == 0) {
            /* pass */
        } else if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            /* pass */
        } else if (WIFEXITED(status) &&  WEXITSTATUS(status)) {
            if (WEXITSTATUS(status) == 127) {
                /* execvp failed */
                fprintf(stderr, "execvp failed.\n");
                exit(WEXITSTATUS(status));
            } else {
                fprintf(stderr, "Fork returned a non-zero status.\n");
                exit(WEXITSTATUS(status));
            }
        } else {
            fprintf(stderr, "is baka? %d\n", WEXITSTATUS(status));
            fprintf(stderr, "Fork didn't terminate normally. %d\n", WEXITSTATUS(status));
            exit(WEXITSTATUS(status));
        }
    }
}

/******************* mace_build ********************/
void mace_link_dynamic_library(Target *target) {
    char    *libv;
    char    *lib;
    size_t   lib_len;
    size_t   oflag_len;

    char      fPICflag[8]    = "-fPIC";
    char      sharedflag[8]  = "-shared";
    int       argc_objects  = target->pr._argc_sources;
    int       arg_len       = 8;
    int       argc          = 0;
    char    **argv  = _mace_calloc(&gl.stackrena, arg_len,
                                    sizeof(*argv));
    char    **argv_objects = target->pr._argv_objects;

    /* stackrena */
    lib = mace_library_path(&gl.stackrena,
                            target->pr._name,
                            MACE_DYNAMIC_LIBRARY);

    if (!gl.silent)
        printf("Linking  %s\n", lib);

    argv[argc++] = glstr.cc;

    /* --- Adding target --- */
    oflag_len   = 2;
    lib_len     = strlen(lib);
    libv        = _mace_calloc(&gl.stackrena,
                    lib_len + oflag_len + 1, 
                    sizeof(*libv));
    memcpy(libv, "-o", oflag_len);
    memcpy(libv + oflag_len, lib, lib_len);
    argv[argc++] = libv;

    /* --- Adding -fPIC flag --- */
    argv[argc++]    = fPICflag;

    /* --- Adding -shared flag --- */
    argv[argc++]    = sharedflag;

    /* --- Adding objects --- */
    if ((argc_objects > 0) && (argv_objects != NULL)) {
        int i;
        for (i = 0; i < argc_objects; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = argv_objects[i] + strlen("-o");
        }
    }

    /* -- argv links -- */
    if ((target->pr._argc_links > 0) && (target->pr._argv_links != NULL)) {
        int i;
        for (i = 0; i < target->pr._argc_links; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = target->pr._argv_links[i];
        }
    }

    /* -- argv link_flags -- */
    if ((target->pr._argc_link_flags > 0) && (target->pr._argv_link_flags != NULL)) {
        int i;
        for (i = 0; i < target->pr._argc_link_flags; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = target->pr._argv_link_flags[i];
        }
    }

    /* -- argv flags -- */
    if ((target->pr._argc_flags > 0) && (target->pr._argv_flags != NULL)) {
        int i;
        for (i = 0; i < target->pr._argc_flags; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = target->pr._argv_flags[i];
        }
    }

    /* --- argv config --- */
    mace_argv_add_config(&argv, &argc, &arg_len);
    mace_argv_add_cflags(&argv, &argc, &arg_len);

    /* --- Actual linking --- */
    mace_argv_print(argv, argc);
    if (!gl.dry_run) {
        pid_t pid = mace_exec(argv[0], argv);
        mace_wait_pid(pid);
    }

    mace_arena_clear(&gl.stackrena);
}

void mace_link_static_library(Target *target) {
    int      i;
    char    *libv;
    char    *buffer;
    char    *token;
    char    *lib;
    size_t   lib_len;

    char     rcsflag[5]     = "-rcs";
    int       argc          = 0;
    int       arg_len       = 8;
    int       argc_objects  = target->pr._argc_sources;
    char    **argv_objects  = target->pr._argv_objects;
    char    **argv          = _mace_calloc(&gl.stackrena, arg_len, sizeof(*argv));

    /* stackrena */
    lib = mace_library_path(&gl.stackrena,
                            target->pr._name,
                            MACE_STATIC_LIBRARY);

    if (!gl.silent)
        printf("Linking  %s\n", lib);

    /* --- Add ar --- */

    /* -- Split ar into tokens -- */
    /* Note: because tcc -ar is not a standalone executable but a flag 'tcc -ar' */
    buffer = _mace_calloc(  &gl.stackrena,
                            (strlen(glstr.ar) + 1),
                            sizeof(*glstr.ar));
    memcpy(buffer, glstr.ar, strlen(glstr.ar));
    token = strtok(buffer, MACE_SEPARATOR);
    do {
        char *flag = mace_calloc(strlen(token) + 1,
                                sizeof(*glstr.ar));
        memcpy(flag, token, strlen(token));
        argv[argc++] = flag;
        token = strtok(NULL, MACE_SEPARATOR);
    } while (token != NULL);

    /* --- Adding -rcs flag --- */
    argv[argc++] = rcsflag;

    /* --- Adding target --- */
    lib_len = strlen(lib);
    libv  = _mace_calloc(   &gl.stackrena, (lib_len + 1),
                            sizeof(*libv));
    memcpy(libv, lib, lib_len);
    argv[argc++] = libv;

    /* --- Adding objects --- */
    if ((argc_objects > 0) && (argv_objects != NULL)) {
        for (i = 0; i < argc_objects; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = argv_objects[i] + strlen("-o");
        }
    }

    /* -- argv links -- */
    if ((target->pr._argc_links > 0) && (target->pr._argv_links != NULL)) {
        for (i = 0; i < target->pr._argc_links; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = target->pr._argv_links[i];
        }
    }

    /* --- Actual linking --- */
    mace_argv_print(argv, argc);
    if (!gl.dry_run) {
        pid_t pid = mace_exec(argv[0], argv);
        mace_wait_pid(pid);
    }
    mace_arena_clear(&gl.stackrena);
}

void mace_link_executable(Target *target) {
    int      i;
    char    *oflag;
    char    *ldirflag;
    size_t   build_dir_len;
    size_t   exec_len;

    int    argc         = 0;
    int    arg_len      = 16;
    int    argc_links   = target->pr._argc_links;
    int    argc_flags   = target->pr._argc_flags;
    int    argc_objects = target->pr._argc_sources;
    char  *exec         = mace_executable_path(&gl.stackrena,
                                        target->pr._name);
    char **argv         = _mace_calloc(&gl.stackrena, arg_len,
                                        sizeof(*argv));
    char **argv_links   = target->pr._argv_links;
    char **argv_flags   = target->pr._argv_flags;
    char **argv_objects = target->pr._argv_objects;

    if (!gl.silent)
        printf("Linking  %s\n", exec);
    argv[argc++] = glstr.cc;

    /* --- Adding executable output --- */
    exec_len    = strlen(exec);
    oflag       = _mace_calloc(&gl.stackrena, (exec_len + 3),
                                sizeof(*oflag));
    memcpy(oflag, "-o", 2);
    memcpy(oflag + 2, exec, exec_len);
    argv[argc++] = oflag;

    /* --- Adding objects --- */
    if ((argc_objects > 0) && (argv_objects != NULL)) {
        for (i = 0; i < argc_objects; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = argv_objects[i] + strlen("-o");
        }
    }

    /* -- argv user flags -- */
    if ((argc_flags > 0) && (argv_flags != NULL)) {
        for (i = 0; i < argc_flags; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = argv_flags[i];
        }
    }

    /* -- argv links -- */
    if ((argc_links > 0) && (argv_links != NULL)) {
        for (i = 0; i < argc_links; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = argv_links[i];
        }
    }

    /* -- argv link_flags -- */
    if ((target->pr._argc_link_flags > 0) &&
        (target->pr._argv_link_flags != NULL)) {
        for (i = 0; i < target->pr._argc_link_flags; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = target->pr._argv_link_flags[i];
        }
    }

    /* -- argv config -- */
    argv = mace_argv_grow(argv, &argc, &arg_len);
    mace_argv_add_config(&argv, &argc, &arg_len);
    mace_argv_add_cflags(&argv, &argc, &arg_len);

    /* -- argv -L flag for build_dir -- */
    argv = mace_argv_grow(argv, &argc, &arg_len);
    build_dir_len = strlen(MACE_BUILD_DIR);

    ldirflag = _mace_calloc(&gl.stackrena, (3 + build_dir_len), sizeof(*ldirflag));
    memcpy(ldirflag,     "-L",      2);
    memcpy(ldirflag + 2, MACE_BUILD_DIR, build_dir_len);
    argv[argc++] = ldirflag;

    /* --- Actual linking  --- */
    mace_argv_print(argv, argc);
    if (!gl.dry_run) {
        pid_t pid = mace_exec(argv[0], argv);
        mace_wait_pid(pid);
    }

    mace_arena_clear(&gl.stackrena);
}

/*  Compile target's obj file all at once. */
void mace_Target_compile_allatonce(Target *target) {
    /* Compile ALL objects at once */
    /* -- Move to obj_dir -- */
    mace_chdir(MACE_OBJ_DIR);

    /* -- Prepare argv -- */
    mace_Target_argv_allatonce(target);

    /* -- Actual compilation -- */
    mace_argv_print(target->pr._argv,
                    target->pr._argc);
    if (!gl.dry_run) {
        pid_t pid = mace_exec(  target->pr._argv[0],
                                target->pr._argv);
        mace_wait_pid(pid);
    }

    /* -- Go back to cwd -- */
    mace_chdir(gl.cwd);
}

/*  Target pre-compilation: check which file */
/*         needs to be recompiled */
void mace_Target_precompile(Target *target) {
    int argc = 0;

    /* Compute latest object dependencies .d file */
    MACE_EARLY_RET(target, MACE_VOID, assert);
    MACE_EARLY_RET(target->pr._argv, MACE_VOID, assert);

    target->pr._argv[MACE_ARGV_CC]     = glstr.cc;
    mace_Target_argv_grow(target);
    target->pr._argv[target->pr._argc++]  = glstr.cc_depflag;
    target->pr._argv[target->pr._argc]    = NULL;

    /* - Single source argv - */
    while (true) {
        /* - Skip if no recompiles - */
        if ((argc < target->pr._argc_sources) &&
            (!target->pr._recompiles[argc])) {
            argc++;
            continue;
        }
        /* - Add process to queue - */
        if (argc < target->pr._argc_sources) {
            pid_t pid;
            size_t len;

            if (gl.verbose)
                printf("Pre-Compile %s\n", target->pr._argv_sources[argc]);
            target->pr._argv[MACE_ARGV_SOURCE] = target->pr._argv_sources[argc];
            target->pr._argv[MACE_ARGV_OBJECT] = target->pr._argv_objects[argc];
            len = strlen(target->pr._argv[MACE_ARGV_OBJECT]);
            target->pr._argv[MACE_ARGV_OBJECT][len - 1] = 'd';

            argc++;

            /* -- Actual pre-compilation -- */
            mace_argv_print(target->pr._argv, target->pr._argc);
            assert(target->pr._argv[target->pr._argc] == NULL);
            pid = mace_exec(target->pr._argv[0], target->pr._argv);
            mace_pqueue_put(pid);

            target->pr._argv[MACE_ARGV_OBJECT][len - 1] = 'o';
        }

        /* Prioritize adding process to queue */
        if ((argc < target->pr._argc_sources) &&
            (gl.pnum < MACE_JOBS)) {
            continue;
        }

        /* Wait for process */
        if (gl.pnum > 0) {
            pid_t wait = mace_pqueue_pop();
            if (wait > 0) {
                mace_wait_pid(wait);
            }
        }

        /* Check if more to compile */
        if ((gl.pnum <= 0) &&
            (argc >= target->pr._argc_sources))
            break;
    }
    target->pr._argv[--target->pr._argc] = NULL;

    /* -- Object dependencies (headers) -- */
    /* - Read .d file and hashes the filenames, write all headers to .ho files. - */
    mace_Target_Parse_Objdeps(target);

    /* - Compute checksums of all headers - */
    mace_Headers_Checksums(target);

    /* - Check if any source's header changed - */
    mace_Headers_Checksums_Checks(target);
}

/*  Compile targets' objects one at a time */
void mace_Target_compile(Target *target) {
    int argc = 0;

    MACE_EARLY_RET(target, MACE_VOID, assert);
    MACE_EARLY_RET(target->pr._argv, MACE_VOID, assert);

    target->pr._argv[MACE_ARGV_CC] = glstr.cc;

    /* - Single source argv - */
    while (true) {
        /* - Skip if no recompiles - */
        if ((argc < target->pr._argc_sources) &&
            (!target->pr._recompiles[argc])) {
            argc++;
            continue;
        }

        /* - Add process to queue - */
        if (argc < target->pr._argc_sources) {
            if (!gl.silent)
                printf("Compiling %s\n", target->pr._argv_sources[argc]);
            target->pr._argv[MACE_ARGV_SOURCE] = target->pr._argv_sources[argc];
            target->pr._argv[MACE_ARGV_OBJECT] = target->pr._argv_objects[argc];
            argc++;

            /* -- Actual compilation -- */
            mace_argv_print(target->pr._argv, target->pr._argc);
            if (!gl.dry_run) {
                pid_t pid = mace_exec(target->pr._argv[0], target->pr._argv);
                mace_pqueue_put(pid);
            }
        }

        /* Prioritize adding process to queue */
        if ((argc < target->pr._argc_sources) &&
            (gl.pnum < MACE_JOBS))
            continue;

        /* Wait for process */
        if (gl.pnum > 0) {
            pid_t wait = mace_pqueue_pop();
            mace_wait_pid(wait);
        }

        /* Check if more to compile */
        if ((gl.pnum <= 0) &&
            (argc >= target->pr._argc_sources))
            break;
    }
}

/*  Alloc _objects_hash_nocoll.  */
/*         Realloc if num close to len. Add hash. */
void Target_Object_Hash_Add_nocoll(Target *target,
                                   u64 hash) {
    size_t bytesize;
    if (target->pr._objects_hash_nocoll == NULL) {
        target->pr._objects_hash_nocoll_num = 0;
        target->pr._objects_hash_nocoll_len = 8;
        bytesize = target->pr._objects_hash_nocoll_len * sizeof(*target->pr._objects_hash_nocoll);
        target->pr._objects_hash_nocoll = mace_calloc(1, bytesize);
    }
    if (target->pr._objects_hash_nocoll_num >= (target->pr._objects_hash_nocoll_len - 1)) {
        target->pr._objects_hash_nocoll_len *= MACE_GROW;
        bytesize = target->pr._objects_hash_nocoll_len * sizeof(*target->pr._objects_hash_nocoll);
        target->pr._objects_hash_nocoll = mace_realloc(target->pr._objects_hash_nocoll, bytesize / MACE_GROW, bytesize);
        memset(target->pr._objects_hash_nocoll + target->pr._objects_hash_nocoll_len / MACE_GROW, 0,
               bytesize / MACE_GROW);
    }

    target->pr._objects_hash_nocoll[target->pr._objects_hash_nocoll_num++] = hash;
}

/*  Check if hash is in _objects_hash_nocoll. */
int Target_hasObjectHash_nocoll(const Target *target,
                                u64 hash) {
    int i;
    MACE_EARLY_RET(target->pr._objects_hash_nocoll, -1, MACE_nASSERT);

    for (i = 0; i < target->pr._objects_hash_nocoll_num; i++) {
        if (hash == target->pr._objects_hash_nocoll[i])
            return (i);
    }

    return (-1);
}

/*  Add object hash to target. */
void Target_Object_Hash_Add(Target *target, u64 hash) {
    MACE_EARLY_RET(target, MACE_VOID, assert);
    MACE_EARLY_RET(target->pr._argv_objects_hash, MACE_VOID, assert);
    MACE_EARLY_RET(target->pr._argv_objects_cnt, MACE_VOID, assert);

    target->pr._argv_objects_hash[target->pr._argc_objects_hash] = hash;
    target->pr._argv_objects_cnt[target->pr._argc_objects_hash++] = 0;
}

/*  Check if target has object hash. */
int Target_hasObjectHash(const Target *target, u64 hash) {
    int i;
    MACE_EARLY_RET(target->pr._argv_objects_hash, -1, MACE_nASSERT);

    for (i = 0; i < target->pr._argc_objects_hash; i++) {
        if (hash == target->pr._argv_objects_hash[i])
            return (i);
    }

    return (-1);
}

/*  Add target to list of recompiles. */
void mace_Target_Recompiles_Add(Target *target, b32 add) {
    target->pr._recompiles[target->pr._argc_sources - 1] = add;
}

/*  Add object needing to be compiled to target. */
b32 mace_Target_Object_Add(Target *target, char *token) {
    u64      hash;
    int      hash_id;
    u64      hash_nocoll;
    char    *arg;
    char    *flag;
    size_t   flag_len;
    size_t   total_len;
    size_t   token_len;

    /* token is object path */
    MACE_EARLY_RET(token, false, MACE_nASSERT);

    hash = mace_hash(token);
    hash_id = Target_hasObjectHash(target, hash);

    if (hash_id < 0) {
        Target_Object_Hash_Add(target, hash);
    } else {
        target->pr._argv_objects_cnt[hash_id]++;
        if (target->pr._argv_objects_cnt[hash_id] >= 10) {
            fprintf(stderr, "Too many same name sources/objects\n");
            exit(1);
        }
    }

    /* -- Append object to arg -- */
    token_len   = strlen(token);
    flag        = "-o";
    flag_len    = strlen(flag);
    total_len   = token_len + flag_len + 1;
    if (hash_id > 0)
        total_len++;
    arg = mace_calloc(total_len, sizeof(*arg));
    memcpy(arg, flag, flag_len);
    memcpy(arg + flag_len, token, token_len);

    if (hash_id > 0) {
        char *pos = strrchr(arg, '.');
        *(pos) = target->pr._argv_objects_cnt[hash_id] + '0';
        *(pos + 1) = '.';
        *(pos + 2) = 'o';
    }

    /* -- Actually adding object here -- */
    hash_nocoll = mace_hash(arg + flag_len);
    Target_Object_Hash_Add_nocoll(target, hash_nocoll);

    mace_Target_sources_grow(target);
    target->pr._argv_objects[target->pr._argc_sources - 1] = arg;

    /* Does object file exist */
    return (mace_fexists(arg + 2));
}

/*  Check if any source/header file changed
**         for object. */
void mace_Headers_Checksums_Checks(Target *target) {
    int i;
    int j;

    MACE_EARLY_RET(target != NULL, MACE_VOID, assert);
    MACE_EARLY_RET(target->pr._hdrs_changed != NULL, MACE_VOID, assert);

    if (gl.build_all) {
        size_t bytesize = target->pr._argc_sources * sizeof(*target->pr._recompiles);
        memset(target->pr._recompiles, 1, bytesize);
        return;
    }

    /* For every source file */
    for (i = 0; i < target->pr._argc_sources; i++) {
        /* Check if any header has changed */
        if (target->pr._recompiles[i] == true) {
            /* Already recompiles, skip */
            continue;
        }
        if (target->pr._deps_headers[i] == NULL) {
            /* No headers, skip */
            continue;
        }
        for (j = 0;  j < target->pr._deps_headers_num[i]; j++) {
            int header_order = target->pr._deps_headers[i][j];
            if (target->pr._hdrs_changed[header_order]) {
                target->pr._recompiles[i] = true;
                break;
            }
        }
    }
}

/*  Compute checksums for all headers. */
void mace_Headers_Checksums(const Target *target) {
    int i;

    mace_chdir(gl.cwd);

    for (i = 0; i < target->pr._headers_num; i++) {
        const char *header_path     = target->pr._headers[i];
        const char *checksum_path   = target->pr._headers_checksum[i];

        b32 changed = mace_file_changed(checksum_path, header_path);

        target->pr._hdrs_changed[i] = changed;
    }

    if (target->base_dir != NULL) {
        mace_chdir(target->base_dir);
    }
}

/*  Compute checksums for all sources. */
b32 mace_Source_Checksum(const Target   *target,
                         const char     *source_path,
                         const char     *obj_path) {
    /* - Compute current checksum - */
    b32      changed        = true;
    char    *checksum_path  = NULL;

    /* - Read existing checksum file - */
    mace_chdir(gl.cwd);
    checksum_path = mace_checksum_filename(obj_path, MACE_CHECKSUM_MODE_SRC);
    changed = mace_file_changed(checksum_path, source_path);

    mace_pop(&gl.arena, strlen(checksum_path) + 1);

    if (target->base_dir != NULL) {
        mace_chdir(target->base_dir);
    }

    return (changed);
}

/*  Add source file to target. */
b32 mace_Target_Source_Add(Target *target, char *token) {
    char    *rpath;

    MACE_EARLY_RET(token != NULL, true, MACE_nASSERT);

    mace_Target_sources_grow(target);

    /* - Expand path - */
    rpath = mace_calloc(PATH_MAX, sizeof(*rpath));
    if (realpath(token, rpath) == NULL) {
        size_t token_len;

        if (!gl.silent)
            printf("Warning! realpath issue: %s\n", rpath);
        token_len = strlen(token) + 1;
        if (token_len < PATH_MAX) {
            fprintf(stderr, "token_len longer than PATH_MAX\n");
            exit(1);
        }
        memcpy(rpath, token, token_len);
    }

    /* -- Actually adding source here -- */
    target->pr._argv_sources[target->pr._argc_sources++] = rpath;

    return (false);
}

/*  Add found source file to target */
/*      - Create object name from source name */
/*      - Make source checksum */
/*      - Check if source needs to be recompiled */
void mace_Target_Parse_Source(Target    *target,
                              char      *path,
                              char      *src) {
    size_t i;
    b32 exists;
    b32 changed_src;
    b32 excluded = mace_Target_Source_Add(target, path);
    if (excluded)
        return;

    mace_object_path(src);
    exists  = mace_Target_Object_Add(target, gl.object);
    i = target->pr._argc_sources - 1;
    changed_src = mace_Source_Checksum(target,
                                       target->pr._argv_sources[i],
                                       target->pr._argv_objects[i]);
    mace_Target_Recompiles_Add(target, !excluded && (changed_src || !exists));
}

/*  Globbed files for sources and parse objects. */
void mace_compile_glob( Target *target,
                        const char *globsrc,
                        const char *flags) {
    int i;
    glob_t globbed = mace_glob_sources(globsrc);

    for (i = 0; i < globbed.gl_pathc; i++) {
        char *pos;
        char *source_file;

        assert(mace_isSource(globbed.gl_pathv[i]));
        pos = strrchr(globbed.gl_pathv[i], LINUX_SEPARATOR[0]);
        source_file = (pos == NULL) ? globbed.gl_pathv[i] : pos + 1;
        /* - Compute source and object filenames - */
        mace_Target_Parse_Source(target, globbed.gl_pathv[i], source_file);
    }
    globfree(&globbed);
}

/******************** mace_is *********************/
int mace_isWildcard(const char *str) {
    MACE_EARLY_RET(str, 0, assert);
    return (strchr(str, MACE_WILDCARD) != NULL);
}

int mace_isSource(const char *path) {
    int     out;
    size_t  len;
    MACE_EARLY_RET(path, 0, assert);
    len      = strlen(path);
    out      = path[len - 1] == MACE_SRC_EXT[1];
    out     &= path[len - 2] == MACE_SRC_EXT[0];
    return (out);
}

int mace_isDir(const char *path) {
    struct stat st = {0};
    MACE_EARLY_RET(path, 0, assert);
    MACE_EARLY_RET(stat(path, &st) == 0, 0, MACE_nASSERT);

    return S_ISDIR(st.st_mode);
}

/***************** mace_filesystem *******************/
void mace_mkdir(const char *path) {
    struct stat st = {0};
    MACE_EARLY_RET(path, MACE_VOID, assert);

    if (stat(path, &st) == -1) {
        mkdir(path, 0777);
    }
}

/*  Create path to executable to compile. */
char *mace_executable_path( Mace_Arena *arena, 
                            const char *target_name) {
    char    *exec;
    size_t   full_len   = 0;
    size_t   bld_len    = 0;
    size_t   tar_len    = 0;

    MACE_EARLY_RET(target_name != NULL, NULL, assert);

    bld_len = strlen(MACE_BUILD_DIR);
    tar_len = strlen(target_name);

    exec = _mace_calloc(arena, (bld_len + tar_len + 2), sizeof(*exec));
    memcpy(exec, MACE_BUILD_DIR, bld_len);
    full_len += bld_len;
    if (MACE_BUILD_DIR[0] != LINUX_SEPARATOR[0]) {
        memcpy(exec + full_len, LINUX_SEPARATOR, 1);
        full_len++;
    }
    memcpy(exec + full_len, target_name, tar_len);
    return (exec);
}

/*  Create path to library to compile. */
char *mace_library_path(Mace_Arena   *arena, 
                        const   char *target_name,
                        int           kind) {
    char    *lib;
    size_t   bld_len    = 0;
    size_t   tar_len    = 0;
    size_t   full_len   = 0;

    MACE_EARLY_RET(target_name, NULL, assert);

    bld_len = strlen(MACE_BUILD_DIR);
    tar_len = strlen(target_name);
    lib     = _mace_calloc(arena, (bld_len + tar_len + 2), sizeof(*lib));
    memcpy(lib, MACE_BUILD_DIR, bld_len);
    full_len += bld_len;

    if (MACE_BUILD_DIR[0] != LINUX_SEPARATOR[0]) {
        memcpy(lib + full_len, LINUX_SEPARATOR, 1);
        full_len++;
    }
    memcpy(lib + full_len,     "lib",       3);
    full_len += 3;
    memcpy(lib + full_len,     target_name, tar_len);
    full_len += tar_len;
    if (kind == MACE_STATIC_LIBRARY) {
        memcpy(lib + full_len,     ".a",        2);
    } else if (kind == MACE_DYNAMIC_LIBRARY) {
        memcpy(lib + full_len,     ".so",        3);
    }
    return (lib);
}

/*************** mace_globals *****************/
/*  Realloc global object. */
void mace_object_grow(void) {
    gl.object_len *= MACE_GROW;
    gl.object = mace_realloc(
        gl.object,
        gl.object_len / MACE_GROW * sizeof(*gl.object),
        gl.object_len * sizeof(*gl.object)
    );
}

/*  Write object path from source file */
/*         to global object. */
void mace_object_path(const char *source) {
    /* --- Expanding path --- */
    char    *objectw;
    size_t   path_len;
    size_t   cwd_len        = strlen(gl.cwd);
    size_t   obj_dir_len    = strlen(MACE_OBJ_DIR);
    size_t   source_len     = strlen(source);

    /* --- Grow object string --- */
    path_len = cwd_len + obj_dir_len + source_len + 4;
    while (path_len >= gl.object_len) {
        mace_object_grow();
    }
    assert(path_len < gl.object_len);

    /* --- Writing path to object --- */
    memset(gl.object, 0, gl.object_len * sizeof(*gl.object));

    objectw = gl.object;
    memcpy(objectw, gl.cwd, cwd_len);
    objectw += cwd_len;
    memcpy(objectw, LINUX_SEPARATOR, 1);
    objectw += 1;
    memcpy(objectw, MACE_OBJ_DIR, obj_dir_len);
    objectw += obj_dir_len;

    if (source[0] != LINUX_SEPARATOR[0]) {
        memcpy(objectw, LINUX_SEPARATOR, 1);
        objectw += 1;
    }
    memcpy(objectw, source, source_len);
    gl.object[strlen(gl.object) - 1] = 'o';
}

void mace_print_message(const char *message) {
    MACE_EARLY_RET(message, MACE_VOID, MACE_nASSERT);
    MACE_EARLY_RET(gl.silent, MACE_VOID, MACE_nASSERT);

    printf("%s\n", message);
}

void mace_chdir(const char *path) {
    MACE_EARLY_RET(path, MACE_VOID, assert);
    if (chdir(path) != 0) {
        const char *msg = "Could not cd to directory '%s'\n";
        fprintf(stderr, msg, path);
        exit(1);
    }
}

b32 mace_fexists(const char *file) {
    return (access(file, F_OK) == 0);
}

void mace_getcwd() {
    MACE_EARLY_RET(gl.cwd, MACE_VOID, assert);

    if (getcwd(gl.cwd, MACE_CWD_BUFFER) == NULL) {
        fprintf(stderr, "getcwd() error %d: '%s'\n",
                errno, strerror(errno));
        exit(1);
    }
}

/**************** mace_build ******************/
/*  Run command input as a single long char string. */
/*      - Split tokens between spaces i.e. " " */
/*      - Reconstitute argv, argc */
/*      - Actually run command */
void mace_run_commands(const char *commands,
                       const char *preorpost,
                       const char *target) {
    char  *buffer;
    char  *token;
    char **argv;

    int argc    = 0;
    int len     = 8;

    if (commands == NULL)
        return;

    printf("Running command-%s, target '%s'\n",
           preorpost, target);
    mace_chdir(gl.cwd);

    argv = _mace_calloc(&gl.stackrena, len, (sizeof(*argv)));

    /* -- Copy sources into modifiable buffer -- */
    buffer = _mace_calloc(&gl.stackrena, 1, (strlen(commands) + 1));
    memcpy(buffer, commands, strlen(commands));

    /* --- Split sources into tokens --- */
    token = strtok(buffer, glstr.mace_command_separator);

    do {
        argc = 0;
        argv = mace_argv_flags(&len, &argc, argv, token, NULL, false, MACE_SEPARATOR);

        mace_argv_print(argv, argc);
        if (!gl.dry_run) {
            pid_t pid = mace_exec(argv[0], argv);
            mace_wait_pid(pid);
        }

        token = strtok(NULL, glstr.mace_command_separator);
    } while (token != NULL);

    mace_arena_clear(&gl.stackrena);
}

/*  Pre-build step. */
/*      - Compute checksums for each source, header file  */
/*      - Check which files need to be re-compiled */
/*        depending on checksums  */
void mace_prebuild_target(Target *target) {
    char *token;
    char *buffer;

    MACE_EARLY_RET(target != NULL, MACE_VOID, assert);

    if (target->kind == MACE_PHONY) {
        return;
    }

    if (!gl.silent) {
        MACE_EARLY_RET(target->pr._name != NULL, MACE_VOID, assert);
        printf("Pre-build target '%s'\n", target->pr._name);
    }

    /* Check which sources don't need to be recompiled */
    /* --- Move to target base_dir, compile there --- */
    if (target->base_dir != NULL) {
        mace_chdir(target->base_dir);
    }

    /* --- Parse sources, put into array --- */
    if ((target->kind <= MACE_TARGET_NULL) ||
        (target->kind >= MACE_TARGET_KIND_NUM)) {
        fprintf(stderr, "Wrong target type.\n");
        exit(1);
    }

    /* --- Compile sources --- */
    if (target->sources == NULL) {
        return;
    }
    /* -- Copy sources into modifiable buffer -- */
    buffer = _mace_calloc(&gl.stackrena, 1, (strlen(target->sources) + 1));
    memcpy(buffer, target->sources, strlen(target->sources));

    /* --- Parse sources --- */
    token = strtok(buffer, MACE_SEPARATOR);
    do {
        if (mace_isDir(token)) {
            /* Glob all sources recursively */
            size_t srclen   = strlen(token);
            size_t bytesize = srclen + 6;
            char  *globstr = _mace_calloc(&gl.stackrena, bytesize, sizeof(*globstr));
            memcpy(globstr,              token,  strlen(token));
            memcpy(globstr + srclen,     LINUX_SEPARATOR,    1);
            memcpy(globstr + srclen + 1, "**" MACE_SRC_EXT, 4);

            mace_compile_glob(target, globstr, target->flags);
            mace_pop(&gl.stackrena, bytesize);

        } else if (mace_isWildcard(token)) {
            /* token has a wildcard in it */
            mace_compile_glob(target, token, target->flags);

        } else if (mace_isSource(token)) {
            /* token is a source file */
            mace_Target_Parse_Source(target, token, token);

        } else {
            fprintf(stderr, "source is neither a .c file, a folder, nor has a wildcard in it\n");
            exit(1);
        }

        token = strtok(NULL, MACE_SEPARATOR);
    } while (token != NULL);

    mace_Target_precompile(target);
    mace_arena_clear(&gl.stackrena);
    mace_chdir(gl.cwd);
}

/*  Build input target: compile then link. */
void mace_build_target(Target *target) {
    /* --- Skip if invalic type target --- */
    if ((target->kind <= MACE_TARGET_NULL) ||
        (target->kind >= MACE_TARGET_KIND_NUM)) {
        fprintf(stderr, "Wrong target type.\n");
        exit(1);
    }

    /* --- Skip if phony target --- */
    if (target->kind == MACE_PHONY) {
        return;
    }

    if (!gl.silent)
        printf("Building target '%s'\n", target->pr._name);

    /* --- Compile now --- */
    if (target->base_dir != NULL) {
        mace_chdir(target->base_dir);
    }

    /* -- allatonce -- */
    if (target->allatonce) {
        mace_Target_compile_allatonce(target);
    } else {
        mace_Target_compile(target);
    }

    /* --- Move back to cwd to link --- */
    mace_chdir(gl.cwd);

    /* --- Linking --- */
    mace_link[target->kind - 1](target);
    mace_chdir(gl.cwd);
}

/*  Check if target order is in build_order */
b32 mace_in_build_order(size_t  order,
                        const int *b_order,
                        int     num) {
    int i;
    b32 out = false;
    MACE_EARLY_RET(b_order != NULL, out, assert);

    for (i = 0; i < num; i++) {
        if (b_order[i] == order) {
            out = true;
            break;
        }
    }
    return (out);
}

/*  Get target order from input hash */
/*  @return Target order (as added by user), */
/*          or -1 if not found */
int mace_target_order(u64 hash) {
    int i;
    for (i = 0; i < gl.target_num; i++) {
        if (hash == gl.targets[i].pr._hash)
            return (i);
    }
    return (-1);
}

int mace_config_order(u64 hash) {
    int i;
    for (i = 0; i < gl.config_num; i++) {
        if (hash == gl.configs[i].pr._hash)
            return (i);
    }
    return (-1);
}

/*  Add target with input order into build_order */
void mace_build_order_add(int order) {
    assert(gl.build_order != NULL);
    assert(gl.build_order_num < gl.target_num);
    assert(order >= 0);
    if (mace_in_build_order(order, gl.build_order, gl.build_order_num)) {
        fprintf(stderr, "Target ID is already in build_order."
                        "Exiting.\n");
        exit(1);
    }
    gl.build_order[gl.build_order_num++] = order;
}

/*  Depth first search through target */
/*         dependencies to construct a build_order */
/*         to for user target and all its dependencies */
void mace_build_order_recursive(Target target,
                                size_t *o_cnt) {
    size_t order;

    /* Recursion necessarily smaller than target_num */
    if ((*o_cnt) >= gl.target_num) {
        return;
    }

    order = mace_target_order(target.pr._hash);
    /* Target already in build order, skip */
    if (mace_in_build_order(order, gl.build_order, gl.build_order_num)) {
        return;
    }

    /* Target has no dependencies, add target to build order */
    if (target.pr._deps_links == NULL) {
        mace_build_order_add(order);
        assert(mace_in_build_order(order, gl.build_order,
                                   gl.build_order_num));
        return;
    }

    /* Visit all target dependencies */
    for (target.pr._d_cnt = 0; target.pr._d_cnt < target.pr._deps_links_num;
         target.pr._d_cnt++) {
        int next_target_order = mace_target_order(target.pr._deps_links[target.pr._d_cnt]);

        if (next_target_order < 0)
            continue;

        /* Recursively search target's next dependency -> depth first search */
        mace_build_order_recursive(gl.targets[next_target_order], o_cnt);
    }

    /* Target already in build order, skip */
    if (mace_in_build_order(order, gl.build_order, gl.build_order_num)) {
        return;
    }

    if (target.pr._d_cnt != target.pr._deps_links_num) {
        fprintf(stderr, "Not all target dependencies before target in build order.\n");
        exit(1);
    }

    /* All dependencies built, add it to build order */
    mace_build_order_add(order);
    return;
}

/*  Check if target depends on target */
/*         with input hash. */
b32 mace_Target_hasDep( const Target *target,
                        u64 target_hash) {
    int i;

    MACE_EARLY_RET(target->pr._deps_links != NULL, false, MACE_nASSERT);

    for (i = 0; i < target->pr._deps_links_num; i++) {
        if (target->pr._deps_links[i] == target_hash)
            return (true);
    }
    return (false);
}

/*  Check if any two targets depend on each other. */
/*      TODO: Resolve circular dependencies somehow. */
b32 mace_circular_deps(Target *targs, size_t len) {
    /* -- Circular dependency conditions -- */
    /*   1- Target i has j dependency       */
    /*   2- Target j has i dependency       */
    int i;
    for (i = 0; i < gl.target_num; i++) {
        u64 hash_i = targs[i].pr._hash;
        /* 1- going through target i's dependencies */
        int z;
        for (z = 0; z < targs[i].pr._deps_links_num; z++) {
            int j = mace_target_order(targs[i].pr._deps_links[z]);

            /* Dependency is not in list of targets */
            if (j < 0)
                continue;

            /* Dependency is self */
            if (i == j) {
                if (!gl.silent)
                    printf("Warning! Target '%s' depends on itself.\n", targs[i].pr._name);
                continue;
            }

            /* Dependency is other target */
            if (mace_Target_hasDep(&targs[j], hash_i))
                /* 2- target i's dependency j has i as dependency as well */
                return (true);
        }
    }
    return (false);
}

/*  Creates obj_dir, build_dir... */
void mace_make_dirs(void) {
    /* obj_dir for intermediary files */
    mace_mkdir(MACE_OBJ_DIR);

    /* Move to obj_dir and make 'src' and 'include' dirs for checksums */
    mace_chdir(MACE_OBJ_DIR);
    mace_mkdir("src");
    mace_mkdir("include");
    mace_chdir(gl.cwd);

    /* build_dir for targets */
    mace_mkdir(MACE_BUILD_DIR);
}

/*  Read cflag string, splitting string
**         into _flags using mace_separator. */
void mace_parse_cflags(void) {
    char  *buffer;
    char  *token;
    int len         = 0;
    size_t len_str  = 0;
    if (gl.cflags == NULL) {
        return;
    }
    len             = 8;
    gl.cflags_num   = 0;
    if (gl.cflags_sep == NULL) {
        gl.cflags_sep = mace_calloc(len, sizeof(*gl.cflags_sep));
    }
    len_str = strlen(gl.cflags);
    buffer = _mace_calloc(&gl.stackrena, 1, (len_str + 1));
    memcpy(buffer, gl.cflags, len_str);
    token  = strtok(buffer, MACE_SEPARATOR);
    do {
        char *cflag = mace_calloc(strlen(token) + 1, sizeof(*cflag));
        memcpy(cflag, token, strlen(token));
        gl.cflags_sep[gl.cflags_num++] = cflag;
        /* Increase config->pr._flags size */
        if (gl.cflags_num >= len) {
            size_t bytesize;
            len *= MACE_GROW;
            bytesize = len * sizeof(*gl.cflags_sep);
            gl.cflags_sep = mace_realloc(gl.cflags_sep, bytesize / MACE_GROW, bytesize);
            memset(gl.cflags_sep + len / MACE_GROW, 0, len / MACE_GROW);
        }
        token = strtok(NULL, MACE_SEPARATOR);
    } while (token != NULL);
    mace_arena_clear(&gl.stackrena);
}

/*  Read config string, splitting string
**         into _flags using mace_separator. */
void mace_parse_config(Config *config) {
    int      len = 8;
    char    *buffer;
    char    *token;

    if (config->flags == NULL) {
        fprintf(stderr, "Config has no flags.\n");
        exit(1);
    }

    buffer = mace_copy_str(config->flags);

    /* -- Split flags string into target orders -- */
    config->pr._flags = mace_calloc(len, sizeof(*config->pr._flags));
    config->pr._flag_num = 0;

    token = strtok(buffer, MACE_SEPARATOR);
    do {
        char *flag = mace_copy_str(token);
        config->pr._flags[config->pr._flag_num++] = flag;
        /* Increase config->pr._flags size */
        if (config->pr._flag_num >= len) {
            size_t bytesize;
            len *= MACE_GROW;
            bytesize = len * sizeof(*config->pr._flags);
            config->pr._flags = mace_realloc(config->pr._flags, bytesize / MACE_GROW, bytesize);
        }
        token = strtok(NULL, MACE_SEPARATOR);
    } while (token != NULL);
}

void mace_parse_configs(void) {
    int i;
    for (i = 0; i < gl.config_num; i++) {
        mace_parse_config(&gl.configs[i]);
    }
}

/*  Determine build_order of user target */
/*         using depth-first  search through its */
/*         dependencies */
void mace_build_order(void) {
#ifndef NDEBUG
    b32 cond;
#endif /* NDEBUG */

    size_t o_cnt = 0; /* order count */

    /* If only 1 include, build order is trivial */
    if (gl.target_num <= 1) {
        mace_build_order_add(0);
        return;
    }

    /* If user_target is not all, or default_target is not all */
    /*  - Build only specified target */
    if ((gl.mace_user_target > MACE_TARGET_NULL) ||
        (gl.mace_default_target > MACE_TARGET_NULL)) {
        /* Build dependencies of default target, and itself only */
        o_cnt = gl.mace_user_target > MACE_TARGET_NULL ? gl.mace_user_target : gl.mace_default_target;
        mace_build_order_recursive(gl.targets[o_cnt], &o_cnt);
        return;
    }
    /* If user_target is all,  */
    /*  or default_target is all and no user_target */
#ifndef NDEBUG
    cond  = (gl.mace_user_target == MACE_TARGET_NULL) &&
            (gl.mace_default_target == MACE_TARGET_NULL);
    cond |= (gl.mace_user_target == MACE_TARGET_NULL);
    assert(cond);
#endif /* NDEBUG */

    o_cnt = 0;
    /* Visit all targets */
    while (o_cnt < gl.target_num) {
        mace_build_order_recursive(gl.targets[o_cnt], &o_cnt);
        o_cnt++;
    }
}

/*  Prepare for build step: check whats */
/*         needs to be recompiled */
void mace_pre_build(void) {
    int z;

    /* --- Make output directories --- */
    mace_make_dirs();

    /* --- Build order from target links, deps --- */
    mace_build_order();

    /* Actually prebuild all targets */
    for (z = 0; z < gl.build_order_num; z++) {
        assert(gl.build_order[z] >= 0);
        mace_Target_Grow_Headers(&gl.targets[gl.build_order[z]]);
        mace_prebuild_target(&gl.targets[gl.build_order[z]]);
    }
}

/*  Actually compile and link target. */
void mace_build(void) {
    /* Actually build all targets */
    int z;
    for (z = 0; z < gl.build_order_num; z++) {
        Target *target = &gl.targets[gl.build_order[z]];
        /* -- config argv -- */
        mace_argv_add_config(   &target->pr._argv,
                                &target->pr._argc,
                                &target->pr._arg_len);
        mace_argv_add_cflags(   &target->pr._argv,
                                &target->pr._argc,
                                &target->pr._arg_len);

        assert(target->pr._name != NULL);
        mace_print_message(target->msg_pre);
        mace_run_commands(target->cmd_pre, "pre", target->pr._name);
        mace_build_target(target);
        mace_print_message(target->msg_post);
        mace_run_commands(target->cmd_post, "post", target->pr._name);
    }
}

/*  Alloc _deps_header arrays if don't exist.  */
/*         Realloc to bigger if num close to len */
void mace_Target_Grow_deps_headers(Target *target,
                                   int source_i) {
    if (target->pr._deps_headers[source_i] == NULL) {
        target->pr._deps_headers_num[source_i] = 0;
        target->pr._deps_headers_len[source_i] = 8;
        target->pr._deps_headers[source_i] = mace_calloc(target->pr._deps_headers_len[source_i],
                                                         sizeof(**target->pr._deps_headers));
    }
    if (target->pr._deps_headers_num[source_i] >= target->pr._deps_headers_len[source_i]) {
        size_t bytesize;
        target->pr._deps_headers_len[source_i] *= MACE_GROW;
        bytesize = target->pr._deps_headers_len[source_i] * sizeof(**target->pr._deps_headers);
        target->pr._deps_headers[source_i] = mace_realloc(target->pr._deps_headers[source_i], bytesize / MACE_GROW, 
                                                          bytesize);
    }
}

/*  Alloc header arrays if don't exist.  */
/*      Realloc to bigger if num close to len */
void mace_Target_Grow_Headers(Target *target) {
    /* -- Alloc headers -- */
    if (target->pr._headers == NULL) {
        target->pr._headers_len = 8;
        target->pr._headers_num = 0;
        target->pr._headers  = mace_calloc(target->pr._headers_len, sizeof(*target->pr._headers));
    }
    if (target->pr._headers_hash == NULL) {
        target->pr._headers_hash  = mace_calloc(target->pr._headers_len,
                                                sizeof(*target->pr._headers_hash));
    }

    /* -- Alloc _hdrs_changed -- */
    if (target->pr._hdrs_changed == NULL) {
        target->pr._hdrs_changed  = mace_calloc(target->pr._headers_len,
                                                sizeof(*target->pr._hdrs_changed));
    }

    /* -- Alloc _headers_checksum -- */
    if (target->pr._headers_checksum == NULL) {
        target->pr._headers_checksum  = mace_calloc(target->pr._headers_len,
                                                    sizeof(*target->pr._headers_checksum));
    }
    if (target->pr._headers_checksum_hash == NULL) {
        /* Always less hashes than _headers_checksum */
        target->pr._headers_checksum_hash = mace_calloc(target->pr._headers_len,
                                                        sizeof(*target->pr._headers_checksum_hash));
    }

    if (target->pr._headers_checksum_cnt == NULL) {
        /* Always less hashes than _headers_checksum */
        target->pr._headers_checksum_cnt = mace_calloc(target->pr._headers_len,
                                                       sizeof(*target->pr._headers_checksum_cnt));
    }

    /* -- Realloc _headers_checksum -- */
    if (target->pr._headers_num >= (target->pr._headers_len - 1)) {
        size_t bytesize = target->pr._headers_len * MACE_GROW * sizeof(*target->pr._hdrs_changed);
        target->pr._hdrs_changed = mace_realloc(target->pr._hdrs_changed, bytesize / MACE_GROW, bytesize);
        memset(target->pr._hdrs_changed + target->pr._headers_len, 0, bytesize / MACE_GROW);
    }

    /* -- Realloc _headers_checksum -- */
    if (target->pr._headers_num >= (target->pr._headers_len - 1)) {
        size_t bytesize = target->pr._headers_len * MACE_GROW * sizeof(*target->pr._headers_checksum);
        target->pr._headers_checksum = mace_realloc(target->pr._headers_checksum, bytesize / MACE_GROW, bytesize);
        memset(target->pr._headers_checksum + target->pr._headers_len, 0, bytesize / MACE_GROW);

        bytesize = target->pr._headers_len * MACE_GROW * sizeof(*target->pr._headers_checksum_hash);
        target->pr._headers_checksum_hash = mace_realloc(target->pr._headers_checksum_hash, bytesize / MACE_GROW, bytesize);
        memset(target->pr._headers_checksum_hash + target->pr._headers_len, 0, bytesize / MACE_GROW);

        bytesize = target->pr._headers_len * MACE_GROW * sizeof(*target->pr._headers_checksum_cnt);
        target->pr._headers_checksum_cnt = mace_realloc(target->pr._headers_checksum_cnt, bytesize / MACE_GROW, bytesize);
        memset(target->pr._headers_checksum_cnt + target->pr._headers_len, 0, bytesize / MACE_GROW);
    }

    /* -- Realloc headers -- */
    if (target->pr._headers_num >= (target->pr._headers_len - 1)) {
        size_t bytesize = target->pr._headers_len * MACE_GROW * sizeof(*target->pr._headers_hash);
        target->pr._headers_hash = mace_realloc(target->pr._headers_hash, bytesize / MACE_GROW, bytesize);
        memset(target->pr._headers_hash + target->pr._headers_len, 0, bytesize / MACE_GROW);
    }

    if (target->pr._headers_num >= (target->pr._headers_len - 1)) {
        size_t bytesize;
        target->pr._headers_len *= MACE_GROW;
        bytesize = target->pr._headers_len * sizeof(*target->pr._headers);
        target->pr._headers = mace_realloc(target->pr._headers, bytesize / MACE_GROW, bytesize);
        memset(target->pr._headers + target->pr._headers_len / MACE_GROW, 0, bytesize / MACE_GROW);
    }
}

/*  Read target object .d files to check */
/*         which headers are required for re-compilation */
void mace_Target_Read_Objdeps(Target *target,
                              char *deps,
                              int source_i) {
    /* --- Split headers into tokens --- */
    char *header   = strtok(deps, MACE_SEPARATOR);
    size_t cwd_len = strlen(gl.cwd);

    /* --- Hash headers into _deps_links --- */
    while (header != NULL) {
        u64     hash;
        int     header_order;
        size_t  ext;

        /* Skip if file is not a header */
        char *dot  = strrchr(header,  '.'); /* last dot in path */
        if (dot == NULL) {
            header = strtok(NULL, MACE_SEPARATOR);
            continue;
        }
        ext = dot - header;

        if (header[ext + 1] != MACE_HDR_EXT[1]) {
            header = strtok(NULL, MACE_SEPARATOR);
            continue;
        }

        /* Skip if header is not in cwd */
        if (target->pr._checkcwd && (memcmp(header, gl.cwd, cwd_len) != 0)) {
            header = strtok(NULL, MACE_SEPARATOR);
            continue;
        }

        /* add header to list of all headers */
        hash = mace_Target_Header_Add(target, header);

        /* Add header to list of header_deps of object */
        header_order = mace_Target_header_order(target, hash);
        mace_Target_Objdep_Add(target, header_order, source_i);

        header = strtok(NULL, MACE_SEPARATOR);
    }
}

/*  Check if target has header from input */
/*         filename hash. */
/*  @return -1 if not found, header_order if found. */
int mace_Target_header_order(Target *target, u64 hash) {
    int i;
    for (i = 0; i < target->pr._headers_num; ++i) {
        if (target->pr._headers_hash[i] == hash)
            return (i);
    }
    return (-1);
}

void mace_Target_Header_Add_Objpath(Target *target,
                                    char *header) {
    int i;
    u64 hash;
    int hash_id = -1;
    char *header_checksum = mace_checksum_filename(header, MACE_CHECKSUM_MODE_INCLUDE);
    hash = mace_hash(header_checksum);

    /* Check if header_checksum already exists */
    for (i = 0; i < target->pr._headers_num; ++i) {
        if (hash == target->pr._headers_checksum_hash[i]) {
            hash_id = i;
            break;
        }
    }

    if (hash_id < 0) {
        /* header_checksum hash not found, adding it at same order */
        assert(target->pr._headers_checksum_hash != NULL);
        target->pr._headers_checksum_hash[target->pr._headers_num] = hash;
    } else {
        /* header_checksum hash found, adding number to path */
        char *pos;
        pos = strrchr(header_checksum, '.');
        *(pos) = target->pr._headers_checksum_cnt[hash_id] + '0';
        memcpy(pos + 1, ".sha1", 4);
        target->pr._headers_checksum_cnt[hash_id]++;
    }

    /* Adding header_checksum */
    assert(header_checksum != NULL);
    target->pr._headers_checksum[target->pr._headers_num] = header_checksum;
}

/*  Add header file name to target. */
u64 mace_Target_Header_Add(Target *target, char *header) {
    /* Check if header hash already in _headers_hash */
    /* Add header hash to _headers_hash */
    u64 hash = mace_hash(header);

    /* Add header name to _headers */
    mace_Target_Grow_Headers(target);
    if (mace_Target_header_order(target, hash) == -1) {
        size_t len = strlen(header);

        /* Add header hash */
        target->pr._headers_hash[target->pr._headers_num] = hash;

        /* Add header name, later used for checksums */
        assert(target->pr._headers[target->pr._headers_num] == NULL);
        target->pr._headers[target->pr._headers_num] = mace_calloc(len + 1,
                                                                 sizeof(**target->pr._headers));
        memcpy(target->pr._headers[target->pr._headers_num], header, len);

        /* Add header obj name */
        mace_Target_Header_Add_Objpath(target, header);

        /* Increment number of headers */
        target->pr._headers_num++;
    }
    return (hash);
}

/*  Add header as dependency of target. */
void mace_Target_Objdep_Add(Target *target,
                            int header_order,
                            int source_i) {
    /* Check if header_order in _deps_headers */
    int i;
    assert(source_i > -1);
    assert(source_i <= target->pr._argc_sources);
    for (i = 0; i < target->pr._deps_headers_num[source_i]; i++) {
        if (target->pr._deps_headers[source_i][i] == header_order)
            return;
    }

    mace_Target_Grow_deps_headers(target, source_i);
    i = target->pr._deps_headers_num[source_i]++;
    assert(target->pr._deps_headers            != NULL);
    assert(target->pr._deps_headers[source_i]  != NULL);
    target->pr._deps_headers[source_i][i] = header_order;
}

/*  Read .d file and build .ho file from it. */
char *mace_Target_Read_d(Target *target, int source_i) {
    b32      fho_exists;
    b32      source_changed;
    int      obj_hash_id;
    char    *dot;
    char     buffer[MACE_OBJDEP_BUFFER];
    char    *obj_file;
    FILE    *fd;
    FILE    *fho;
    size_t   size;
    size_t   ext;
    size_t   obj_len;

    int   oflagl = 2;
    char *obj_file_flag = target->pr._argv_objects[source_i];

    /* obj_file_flag should start with "-o" */
    if ((obj_file_flag[0] != '-') ||
        (obj_file_flag[1] != 'o')) {
        /* error? */
        fprintf(stderr, "obj_file_flag '%s' missing the -o flag.\n", obj_file_flag);
        exit(1);
    }
    obj_len  = strlen(obj_file_flag);
    assert((obj_len + 5) > oflagl);
    obj_file = mace_calloc(obj_len - oflagl + 5, sizeof(*obj_file));
    memcpy(obj_file, obj_file_flag + oflagl, obj_len - oflagl);
    size = 0;
    dot  = strrchr(obj_file, '.'); /* last dot in path */
    ext  = dot - obj_file;

    /* Check if .ho exists */
    memcpy(obj_file + ext + 1, MACE_HO_EXT, 2);
    obj_file[ext + 3] = '\0';

    fho = fopen(obj_file, "rb");
    fho_exists = false;
    if (fho != NULL) {
        fho_exists = true;
        fclose(fho);
    }

    /* Check if .d exists */
    obj_file[ext + 1] = MACE_DEP_EXT;
    obj_file[ext + 2] = '\0';
    fd = fopen(obj_file, "rb");
    if (fd == NULL) {
        fprintf(stderr, "Object dependency file '%s' does not exist.\n", obj_file);
        exit(1);
    }
    target->pr._deps_headers_num[source_i] = 0;

    /* Parse all dependencies, " " separated */
    while (fgets(buffer, MACE_OBJDEP_BUFFER, fd) != NULL) {
        size_t len = strlen(buffer);
        u64 obj_hash;
        /* - Replace \n with \0 ' ' - */
        b32 line_end = false;
        if (buffer[len - 1] == '\n') {
            line_end = true;
            buffer[len - 1] = '\0';
        }

        /* Check that target has object with nocoll hashes */
        obj_file[ext + 1] = 'o';
        obj_hash = mace_hash(obj_file);
        obj_hash_id = Target_hasObjectHash_nocoll(target, obj_hash);
        assert(obj_hash_id < target->pr._objects_hash_nocoll_num);
        assert(obj_hash_id > -1);

        /* - Parsing dependencies read from fd - */
        mace_Target_Read_Objdeps(target, buffer, source_i);

        if (!line_end) {
            /* - Go back to last ' ', to read line more - */
            char *last_ptr = strrchr(buffer, ' ');
            /* [                  size                  ]  */
            /* [    last_ptr - buffer    ] [ last_space ]  */
            /* b--------------------------l-------------\0 */
            int last_space = (int)(last_ptr - buffer) - size;
            fseek(fd, last_space, SEEK_CUR);
        }
    }
    fclose(fd);

    /* - Only need to compute .ho file if source changed OR
    **  .ho doesn't exist - */
    source_changed = target->pr._recompiles[source_i];
    if ((!source_changed) && (fho_exists)) {
        return (NULL);
    }
    mace_arena_clear(&gl.stackrena);
    return (obj_file);
}

/*  Parse .d file, recording all header */
/*         files to .ho files. Should only be */
/*         called if source file changed */
void mace_Target_Parse_Objdep(Target *target, int source_i) {
    char    *obj_file;
    char    *dot;
    FILE    *fho;
    size_t   ext;
    /* Set _deps_headers_num to invalid */
    target->pr._deps_headers_num[source_i] = -1;
    obj_file = mace_Target_Read_d(target, source_i);
    if (obj_file == NULL) {
        /* Skip: no need to write .ho file */
        return;
    }

    if (target->pr._deps_headers_num[source_i] <= 0) {
        /* Skip: no headers */
        mace_pop(&gl.arena, strlen(obj_file) + 1);
        return;
    }

    /* Write _deps_header to .ho file */
    dot = strrchr(obj_file, '.'); /* last dot in path */
    ext = dot - obj_file;

    memcpy(obj_file + ext + 1, MACE_HO_EXT, 2);
    obj_file[ext + 3] = '\0';

    fho = fopen(obj_file, "wb");
    assert(target->pr._deps_headers[source_i] != NULL);
    fwrite(target->pr._deps_headers[source_i],
           sizeof(**target->pr._deps_headers),
           target->pr._deps_headers_num[source_i], fho);
    fclose(fho);

    mace_pop(&gl.arena, strlen(obj_file) + 1);
}

/*  Read .ho file and put all read headers */
/*         in _deps_headers */
void mace_Target_Read_ho(Target *target, int source_i) {
    int      bytesize;
    int      oflagl;
    char    *dot;
    char    *obj_file;
    char    *obj_file_flag;
    FILE    *fho;
    size_t   obj_len;
    size_t   ext;

    /* Only read if .ho file was not created. */
    if (target->pr._deps_headers_num[source_i] > 0)
        return;

    target->pr._deps_headers_num[source_i] = 0;

    /* read .ho file. It should exist. */
    obj_file_flag = target->pr._argv_objects[source_i];

    /* obj_file_flag should start with "-o" */
    if ((obj_file_flag[0] != '-') ||
        (obj_file_flag[1] != 'o')) {
        /* error? */
        fprintf(stderr, "obj_file_flag '%s' missing the -o flag.\n", obj_file_flag);
        exit(1);
    }
    oflagl = 2;
    obj_len = strlen(obj_file_flag);
    obj_file = _mace_calloc(&gl.stackrena, (obj_len - oflagl + 5), sizeof(*obj_file));
    memcpy(obj_file, obj_file_flag + oflagl, obj_len - oflagl);

    dot = strrchr(obj_file,  '.'); /* last dot in path */
    ext = dot - obj_file;

    /* Check if .ho exists */
    memcpy(obj_file + ext + 1, MACE_HO_EXT, 2);
    obj_file[ext + 3] = '\0';
    fho = fopen(obj_file, "rb");
    if (fho == NULL) {
        /* .ho file does not exist: no dependencies */
        mace_arena_clear(&gl.stackrena);
        return;
    }

    /* Get total number of bytes in file */
    fseek(fho, 0L, SEEK_END);
    bytesize = ftell(fho);
    fseek(fho, 0L, SEEK_SET);

    /* Realloc _deps_headers */
    target->pr._deps_headers_num[source_i] = bytesize / sizeof(**target->pr._deps_headers);
    target->pr._deps_headers_len[source_i] = target->pr._deps_headers_num[source_i];

    /* Read all bytes into _deps_headers */
    target->pr._deps_headers[source_i] = mace_calloc(1, bytesize);
    fread(target->pr._deps_headers[source_i], bytesize, 1, fho);
    fclose(fho);

    mace_arena_clear(&gl.stackrena);
}

/* Save header order dependencies to .ho */
/* Note: .d should exist. */
void mace_Target_Parse_Objdeps(Target *target) {
    /* Loop over all _argv_sources */
    int i;
    for (i = 0; i < target->pr._argc_sources; i++) {
        mace_Target_Parse_Objdep(target, i);
        mace_Target_Read_ho(target, i);
    }
}

/*  Alloc stuff in preparation for user  */
/*         to set targets, configs, etc. */
void mace_pre_user(const Mace_Args *args) {
    mace_post_build(NULL);

    assert(strlen(MACE_SEPARATOR) == 1);
    /* --- 1. Initialize variables --- */
    gl.target_num       = 0;
    gl.config_num       = 0;
    gl.build_order_num  = 0;
    gl.target_len       = MACE_DEFAULT_TARGET_LEN;
    gl.config_len       = MACE_DEFAULT_TARGET_LEN;
    gl.object_len       = MACE_DEFAULT_TARGET_LEN;

    gl.mace_default_target_hash = 0ul;
    gl.mace_default_config_hash = 0ul;

    /* --- 2. Set switches --- */
    if (args != NULL) {
        gl.silent       = args->silent;
        gl.dry_run      = args->dry_run;
        gl.verbose      = gl.dry_run ? true : args->debug;
        gl.build_all    = args->build_all;
    }

    /* --- 3. Record cwd --- */
    mace_getcwd();

    /* --- 4. Memory allocation --- */
#ifndef MACE_NO_HEAP
    gl.arena        = mace_arena_init(MACE_MEM / 2);
    gl.stackrena    = mace_arena_init(MACE_MEM / 2);
#else
    gl.arena.mem        = mem;
    gl.arena.size       = MACE_MEM / 2;
    gl.stackrena.mem    = stackmem;
    gl.stackrena.size   = MACE_MEM / 2;
#endif /* MACE_NO_HEAP */

    if (gl.object == NULL) {
        gl.object   = mace_calloc(gl.object_len, sizeof(*gl.object));
    }
    if (gl.targets == NULL) {
        gl.targets  = mace_calloc(gl.target_len, sizeof(*gl.targets));
    }
    if (gl.configs == NULL) {
        gl.configs  = mace_calloc(gl.config_len, sizeof(*gl.configs));
    }
    if (gl.build_order == NULL) {
        gl.build_order = mace_calloc(gl.target_len, sizeof(*gl.build_order));
    }
}

/*  Prepare for build after user added */
/*         targets, configs, etc. */
void mace_post_user(const Mace_Args *args) {
    /*   1- Moves to user set dir if not NULL. */
    /*   2- Checks that > 1 target exists */
    /*   3- Check that no circular dependency. */
    /*   4- Parse configs to set flags. */
    /*   5- Computes user_target order with priority: */
    /*      a- input argument */
    /*      b- config */
    /*      c- macefile (set before post-user) */
    /*   6- Computes default target order from default target_hash. */
    /*   7- Allocs queue for processes. */
    /*   8- Overrides compiler with priority: */
    /*      a- defines */
    /*      b- input argument */
    /*      c- config */
    /*      d- macefile (set before post-user) */
    /*   9- Overrides archiver with priority: */
    /*      a- defines */
    /*      b- input argument */
    /*      c- config */
    /*      d- macefile (set before post-user) */
    /*   10- Override cc_depflag with input arguments */
    /*   11- Override cflags with input arguments */
    /*   12- Parse cflags */

    Config *config;

    /* 1. Move to args->dir */
    if ((args != NULL) && (args->dir != NULL)) {
        mace_chdir(args->dir);
    }

    /* 2. Check that a target exists */
    if ((gl.targets == NULL) || (gl.target_num <= 0)) {
        fprintf(stderr, "No targets to compile. Exiting.\n");
        exit(1);
    }

    /* 3. Check for circular dependency */
    if (mace_circular_deps(gl.targets, gl.target_num)) {
        fprintf(stderr, "Circular dependency in linked library detected. Exiting\n");
        exit(1);
    }

    if (gl.targets == NULL) {
        return;
    }

    if (gl.target_num == 0) {
        return;
    }

    /* 4. Parsing configs */
    mace_parse_configs();

    /* 5. Computes default target order from default target_hash */
    mace_default_target_order();
    mace_default_config_order();

    /* 6. Check which target should be compiled */
    mace_user_target_set(args->user_target_hash);
    mace_target_resolve();

    /* 7. Check which config should be compiled */
    mace_user_config_set(args->user_config_hash);
    assert(gl.mace_target < gl.target_num);
    mace_config_resolve(&gl.targets[gl.mace_target]);
    config = &gl.configs[gl.mace_config];

    /* 8. Process queue alloc */
    assert(args->jobs >= 1);
    if (args->jobs > MACE_JOBS) {
        fprintf(stderr,
                "Error: Buffer for jobs too small.\n"
                "Increase it with -DMACE_JOBS >= %d when"
                "bootstrapping.\n",
                args->jobs);
        exit(1);
    }

    /* 8.c Override compiler with config */
    mace_set_compiler(config->cc);

    /* 8.b Override compiler with input arguments */
    mace_set_compiler(args->cc);

    /* 8.a Override compiler with defines */
#ifdef MACE_CC
    mace_set_compiler(STRINGIFY(MACE_CC));
#endif

    /* 9.c Override archiver with config */
    mace_set_archiver(config->ar);

    /* 9.b Override archiver with input arguments */
    mace_set_archiver(args->ar);

    /* 9.a Override archiver with defines */
#ifdef MACE_AR
    mace_set_archiver(STRINGIFY(MACE_AR));
#endif

    /* 10.a Override cc_depflag with input arguments */
    mace_set_cc_depflag(args->cc_depflag);

    /* 10.b Override cflags with input arguments */
    mace_set_cflags(args->cflags);

    /* 10.a Override depflag with defines */
#ifdef MACE_CC_DEPFLAG
    mace_set_cc_depflag(STRINGIFY(MACE_CC_DEPFLAG));
#endif

    /* 12. Parse cflags */
    mace_parse_cflags();
}

void mace_post_build(   Mace_Args   *args) {
    /* --- 1. Reset variables --- */
    gl.object           = NULL;
    gl.targets          = NULL;
    gl.configs          = NULL;
    gl.build_order      = NULL;
    gl.target_num       = 0;
    gl.config_num       = 0;
    gl.build_order_num  = 0;
    gl.target_len       = MACE_DEFAULT_TARGET_LEN;
    gl.config_len       = MACE_DEFAULT_TARGET_LEN;

    /* --- 3. Free arenas --- */
    mace_arena_free(&gl.arena);
    mace_arena_free(&gl.stackrena);
}

/*  Realloc _deps_links to bigger */
/*         if num close to len */
void mace_Target_Deps_Grow(Target *target) {
    size_t bytesize;
    if (target->pr._deps_links_len > 
        target->pr._deps_links_num)
        return;

    target->pr._deps_links_len *= MACE_GROW;
    bytesize =  target->pr._deps_links_len * 
                sizeof(*target->pr._deps_links);
    target->pr._deps_links = mace_realloc(target->pr._deps_links, bytesize / MACE_GROW, bytesize);
}

void mace_Target_Deps_Add(Target *target, u64 target_hash) {
    if (mace_Target_hasDep(target, target_hash))
        return;

    mace_Target_Deps_Grow(target);
    target->pr._deps_links[target->pr._deps_links_num++] = target_hash;
}

void mace_Target_Deps_Hash(Target *target) {
    /* --- Preliminaries --- */
    char *buffer;
    char *token;

    if ((target->links          == NULL) &&
        (target->dependencies   == NULL))
        return;

    /* --- Alloc space for deps --- */
    target->pr._deps_links_num =  0;
    target->pr._deps_links_len = 16;
    target->pr._deps_links = mace_calloc(target->pr._deps_links_len,
                                        sizeof(*target->pr._deps_links));
    /* --- Add links to _deps_links --- */
    do {
        size_t len_str;
        if (target->links == NULL)
            break;

        /* --- Copy links into modifiable buffer --- */
        len_str = strlen(target->links);
        buffer = _mace_calloc(&gl.stackrena, 1, (len_str + 1));
        memcpy(buffer, target->links, len_str);

        /* --- Split links into tokens, --- */
        token = strtok(buffer, MACE_SEPARATOR);
        /* --- Hash tokens into _deps_links --- */
        do {
            mace_Target_Deps_Add(target, mace_hash(token));
            token = strtok(NULL, MACE_SEPARATOR);
        } while (token != NULL);
        mace_pop(&gl.stackrena, (len_str + 1));
    } while (false);

    /* --- Add dependencies to _deps_links --- */
    do {
        size_t len_str = 0;
        if (target->dependencies == NULL)
            break;

        /* --- Copy deps into modifiable buffer --- */
        len_str = strlen(target->dependencies);
        buffer = _mace_calloc(&gl.stackrena, 1, (len_str + 1));
        memcpy(buffer, target->dependencies, len_str);

        /* --- Split links into tokens, --- */
        token = strtok(buffer, MACE_SEPARATOR);

        /* --- Hash tokens into _deps_links --- */
        do {
            mace_Target_Deps_Add(target, mace_hash(token));
            token = strtok(NULL, MACE_SEPARATOR);
        } while (token != NULL);
        mace_pop(&gl.stackrena, (len_str + 1));
    } while (false);
    mace_arena_clear(&gl.stackrena);
}

/******************* checksums ******************/
#define MACE_SRC_FOLDER_STR_LEN 4
#define MACE_INCLUDE_FOLDER_STR_LEN 8
#define MACE_SEPARATOR_STR_LEN 1

/*  Compute sha1dc checksum of file. */
struct Mace_Checksum_Stats {
    int      dot_i;
    int      slash_i;
    char    *dot;
    char    *slash;
    size_t   file_len;
    size_t   obj_dir_len;
    size_t   checksum_len;
};

struct Mace_Checksum_Stats mace_checksum_stats(
        const char *file, int mode) {
    struct Mace_Checksum_Stats out;

    /* last dot in path      */
    out.dot        = strrchr(file, '.');
    /* last slash in path    */
    out.slash      = strrchr(file, LINUX_SEPARATOR[0]);
    if (out.dot == NULL) {
        fprintf(stderr, "Could not find extension in filename.\n");
        exit(1);
    }

    /* File length of just file without extension */
    out.dot_i   = (int)(out.dot - file);
    out.slash_i = (out.slash == NULL) ? 0 : (int)(out.slash - file + 1);
    assert(out.dot_i > out.slash_i);
    out.file_len     = out.dot_i - out.slash_i;
    out.obj_dir_len  = strlen(MACE_OBJ_DIR);

    /* Alloc new file */
    out.checksum_len  = (out.file_len + MACE_SEPARATOR_STR_LEN + MACE_SHA1_EXT_LEN) +
                        out.obj_dir_len + 1;
    if (mode == MACE_CHECKSUM_MODE_SRC) {
        out.checksum_len += MACE_SRC_FOLDER_STR_LEN;
    } else if (mode == MACE_CHECKSUM_MODE_INCLUDE) {
        out.checksum_len += MACE_INCLUDE_FOLDER_STR_LEN;
    }

    return (out);
}

char *mace_checksum_filename(   const char *file,
                                int mode) {
    struct Mace_Checksum_Stats stats;
    size_t   total;
    char    *sha1;

    /* Files should be .c or .h */
    stats = mace_checksum_stats(file, mode);
    sha1 = mace_calloc(stats.checksum_len, sizeof(*sha1));

    memcpy(sha1, MACE_OBJ_DIR, stats.obj_dir_len);
    total = stats.obj_dir_len;

    /* Add slash to obj_dir if not present */
    if (sha1[stats.obj_dir_len - 1] != LINUX_SEPARATOR[0]) {
        memcpy(sha1 + total, LINUX_SEPARATOR, MACE_SEPARATOR_STR_LEN);
        total += MACE_SEPARATOR_STR_LEN;
    }

    /* Add mid folder */
    if (mode == MACE_CHECKSUM_MODE_SRC) {
        memcpy(sha1 + total, "src/", MACE_SRC_FOLDER_STR_LEN);
        total += MACE_SRC_FOLDER_STR_LEN;
    } else if (mode == MACE_CHECKSUM_MODE_INCLUDE) {
        memcpy(sha1 + total, "include/", MACE_INCLUDE_FOLDER_STR_LEN);
        total += MACE_INCLUDE_FOLDER_STR_LEN;
    }

    /* Add file name */
    memcpy(sha1 + total, file + stats.slash_i, stats.file_len);
    total += stats.file_len;

    /* Add extension */
    memcpy(sha1 + total, MACE_SHA1_EXT,
           MACE_SHA1_EXT_LEN);
    assert((strlen(sha1) + 1) == stats.checksum_len);
    return (sha1);
}

void mace_checksum_w(Mace_Checksum *checksum) {
    MACE_EARLY_RET(checksum->file == NULL, MACE_VOID, assert);

    checksum->file = fopen(checksum->checksum_path, "wb");
    if (checksum->file == NULL) {
        fprintf(stderr,
                "Could not write to checksum file '%s'\n",
                checksum->checksum_path);
        exit(1);
    }

    fwrite(checksum->hash_current, 1, SHA1DC_LEN, checksum->file);
    fclose(checksum->file);
    checksum->file = NULL;
}

void mace_checksum_r(Mace_Checksum *checksum) {
    size_t size;

    MACE_EARLY_RET(checksum->file != NULL, MACE_VOID, assert);
    fseek(checksum->file, 0, SEEK_SET);

    size = fread(   checksum->hash_previous, 1,
                    SHA1DC_LEN, checksum->file);
    if (size != SHA1DC_LEN) {
        fprintf(stderr, "Could not read checksum from '%s'. Try deleting it. \n", checksum->checksum_path);
        fclose(checksum->file);
        exit(1);
    }

    fclose(checksum->file);
    checksum->file = NULL;
}

b32 mace_file_changed(const char *checksum_path,
                      const char *file_path) {
    /* Returns true if
    **      1. hash changed.
    **      2. file didn't exist.
    ** Also writes new checksum file if changed */
    Mace_Checksum checksum  = {0};
    checksum.checksum_path  = checksum_path;
    checksum.file_path      = file_path;
    checksum.file = fopen(checksum.checksum_path, "rb");

    /* --- Did checksum file exist? --- */
    mace_checksum(&checksum);
    if (checksum.file == NULL) {
        mace_checksum_w(&checksum);
        return (true);
    }

    /* --- File exists, comparing checksums --- */
    mace_checksum_r(&checksum);
    if (!mace_checksum_cmp(&checksum)) {
        if (checksum.file != NULL) {
            fclose(checksum.file);
            checksum.file = NULL;
        }
        mace_checksum_w(&checksum);

        return (true);
    }
    return (0);
}

b32 mace_checksum_cmp(const Mace_Checksum *checksum) {
    return (memcmp( checksum->hash_current,
                    checksum->hash_previous,
                    SHA1DC_LEN) == 0);
}

void mace_checksum(Mace_Checksum *checksum) {
    /*  1. Compute hash of input file
    **  2. Check for collision input file and hash */
    int      foundcollision;
    char     buffer[USHRT_MAX + 1];
    FILE    *file;
    size_t   size;
    SHA1_CTX ctx2;

    MACE_EARLY_RET(checksum->file_path != NULL, MACE_VOID, assert);

    /* - open file - */
    file = fopen(checksum->file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "cannot open file: '%s'\n", checksum->file_path);
        exit(1);
    }

    /* - compute checksum - */
    SHA1DCInit(&ctx2);
    while (true) {
        size = fread(buffer, 1, (USHRT_MAX + 1), file);
        SHA1DCUpdate(&ctx2, buffer, (unsigned)(size));
        if (size != (USHRT_MAX + 1))
            break;
    }
    if (ferror(file)) {
        fprintf(stderr, "file read error: '%s'\n", checksum->file_path);
        exit(1);
    }
    if (!feof(file)) {
        fprintf(stderr, "not end of file?: '%s'\n", checksum->file_path);
        exit(1);
    }

    /* - check for collision - */
    foundcollision = SHA1DCFinal(checksum->hash_current, &ctx2);

    /* TODO: Any way to solve collision? */
    if (foundcollision) {
        fprintf(stderr, "sha1dc: collision detected");
        exit(1);
    }

    fclose(file);
}

/************** argument parsing **************/
/* list of parg options to be parsed, with usage */
#define LONGOPT_NUM 14
enum PARG_OPT_CATEGORIES {
    PARG_OPT_GENERAL    =  0,
    PARG_OPT_BUILD      =  6,
    PARG_OPT_OVERRIDE   =  9,
    PARG_OPT_LOG        = 12
};
static struct parg_opt longopts[LONGOPT_NUM] = {
    /* General options: */
    {"directory",   PARG_REQARG, 0, 'C',    "STR",  "Move to directory before anything else"},
    {"file",        PARG_REQARG, 0, 'f',    "STR",  "Specify macefile name. default: macefile.c"},
    {"config",      PARG_REQARG, 0, 'g',    "STR",  "Specify config name."},
    {"help",        PARG_NOARG,  0, 'h',    NULL,   "Display help and exit"},
    {"jobs",        PARG_REQARG, 0, 'j',    "INT",  "Allow N jobs at once"},
    {"version",     PARG_NOARG,  0, 'v',    NULL,   "Display version and exit"},
    /* Build options: */
    {"always-make", PARG_NOARG,  0, 'B',    NULL,   "Build all targets without condition"},
    {"c-flags",     PARG_REQARG, 0, 'F',    "STR",  "Additional flags passed to compiler"},
    {"dry-run",     PARG_NOARG,  0, 'n',    NULL,   "Don't build, just echo commands"},
    /* Override options: */
    {"ar",          PARG_REQARG, 0, 'a',    NULL,   "Override archiver"},
    {"cc",          PARG_REQARG, 0, 'c',    "STR",  "Override C compiler"},
    {"dep-flag",    PARG_REQARG, 0, 'E',    "STR",  "Override compiler create dependency file flag"},
    /* Log options: */
    {"debug",       PARG_NOARG,  0, 'd',    NULL,   "Print debug info"},
    {"silent",      PARG_NOARG,  0, 's',    NULL,   "Don't echo commands"}
};

Mace_Args Mace_Args_default = {
    /* .user_target         = */ NULL,
    /* .macefile            = */ NULL,
    /* .user_config         = */ NULL,
    /* .dir                 = */ NULL,
    /* .cc                  = */ NULL,
    /* .ar                  = */ NULL,
    /* .cflags              = */ NULL,
    /* .cc_depflag          = */ NULL,
    /* .user_target_hash    = */    0,
    /* .user_config_hash    = */    0,
    /* .jobs                = */ MACE_JOBS,
    /* .debug               = */ false,
    /* .silent              = */ false,
    /* .dry_run             = */ false,
    /* .build_all           = */ false
};

/*  Compare user flag input arguments */
/*         to enviroment variables */
Mace_Args mace_combine_args_env(Mace_Args user,
                                Mace_Args env) {
    Mace_Args out;
    /* Args Hierarchy: CLI args > Env > Defaults.
    **  - Use user values only if non-default. */

    /* Are CLI args default? */
    b32 _user_target        = (user.user_target         != Mace_Args_default.user_target);
    b32 _macefile           = (user.macefile            != Mace_Args_default.macefile);
    b32 _user_config        = (user.user_config         != Mace_Args_default.user_config);
    b32 _dir                = (user.dir                 != Mace_Args_default.dir);
    b32 _cc                 = (user.cc                  != Mace_Args_default.cc);
    b32 _ar                 = (user.ar                  != Mace_Args_default.cc);
    b32 _cflags             = (user.cflags              != Mace_Args_default.cflags);
    b32 _cc_depflag         = (user.cc_depflag          != Mace_Args_default.cc_depflag);
    b32 _user_target_hash   = (user.user_target_hash    != Mace_Args_default.user_target_hash);
    b32 _user_config_hash   = (user.user_config_hash    != Mace_Args_default.user_config_hash);
    b32 _jobs               = (user.jobs                >= 1);
    b32 _debug              = (user.debug               != Mace_Args_default.debug);
    b32 _silent             = (user.silent              != Mace_Args_default.silent);
    b32 _dry_run            = (user.dry_run             != Mace_Args_default.dry_run);
    b32 _build_all          = (user.build_all           != Mace_Args_default.build_all);

    /* If CLI arg non-default, use it. Otherwise use Env. */
    out.user_target      = _user_target      ? user.user_target      : env.user_target;
    out.macefile         = _macefile         ? user.macefile         : env.macefile;
    out.user_config      = _user_config      ? user.user_config      : env.user_config;
    out.dir              = _dir              ? user.dir              : env.dir;
    out.cc               = _cc               ? user.cc               : env.cc;
    out.ar               = _ar               ? user.ar               : env.ar;
    out.cflags           = _cflags           ? user.cflags           : env.cflags;
    out.cc_depflag       = _cc_depflag       ? user.cc_depflag       : env.cc_depflag;
    out.user_target_hash = _user_target_hash ? user.user_target_hash : env.user_target_hash;
    out.user_config_hash = _user_config_hash ? user.user_config_hash : env.user_config_hash;
    out.jobs             = _jobs             ? user.jobs             : env.jobs;
    out.debug            = _debug            ? user.debug            : env.debug;
    out.silent           = _silent           ? user.silent           : env.silent;
    out.dry_run          = _dry_run          ? user.dry_run          : env.dry_run;
    out.build_all        = _build_all        ? user.build_all        : env.build_all;
    return (out);
}

/*  Parse MACEFLAGS environment variable */
Mace_Args mace_parse_env(void) {
    char *env_args = getenv("MACEFLAGS");
    if (env_args != NULL) {
        Mace_Args out   = {0};
        int argc        = 1;
        int len         = 8;
        char *tmp       = env_args;
        char **argv;
        /* Count number of spaces, split into argv */
        while ((tmp = strstr(tmp, " "))) {
            argc++;
            tmp++;
        }

        argv = mace_calloc(len, sizeof(*argv));
        argv = mace_argv_flags(&len, &argc, argv, env_args, NULL, false, MACE_SEPARATOR);
        argc++;
        out = mace_parse_args(argc, argv);
        return (out);
    }

    return (Mace_Args_default);
}

char *mace_copy_str(const char *tocpy) {
    return(_mace_copy_str(&gl.arena, tocpy));
}

char *_mace_copy_str(   Mace_Arena *arena, 
                        const char *tocpy) {
    size_t   len = strlen(tocpy);
    char *out = _mace_calloc(arena, len + 1, sizeof(*out));
    memcpy(out, tocpy, len);
    return (out);
}

/*  Automatic usage/help printing */
void mace_help( const char              *name,
                const struct parg_opt   *longopts) {
    int i;
    b32 is_mace;

    MACE_EARLY_RET(name        != NULL, MACE_VOID, assert);
    MACE_EARLY_RET(longopts    != NULL, MACE_VOID, assert);

    is_mace = (name[0] == 'm') && (name[1] == 'a') &&
              (name[2] == 'c') && (name[3] == 'e');
    if (is_mace) {
        printf("\nmace convenience executable\n\n");
    } else {
        printf("\nmace builder executable: %s\n\n", name);
    }
    printf("Usage: %s [TARGET] [OPTIONS]\n\n", name);
    for (i = 0; longopts[i].doc; ++i) {
        if ((i >= LONGOPT_NUM) && !is_mace) {
            break;
        }

        if (i == PARG_OPT_GENERAL) {
            printf("General options:\n");
        } else if (i == PARG_OPT_BUILD) {
            printf("Build options:\n");
        } else if (i == PARG_OPT_OVERRIDE) {
            printf("Override options:\n");
        } else if (i == PARG_OPT_LOG) {
            printf("Log options:\n");
        }

        if (longopts[i].val)
            printf(" -%c,", longopts[i].val);
        else

            if (!longopts[i].val && longopts[i].name)
                printf("    ");

        if (longopts[i].name)
            printf("  --%-15s", longopts[i].name);

        if (longopts[i].arg) {
            printf("[=%s]", longopts[i].arg);
            printf("%*c", (int)(MACE_USAGE_MIDCOLW - 3 - strlen(longopts[i].arg)), ' ');
        } else if (longopts[i].val || longopts[i].name)
            printf("%*c", MACE_USAGE_MIDCOLW, ' ');

        if (longopts[i].doc)
            printf("%s", longopts[i].doc);
        printf("\n");
    }
}

/*  Parse builder/mace convenience */
/*         executable input args using parg */
Mace_Args mace_parse_args(int argc, char *argv[]) {
    int      c;
    int      longindex;
    char    *optstring;

    Mace_Args out_args      = Mace_Args_default;
    struct parg_state ps    = parg_state_default;

    MACE_EARLY_RET(argc > 1, out_args, MACE_nASSERT);

    optstring = "a:Bc:C:dE:f:F:g:hj:nP:o:sv:";
    while ((c = parg_getopt_long(&ps, argc, argv, optstring,
                                 longopts, &longindex)) != -1) {
        switch (c) {
            case 1:
                out_args.user_target = mace_copy_str(ps.optarg);
                out_args.user_target_hash = mace_hash(ps.optarg);
                break;
            case 'E':
                out_args.cc_depflag = mace_copy_str(ps.optarg);
                break;
            case 'F':
                out_args.cflags = mace_copy_str(ps.optarg);
                break;
            case 'a':
                out_args.ar = mace_copy_str(ps.optarg);
                break;
            case 'B':
                out_args.build_all = true;
                break;
            case 'C':
                out_args.dir = mace_copy_str(ps.optarg);
                break;
            case 'c':
                out_args.cc = mace_copy_str(ps.optarg);
                break;
            case 'd':
                out_args.debug = true;
                break;
            case 'f': {
                out_args.macefile = mace_copy_str(ps.optarg);
                break;
            }
            case 'g':
                out_args.user_config = mace_copy_str(ps.optarg);
                out_args.user_config_hash = mace_hash(ps.optarg);
                break;
            case 'h':
                mace_help(argv[0], longopts);
                exit(0);
                break;
            case 'j':
                out_args.jobs = atoi(ps.optarg);
                if (out_args.jobs < 1) {
                    fprintf(stderr, "Set number of jobs above 1.\n");
                }
                break;
            case 'n':
                out_args.dry_run = true;
                break;
            case 's':
                out_args.silent = true;
                break;
            case 'l':
                break;
            case 'v':
                printf("mace version %s\n", MACE_VER_STRING);
                exit(0);
            case '?':
                if (ps.optopt == 'C') {
                    printf("option -C/--directory requires an argument\n");
                } else if (ps.optopt == 'o') {
                    printf("option -o/--old-file requires an argument\n");
                } else if (ps.optopt == 'a') {
                    printf("option -a/--ar requires an argument\n");
                } else if (ps.optopt == 'j') {
                    printf("option -j/--jobs requires an argument\n");
                } else if (ps.optopt == 'F') {
                    printf("option -F/--c-flags requires an argument\n");
                } else if (ps.optopt == 'f') {
                    printf("option -f/--file requires an argument\n");
                } else if (ps.optopt == 'E') {
                    printf("option -E/--dep-flag requires an argument\n");
                } else if (ps.optopt == 'g') {
                    printf("option -g/--config requires an argument\n");
                } else {
                    printf("unknown option -%c\n", ps.optopt);
                }
                exit(1);
                break;
            default:
                printf("Error: unhandled option -%c\n", c);
                exit(1);
                break;
        }
    }

    return (out_args);
}

/******************* main *******************/

/* Override main for:
**  1. convenience_executable.c
**  2. benchmarks.c */
#ifndef MACE_OVERRIDE_MAIN
/* --- mace.h implementation of main --- */
/* To control what builder executable does:
**     1- Run "mace" function, getting user info:
**       - Compiler, Targets, Configs, Directories
**     2- Create target dependency graph
**     3- Find which target to recompile
**     4- Build the targets */
int main(int argc, char *argv[]) {
    /* --- Parse user arguments --- */
    Mace_Args args      = mace_parse_args(argc, argv);
    Mace_Args args_env  = mace_parse_env();
    args = mace_combine_args_env(args, args_env);

    /* --- Pre-user ops --- */
    /* Get cwd, alloc memory, set defaults. */
    mace_pre_user(&args);

    /* --- User ops --- */
    /* Sets compiler, add targets and commands. */
    mace(argc, argv);

    /* --- Post-user ops --- */
    /* Checks, allocs, default target. */
    mace_post_user(&args);

    /* --- Pre-build --- */
    /* Check which objects need recompilation */
    mace_pre_build();

    /* --- Build --- */
    /* Perform compilation with build_order */
    mace_build();

    /* --- Finish --- */
    /* Free everything */
    mace_post_build(&args);

    return (0);
}
#endif /* MACE_OVERRIDE_MAIN */

/*----------------------------------------------*/
/*               MACE SOURCE END                */
/*----------------------------------------------*/
