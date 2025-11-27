/*
**  Copyright 2023-2025 Gabriel Taillon
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
**  One step build (with mace convenience executable)
**     -1. Bootstrap: `gcc installer_macefile.c -o installer`
**      0. Install `mace`: `./installer`
**      1. Build: `mace`
**
*/

#define _XOPEN_SOURCE 500 /* Include POSIX 1995 */
#define SHA1DC_NO_STANDARD_INCLUDES
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <glob.h>
#include <ftw.h>

/*----------------------------------------------*/
/*                  PUBLIC API                  */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*               USER ENTRY POINT               */
/*----------------------------------------------*/

// The 'mace' function must be implemented by the user.
extern int mace(int argc, char *argv[]);
// Required:
//   1- Add targets         -> MACE_ADD_TARGET
// Optional:
//   2- Set compiler        -> MACE_SET_COMPILER
//   3- Set build_dir       -> MACE_SET_OBJ_DIR
//   4- Set obj_dir         -> MACE_SET_BUILD_DIR
//   5- Set separator       -> MACE_SET_SEPARATOR
//   6- Set default target  -> MACE_SET_DEFAULT_TARGET
//   7- Add configs         -> MACE_ADD_CONFIG
//      - First added config is default
//   8- Set target config   -> MACE_TARGET_CONFIG

/*----------------------------------------------*/
/*                   EXAMPLE                     /
*                 MACE FUNCTION                  /
* int mace(int argc, char *argv[]) {             /
*   MACE_SET_COMPILER(gcc);                      /
*   MACE_SET_OBJ_DIR(obj);                       /
*   MACE_SET_BUILD_DIR(build);                   /
*   ...                                          /
*   MACE_ADD_TARGET(foo);                        /
* };                                             /
*-----------------------------------------------*/

/* -- Types -- */
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;
typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;
typedef int32_t     b32;

/* -- Targets -- */
struct Target;

// Note: stringifies variable name for hashing
#define STRINGIFY(x) _STRINGIFY(x)
#define _STRINGIFY(x) #x

#define MACE_ADD_TARGET(target) mace_add_target(&target, STRINGIFY(target))

// When default target set by user, mace builds
// all only default target and its dependencies.
// If no default target is set,
// mace builds all targets.
#define MACE_SET_DEFAULT_TARGET(target) mace_set_default_target(#target)

/* -- Compiler -- */
// Compiler setting priority:
//      a- input argument (with -c,--cc)
//      b- config
//      c- macefile       (with MACE_SET_COMPILER)
#define MACE_SET_COMPILER(compiler) _MACE_SET_COMPILER(compiler)
#define _MACE_SET_COMPILER(compiler) mace_set_compiler(#compiler)

/* -- Directories -- */
/* - obj_dir - */
// Folder for intermediary files: .o, .d .sha1, etc.
#define  MACE_SET_OBJ_DIR(dir) _MACE_SET_OBJ_DIR(dir)
#define _MACE_SET_OBJ_DIR(dir)  mace_set_obj_dir(#dir)

/* - build_dir - */
// Folder for targets: binaries, libraries.
#define  MACE_SET_BUILD_DIR(dir) _MACE_SET_BUILD_DIR(dir)
#define _MACE_SET_BUILD_DIR(dir)  mace_set_build_dir(#dir)

/* -- Separator -- */
// Separator for files/folders in target
// member variables. Default is " ".
#define  MACE_SET_SEPARATOR(sep) _MACE_SET_SEPARATOR(sep)
#define _MACE_SET_SEPARATOR(sep)  mace_set_separator(#sep)

/* -- Configs -- */
struct Config;
// Note: stringifies variable name for hashing
#define MACE_ADD_CONFIG(config) mace_add_config(&config, #config)

// Set default config for target.
#define MACE_TARGET_CONFIG(target, config) mace_target_config(#target, #config)

/* -- Archiver -- */
// Archiver setting priority:
//      a- input argument (with -a,--ar)
//      b- config
//      c- macefile       (with MACE_SET_ARCHIVER)
#define MACE_SET_ARCHIVER(archiver) _MACE_SET_ARCHIVER(archiver)
#define _MACE_SET_ARCHIVER(archiver) mace_set_archiver(#archiver)

/* -- cc_depflag -- */
// Set cc_depflag: compiler flag to
// build .d dependency files.
// Ex: gcc -MM ...
#define MACE_SET_CC_DEPFLAG(cc_depflag) _MACE_SET_CC_DEPFLAG(cc_depflag)
#define _MACE_SET_CC_DEPFLAG(cc_depflag) mace_set_cc_depflag(#cc_depflag)

/* -- Target kinds -- */
enum MACE_TARGET_KIND { /* for target.kind */
    MACE_TARGET_NULL        = 0,
    MACE_EXECUTABLE,
    MACE_STATIC_LIBRARY,
    MACE_SHARED_LIBRARY,
    MACE_DYNAMIC_LIBRARY,
    MACE_PHONY,
    MACE_TARGET_KIND_NUM,
    MACE_JOBS_DEFAULT       = 12,
};

/******************* STRUCTS ******************/
typedef struct Target {
    /*------------- PUBLIC MEMBERS --------------*/
    const char *includes;   /* dirs */
    const char *sources;    /* files, dirs, glob */
    const char *excludes;   /* files */
    const char *base_dir;   /* dir  */

    /* Links are targets or libraries. 
    ** If target, its built before self. */
    const char *links;      /* libraries or targets */
    
    /* Linker flags are passed to the linker.
    ** Prepended "-Wl,", passed to compiler     */
    const char *link_flags; 
    
    /* Dependencies are targets,
    ** built before self. */
    const char *dependencies;   /* targets */
    const char *flags;          /* passed as is */

    const char *cmd_pre;    /* ran before build     */
    const char *cmd_post;   /* ran after  build     */
    const char *msg_pre;    /* printed before build */
    const char *msg_post;   /* printed after build  */

    int kind;   /* MACE_TARGET_KIND */

    /* allatonce: Compile all .o with one gcc call.
    ** It's slightly faster.
    ** WARNING: DOES NOT WORK if multiple source
    ** files have the same filename. */
    b32 allatonce;

    /*-------------------------------------------------*/
    /*                      EXAMPLE                     /
    **                 TARGET DEFINITION                /
    **                                                  /
    ** Target mytarget = {                              /
    **     .includes           = "include include/foo", /
    **     .sources            = "src/'*'' src/bar.c",  /
    **     .sources_exclude    = "src/main.c",          /
    **     .dependencies       = "mytarget1",           /
    **     .links              = "lib1 lib2 mytarget2", /
    **     .kind               = MACE_LIBRARY,          /
    ** };                                               /
    **      NOTE: default separator is " ",             /
    **      set with 'mace_set_separator'               /
    *                                                   /
    *--------------------------------------------------*/

    /*----------------- PRIVATE MEMBERS ---------------*/
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

    // WARNING: _argv_objects_hash DOES NOT
    // include objects with number to prevent
    // collisions!
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
    /* Note: Same number of _headers and _headers_checksum */
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
} Target;

typedef struct Config {
    /*----------------- PUBLIC MEMBERS --------------*/
    char *cc;           /* compiler     */
    char *ar;           /* archiver     */
    const char *flags;  /* passed as is */

    /*-------------------------------------------------*/
    /*                   EXAMPLE                        /
    **               CONFIG DEFINITION                  /
    **                                                  /
    ** Config myconfig = {                              /
    **     .target             = "foo",                 /
    **     .flags              = "-g -O0 -rdynamic",    /
    ** };                                               /
    **    NOTE: default separator is " ",               /
    **          set with 'mace_set_separator'           /
    **                                                  /
    *--------------------------------------------------*/

    /*---------------- PRIVATE MEMBERS ----------------*/
    /* config name                          */
    char    *_name;
    /* config name hash                     */
    u64      _hash;
    /* config order added by user           */
    int      _order;

    u64      _target_order;
    
    char   **_flags;
    
    /* Number of flags                      */
    int      _flag_num;
} Config;

/*-----------------------------------------------*/
/*                   PRIVATE                     */
/*-----------------------------------------------*/

typedef struct Mace_Args {
    char *user_target;
    char *macefile;
    char *user_config;
    char *dir;
    char *cc;
    char *ar;
    u64   user_target_hash;
    u64   user_config_hash;
    int   jobs;
    b32   debug;
    b32   silent;
    b32   dry_run;
    b32   build_all;
} Mace_Args;
void Mace_Args_Free(Mace_Args *args);

/***************** CONSTANTS ****************/
#define MACE_VER_PATCH 3
#define MACE_VER_MINOR 0
#define MACE_VER_MAJOR 1
#define MACE_VER_STRING "3.0.1"
#define MACE_USAGE_MIDCOLW 12

/* Reserved targets */
#define MACE_ALL "all"

enum MACE {
    MACE_DEFAULT_TARGET_LEN     =    8,
    MACE_MAX_ITERATIONS         = 1024,
    MACE_DEFAULT_OBJECT_LEN     =   16,
    MACE_CWD_BUFFERSIZE         =  256,
    SHA1_LEN                    =   20, /* [bytes] */
    MACE_OBJDEP_BUFFER          = 4096,
};

enum MACE_CONFIG {
    MACE_NULL_CONFIG            =  -1,
    MACE_DEFAULT_CONFIG         =   0,
};

enum MACE_RESERVED_TARGETS {
    /* Order of ALL target */
    MACE_NULL_ORDER             =   -2,
    MACE_ALL_ORDER              =   -1,
    MACE_ORDER_START            =    0,
    MACE_RESERVED_TARGETS_NUM   =    2,
};

enum MACE_ARGV {
    /* single source compilation */
    MACE_ARGV_CC                =    0,
    MACE_ARGV_SOURCE,
    MACE_ARGV_OBJECT,
    MACE_ARGV_OTHER,
};

enum MACE_CHECKSUM_MODE {
    MACE_CHECKSUM_MODE_NULL     =    0,
    MACE_CHECKSUM_MODE_SRC,
    MACE_CHECKSUM_MODE_INCLUDE,
};

/****************** DECLARATIONS ******************/
/* --- mace --- */
static void mace_build(void);
static void mace_pre_build(void);

static void mace_pre_user(     Mace_Args *args);
static void mace_post_user(    Mace_Args *args);
static void mace_post_build(   Mace_Args *args);

/* --- mace_args --- */
static Mace_Args mace_parse_args(int     argc,
                                 char   *argv[]);
static Mace_Args mace_parse_env(void);
static Mace_Args mace_combine_args_env(Mace_Args args, 
                                       Mace_Args env);

/* --- mace_utils --- */
static char  *mace_str_buffer(const char *const strlit);

/* --- mace_checksum --- */
static void mace_sha1dc(char *file,
                        u8 hash2[SHA1_LEN]);
static b32  mace_sha1dc_cmp(u8 hash1[SHA1_LEN],
                            u8 hash2[SHA1_LEN]);
static char *mace_checksum_filename(char *file, int mode);

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
static char *mace_set_obj_dir(      char *obj);
static void  mace_set_compiler(     char *cc);
static void  mace_set_archiver(     char *ar);
static char *mace_set_build_dir(    char *build);
static void  mace_set_cc_depflag(   char *depflag);

/* --- mace add --- */
static void mace_add_target(Target *target, char *name);
static void mace_add_config(Config *config, char *name);

/* --- mace target config --- */
static void mace_target_config(char *ntarget,
                               char *nconfig);

/* -- Config struct OOP -- */
static void mace_Config_Free(    Config *config);

/* -- Target struct OOP -- */
/* - Free - */
static void mace_Target_Free(             Target *target);
static void mace_Target_Free_argv(        Target *target);
static void mace_Target_Free_notargv(     Target *target);
static void mace_Target_Free_excludes(    Target *target);
static void mace_Target_Free_deps_headers(Target *target);

/* - Grow - */
static void mace_Target_Grow_Headers(     Target *target);
static void mace_Target_Grow_deps_headers(Target *target,
                                          int obj_hash_id);

/* - hash - */
static int  Target_hasObjectHash(           Target *target,
                                            u64 hash);
static void Target_Object_Hash_Add(         Target *target,
                                            u64 hash);
static int  Target_hasObjectHash_nocoll(    Target *target,
                                            u64 hash);
static void Target_Object_Hash_Add_nocoll(  Target *target,
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
static b32 mace_Target_hasDep(      Target *target,
                                    u64 hash);
static void mace_Target_Deps_Add(   Target *target,
                                    u64 hash);
static void mace_Target_Deps_Hash(  Target *target);
static void mace_Target_Deps_Grow(  Target *target);

/* - Adding Files - */
static b32  mace_Target_Source_Add( Target *target,
                                    char *token);
static b32  mace_Target_Object_Add( Target *target,
                                    char *token);
static u64  mace_Target_Header_Add( Target *target,
                                    char *header);
static void mace_Target_Objdep_Add( Target *target,
                                    int header_order,
                                    int obj_hash_id);
static void mace_Target_Header_Add_Objpath(Target *target,
                                           char *header);

/* - Checksums - */
static b32 mace_Source_Checksum(Target *target,
                                char *s,
                                char *o);
static void mace_Headers_Checksums(       Target *target);
static void mace_Headers_Checksums_Checks(Target *target);

/* - argv - */
static void mace_argv_add_config(Target *target,
                                 char ** *argv,
                                 int *argc,
                                 int *arg_len);

static void mace_Target_argv_grow(   Target *target);
static void mace_Target_Parse_User(  Target *target);
static void mace_Target_argv_compile(Target *target);
static void mace_Target_Parse_Source(Target *target,
                                     char *path,
                                     char *src);
static void mace_Target_argv_allatonce(Target *target);

static char **mace_argv_grow(char   **argv,
                             int     *argc,
                             int     *arg_len);
static void   mace_argv_free(char   **argv,
                             int      argc);

/* - recompilation flag - */
static void mace_Target_Recompiles_Add(Target *target,
                                       b32 add);

/* - compilation - */
static void mace_Target_compile(          Target *target);
static void mace_Target_precompile(       Target *target);
static void mace_Target_compile_allatonce(Target *target);

/* --- mace_glob --- */
static int     mace_globerr(const char *path, int eerrno);
static glob_t  mace_glob_sources(const char *path);

/* --- mace_exec --- */
static pid_t mace_exec(const char *exec,
                       char *const arguments[]);
static void  mace_wait_pid(int pid);
static void  mace_exec_print(char *const arguments[],
                             size_t argnum);

/* --- mace_build --- */
/* -- linking -- */
static void mace_link_executable(      Target *target);
static void mace_link_static_library(  Target *target);
static void mace_link_dynamic_library( Target *target);

typedef void (*mace_link_t)(Target *);
mace_link_t mace_link[MACE_TARGET_KIND_NUM - 1] = {mace_link_executable,
    mace_link_static_library,
    mace_link_dynamic_library
};

/* -- compiling object files -> .o -- */
static void mace_compile_glob(  Target      *target,
                                char        *globsrc,
                                const char  *flags);
static void mace_build_target(  Target      *target);
static void mace_run_commands(  const char  *commands, 
                                const char  *preorpost,
                                const char  *target_name);
static void mace_print_message( const char  *message);

/* -- build_order -- */
static b32 mace_in_build_order(size_t    order,
                               int      *build_order,
                               int       num);
static void mace_user_target_set(u64     hash,
                                 char   *name);
static void mace_user_config_set(u64     hash,
                                 char   *name);
static void mace_config_resolve(Target  *target);
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
static void  mace_object_path(char  *source);
static char *mace_library_path(char *target_name, int kind);
static char *mace_checksum_filename(char *file, int mode);
static char *mace_executable_path(const char *target_name);

/* --- mace_pqueue --- */
static void  mace_pqueue_put(pid_t pid);
static pid_t mace_pqueue_pop(void);

/* --- mace utils --- */
static void mace_chdir(const char * path);

/******************* GLOBALS ********************/
#define false 0
#define true 1

static b32 silent     = false;
static b32 verbose    = false;
/* Pre-compile, don't compile */
static b32 dry_run    = false; 
/* Don't check object dependencies */
static b32 build_all  = false; 

/* --- Processes --- */
// 1. Compile objects in parallel.
// 2. Compile targets in series.
static pid_t   *pqueue  = NULL;
static int      pnum    =  0;
static int      plen    = -1;

/* -- separator -- */
static char *mace_separator             = " ";
static char *mace_command_separator     = "&&";

    /* -- Compiler -- */
    static char *cc         = "gcc";
    static char *ar         = "ar";
    /* flag to create .d file */
    static char *cc_depflag = "-MM";

    /* -- current working directory -- */
    static char cwd[MACE_CWD_BUFFERSIZE];

    /* -- Reserved targets hashes -- */
    static u64 mace_reserved_targets[MACE_RESERVED_TARGETS_NUM];
    static u64 mace_default_target_hash = 0;
    static u64 mace_default_config_hash = 0;

    /* Default target (may be set by user) [order] */
    static int mace_default_target = MACE_ALL_ORDER; 
    /* Target input by user */
    static int mace_user_target    = MACE_NULL_ORDER;
    /* Target to compile */
    static int mace_target         = MACE_NULL_ORDER;

    /* Default config (may be set by user) [order] */
    static int mace_default_config = MACE_DEFAULT_CONFIG;
    /* Config input by user */
    static int mace_user_config    = MACE_NULL_CONFIG;
    /* Config to use */
    static int mace_config         = MACE_DEFAULT_CONFIG;

    /* -- build order for user target -- */
    static int *build_order     = NULL;
    static int  build_order_num = 0;

    /* -- list of targets added by user -- */
    /* [order] as added    */
    static Target   *targets     = NULL;
    static size_t    target_num  = 0;
    static size_t    target_len  = 0;

    /* -- list of configs added by user -- */
    /* [order] as added    */
    static Config   *configs     = NULL;
    static size_t    config_num  = 0;
    static size_t    config_len  = 0;

    /* -- buffer to write object -- */
    static char     *object      = NULL;
    static size_t    object_len  = 0;

    /* -- directories -- */
    /* intermediary files  */
    static char     *obj_dir     = NULL;
    /* targets */
    static char     *build_dir   = NULL;

    /* -- mace_globals control -- */
    static void mace_object_grow(void);

/*************** SHA1DC DECLARATION ***************/

/***
* Copyright 2017 Marc Stevens <marc@marc-stevens.nl>, Dan Shumow <danshu@microsoft.com>
* Distributed under the MIT Software License.
* See accompanying file LICENSE.txt or copy at
* https://opensource.org/licenses/MIT
***/

#ifndef SHA1DC_SHA1_H
#define SHA1DC_SHA1_H

#ifndef SHA1DC_NO_STANDARD_INCLUDES
    #include <stdint.h>
#endif /* SHA1DC_NO_STANDARD_INCLUDES */

/* sha-1 compression function that takes an already expanded message, and additionally store intermediate states */
/* only stores states ii (the state between step ii-1 and step ii) when DOSTORESTATEii is defined in ubc_check.h */
void sha1_compression_states(u32[5], const u32[16], u32[80], u32[80][5]);

/*
// Function type for sha1_recompression_step_T (u32 ihvin[5], u32 ihvout[5], const u32 me2[80], const u32 state[5]).
// Where 0 <= T < 80
//       me2 is an expanded message (the expansion of an original message block XOR'ed with a disturbance vector's message block difference.)
//       state is the internal state (a,b,c,d,e) before step T of the SHA-1 compression function while processing the original message block.
// The function will return:
//       ihvin: The reconstructed input chaining value.
//       ihvout: The reconstructed output chaining value.
*/
typedef void(*sha1_recompression_type)(u32 *, u32 *, const u32 *, const u32 *);

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

/*
    Function to enable safe SHA-1 hashing:
    Collision attacks are thwarted by hashing a detected near-collision block 3 times.
    Think of it as extending SHA-1 from 80-steps to 240-steps for such blocks:
        The best collision attacks against SHA-1 have complexity about 2^60,
        thus for 240-steps an immediate lower-bound for the best cryptanalytic attacks would be 2^180.
        An attacker would be better off using a generic birthday search of complexity 2^80.

   Enabling safe SHA-1 hashing will result in the correct SHA-1 hash for messages where no collision attack was detected,
   but it will result in a different SHA-1 hash for messages where a collision attack was detected.
   This will automatically invalidate SHA-1 based digital signature forgeries.
   Enabled by default.
*/
void SHA1DCSetSafeHash(SHA1_CTX *, int);

/*
    Function to disable or enable the use of Unavoidable Bitconditions (provides a significant speed up).
    Enabled by default
 */
void SHA1DCSetUseUBC(SHA1_CTX *, int);

/*
    Function to disable or enable the use of Collision Detection.
    Enabled by default.
 */
void SHA1DCSetUseDetectColl(SHA1_CTX *, int);

/* function to disable or enable the detection of reduced-round SHA-1 collisions */
/* disabled by default */
void SHA1DCSetDetectReducedRoundCollision(SHA1_CTX *, int);

/* function to set a callback function, pass NULL to disable */
/* by default no callback set */
void SHA1DCSetCallback(SHA1_CTX *, collision_block_callback);

/* update SHA-1 context with buffer contents */
void SHA1DCUpdate(SHA1_CTX *, const char *, size_t);

/* obtain SHA-1 hash from SHA-1 context */
/* returns: 0 = no collision detected, otherwise = collision found => warn user for active attack */
int  SHA1DCFinal(unsigned char[SHA1_LEN], SHA1_CTX *);

#ifdef SHA1DC_CUSTOM_TRAILING_INCLUDE_SHA1_H
    #include SHA1DC_CUSTOM_TRAILING_INCLUDE_SHA1_H
#endif /* SHA1DC_CUSTOM_TRAILING_INCLUDE_SHA1_H */

#endif /* SHA1DC_SHA1_H */
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
*/

#ifndef SHA1DC_UBC_CHECK_H
#define SHA1DC_UBC_CHECK_H

#ifndef SHA1DC_NO_STANDARD_INCLUDES
    #include <stdint.h>
#endif /* SHA1DC_NO_STANDARD_INCLUDES */

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
extern dv_info_t sha1_dvs[];
void ubc_check(const u32 W[80], u32 dvmask[DVMASKSIZE]);

#define DOSTORESTATE58
#define DOSTORESTATE65

#define CHECK_DVMASK(_DVMASK) (0 != _DVMASK[0])

#ifdef SHA1DC_CUSTOM_TRAILING_INCLUDE_UBC_CHECK_H
    #include SHA1DC_CUSTOM_TRAILING_INCLUDE_UBC_CHECK_H
#endif /* SHA1DC_CUSTOM_TRAILING_INCLUDE_UBC_CHECK_H */

#endif /* SHA1DC_UBC_CHECK_H */
/************* SHA1DC DECLARATION END *************/

/**************** PARG DECLARATION ****************/
/*
** parg - parse argv
**
** Modified in 2023 by Gabriel Taillon for IES
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

#ifndef PARG_INCLUDED
#define PARG_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PARG_VER_MAJOR 1
#define PARG_VER_MINOR 0
#define PARG_VER_PATCH 2
#define PARG_VER_STRING "1.0.2"

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
extern struct parg_state parg_state_default;

/* Long options for `parg_getopt_long()` */
struct parg_opt {
    const char *name;
    int has_arg;
    int *flag;
    int val;
    const char *arg;
    const char *doc;
};

/* - parg help - */
extern void mace_parg_usage(const char *n, const struct parg_opt *lo);

/* - option matching - */
extern int match_long(struct parg_state *ps, int c, char *const v[], const char *o,
                      const struct parg_opt *lo, int *li);
extern int match_short(struct parg_state *ps, int c, char *const v[], const char *os);

/* - utilities - */
extern void reverse(char *v[], int i, int j);
extern int is_argv_end(const struct parg_state *ps, int c, char *const v[]);

/* - argv reordering - */
extern int parg_reorder_simple(int c, char *v[], const char *os, const struct parg_opt *lo);
extern int parg_reorder(int c, char *v[], const char *os, const struct parg_opt *lo);

/* - parg public API: getopt and getopt_long - */
extern int parg_getopt(struct parg_state *ps, int c, char *const v[], const char *os);
extern int parg_getopt_long(struct parg_state *ps, int c, char *const v[],
                            const char *os, const struct parg_opt *lo, int *li);


#endif /* PARG_INCLUDED */
/*************** PARG DECLARATION END ***************/

/******************* SHA1DC SOURCE ******************/
/***
* Copyright 2017 Marc Stevens <marc@marc-stevens.nl>, Dan Shumow (danshu@microsoft.com)
* Distributed under the MIT Software License.
* See accompanying file LICENSE.txt or copy at
* https://opensource.org/licenses/MIT
***/

#ifndef SHA1DC_NO_STANDARD_INCLUDES
    #include <string.h>
    #include <memory.h>
    #include <stdio.h>
    #include <stdlib.h>
    #ifdef __unix__
        #include <sys/types.h> /* make sure macros like _BIG_ENDIAN visible */
    #endif /* __unix__ */
#endif /* SHA1DC_NO_STANDARD_INCLUDES */

#ifdef SHA1DC_CUSTOM_INCLUDE_SHA1_C
    #include SHA1DC_CUSTOM_INCLUDE_SHA1_C
#endif /* SHA1DC_CUSTOM_INCLUDE_SHA1_C */

#ifndef SHA1DC_INIT_SAFE_HASH_DEFAULT
    #define SHA1DC_INIT_SAFE_HASH_DEFAULT 1
#endif /* SHA1DC_INIT_SAFE_HASH_DEFAULT */

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

#ifdef BUILDNOCOLLDETECTSHA1COMPRESSION
void sha1_compression(u32 ihv[5], const u32 m[16]) {
    u32 W[80];
    u32 a, b, c, d, e;
    unsigned i;

    memcpy(W, m, 16 * 4);
    for (i = 16; i < 80; ++i)
        W[i] = sha1_mix(W, i);

    a = ihv[0];
    b = ihv[1];
    c = ihv[2];
    d = ihv[3];
    e = ihv[4];

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
#endif /*BUILDNOCOLLDETECTSHA1COMPRESSION*/


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

void SHA1DCSetSafeHash(SHA1_CTX *ctx, int safehash) {
    if (safehash)
        ctx->safe_hash = 1;
    else
        ctx->safe_hash = 0;
}


void SHA1DCSetUseUBC(SHA1_CTX *ctx, int ubc_check) {
    if (ubc_check)
        ctx->ubc_check = 1;
    else
        ctx->ubc_check = 0;
}

void SHA1DCSetUseDetectColl(SHA1_CTX *ctx, int detect_coll) {
    if (detect_coll)
        ctx->detect_coll = 1;
    else
        ctx->detect_coll = 0;
}

void SHA1DCSetDetectReducedRoundCollision(SHA1_CTX *ctx, int reduced_round_coll) {
    if (reduced_round_coll)
        ctx->reduced_round_coll = 1;
    else
        ctx->reduced_round_coll = 0;
}

void SHA1DCSetCallback(SHA1_CTX *ctx, collision_block_callback callback) {
    ctx->callback = callback;
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

int SHA1DCFinal(unsigned char output[SHA1_LEN], SHA1_CTX *ctx) {
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

#ifdef SHA1DC_CUSTOM_TRAILING_INCLUDE_SHA1_C
    #include SHA1DC_CUSTOM_TRAILING_INCLUDE_SHA1_C
#endif /* SHA1DC_CUSTOM_TRAILING_INCLUDE_SHA1_C */
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

// #ifndef SHA1DC_NO_STANDARD_INCLUDES
//     #include <stdint.h>
// #endif
#ifdef SHA1DC_CUSTOM_INCLUDE_UBC_CHECK_C
    #include SHA1DC_CUSTOM_INCLUDE_UBC_CHECK_C
#endif

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

#ifdef SHA1DC_CUSTOM_TRAILING_INCLUDE_UBC_CHECK_C
    #include SHA1DC_CUSTOM_TRAILING_INCLUDE_UBC_CHECK_C
#endif

/**************** SHA1DC SOURCE END ***************/

/******************* PARG SOURCE ******************/

struct parg_state parg_state_default = {
    .optarg = NULL,
    .optind = 1,
    .optopt = '?',
    .nextchar = NULL
};

/// @brief Automatic usage/help printing
void mace_parg_usage(   const char              *name,
                        const struct parg_opt   *longopts) {
    if (name        == NULL) {
        assert(0);
        return;
    }
    if (longopts    == NULL) {
        assert(0);
        return;
    }
    b32 is_mace =   (name[0] == 'm') && (name[1] == 'a') &&
                    (name[2] == 'c') && (name[3] == 'e');
    if (is_mace) {
        printf("\nmace convenience executable\n");
    } else {
        printf("\nmace builder executable: %s\n", name);
    }
    printf("Usage: %s [TARGET] [OPTIONS]\n", name);
    for (int i = 0; longopts[i].doc; ++i) {
        if ((i >= 11) && !is_mace) {
            break;
        }
        if (longopts[i].val)
            printf(" -%c", longopts[i].val);

        if (longopts[i].name)
            printf(",  --%-15s", longopts[i].name);

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

/// @brief Invert a string from i to j
void reverse(char *v[], int i, int j) {
    while (j - i > 1) {
        char *tmp = v[i];
        v[i] = v[j - 1];
        v[j - 1] = tmp;
        ++i;
        --j;
    }
}

/* Check if state is at end of argv */
int is_argv_end(const struct parg_state *ps, int argc,
                char *const argv[]) {
    return ps->optind >= argc || argv[ps->optind] == NULL;
}

// *INDENT-OFF*
/* Match string at nextchar against longopts. */
int match_long( struct parg_state *ps, int argc,
                char *const argv[], const char *optstring,
                const struct parg_opt *longopts,
                int *longindex) {
    int i, match = -1, num_match = 0;
    size_t len = strcspn(ps->nextchar, "=");

    for (i = 0; longopts[i].name; ++i) {
        if (strncmp(ps->nextchar, longopts[i].name, len) == 0) {
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
// *INDENT-ON*


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
// *INDENT-OFF*
int parg_getopt_long(struct parg_state *ps, int argc, char *const argv[],
                     const char *optstring, const struct parg_opt *longopts, int *longindex) {
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
// *INDENT-ON*

/*
 * Reorder elements of `argv` with no special cases.
 *
 * This function assumes there is no `--` element, and the last element
 * is not an option missing a required argument.
 *
 * The algorithm is described here:
 * http://hardtoc.com/2016/11/07/reordering-arguments.html
 */
int parg_reorder_simple(int argc, char *argv[], const char *optstring,
                        const struct parg_opt *longopts) {
    struct parg_state ps;
    int change, l = 0, m = 0, r = 0;

    if (argc < 2) {
        return argc;
    }

    do {
        int c, nextind;
        ps = parg_state_default;
        nextind = ps.optind;

        /* Parse until end of argument */
        do {
            c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
        } while (ps.nextchar != NULL && *ps.nextchar != '\0');

        change = 0;

        do {
            /* Find next non-option */
            for (l = nextind; c != 1 && c != -1;) {
                l = ps.optind;

                do {
                    c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
                } while (ps.nextchar != NULL && *ps.nextchar != '\0');
            }

            /* Find next option */
            for (m = l; c == 1;) {
                m = ps.optind;

                do {
                    c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
                } while (ps.nextchar != NULL && *ps.nextchar != '\0');
            }

            /* Find next non-option */
            for (r = m; c != 1 && c != -1;) {
                r = ps.optind;

                do {
                    c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
                } while (ps.nextchar != NULL && *ps.nextchar != '\0');
            }

            /* Find next option */
            for (nextind = r; c == 1;) {
                nextind = ps.optind;

                do {
                    c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
                } while (ps.nextchar != NULL && *ps.nextchar != '\0');
            }

            if (m < r) {
                change = 1;
                reverse(argv, l, m);
                reverse(argv, m, r);
                reverse(argv, l, r);
            }
        } while (c != -1);
    } while (change != 0);

    return l + (r - m);
}

/**
 * Parse next short option in `argv`.
 * Check GNU getopt example for details:
 * https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
 */
int parg_getopt(struct parg_state *ps, int argc, char *const argv[], const char *optstring) {
    return parg_getopt_long(ps, argc, argv, optstring, NULL, NULL);
}

/**
 * Reorder elements of `argv` so options appear first.
 *
 * If there are no long options, `longopts` may be `NULL`.
 *
 * The return value can be used as `argc` parameter for `parg_getopt()` and
 * `parg_getopt_long()`.
 *
 * @param argc number of elements in `argv`
 * @param argv array of pointers to command-line arguments
 * @param optstring string containing option characters
 * @param longopts array of `parg_option` structures
 * @return index of first argument in `argv` on success, `-1` on error
 */
int parg_reorder(int argc, char *argv[], const char *optstring,
                 const struct parg_opt *longopts) {
    struct parg_state ps;
    int lastind, optend, c;

    assert(argv != NULL);
    assert(optstring != NULL);

    if (argc < 2) {
        return argc;
    }

    ps = parg_state_default;

    /* Find end of normal arguments */
    do {
        lastind = ps.optind;

        c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);

        /* Check for trailing option with error */
        if ((c == '?' || c == ':') && is_argv_end(&ps, argc, argv)) {
            lastind = ps.optind - 1;
            break;
        }
    } while (c != -1);

    optend = parg_reorder_simple(lastind, argv, optstring, longopts);

    /* Rotate `--` or trailing option with error into position */
    if (lastind < argc) {
        reverse(argv, optend, lastind);
        reverse(argv, optend, lastind + 1);
        ++optend;
    }

    return optend;
}

// *INDENT-OFF*
int parg_zgetopt_long(struct parg_state *ps, int argc, char *const argv[], 
                      const char *optstring,const struct parg_opt *longopts, int *longindex) {
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

        /* Check for argument element (including '-') */
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
// *INDENT-ON*

/**************** PARG SOURCE END ***************/

/*----------------------------------------------*/
/*                MACE SOURCE                   */
/*----------------------------------------------*/
#define Vprintf(...) do {\
        if (verbose)\
            printf(__VA_ARGS__);\
    } while(0)

#define VSprintf(...) do {\
        if (verbose && !silent)\
            printf(__VA_ARGS__);\
    } while(0)

#define Sprintf(...) do {\
        if (!silent) {\
            printf(__VA_ARGS__);\
        }\
    } while(0)

#define MACE_MEMCHECK(var) do {\
    if (var == NULL) {\
        fprintf(stderr, "Out of memory\n");\
        assert(0);\
        exit(1);\
    }\
    } while(0)

#define MACE_FREE(var) do {\
    if (var != NULL) {\
        free(var);\
        var = NULL;\
    }\
    } while(0)

/***************** MACE_ADD_CONFIG *****************/
/// @brief Add config to list of configs. 
///     Note: Expects name to be stringified
///           config variable name
void mace_add_config(Config *config, char *name) {
    configs[config_num]        = *config;
    u64 hash = mace_hash(name);
    configs[config_num]._name  = name;
    configs[config_num]._hash  = hash;
    configs[config_num]._order = target_num;
    if (++config_num >= config_len) {
        config_len *= 2;
        size_t bytesize = config_len * sizeof(*targets);
        configs = realloc(configs, bytesize);
    }
}

/**************** MACE_ADD_TARGET ***************/
/// @brief Add target to list of targets. 
///     Note: Expects name to be stringified
///           config variable name
void mace_add_target(Target *target, char *name) {
    targets[target_num]          = *target;
    targets[target_num]._name    = name;
    u64 hash = mace_hash(name);
    for (int i = 0; i < MACE_RESERVED_TARGETS_NUM; i++) {
        if (hash == mace_reserved_targets[i]) {
            fprintf(stderr,  "Error: '%s' is a reserved target name.\n", name);
            exit(1);
        }
    }
    targets[target_num]._hash    = hash;
    targets[target_num]._order   = target_num;
    targets[target_num]._checkcwd = true;
    mace_Target_Deps_Hash(&targets[target_num]);
    mace_Target_Parse_User(&targets[target_num]);
    mace_Target_argv_compile(&targets[target_num]);
    if (++target_num >= target_len) {
        target_len *= 2;
        size_t bytesize = target_len * sizeof(*targets);
        targets     = realloc(targets, bytesize);
        memset(targets + target_len / 2, 0, bytesize / 2);
        bytesize = target_len * sizeof(*build_order);
        build_order = realloc(build_order, bytesize);
    }
}

/// @brief Set target built by default when
///        running mace without target
void mace_set_default_target(char *name) {
    mace_default_target_hash = (name == NULL) ? 0 : mace_hash(name);
}

/// @brief Compute default target order from hash.
///        Called post-user. 
void mace_default_target_order(void) {
    if (mace_default_target_hash == 0)
        return;

    for (int i = 0; i < target_num; i++) {
        if (mace_default_target_hash == targets[i]._hash) {
            mace_default_target = i;
            return;
        }
    }

    fprintf(stderr, "Default target not found. Exiting\n");
    exit(1);
}

/// @brief Set config used to build targets by default
void mace_set_default_config(char *name) {
    mace_default_config_hash = (name == NULL) ? 0 : mace_hash(name);
}

/// @brief Compute default target order from hash.
///        Called post-user.
void mace_default_config_order(void) {
    if (mace_default_config_hash == 0)
        return;

    for (int i = 0; i < config_num; i++) {
        if (mace_default_config_hash == configs[i]._hash) {
            mace_default_config = i;
            return;
        }
    }

    fprintf(stderr, "Default config not found. Exiting\n");
    exit(1);
}

/// @brief Find user config from its hash.
void mace_user_config_set(u64 hash, char *name) {
    if (hash == 0)
        return;

    for (int i = 0; i < config_num; i++) {
        if (hash == configs[i]._hash) {
            mace_user_config = i;
            return;
        }
    }

    fprintf(stderr, "User config '%s' not found. Exiting\n", name);
    exit(1);
}

/// @brief Decide if user target or default
///        target should be compiled.
void mace_target_resolve(void) {
    /* Target priority: */
    //  - user      target
    //  - default   target

    if ((mace_user_target >= MACE_ORDER_START) && (mace_user_target < target_num)) {
        /* Using user target */
        mace_target = mace_user_target;
        return;
    }

    /* Using default target (may be set by user) */
    mace_target = mace_default_target;
}

/// @brief Decide if user config or default
///        config should be used.
void mace_config_resolve(Target *target) {
    /* Config priority: */
    //  - user      config
    //  - target    config
    //  - default   config

    if ((mace_user_config >= MACE_DEFAULT_CONFIG) &&
        (mace_user_config < config_num)) {
        /* Using user config */
        mace_config = mace_user_config;
        return;
    }

    if ((target->_config >= MACE_DEFAULT_CONFIG) && 
        (target->_config < config_num)) {
        /* Using target config */
        mace_config = target->_config;
        return;
    }

    /* Using default config (may be set by user) */
    mace_config = mace_default_config;
}

/// @brief Set mace_user_target from input hash.
void mace_user_target_set(u64 hash, char *name) {
    if (hash == 0)
        return;

    for (int i = 0; i < target_num; i++) {
        if (hash == targets[i]._hash) {
            mace_user_target = i;
            return;
        }
    }

    fprintf(stderr, "User target '%s' not found. Exiting.\n", name);
    exit(1);
}

/// @brief Set default config for target.
void mace_target_config(char *target_name, char *config_name) {
    u64 target_hash = mace_hash(target_name);
    u64 config_hash = mace_hash(config_name);
    int target_order = -1;
    for (int i = 0; i < target_num; i++) {
        if (target_hash == targets[i]._hash) {
            target_order = i;
            break;
        }
    }
    if (target_order < 0)
        return;

    int config_order = -1;
    for (int i = 0; i < target_num; i++) {
        if (config_hash == configs[i]._hash) {
            config_order = i;
            break;
        }
    }

    if (config_order < 0)
        return;

    targets[target_order]._config = config_order;
}

/************** mace_hash ***************/
/// @brief Main hashing algorithm of mace: djb2.
u64 mace_hash(const char *str) {
    /* djb2 hashing algorithm by Dan Bernstein.
    * Description: This algorithm (k=33) was first reported by dan bernstein many
    * years ago in comp.lang.c. Another version of this algorithm (now favored by bernstein)
    * uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33
    * (why it works better than many other constants, prime or not) has never been adequately explained.
    * [1] https://stackoverflow.com/questions/7666509/hash-function-for-string
    * [2] http://www.cse.yorku.ca/~oz/hash.html */
    u64 hash = 5381;
    i32 str_char;
    while ((str_char = *str++))
        hash = ((hash << 5) + hash) + str_char; /* hash * 33 + c */
    return (hash);
}

/***************** MACE_SETTERS *****************/
/// @brief Sets where the object files will
///        be placed during build.
char *mace_set_obj_dir(char *obj) {
    MACE_FREE(obj_dir);
    return (obj_dir = mace_str_buffer(obj));
}

/// @brief Sets where the executables, libraries
///        will be built to.
char *mace_set_build_dir(char *build) {
    MACE_FREE(build_dir);
    return (build_dir = mace_str_buffer(build));
}

/// @brief Only place where archiver ar is set.
void mace_set_archiver(char *archiver) {
    ar = archiver;
}

/// @brief Only place where cc_depflag is set.
void mace_set_cc_depflag(char *depflag) {
    cc_depflag = depflag;
}

/// @brief Only place where compiler cc is set.
void mace_set_compiler(char *compiler) {
    cc = compiler;

    if (strstr(cc, "gcc") != NULL) {
        mace_set_cc_depflag("-MM");
        mace_set_archiver("ar");
    } else if (strstr(cc, "tcc") != NULL) {
        mace_set_cc_depflag("-MD");
        mace_set_archiver("tcc -ar");
    } else if (strstr(cc, "clang") != NULL) {
        mace_set_cc_depflag("-MM");
        mace_set_archiver("llvm-ar");
    } else {
        fprintf(stderr, "unknown compiler '%s'. \n", compiler);
        exit(1);
    }
}

/// @brief Only place were mace_separator is set.
void mace_set_separator(char *sep) {
    if (sep == NULL) {
        fprintf(stderr, "Separator should not be NULL.\n");
        exit(1);
    }
    if (strlen(sep) != 1) {
        fprintf(stderr, "Separator should have length one.\n");
        exit(1);
    }
    mace_separator = sep;
}

/********************* argv *********************/
/// @brief Free all arrays in argv array of pointers.
void mace_argv_free(char **argv, int argc) {
    for (int i = 0; i < argc; i++) {
        MACE_FREE(argv[i]);
    }
    MACE_FREE(argv);
}

/// @brief Build all argv flags from input user string.
///     - Split user_str using separator 
///     - Copy each token into a new buffer, 
///       put pointer in argv 
char **mace_argv_flags( int         *len,   int *argc,
                        char       **argv,  const char *user_str,
                        const char  *flag,  b32 path, 
                        const char  *separator) {
    if ((len == NULL) || (*len) <= 0) {
        assert(0);
        return(NULL);
    }
    if (argc == NULL) {
        assert(0);
        return(NULL);
    }

    size_t flag_len = (flag == NULL) ? 0 : strlen(flag);

    /* -- Copy user_str into modifiable buffer -- */
    char *buffer    = mace_str_buffer(user_str);
    char *sav       = NULL;
    char *token     = strtok_r(buffer, separator, &sav);
    while (token != NULL) {
        argv = mace_argv_grow(argv, argc, len);
        char *to_use    = token;
        char *rpath     = NULL;
        if (path) {
            /* - Expand path - */
            rpath = calloc(PATH_MAX, sizeof(*rpath));
            MACE_MEMCHECK(rpath);
            if (realpath(token, rpath) == NULL) {
                to_use = token;
                MACE_FREE(rpath);
            } else {
                size_t bytesize = (strlen(rpath) + 1) * sizeof(*rpath);
                rpath = realloc(rpath, bytesize);
                MACE_MEMCHECK(rpath);
                rpath[strlen(rpath)]    = '\0';
                to_use                  = rpath;
            }
        }

        size_t to_use_len   = strlen(to_use);
        size_t total_len    = (to_use_len + flag_len + 1);
        size_t i            = 0;
        char *arg = calloc(total_len, sizeof(*arg));
        MACE_MEMCHECK(arg);

        /* - Copy flag into arg - */
        if ((flag_len > 0) && (flag != NULL)) {
            strncpy(arg, flag, flag_len);
            i += flag_len;
        }

        /* - Copy token into arg - */
        strncpy(arg + i, to_use, to_use_len);
        i += to_use_len;
        argv[(*argc)++] = arg;
        token = strtok_r(NULL, separator, &sav);
        MACE_FREE(rpath);
    }

    MACE_FREE(buffer);
    return (argv);
}

/// @brief Parse user input target->excludes string. 
///     - Split excludes using mace_separator
///     - Hash file into _excludes list
void mace_Target_excludes(Target *target) {
    if (target->excludes == NULL) {
        return;
    }
    mace_Target_Free_excludes(target);

    target->_excludes_num = 0;
    target->_excludes_len = 8;
    target->_excludes     = calloc(target->_excludes_len, sizeof(*target->_excludes));
    MACE_MEMCHECK(target->_excludes);

    /* -- Copy user_str into modifiable buffer -- */
    char *buffer = mace_str_buffer(target->excludes);

    /* --- Split sources into tokens --- */
    char *token = strtok(buffer, mace_separator);
    do {
        size_t token_len = strlen(token);
        char *arg = calloc(token_len + 1, sizeof(*arg));
        MACE_MEMCHECK(arg);
        strncpy(arg, token, token_len);

        char *rpath = calloc(PATH_MAX, sizeof(*rpath));
        MACE_MEMCHECK(rpath);
        if (realpath(token, rpath) == NULL)
            Sprintf("Warning! excluded source '%s' does not exist\n", arg);
        rpath = realloc(rpath, (strlen(rpath) + 1) * sizeof(*rpath));
        MACE_MEMCHECK(rpath);

        if (mace_isDir(rpath)) {
            fprintf(stderr, "dir '%s' in excludes: files only!\n", rpath);
        } else {
            target->_excludes[target->_excludes_num++] = mace_hash(rpath);
            MACE_FREE(rpath);
        }
        MACE_FREE(arg);
        token = strtok(NULL, mace_command_separator);
    } while (token != NULL);

    MACE_FREE(buffer);
}

/// @brief Makes flags for target includes, 
///        links libraries, and flags
///     - NOT sources: they can be folders, 
///       so need to be globbed
void mace_Target_Parse_User(Target *target) {
    mace_Target_Free_argv(target);
    int len;
    int bytesize;

    /* -- Make _argv_includes to argv -- */
    if (target->includes != NULL) {
        len = 8;
        target->_argc_includes = 0;
        target->_argv_includes = calloc(len, sizeof(*target->_argv_includes));
        target->_argv_includes = mace_argv_flags(&len,
                                                 &target->_argc_includes,
                                                 target->_argv_includes,
                                                 target->includes,
                                                 "-I",
                                                 true,
                                                 mace_separator);
        bytesize               = target->_argc_includes * sizeof(*target->_argv_includes);
        target->_argv_includes = realloc(target->_argv_includes, bytesize);
    }

    /* -- Make _argv_linker_flags to argv -- */
    if (target->link_flags != NULL) {
        len = 8;
        target->_argc_link_flags = 0;
        target->_argv_link_flags = calloc(len, sizeof(*target->_argv_link_flags));
        target->_argv_link_flags = mace_argv_flags(&len, 
                                                   &target->_argc_link_flags,
                                                   target->_argv_link_flags,
                                                   target->link_flags,
                                                   "-Wl,",
                                                   true,
                                                   mace_separator);
        bytesize                 = target->_argc_link_flags * sizeof(*target->_argv_link_flags);
        target->_argv_link_flags = realloc(target->_argv_link_flags, bytesize);
    }

    /* -- Make _argv_links to argv -- */
    if (target->links != NULL) {
        len = 8;
        target->_argc_links = 0;
        target->_argv_links = calloc(len, sizeof(*target->_argv_links));
        target->_argv_links = mace_argv_flags(&len,
                                              &target->_argc_links,
                                              target->_argv_links,
                                              target->links,
                                              "-l",
                                              false,
                                              mace_separator);
        bytesize            = target->_argc_links * sizeof(*target->_argv_links);
        target->_argv_links = realloc(target->_argv_links, bytesize);
    }

    /* -- Make _argv_flags to argv -- */
    if (target->flags != NULL) {
        len = 8;
        target->_argc_flags = 0;
        target->_argv_flags = calloc(len, sizeof(*target->_argv_flags));
        target->_argv_flags = mace_argv_flags(&len,
                                              &target->_argc_flags,
                                              target->_argv_flags,
                                              target->flags,
                                              NULL,
                                              false,
                                              mace_separator);
        bytesize            = target->_argc_flags * sizeof(*target->_argv_flags);
        target->_argv_flags = realloc(target->_argv_flags, bytesize);
    }

    /* -- Exclusions -- */
    mace_Target_excludes(target);
}

/// @brief Realloc target->argv to bigger
///        if num close to len
void mace_Target_argv_grow(Target *target) {
    target->_argv = mace_argv_grow(target->_argv,
                                   &target->_argc,
                                   &target->_arg_len);
}

/// @brief Alloc target sources stuff. 
///        Realloc to bigger if num close to len.
void mace_Target_sources_grow(Target *target) {
    size_t bytesize;

    /* -- Alloc sources -- */
    if (target->_argv_sources == NULL) {
        target->_len_sources  = 8;
        target->_argv_sources = calloc(target->_len_sources, sizeof(*target->_argv_sources));
    }

    /* -- Alloc deps_headers -- */
    if (target->_deps_headers == NULL) {
        target->_deps_headers  = calloc(target->_len_sources, sizeof(*target->_deps_headers));
    }
    if (target->_deps_headers_num == NULL) {
        target->_deps_headers_num  = calloc(target->_len_sources, sizeof(*target->_deps_headers_num));
    }
    if (target->_deps_headers_len == NULL) {
        target->_deps_headers_len  = calloc(target->_len_sources, sizeof(*target->_deps_headers_len));
    }

    /* -- Alloc recompiles -- */
    if (target->_recompiles == NULL) {
        bytesize = target->_len_sources * sizeof(*target->_recompiles);
        target->_recompiles = calloc(1, bytesize);
    }

    /* -- Alloc objects -- */
    if (target->_argv_objects == NULL) {
        bytesize = sizeof(*target->_argv_objects);
        target->_argv_objects       = calloc(target->_len_sources, bytesize);
    }
    if (target->_argv_objects_cnt == NULL) {
        bytesize = sizeof(*target->_argv_objects_cnt);
        target->_argv_objects_cnt   = calloc(target->_len_sources, bytesize);
    }
    if (target->_argv_objects_hash == NULL) {
        bytesize = sizeof(*target->_argv_objects_hash);
        target->_argv_objects_hash  = calloc(target->_len_sources, bytesize);
    }
    /* -- Realloc sources -- */
    int previous_len  = target->_len_sources;
    target->_argv_sources = mace_argv_grow( target->_argv_sources, 
                                            &target->_argc_sources,
                                            &target->_len_sources);
    int new_len  = target->_len_sources;

    /* -- Alloc object dependencies -- */
    if (previous_len != new_len) {
        /* -- Realloc recompiles -- */
        bytesize = target->_len_sources * sizeof(*target->_recompiles);
        target->_recompiles = realloc(target->_recompiles, bytesize);
        memset(target->_recompiles + target->_len_sources / 2, 0, bytesize / 2);

        /* -- Realloc objects -- */
        bytesize = target->_len_sources * sizeof(*target->_argv_objects);
        target->_argv_objects = realloc(target->_argv_objects, bytesize);
        memset(target->_argv_objects + target->_len_sources / 2, 0, bytesize / 2);
    }

    /* -- Realloc deps_headers -- */
    if (previous_len != new_len) {
        size_t bytesize = target->_len_sources * sizeof(*target->_deps_headers);
        target->_deps_headers = realloc(target->_deps_headers, bytesize);
        memset(target->_deps_headers + target->_len_sources / 2, 0, bytesize / 2);

        bytesize = target->_len_sources * sizeof(*target->_deps_headers_num);
        target->_deps_headers_num = realloc(target->_deps_headers_num, bytesize);
        memset(target->_deps_headers_num + target->_len_sources / 2, 0, bytesize / 2);
        
        bytesize = target->_len_sources * sizeof(*target->_deps_headers_len);
        target->_deps_headers_len = realloc(target->_deps_headers_len, bytesize);
        memset(target->_deps_headers_num + target->_len_sources / 2, 0, bytesize / 2);
    }

    /* -- Realloc objects -- */
    if (target->_len_sources >= target->_argc_objects_hash) {
        bytesize = target->_len_sources * sizeof(*target->_argv_objects_hash);
        target->_argv_objects_hash = realloc(target->_argv_objects_hash, bytesize);
        bytesize = target->_len_sources * sizeof(*target->_argv_objects_cnt);
        target->_argv_objects_cnt = realloc(target->_argv_objects_cnt, bytesize);
    }
}

/// @brief Realloc argv if argc close to arg_len.
char **mace_argv_grow(char **argv,
                      int *argc,
                      int *arg_len) {
    if ((*argc + 1) >= *arg_len) {
        size_t new_len = (*arg_len) * 2;
        size_t bytesize = new_len * sizeof(*argv);
        argv = realloc(argv, bytesize);
        MACE_MEMCHECK(argv);
        memset(argv + (*arg_len), 0, bytesize / 2);
        (*arg_len) = new_len;
    }
    return (argv);
}

/// @brief Create argv, argc for compiling objects
///        all at once. Should be called after all
///        sources have been added.
void mace_Target_argv_allatonce(Target *target) {
    if (target->_argv == NULL) {
        target->_arg_len = 8;
        target->_argc = 0;
        target->_argv = calloc(target->_arg_len, sizeof(*target->_argv));
    }
    target->_argv[MACE_ARGV_CC] = cc;
    target->_argc = MACE_ARGV_CC + 1;

    /* -- argv sources -- */
    if ((target->_argc_sources > 0) && (target->_argv_sources != NULL)) {
        for (int i = 0; i < target->_argc_sources; i++) {
            mace_Target_argv_grow(target);
            target->_argv[target->_argc++] = target->_argv_sources[i];
        }
    }

    /* -- argv includes -- */
    if ((target->_argc_includes > 0) && (target->_argv_includes != NULL)) {
        for (int i = 0; i < target->_argc_includes; i++) {
            mace_Target_argv_grow(target);
            target->_argv[target->_argc++] = target->_argv_includes[i];
        }
    }

    /* -- argv -L flag for build_dir -- */
    target->_argc_tail =    target->_argc;
    mace_Target_argv_grow(target);
    if (build_dir == NULL) {
        assert(0);
        return;
    }
    size_t build_dir_len = strlen(build_dir);
    char *ldirflag = calloc(3 + build_dir_len, sizeof(*ldirflag));
    MACE_MEMCHECK(ldirflag);
    memcpy(ldirflag, "-L", 2);
    strncpy(ldirflag + 2, build_dir, build_dir_len);
    target->_argv[target->_argc++] = ldirflag;

    /* -- argv -c flag for libraries -- */
    mace_Target_argv_grow(target);
    char *compflag = calloc(3, sizeof(*compflag));
    strncpy(compflag, "-c", 3);
    target->_argv[target->_argc++] = compflag;

    /* -- add config -- */
    mace_argv_add_config(target, &target->_argv, &target->_argc, &target->_arg_len);
    target->_argv[target->_argc] = NULL;
}

/// @brief Create argv, argc for compiling objects
///        one at a time. Should be called after
///        mace_Target_Parse_User
void mace_Target_argv_compile(Target *target) {
    if (target->_argv == NULL) {
        target->_arg_len = 8;
        target->_argc = 0;
        target->_argv = calloc(target->_arg_len, sizeof(*target->_argv));
    }

    /* --- Adding argvs common to all --- */
    target->_argc = MACE_ARGV_OTHER;
    /* -- argv user flags -- */
    if ((target->_argc_flags > 0) && (target->_argv_flags != NULL)) {
        for (int i = 0; i < target->_argc_flags; i++) {
            mace_Target_argv_grow(target);
            target->_argv[target->_argc++] = target->_argv_flags[i];
        }
    }

    /* -- argv link_flags -- */
    if ((target->_argc_link_flags > 0) && (target->_argv_link_flags != NULL)) {
        for (int i = 0; i < target->_argc_link_flags; i++) {
            mace_Target_argv_grow(target);
            target->_argv[target->_argc++] = target->_argv_link_flags[i];
        }
    }

    /* -- argv includes -- */
    if ((target->_argc_includes > 0) && (target->_argv_includes != NULL)) {
        for (int i = 0; i < target->_argc_includes; i++) {
            mace_Target_argv_grow(target);
            target->_argv[target->_argc++] = target->_argv_includes[i];
        }
    }

    /* -- argv -c flag for objects -- */
    target->_argc_tail =    target->_argc;
    mace_Target_argv_grow(target);
    char *compflag = calloc(3, sizeof(*compflag));
    strncpy(compflag, "-c", 3);
    target->_argv[target->_argc++] = compflag;

    /* -- argv -fPIC flag for objects -- */
    if (target->kind == MACE_DYNAMIC_LIBRARY) {
        mace_Target_argv_grow(target);
        char *fPICflag = calloc(6, sizeof(*compflag));
        memcpy(fPICflag, "-fPIC", 5);
        target->_argv[target->_argc++] = fPICflag;
    }

    target->_argv[target->_argc] = NULL;
}

/// @brief Add config as flags to argv for compilation.
void mace_argv_add_config(Target *target,
                          char ** *argv,
                          int *argc,
                          int *arg_len) {
    if (config_num <= 0)
        return;

    for (int i = 0; i < configs[mace_config]._flag_num; ++i) {
        *argv       = mace_argv_grow(*argv, argc, arg_len);
        size_t len  = strlen(configs[mace_config]._flags[i]) + 1;
        char *flag  = calloc(len, sizeof(*flag));
        strncpy(flag, configs[mace_config]._flags[i],  len);
        (*argv)[(*argc)++] = flag;
    }
}

/***************** mace_pqueue ******************/

pid_t mace_pqueue_pop(void) {
    if (pnum <= 0) {
        assert(0);
        return(0);
    }

    return (pqueue[--pnum]);
}

void mace_pqueue_put(pid_t pid) {
    if (pnum >= plen) {
        mace_wait_pid(pid);
    }
    if (plen > 1) {
        // TODO  queue with no memmove
        size_t bytes = (plen - 1) * sizeof(*pqueue);
        memmove(pqueue + 1, pqueue, bytes);
    }
    pqueue[0] = pid;
    pnum++;
}

/***************** mace_glob_sources ****************/
/// @brief If source is a folder, get all .c files in it.
glob_t mace_glob_sources(const char *path) {
    glob_t  globbed;
    int     flags   = 0;
    int     ret     = glob(path, flags, mace_globerr, &globbed);
    if (ret != 0) {
        fprintf(stderr, "problem with %s (%s), quitting\n", path,
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
/// @brief Print command to be run in forked process.
void mace_exec_print(char *const arguments[], size_t argnum) {
    for (int i = 0; i < argnum; i++) {
        VSprintf("%s ", arguments[i]);
    }
    VSprintf("%s", "\n");
}

/// @brief Put back arguments array (argv) into a
///        single line for execvp.
char *mace_args2line(char *const arguments[]) {
    int i   = 0;
    int num = 0; 
    int len = 128;

    char *argline = calloc(len, sizeof(*argline));
    MACE_MEMCHECK(argline);

    while ((arguments[i] != NULL) && (i < MACE_MAX_ITERATIONS)) {
        size_t ilen = strlen(arguments[i]);
        while ((num + ilen + 1) > len) {
            size_t bytesize = len * 2 * sizeof(*argline);
            argline = realloc(argline, bytesize);
            MACE_MEMCHECK(argline);
            memset(argline + len, 0, len);
            len *= 2;
        }
        assert(num < len);
        memcpy(argline + num, arguments[i], ilen);
        num += ilen;
        argline[num++] = ' ';
        i++;
    }
    if (i == MACE_MAX_ITERATIONS) {
        Sprintf("Warning! Max iterations reached. Truncating argv.");
    }
    argline[num] = '\0';
    return (argline);
}

/// @brief Execute command in a different fork with
///        execvp and bash.
pid_t mace_exec_wbash(const char *exec, char *const arguments[]) {
    char *argline = mace_args2line(arguments);
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "forking issue\n");
        assert(0);
        exit(1);
    } else if (pid == 0) {
        char *bashargs[] = {
            "/bin/bash",    "-c",
            argline,        NULL
        };
        mace_exec_print(bashargs, 3);
        execvp("/bin/bash", bashargs);
        exit(0);
    }
    MACE_FREE(argline);
    return (pid);
}

/// @brief Execute command in a different fork
///        with execvp.
pid_t mace_exec(const char *exec, char *const arguments[]) {
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

/// @brief Wait on process with pid to finish
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
            fprintf(stderr,"is baka? %d\n", WEXITSTATUS(status));
            fprintf(stderr, "Fork didn't terminate normally. %d\n", WEXITSTATUS(status));
            exit(WEXITSTATUS(status));
        }
    }
}

/******************* mace_build ********************/
void mace_link_dynamic_library(Target *target) {
    char *lib = mace_library_path(target->_name, MACE_DYNAMIC_LIBRARY);
    Sprintf("Linking  %s\n", lib);
    int    argc_objects = target->_argc_sources;
    char **argv_objects = target->_argv_objects;

    int argc = 0, arg_len = 8;
    char **argv  = calloc(arg_len, sizeof(*argv));
    MACE_MEMCHECK(argv);
    argv[argc++] = cc;

    /* --- Adding target --- */
    size_t oflag_len = 2;
    size_t lib_len   = strlen(lib);
    char *libv       = calloc(lib_len + oflag_len + 1, sizeof(*libv));
    MACE_MEMCHECK(libv);
    memcpy(libv, "-o", oflag_len);
    strncpy(libv + oflag_len, lib, lib_len);
    int libc  = argc;
    argv[argc++] = libv;

    /* --- Adding -fPIC flag --- */
    char *fPICflag     = calloc(6, sizeof(*fPICflag));
    MACE_MEMCHECK(fPICflag);
    memcpy(fPICflag, "-fPIC", 5);
    int cfPICflag   = argc;
    argv[argc++]    = fPICflag;

    /* --- Adding -shared flag --- */
    char *sharedflag     = calloc(8, sizeof(*sharedflag));
    MACE_MEMCHECK(sharedflag);
    memcpy(sharedflag, "-shared", 7);
    int csharedflag = argc;
    argv[argc++]    = sharedflag;

    /* --- Adding objects --- */
    if ((argc_objects > 0) && (argv_objects != NULL)) {
        for (int i = 0; i < argc_objects; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = argv_objects[i] + strlen("-o");
        }
    }

    /* -- argv links -- */
    if ((target->_argc_links > 0) && (target->_argv_links != NULL)) {
        for (int i = 0; i < target->_argc_links; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = target->_argv_links[i];
        }
    }

    /* -- argv link_flags -- */
    if ((target->_argc_link_flags > 0) && (target->_argv_link_flags != NULL)) {
        for (int i = 0; i < target->_argc_link_flags; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = target->_argv_link_flags[i];
        }
    }

    /* -- argv flags -- */
    if ((target->_argc_flags > 0) && (target->_argv_flags != NULL)) {
        for (int i = 0; i < target->_argc_flags; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = target->_argv_flags[i];
        }
    }

    /* --- argv config --- */
    int config_startc   = argc;
    mace_argv_add_config(target, &argv, &argc, &arg_len);
    int config_endc     = argc;

    /* --- Actual linking --- */
    mace_exec_print(argv, argc);
    if (!dry_run) {
        pid_t pid = mace_exec_wbash(argv[0], argv);
        mace_wait_pid(pid);
    }

    MACE_FREE(argv[cfPICflag]);
    MACE_FREE(argv[csharedflag]);
    MACE_FREE(argv[libc]);
    for (int i = config_startc; i < config_endc; i++) {
        MACE_FREE(argv[i]);
    }
    MACE_FREE(argv);
    MACE_FREE(lib);
}

void mace_link_static_library(Target *target) {
    char *lib = mace_library_path(target->_name, MACE_STATIC_LIBRARY);
    Sprintf("Linking  %s\n", lib);
    int    argc_objects = target->_argc_sources;
    char **argv_objects = target->_argv_objects;

    /* --- Add ar --- */
    int arg_len = 8, argc = 0, argc_ar = 0;
    char **argv  = calloc(arg_len, sizeof(*argv));

    /* -- Split ar into tokens -- */
    // Note: because tcc -ar is not a standalone executable but a flag 'tcc -ar'
    char *buffer = calloc(strlen(ar) + 1, sizeof(*ar));
    strncpy(buffer, ar, strlen(ar));
    char *token = strtok(buffer, mace_separator);
    do {
        char *flag = calloc(strlen(token) + 1, sizeof(*ar));
        strncpy(flag, token, strlen(token));
        argv[argc++] = flag;
        argc_ar = argc;
        token = strtok(NULL, mace_separator);
    } while (token != NULL);

    /* --- Adding -rcs flag --- */
    char *rcsflag     = calloc(5, sizeof(*rcsflag));
    memcpy(rcsflag, "-rcs", 4);
    int crcsflag = argc;
    argv[argc++] = rcsflag;

    /* --- Adding target --- */
    size_t lib_len = strlen(lib);
    char *libv     = calloc(lib_len + 1, sizeof(*rcsflag));
    strncpy(libv, lib, lib_len);
    int libc = argc;
    argv[argc++] = libv;

    /* --- Adding objects --- */
    if ((argc_objects > 0) && (argv_objects != NULL)) {
        for (int i = 0; i < argc_objects; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = argv_objects[i] + strlen("-o");
        }
    }

    /* -- argv links -- */
    if ((target->_argc_links > 0) && (target->_argv_links != NULL)) {
        for (int i = 0; i < target->_argc_links; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = target->_argv_links[i];
        }
    }

    /* --- Actual linking --- */
    mace_exec_print(argv, argc);
    if (!dry_run) {
        pid_t pid = mace_exec_wbash(argv[0], argv);
        mace_wait_pid(pid);
    }
    MACE_FREE(buffer);
    for (int i = 0; i < argc_ar; ++i) {
        MACE_FREE(argv[i]);
    }
    MACE_FREE(argv[crcsflag]);
    MACE_FREE(argv[libc]);
    MACE_FREE(argv);
    MACE_FREE(lib);
}

void mace_link_executable(Target *target) {
    char *exec = mace_executable_path(target->_name);
    Sprintf("Linking  %s\n", exec);

    char **argv_links    = target->_argv_links;
    char **argv_flags    = target->_argv_flags;
    char **argv_objects  = target->_argv_objects;
    int    argc_links    = target->_argc_links;
    int    argc_flags    = target->_argc_flags;
    int    argc_objects  = target->_argc_sources;

    int arg_len = 16;
    int argc    = 0;
    char **argv = calloc(arg_len, sizeof(*argv));
    argv[argc++] = cc;

    /* --- Adding executable output --- */
    size_t exec_len = strlen(exec);
    char *oflag     = calloc(exec_len + 3, sizeof(*oflag));
    memcpy(oflag, "-o", 2);
    strncpy(oflag + 2, exec, exec_len);
    int oflag_i = argc++;
    argv[oflag_i] = oflag;

    /* --- Adding objects --- */
    if ((argc_objects > 0) && (argv_objects != NULL)) {
        for (int i = 0; i < argc_objects; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = argv_objects[i] + strlen("-o");
        }
    }

    /* -- argv user flags -- */
    if ((argc_flags > 0) && (argv_flags != NULL)) {
        for (int i = 0; i < argc_flags; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = argv_flags[i];
        }
    }

    /* -- argv links -- */
    if ((argc_links > 0) && (argv_links != NULL)) {
        for (int i = 0; i < argc_links; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = argv_links[i];
        }
    }

    /* -- argv link_flags -- */
    if ((target->_argc_link_flags > 0) && (target->_argv_link_flags != NULL)) {
        for (int i = 0; i < target->_argc_link_flags; i++) {
            argv = mace_argv_grow(argv, &argc, &arg_len);
            argv[argc++] = target->_argv_link_flags[i];
        }
    }

    /* -- argv config -- */
    argv = mace_argv_grow(argv, &argc, &arg_len);
    int config_startc = argc;
    mace_argv_add_config(target, &argv, &argc, &arg_len);
    int config_endc = argc;

    /* -- argv -L flag for build_dir -- */
    argv = mace_argv_grow(argv, &argc, &arg_len);
    size_t build_dir_len = strlen(build_dir);
    char *ldirflag = calloc(3 + build_dir_len, sizeof(*ldirflag));
    memcpy(ldirflag,     "-L",      2);
    strncpy(ldirflag + 2, build_dir, build_dir_len);
    int ldirflag_i   = argc++;
    argv[ldirflag_i] = ldirflag;

    /* --- Actual linking  --- */
    mace_exec_print(argv, argc);
    if (!dry_run) {
        pid_t pid = mace_exec_wbash(argv[0], argv);
        mace_wait_pid(pid);
    }

    MACE_FREE(argv[oflag_i]);
    MACE_FREE(argv[ldirflag_i]);
    for (int i = config_startc; i < config_endc; i++) {
        MACE_FREE(argv[i]);
    }
    MACE_FREE(argv);
    MACE_FREE(exec);
}

/// @brief Compile target's obj file all at once.
void mace_Target_compile_allatonce(Target *target) {
    // Compile ALL objects at once
    /* -- Move to obj_dir -- */
    mace_chdir(obj_dir);

    /* -- Prepare argv -- */
    mace_Target_argv_allatonce(target);

    /* -- Actual compilation -- */
    mace_exec_print(target->_argv, target->_argc);
    if (!dry_run) {
        pid_t pid = mace_exec_wbash(target->_argv[0], target->_argv);
        mace_wait_pid(pid);
    }

    /* -- Go back to cwd -- */
    mace_chdir(cwd);
}

/// @brief Target pre-compilation: check which file
///        needs to be recompiled
void mace_Target_precompile(Target *target) {
    /* Compute latest object dependencies .d file */
    if (target == NULL) {
        assert(0);
        return;
    }
    if (target->_argv == NULL) {
        assert(0);
        return;
    }
    int argc = 0;

    target->_argv[MACE_ARGV_CC]     = cc;
    mace_Target_argv_grow(target);
    target->_argv[target->_argc++]  = cc_depflag;
    target->_argv[target->_argc]    = NULL;
    /* - Single source argv - */
    while (true) {
        /* - Skip if no recompiles - */
        if ((argc < target->_argc_sources) &&
            (!target->_recompiles[argc])) {
            argc++;
            continue;
        }
        /* - Add process to queue - */
        if (argc < target->_argc_sources) {
            Vprintf("Pre-Compile %s\n", target->_argv_sources[argc]);
            target->_argv[MACE_ARGV_SOURCE] = target->_argv_sources[argc];
            target->_argv[MACE_ARGV_OBJECT] = target->_argv_objects[argc];
            size_t len = strlen(target->_argv[MACE_ARGV_OBJECT]);
            target->_argv[MACE_ARGV_OBJECT][len - 1] = 'd';

            argc++;

            /* -- Actual pre-compilation -- */
            mace_exec_print(target->_argv, target->_argc);
            assert(target->_argv[target->_argc] == NULL);
            pid_t pid = mace_exec_wbash(target->_argv[0], target->_argv);
            mace_pqueue_put(pid);

            target->_argv[MACE_ARGV_OBJECT][len - 1] = 'o';
        }

        /* Prioritize adding process to queue */
        if ((argc < target->_argc_sources) &&
            (pnum < plen))
            continue;
        /* Wait for process */
        if (pnum > 0) {
            pid_t wait = mace_pqueue_pop();
            if (wait > 0) {
                mace_wait_pid(wait);
            }
        }

        /* Check if more to compile */
        if ((pnum == 0) &&
            (argc == target->_argc_sources))
            break;
    }
    target->_argv[--target->_argc] = NULL;

    /* -- Object dependencies (headers) -- */
    /* - Read .d file and hashes the filenames, write all headers to .ho files. - */
    mace_Target_Parse_Objdeps(target);

    /* - Compute checksums of all headers - */
    mace_Headers_Checksums(target);

    /* - Check if any source's header changed - */
    mace_Headers_Checksums_Checks(target);
}

/// @brief Compile targets' objects one at a time
void mace_Target_compile(Target *target) {
    if (target == NULL) {
        assert(0);
        return;
    }
    if (target->_argv == NULL) {
        assert(0);
        return;
    }

    target->_argv[MACE_ARGV_CC] = cc;

    int argc = 0;
    /* - Single source argv - */
    while (true) {
        /* - Skip if no recompiles - */
        if ((argc < target->_argc_sources) && (!target->_recompiles[argc])) {
            argc++;
            continue;
        }

        /* - Add process to queue - */
        if (argc < target->_argc_sources) {
            Sprintf("Compiling %s\n", target->_argv_sources[argc]);
            target->_argv[MACE_ARGV_SOURCE] = target->_argv_sources[argc];
            target->_argv[MACE_ARGV_OBJECT] = target->_argv_objects[argc];
            argc++;

            /* -- Actual compilation -- */
            mace_exec_print(target->_argv, target->_argc);
            if (!dry_run) {
                pid_t pid = mace_exec_wbash(target->_argv[0], target->_argv);
                mace_pqueue_put(pid);
            }
        }

        /* Prioritize adding process to queue */
        if ((argc < target->_argc_sources) &&
            (pnum < plen))
            continue;

        /* Wait for process */
        if (pnum > 0) {
            pid_t wait = mace_pqueue_pop();
            mace_wait_pid(wait);
        }

        /* Check if more to compile */
        if ((pnum == 0) &&
            (argc == target->_argc_sources))
            break;
    }
}

/// @brief Alloc _objects_hash_nocoll. 
///        Realloc if num close to len. Add hash.
void Target_Object_Hash_Add_nocoll(Target *target,
                                   u64 hash) {
    if (target->_objects_hash_nocoll == NULL) {
        target->_objects_hash_nocoll_num = 0;
        target->_objects_hash_nocoll_len = 8;
        size_t bytesize = target->_objects_hash_nocoll_len * sizeof(*target->_objects_hash_nocoll);
        target->_objects_hash_nocoll = malloc(bytesize);
        memset(target->_objects_hash_nocoll, 0, bytesize);
    }
    if (target->_objects_hash_nocoll_num >= (target->_objects_hash_nocoll_len - 1)) {
        target->_objects_hash_nocoll_len *= 2;
        size_t bytesize = target->_objects_hash_nocoll_len * sizeof(*target->_objects_hash_nocoll);
        target->_objects_hash_nocoll = realloc(target->_objects_hash_nocoll, bytesize);
        memset(target->_objects_hash_nocoll + target->_objects_hash_nocoll_len / 2, 0, bytesize / 2);
    }

    target->_objects_hash_nocoll[target->_objects_hash_nocoll_num++] = hash;
}

/// @brief Check if hash is in _objects_hash_nocoll.
int Target_hasObjectHash_nocoll(Target *target, u64 hash) {
    if (target->_objects_hash_nocoll == NULL)
        return (-1);

    for (int i = 0; i < target->_objects_hash_nocoll_num; i++) {
        if (hash == target->_objects_hash_nocoll[i])
            return (i);
    }

    return (-1);
}

/// @brief Add object hash to target.
void Target_Object_Hash_Add(Target *target, u64 hash) {
    if (target   == NULL) {
        assert(0);
        return;
    }
    if (target->_argv_objects_hash   == NULL) {
        assert(0);
        return;
    }
    if (target->_argv_objects_cnt   == NULL) {
        assert(0);
        return;
    }
    target->_argv_objects_hash[target->_argc_objects_hash] = hash;
    target->_argv_objects_cnt[target->_argc_objects_hash++] = 0;
}

/// @brief Check if target has object hash.
int Target_hasObjectHash(Target *target, u64 hash) {
    if (target->_argv_objects_hash == NULL)
        return (-1);

    for (int i = 0; i < target->_argc_objects_hash; i++) {
        if (hash == target->_argv_objects_hash[i])
            return (i);
    }

    return (-1);
}

/// @brief Add target to list of recompiles.
void mace_Target_Recompiles_Add(Target *target, b32 add) {
    target->_recompiles[target->_argc_sources - 1] = add;
}

/// @brief Add object needing to be compiled to target.
b32 mace_Target_Object_Add(Target *target, char *token) {
    /* token is object path */
    if (token == NULL)
        return (false);
    u64 hash = mace_hash(token);
    int hash_id = Target_hasObjectHash(target, hash);

    if (hash_id < 0) {
        Target_Object_Hash_Add(target, hash);
    } else {
        target->_argv_objects_cnt[hash_id]++;
        if (target->_argv_objects_cnt[hash_id] >= 10) {
            fprintf(stderr, "Too many same name sources/objects\n");
            exit(1);
        }
    }

    /* -- Append object to arg -- */
    size_t token_len = strlen(token);
    char *flag = "-o";
    size_t flag_len = strlen(flag);
    size_t total_len = token_len + flag_len + 1;
    if (hash_id > 0)
        total_len++;
    char *arg = calloc(total_len, sizeof(*arg));
    strncpy(arg, flag, flag_len);
    strncpy(arg + flag_len, token, token_len);

    if (hash_id > 0) {
        char *pos = strrchr(arg, '.');
        *(pos) = target->_argv_objects_cnt[hash_id] + '0';
        *(pos + 1) = '.';
        *(pos + 2) = 'o';
    }

    /* -- Actually adding object here -- */
    u64 hash_nocoll = mace_hash(arg + flag_len);
    Target_Object_Hash_Add_nocoll(target, hash_nocoll);

    mace_Target_sources_grow(target);
    target->_argv_objects[target->_argc_sources - 1] = arg;

    // Does object file exist
    b32 exists = access(arg + 2, F_OK) == 0;
    return (exists);
}

/// @brief Check if any source/header file changed
///        for object.
void mace_Headers_Checksums_Checks(Target *target) {
    if (target == NULL) {
        assert(0);
        return;
    }
    if (target->_hdrs_changed == NULL) {
        assert(0);
        return;
    }

    if (build_all) {
        size_t bytesize = target->_argc_sources * sizeof(*target->_recompiles);
        memset(target->_recompiles, 1, bytesize);
        return;
    }

    // For every source file
    for (int i = 0; i < target->_argc_sources; i++) {
        // Check if any header file it depends on has changed
        if (target->_recompiles[i] == true) {
            continue;
        }
        if (target->_deps_headers[i] == NULL) {
            // No headers
            continue;
        }
        for (int j = 0;  j < target->_deps_headers_num[i]; j++) {
            int header_order = target->_deps_headers[i][j];
            if (target->_hdrs_changed[header_order]) {
                target->_recompiles[i] = true;
                break;
            }
        }
    }
}

/// @brief Compute checksums for all headers.
void mace_Headers_Checksums(Target *target) {
    /* --- HEADERS CHECKSUMS --- */
    mace_chdir(cwd);

    u8 hash_current[SHA1_LEN];
    u8 hash_previous[SHA1_LEN];

    for (int i = 0; i < target->_headers_num; i++) {
        /* - Compute current checksum - */
        memset(hash_previous, 0, SHA1_LEN);
        char *checksum_path = target->_headers_checksum[i];
        char *header_path = target->_headers[i];
        mace_sha1dc(header_path, hash_current);

        /* - Check if previous checksum exists - */
        b32 changed = true; // set to false only if checksum file exists, changed
        FILE *fd = fopen(checksum_path, "r");
        if (fd != NULL) {
            fseek(fd, 0, SEEK_SET);
            size_t size = fread(hash_previous, 1, SHA1_LEN, fd);
            if (size != SHA1_LEN) {
                fprintf(stderr, "Could not read checksum from '%s'. Deleting. \n", checksum_path);
                fclose(fd);
                remove(checksum_path);
                exit(1);
            }
            changed = !mace_sha1dc_cmp(hash_previous, hash_current);
            fclose(fd);
        }

        /* - Write checksum file, if changed or didn't exist - */
        if (changed) {
            fd = fopen(checksum_path, "w");
            if (fd == NULL) {
                fprintf(stderr, "Could not open file %s\n", checksum_path);
                exit(1);
            }
            fwrite(hash_current, 1, SHA1_LEN, fd); // SHA1_LEN
            fclose(fd);
        }
        target->_hdrs_changed[i] = changed;
    }

    if (target->base_dir != NULL) {
        mace_chdir(target->base_dir);
    }
}

/// @brief Compute checksums for all sources.
b32 mace_Source_Checksum(Target *target,
                         char *source_path,
                         char *obj_path) {
    /* --- SOURCE CHECKSUM --- */
    /* - Compute current checksum - */
    u8 hash_current[SHA1_LEN];
    mace_sha1dc(source_path, hash_current);

    /* - Read existing checksum file - */
    mace_chdir(cwd);
    b32 changed = true; // set to false only if checksum file exists, changed
    u8 hash_previous[SHA1_LEN] = {0};
    char *checksum_path = mace_checksum_filename(obj_path, MACE_CHECKSUM_MODE_SRC);
    FILE *fd = fopen(checksum_path, "r");
    if (fd != NULL) {
        fseek(fd, 0, SEEK_SET);
        size_t size = fread(hash_previous, 1, SHA1_LEN, fd);
        if (size != SHA1_LEN) {
            fprintf(stderr, "Could not read checksum from '%s'. Deleting. \n", checksum_path);
            fclose(fd);
            remove(checksum_path);
            exit(1);
        }
        changed = !mace_sha1dc_cmp(hash_previous, hash_current);
        fclose(fd);
    }

    /* - Write checksum file, if changed or didn't exist - */
    if (changed) {
        fd = fopen(checksum_path, "w");
        if (fd == NULL) {
            fprintf(stderr, "Could not open file %s\n", checksum_path);
            exit(1);
        }
        fwrite(hash_current, 1, SHA1_LEN, fd); // SHA1_LEN
        fclose(fd);
    }
    MACE_FREE(checksum_path);

    if (target->base_dir != NULL) {
        mace_chdir(target->base_dir);
    }

    return (changed);
}

/// @brief Add source file to target.
b32 mace_Target_Source_Add(Target *target, char *token) {
    if (token == NULL)
        return (true);

    mace_Target_sources_grow(target);

    /* - Expand path - */
    char *rpath = calloc(PATH_MAX, sizeof(*rpath));
    if (realpath(token, rpath) == NULL) {
        Sprintf("Warning! realpath issue: %s\n", rpath);
        size_t token_len = strlen(token) + 1;
        if (token_len < PATH_MAX) {
            fprintf(stderr, "token_len longer than PATH_MAX\n");
            exit(1);
        }
        strncpy(rpath, token, token_len);
    }

    /* - Check if file is excluded - */
    u64 rpath_hash = mace_hash(rpath);
    for (int i = 0; i < target->_excludes_num; i++) {
        if (target->_excludes[i] == rpath_hash) {
            MACE_FREE(rpath);
            return (true);
        }
    }

    /* -- Actually adding source here -- */
    target->_argv_sources[target->_argc_sources++] = rpath;

    return (false);
}

/// @brief Add found source file to target
///     - Create object name from source name
///     - Make source checksum
///     - Check if source needs to be recompiled
void mace_Target_Parse_Source(Target *target,
                              char *path,
                              char *src) {
    b32 excluded = mace_Target_Source_Add(target, path);
    if (!excluded) {
        mace_object_path(src);
        b32 exists  = mace_Target_Object_Add(target, object);
        size_t i = target->_argc_sources - 1;
        b32 changed_src  = mace_Source_Checksum(target, target->_argv_sources[i],
                                                 target->_argv_objects[i]);
        mace_Target_Recompiles_Add(target, !excluded && (changed_src || !exists));
    }
}

/// @brief Globbed files for sources and parse objects.
void mace_compile_glob(Target *target, char *globsrc,
                           const char *flags) {
    glob_t globbed = mace_glob_sources(globsrc);
    for (int i = 0; i < globbed.gl_pathc; i++) {
        assert(mace_isSource(globbed.gl_pathv[i]));
        char *pos = strrchr(globbed.gl_pathv[i], '/');
        char *source_file = (pos == NULL) ? globbed.gl_pathv[i] : pos + 1;
        /* - Compute source and object filenames - */
        mace_Target_Parse_Source(target, globbed.gl_pathv[i], source_file);
    }
    globfree(&globbed);
}

/******************** mace_is *********************/
int mace_isTarget(u64 hash) {
    for (int i = 0; i < target_num; i++) {
        if (targets[i]._hash == hash)
            return (true);
    }
    return (false);
}

int mace_isWildcard(const char *str) {
    return ((strchr(str, '*') != NULL));
}

int mace_isSource(const char *path) {
    size_t len  = strlen(path);
    int out     = path[len - 1]       == 'c'; /* C source extension: .c */
    out        &= path[len - 2]       == '.'; /* C source extension: .c */
    return (out);
}

int mace_isDir(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

/***************** mace_filesystem *******************/
void mace_mkdir(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0777);
        chmod(path, 0777);
    }
}

/// @brief Create path to executable to compile.
char *mace_executable_path(const char *target_name) {
    if (target_name == NULL) {
        assert(0);
        return(NULL);
    }
    size_t bld_len = strlen(build_dir);
    size_t tar_len = strlen(target_name);

    char *exec = calloc((bld_len + tar_len + 2), sizeof(*exec));
    size_t full_len = 0;
    strncpy(exec,            build_dir,   bld_len);
    full_len += bld_len;
    if (build_dir[0] != '/') {
        memcpy(exec + full_len, "/",         1);
        full_len++;
    }
    strncpy(exec + full_len, target_name, tar_len);
    return (exec);
}

/// @brief Create path to library to compile.
char *mace_library_path(char *target_name, int kind) {
    if (target_name == NULL) {
        return(NULL);
    }
    size_t bld_len = strlen(build_dir);
    size_t tar_len = strlen(target_name);

    char *lib = calloc((bld_len + tar_len + 8), sizeof(*lib));
    size_t full_len = 0;
    strncpy(lib,                build_dir,   bld_len);
    full_len += bld_len;
    if (build_dir[0] != '/') {
        memcpy(lib + full_len, "/",         1);
        full_len++;
    }
    memcpy(lib + full_len,     "lib",       3);
    full_len += 3;
    strncpy(lib + full_len,     target_name, tar_len);
    full_len += tar_len;
    if (kind == MACE_STATIC_LIBRARY) {
        memcpy(lib + full_len,     ".a",        2);
    } else if (kind == MACE_DYNAMIC_LIBRARY) {
        memcpy(lib + full_len,     ".so",        3);
    }
    return (lib);
}

/*************** mace_globals *****************/
/// @brief Realloc global object.
void mace_object_grow(void) {
    object_len *= 2;
    object      = realloc(object,   object_len  * sizeof(*object));
}

/// @brief Write object path from source file
///        to global object.
void mace_object_path(char *source) {
    /* --- Expanding path --- */
    size_t cwd_len      = strlen(cwd);
    size_t obj_dir_len  = strlen(obj_dir);
    char *path = calloc(cwd_len + obj_dir_len + 2, sizeof(*path));
    MACE_MEMCHECK(path);
    strncpy(path,                cwd,        cwd_len);
    memcpy(path + cwd_len,      "/",        1);
    strncpy(path + cwd_len + 1,  obj_dir,    obj_dir_len);

    if (path == NULL) {
        fprintf(stderr, "Object directory '%s' does not exist.\n", obj_dir);
        exit(1);
    }

    /* --- Grow object string --- */
    size_t source_len = strlen(source);
    size_t path_len;
    while (((path_len = strlen(path)) + source_len + 2) >= object_len)
        mace_object_grow();
    memset(object, 0, object_len * sizeof(*object));

    /* --- Writing path to object --- */
    strncpy(object, path, path_len);
    if (source[0] != '/')
        object[path_len++] = '/';
    strncpy(object + path_len, source, source_len);
    object[strlen(object) - 1] = 'o';

    MACE_FREE(path);
}
/// @brief Copy input str into calloc'ed buffer
char *mace_str_buffer(const char *strlit) {
    size_t  litlen  = strlen(strlit);
    char   *buffer  = calloc(litlen + 1, sizeof(*buffer));
    MACE_MEMCHECK(buffer);
    strncpy(buffer, strlit, litlen);
    return (buffer);
}

void mace_print_message(const char *message) {
    if (message == NULL) {
        return;
    }

    Sprintf("%s\n", message);
}

void mace_chdir(const char *path) {
    if (chdir(path) != 0) {
        fprintf(stderr, "Could not cd to directory '%s'", path);
        exit(1);
    }
}

/**************** mace_build ******************/
/// @brief Run command input as a single long char string.
///     - Split tokens between spaces i.e. " "
///     - Reconstitute argv, argc
///     - Actually run command
void mace_run_commands( const char *commands,
                        const char *preorpost,
                        const char *target) {
    if (commands == NULL)
        return;

    printf("Running command-%s, target '%s'\n", preorpost, target);
    mace_chdir(cwd);

    int argc = 0, len = 8;
    char **argv = calloc(len, sizeof(*argv));

    /* -- Copy sources into modifiable buffer -- */
    char *buffer = mace_str_buffer(commands);

    /* --- Split sources into tokens --- */
    char *token = strtok(buffer, mace_command_separator);

    do {
        for (int i = 0; i < argc; i++) {
            MACE_FREE(argv[i]);
        }

        argc = 0;
        argv = mace_argv_flags(&len, &argc, argv, token, NULL, false, mace_separator);

        mace_exec_print(argv, argc);
        if (!dry_run) {
            pid_t pid = mace_exec(argv[0], argv);
            mace_wait_pid(pid);
        }

        token = strtok(NULL, mace_command_separator);
    } while (token != NULL);

    /* FREE */
    if (argv != NULL) {
        for (int i = 0; i < argc; ++i) {
            MACE_FREE(argv[i]);
        }
    }
    MACE_FREE(argv);
    MACE_FREE(buffer);
}

/// @brief Pre-build step.
///     - Compute checksums for each source, header file 
///     - Check which files need to be re-compiled
///       depending on checksums 
void mace_prebuild_target(Target *target) {
    if (target == NULL) {
        assert(0);
        return;
    }

    if (target->kind == MACE_PHONY) {
        return;   
    }

    Sprintf("Pre-build target '%s'\n", target->_name);

    // Check which sources don't need to be recompiled
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
    /* --- Preliminaries --- */
    mace_Target_Free_notargv(target);

    if (target->sources == NULL) {
        return;   
    }
    /* -- Copy sources into modifiable buffer -- */
    char *buffer = mace_str_buffer(target->sources);

    /* --- Parse sources --- */
    char *token = strtok(buffer, mace_separator);
    do {

        if (mace_isDir(token)) {
            /* Glob all sources recursively */
            size_t srclen  = strlen(token);
            char  *globstr = calloc(srclen + 6, sizeof(*globstr));
            MACE_MEMCHECK(globstr);
            strncpy(globstr,              token,  strlen(token));
            memcpy(globstr + srclen,     "/",    1);
            memcpy(globstr + srclen + 1, "**.c", 4);

            mace_compile_glob(target, globstr, target->flags);
            MACE_FREE(globstr);

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

        token = strtok(NULL, mace_separator);
    } while (token != NULL);

    mace_Target_precompile(target);
    MACE_FREE(buffer);
    mace_chdir(cwd);
}

/// @brief Build input target: compile then link.
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

    Sprintf("Building target '%s'\n", target->_name);

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
    mace_chdir(cwd);

    /* --- Linking --- */
    mace_link[target->kind - 1](target);
    mace_chdir(cwd);

}

/// @brief Check if target order is in build_order
b32 mace_in_build_order(size_t  order, int *b_order,
                        int     num) {
    b32 out = false;
    if (b_order == NULL) {
        assert(0);
        return(out);
    }
    for (int i = 0; i < num; i++) {
        if (b_order[i] == order) {
            out = true;
            break;
        }
    }
    return (out);
}

/// @brief Get target order from input hash
/// @return Target order (as added by user),
///         or -1 if not found
int mace_hash_order(u64 hash) {
    int order = -1;
    for (int i = 0; i < target_num; i++) {
        if (hash == targets[i]._hash) {
            order = i;
            break;
        }
    }
    return (order);
}

/// @brief Get target order (as added by user)
int mace_target_order(Target target) {
    return (mace_hash_order(target._hash));
}

/// @brief Add target with input order into build_order
void mace_build_order_add(size_t order) {
    assert(build_order != NULL);
    assert(build_order_num < target_num);
    if (mace_in_build_order(order, build_order, build_order_num)) {
        fprintf(stderr, "Target ID is already in build_order."
                        "Exiting.\n");
        exit(1);
    }
    build_order[build_order_num++] = order;
}

/// @brief Depth first search through target
///        dependencies to construct a build_order
///        to for user target and all its dependencies
void mace_build_order_recursive(Target target,
                                size_t *o_cnt) {
    /* Make sure recursion number isn't greater than target number */
    if ((*o_cnt) >= target_num) {
        return;
    }

    size_t order = mace_target_order(target); // target order
    /* Target already in build order, skip */
    if (mace_in_build_order(order, build_order, build_order_num)) {
        return;
    }

    /* Target has no dependencies, add target to build order */
    if (target._deps_links == NULL) {
        mace_build_order_add(order);
        assert(mace_in_build_order( order, build_order,
                                    build_order_num));
        return;
    }

    /* Visit all target dependencies */
    for (target._d_cnt = 0; target._d_cnt < target._deps_links_num; target._d_cnt++) {
        if (!mace_isTarget(target._deps_links[target._d_cnt]))
            continue;

        size_t next_target_order = mace_hash_order(target._deps_links[target._d_cnt]);
        /* Recursively search target's next dependency -> depth first search */
        mace_build_order_recursive(targets[next_target_order], o_cnt);
    }

    /* Target already in build order, skip */
    if (mace_in_build_order(order, build_order, build_order_num)) {
        return;
    }

    if (target._d_cnt != target._deps_links_num) {
        fprintf(stderr, "Not all target dependencies before target in build order.\n");
        exit(1);
    }

    /* All dependencies of target were built, add it to build order */
    mace_build_order_add(order);
    return;
}

/// @brief Check if target depends on target
///        with input hash.
b32 mace_Target_hasDep(Target *target, u64 target_hash) {
    if (target->_deps_links == NULL)
        return (false);

    for (int i = 0; i < target->_deps_links_num; i++) {
        if (target->_deps_links[i] == target_hash)
            return (true);
    }
    return (false);
}

/// @brief Check if any two targets depend on each other.
///     TODO: Resolve circular dependencies somehow.
b32 mace_circular_deps(Target *targs, size_t len) {
    /* -- Circular dependency conditions -- */
    /*   1- Target i has j dependency       */
    /*   2- Target j has i dependency       */
    for (int i = 0; i < target_num; i++) {
        u64 hash_i = targs[i]._hash;
        /* 1- going through target i's dependencies */
        for (int z = 0; z < targs[i]._deps_links_num; z++) {
            int j = mace_hash_order(targs[i]._deps_links[z]);

            /* Dependency is not in list of targets */
            if (j < 0)
                continue;

            /* Dependency is self */
            if (i == j) {
                Sprintf("Warning! Target '%s' depends on itself.\n", targs[i]._name);
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

/// @brief Creates obj_dir, build_dir...
void mace_make_dirs(void) {
    /* obj_dir for intermediary files */
    mace_mkdir(obj_dir);

    /* Move to obj_dir and make 'src' and 'include' dirs for checksums */
    mace_chdir(obj_dir);
    mace_mkdir("src");
    mace_mkdir("include");
    mace_chdir(cwd);

    /* build_dir for targets */
    mace_mkdir(build_dir);
}

/// @brief Read config string, splitting string
///        into _flags using mace_separator.
void mace_parse_config(Config *config) {
    mace_Config_Free(config);
    if (config->flags == NULL) {
        fprintf(stderr, "Config has no flags.\n");
        exit(1);
    }

    /* -- Split flags string into target orders -- */
    int len = 8;
    config->_flags    = calloc(len, sizeof(*config->_flags));
    MACE_MEMCHECK(config->_flags);
    config->_flag_num = 0;

    char *buffer = mace_str_buffer(config->flags);
    char *token  = strtok(buffer, mace_separator);
    do {
        char *flag = calloc(strlen(token) + 1, sizeof(*flag));
        MACE_MEMCHECK(flag);
        strncpy(flag, token, strlen(token));
        config->_flags[config->_flag_num++] = flag;
        /* Increase config->_flags size */
        if (config->_flag_num >= len) {
            len *= 2;
            size_t bytesize = len * sizeof(*config->_flags);
            config->_flags  = realloc(config->_flags, bytesize);
            MACE_MEMCHECK(config->_flags);
            memset(config->_flags + len / 2, 0, len / 2);
        }
        token = strtok(NULL, mace_separator);
    } while (token != NULL);
    MACE_FREE(buffer);
}

void mace_parse_configs(void) {
    for (int i = 0; i < config_num; i++) {
        mace_parse_config(&configs[i]);
    }
}

/// @brief Determine build_order of user target
///        using depth-first  search through its
///        dependencies
void mace_build_order(void) {
    size_t o_cnt = 0; /* order count */

    /* If only 1 include, build order is trivial */
    if (target_num <= 1) {
        mace_build_order_add(0);
        return;
    }

    // If user_target is not all, or default_target is not all
    //  - Build only specified target
    if ((mace_user_target > MACE_ALL_ORDER) ||
        (mace_default_target > MACE_ALL_ORDER)) {
        /* Build dependencies of default target, and itself only */
        o_cnt = mace_user_target > MACE_ALL_ORDER ? mace_user_target : mace_default_target;
        mace_build_order_recursive(targets[o_cnt], &o_cnt);
        return;
    }
    // If user_target is all, 
    //  or default_target is all and no user_target
    #ifndef NDEBUG
    b32 cond;
    cond  = (mace_user_target == MACE_NULL_ORDER) && (mace_default_target == MACE_ALL_ORDER);
    cond |= (mace_user_target == MACE_ALL_ORDER);
    assert(cond);
    #endif /* NDEBUG */

    o_cnt = 0;
    /* Visit all targets */
    while (o_cnt < target_num) {
        mace_build_order_recursive(targets[o_cnt], &o_cnt);
        o_cnt++;
    }
}

/// @brief Prepare for build step: check whats
///        needs to be recompiled
void mace_pre_build(void) {
    /* --- Make output directories. --- */
    mace_make_dirs();

    /* --- Build order using targets links and deps lists. --- */
    mace_build_order();

    /* Actually prebuild all targets */
    for (int z = 0; z < build_order_num; z++) {
        mace_Target_Grow_Headers(&targets[build_order[z]]);
        mace_prebuild_target(&targets[build_order[z]]);
    }
}

/// @brief Actually compile and link target.
void mace_build(void) {
    /* Actually build all targets */
    for (int z = 0; z < build_order_num; z++) {
        Target *target = &targets[build_order[z]];
        /* -- config argv -- */
        mace_argv_add_config(target, &target->_argv, &target->_argc, &target->_arg_len);

        mace_print_message(target->msg_pre);
        mace_run_commands(target->cmd_pre, "pre", target->_name);
        mace_build_target(target);
        mace_print_message(target->msg_post);
        mace_run_commands(target->cmd_post, "post", target->_name);
    }
}

void mace_Config_Free(Config *config) {
    if (config == NULL)
        return;

    if (config->_flags != NULL) {
        for (int i = 0; i < config->_flag_num; i++) {
            MACE_FREE(config->_flags[i]);
        }
    }
    MACE_FREE(config->_flags);
}

void mace_Target_Free(Target *target) {
    mace_Target_Free_argv(target);
    mace_Target_Free_notargv(target);
    mace_Target_Free_excludes(target);
    mace_Target_Free_deps_headers(target);
}

void mace_Target_Free_deps_headers(Target *target) {
    if (target->_headers != NULL) {
        for (int i = 0; i < target->_headers_num; i++) {
            MACE_FREE(target->_headers[i]);
        }
    }
    MACE_FREE(target->_headers);

    if (target->_deps_headers != NULL) {
        for (int i = 0; i < target->_len_sources; i++) {
            MACE_FREE(target->_deps_headers[i]);
        }
    }
    MACE_FREE(target->_deps_headers);
    MACE_FREE(target->_deps_headers_len);
    MACE_FREE(target->_deps_headers_num);
    MACE_FREE(target->_headers_hash);

    if (target->_headers_checksum != NULL) {
        for (int i = 0; i < target->_headers_num; i++) {
            MACE_FREE(target->_headers_checksum[i]);
        }
    }
    MACE_FREE(target->_headers_checksum);

    MACE_FREE(target->_headers_checksum_hash);
    MACE_FREE(target->_headers_checksum_cnt);
    MACE_FREE(target->_objects_hash_nocoll);
    MACE_FREE(target->_hdrs_changed);
}

void mace_Target_Free_excludes(Target *target) {
    MACE_FREE(target->_excludes);
}

void mace_Target_Free_notargv(Target *target) {
    MACE_FREE(target->_deps_links);
    MACE_FREE(target->_recompiles);
}

void mace_Target_Free_argv(Target *target) {
    mace_argv_free(target->_argv_includes, target->_argc_includes);
    target->_argv_includes  = NULL;
    target->_argc_includes  = 0;
    mace_argv_free(target->_argv_link_flags, target->_argc_link_flags);
    target->_argv_link_flags    = NULL;
    target->_argc_link_flags    = 0;
    mace_argv_free(target->_argv_links, target->_argc_links);
    target->_argv_links     = NULL;
    target->_argc_links     = 0;
    mace_argv_free(target->_argv_flags, target->_argc_flags);
    target->_argv_flags     = NULL;
    target->_argc_flags     = 0;
    mace_argv_free(target->_argv_sources, target->_argc_sources);
    target->_argv_sources   = NULL;
    mace_argv_free(target->_argv_objects, target->_argc_sources);
    target->_argv_objects   = NULL;
    target->_argc_sources   = 0;
    
    MACE_FREE(target->_argv_objects_cnt);
    MACE_FREE(target->_argv_objects_hash);
    if ((target->_argv != NULL) && (target->_argc > 0))  {
        if (target->_argc_tail > 0) {
            for (int i = target->_argc_tail; i < target->_argc; i++) {
                MACE_FREE(target->_argv[i]);
            }
        }
        MACE_FREE(target->_argv);
    }
}

/// @brief Alloc _deps_header arrays if don't exist. 
///        Realloc to bigger if num close to len
void mace_Target_Grow_deps_headers(Target *target,
                                   int source_i) {
    if (target->_deps_headers[source_i] == NULL) {
        target->_deps_headers_num[source_i] = 0;
        target->_deps_headers_len[source_i] = 8;
        target->_deps_headers[source_i] = calloc(target->_deps_headers_len[source_i],
                                                 sizeof(**target->_deps_headers));
    }
    if (target->_deps_headers_num[source_i] >= target->_deps_headers_len[source_i]) {
        target->_deps_headers_len[source_i] *= 2;
        size_t bytesize = target->_deps_headers_len[source_i] * sizeof(**target->_deps_headers);
        target->_deps_headers[source_i] = realloc(target->_deps_headers[source_i], bytesize);
    }
    MACE_MEMCHECK(target->_deps_headers[source_i]);
}

/// @brief Alloc header arrays if don't exist. 
///     Realloc to bigger if num close to len
void mace_Target_Grow_Headers(Target *target) {
    /* -- Alloc headers -- */
    if (target->_headers == NULL) {
        target->_headers_len = 8;
        target->_headers_num = 0;
        target->_headers  = calloc(target->_headers_len, sizeof(*target->_headers));
    }
    if (target->_headers_hash == NULL) {
        target->_headers_hash  = calloc(target->_headers_len, sizeof(*target->_headers_hash));
    }

    /* -- Alloc _hdrs_changed -- */
    if (target->_hdrs_changed == NULL) {
        target->_hdrs_changed  = calloc(target->_headers_len, sizeof(*target->_hdrs_changed));
    }

    /* -- Alloc _headers_checksum -- */
    if (target->_headers_checksum == NULL) {
        target->_headers_checksum  = calloc(target->_headers_len, sizeof(*target->_headers_checksum));
    }
    if (target->_headers_checksum_hash == NULL) {
        // Always less hashes than _headers_checksum
        target->_headers_checksum_hash = calloc(target->_headers_len,
                                                sizeof(*target->_headers_checksum_hash));
    }

    if (target->_headers_checksum_cnt == NULL) {
        // Always less hashes than _headers_checksum
        target->_headers_checksum_cnt = calloc(target->_headers_len,
                                               sizeof(*target->_headers_checksum_cnt));
    }

    /* -- Realloc _headers_checksum -- */
    if (target->_headers_num >= (target->_headers_len - 1)) {
        size_t bytesize = target->_headers_len * 2 * sizeof(*target->_hdrs_changed);
        target->_hdrs_changed = realloc(target->_hdrs_changed, bytesize);
        memset(target->_hdrs_changed + target->_headers_len, 0, bytesize / 2);
    }

    /* -- Realloc _headers_checksum -- */
    if (target->_headers_num >= (target->_headers_len - 1)) {
        size_t bytesize = target->_headers_len * 2 * sizeof(*target->_headers_checksum);
        target->_headers_checksum = realloc(target->_headers_checksum, bytesize);
        memset(target->_headers_checksum + target->_headers_len, 0, bytesize / 2);

        bytesize = target->_headers_len * 2 * sizeof(*target->_headers_checksum_hash);
        target->_headers_checksum_hash = realloc(target->_headers_checksum_hash, bytesize);
        memset(target->_headers_checksum_hash + target->_headers_len, 0, bytesize / 2);

        bytesize = target->_headers_len * 2 * sizeof(*target->_headers_checksum_cnt);
        target->_headers_checksum_cnt = realloc(target->_headers_checksum_cnt, bytesize);
        memset(target->_headers_checksum_cnt + target->_headers_len, 0, bytesize / 2);
    }

    MACE_MEMCHECK(target->_headers_checksum_hash);

    /* -- Realloc headers -- */
    if (target->_headers_num >= (target->_headers_len - 1)) {
        size_t bytesize = target->_headers_len * 2 * sizeof(*target->_headers_hash);
        target->_headers_hash = realloc(target->_headers_hash, bytesize);
        memset(target->_headers_hash + target->_headers_len, 0, bytesize / 2);
    }

    if (target->_headers_num >= (target->_headers_len - 1)) {
        target->_headers_len *= 2;
        size_t bytesize = target->_headers_len * sizeof(*target->_headers);
        target->_headers = realloc(target->_headers, bytesize);
        memset(target->_headers + target->_headers_len / 2, 0, bytesize / 2);
    }
}
/// @brief Read target object .d files to check
///        which headers are required for re-compilation
void mace_Target_Read_Objdeps(Target *target,
                              char *deps,
                              int source_i) {
    /* --- Split headers into tokens --- */
    char *header   = strtok(deps, mace_separator);
    size_t cwd_len = strlen(cwd);

    /* --- Hash headers into _deps_links --- */
    while (header != NULL) {
        /* Skip if file is not a header */
        char *dot  = strrchr(header,  '.'); /* last dot in path */
        if (dot == NULL) {
            header = strtok(NULL, mace_separator);
            continue;
        }
        size_t ext = dot - header;

        if (header[ext + 1] != 'h') {
            header = strtok(NULL, mace_separator);
            continue;
        }

        /* Skip if header is not in cwd */
        if (target->_checkcwd && (strncmp(header, cwd, cwd_len) != 0)) {
            header = strtok(NULL, mace_separator);
            continue;
        }

        /* add header to list of all headers */
        u64 hash = mace_Target_Header_Add(target, header);

        /* Add header to list of header_deps of object */
        int header_order = mace_Target_header_order(target, hash);
        mace_Target_Objdep_Add(target, header_order, source_i);

        header = strtok(NULL, mace_separator);
    }
}

/// @brief Check if target has header from input
///        filename hash.
/// @return -1 if not found, header_order if found.
int mace_Target_header_order(Target *target, u64 hash) {
    for (int i = 0; i < target->_headers_num; ++i) {
        if (target->_headers_hash[i] == hash)
            return (i);
    }
    return (-1);
}

void mace_Target_Header_Add_Objpath(Target *target,
                                    char *header) {
    char *header_checksum = mace_checksum_filename(header, MACE_CHECKSUM_MODE_INCLUDE);
    u64 hash = mace_hash(header_checksum);

    /* Check if header_checksum already exists */
    int hash_id = -1;
    for (int i = 0; i < target->_headers_num; ++i) {
        if (hash == target->_headers_checksum_hash[i]) {
            hash_id = i;
            break;
        }
    }

    if (hash_id < 0) {
        /* header_checksum hash not found, adding it at same order */
        assert(target->_headers_checksum_hash != NULL);
        target->_headers_checksum_hash[target->_headers_num] = hash;
    } else {
        /* header_checksum hash found, adding number to path */
        size_t bytesize = (strlen(header_checksum) + 2) * sizeof(*header_checksum);
        header_checksum = realloc(header_checksum, bytesize);
        MACE_MEMCHECK(header_checksum);
        char *pos = strrchr(header_checksum, '.');
        *(pos) = target->_headers_checksum_cnt[hash_id] + '0';
        memcpy(pos + 1, ".sha1", 4);
        target->_headers_checksum_cnt[hash_id]++;
    }

    /* Adding header_checksum */
    assert(header_checksum != NULL);
    target->_headers_checksum[target->_headers_num] = header_checksum;
}

/// @brief Add header file name to target.
u64 mace_Target_Header_Add(Target *target, char *header) {
    /* Check if header hash already in _headers_hash */
    /* Add header hash to _headers_hash */
    u64 hash = mace_hash(header);

    /* Add header name to _headers */
    mace_Target_Grow_Headers(target);
    if (mace_Target_header_order(target, hash) == -1) {
        /* Add header hash */
        target->_headers_hash[target->_headers_num] = hash;

        /* Add header name, later used for checksums */
        size_t len = strlen(header);
        assert(target->_headers[target->_headers_num] == NULL);
        target->_headers[target->_headers_num] = calloc(len + 1, sizeof(**target->_headers));
        strncpy(target->_headers[target->_headers_num], header, len);

        /* Add header obj name */
        mace_Target_Header_Add_Objpath(target, header);

        /* Increment number of headers */
        target->_headers_num++;
    }
    return (hash);
}

/// @brief Add header as dependency of target.
void mace_Target_Objdep_Add(Target *target,
                            int header_order,
                            int source_i) {
    /* Check if header_order in _deps_headers */
    assert(source_i > -1);
    assert(source_i <= target->_argc_sources);
    for (int i = 0; i < target->_deps_headers_num[source_i]; i++) {
        if (target->_deps_headers[source_i][i] == header_order)
            return;
    }

    mace_Target_Grow_deps_headers(target, source_i);
    int i = target->_deps_headers_num[source_i]++;
    assert(target->_deps_headers            != NULL);
    assert(target->_deps_headers[source_i]  != NULL);
    target->_deps_headers[source_i][i] = header_order;
}

/// @brief Read .d file and built .ho file from it.
char *mace_Target_Read_d(Target *target, int source_i) {
    char *obj_file_flag = target->_argv_objects[source_i];

    /* obj_file_flag should start with "-o" */
    if ((obj_file_flag[0] != '-') || (obj_file_flag[1] != 'o')) {
        /* error? */
        fprintf(stderr, "obj_file_flag '%s' missing the -o flag.\n", obj_file_flag);
        exit(1);
    }
    int oflagl = 2;
    int obj_hash_id;
    size_t obj_len  = strlen(obj_file_flag);
    assert((obj_len + 5) > oflagl);
    char  *obj_file = calloc(   obj_len - oflagl + 5,
                                sizeof(*obj_file));
    MACE_MEMCHECK(obj_file);
    strncpy(obj_file, obj_file_flag + oflagl, obj_len - oflagl);
    char buffer[MACE_OBJDEP_BUFFER];
    size_t size = 0;
    char *dot   = strrchr(obj_file,  '.'); /* last dot in path */
    size_t ext  = dot - obj_file;

    /* Check if .ho exists */
    memcpy(obj_file + ext + 1, "ho", 2);
    obj_file[ext + 3] = '\0';

    FILE *fho = fopen(obj_file, "r");
    b32 fho_exists = false;
    if (fho != NULL) {
        fho_exists = true;
        fclose(fho);
    }

    /* Check if .d exists */
    obj_file[ext + 1] = 'd';
    obj_file[ext + 2] = '\0';
    FILE *fd = fopen(obj_file, "rb");
    if (fd == NULL) {
        fprintf(stderr, "Object dependency file '%s' does not exist.\n", obj_file);
        exit(1);
    }
    target->_deps_headers_num[source_i] = 0;

    /* Parse all dependencies, " " separated */
    while (fgets(buffer, MACE_OBJDEP_BUFFER, fd) != NULL) {
        size_t len = strlen(buffer);
        /* - Replace \n with \0 ' ' - */
        b32 line_end = false;
        if (buffer[len - 1] == '\n') {
            line_end = true;
            buffer[len - 1] = '\0';
        }

        /* Check that target has object with nocoll hashes */
        obj_file[ext + 1] = 'o';
        u64 obj_hash = mace_hash(obj_file);
        obj_hash_id = Target_hasObjectHash_nocoll(target, obj_hash);
        assert(obj_hash_id < target->_objects_hash_nocoll_num);
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
    b32 source_changed = target->_recompiles[source_i];
    if ((!source_changed) && (fho_exists)) {
        MACE_FREE(obj_file);
        return (NULL);
    }
    return (obj_file);
}


/// @brief Parse .d file, recording all header
///        files to .ho files. Should only be
///        called if source file changed
void mace_Target_Parse_Objdep(Target *target, int source_i) {
    /* Set _deps_headers_num to invalid */
    target->_deps_headers_num[source_i] = -1;

    char *obj_file = mace_Target_Read_d(target, source_i);
    if (obj_file == NULL) {
        /* Skip: no need to write .ho file */
        return;
    }
    if (target->_deps_headers_num[source_i] <= 0) { 
        /* Skip: no headers */
        return;
    }

    /* Write _deps_header to .ho file */
    char *dot  = strrchr(obj_file,  '.'); /* last dot in path */
    size_t ext = dot - obj_file;

    memcpy(obj_file + ext + 1, "ho", 2);
    obj_file[ext + 3] = '\0';

    FILE *fho = fopen(obj_file, "wb");
    assert( target->_deps_headers[source_i] != NULL);    
    fwrite( target->_deps_headers[source_i], 
            sizeof(**target->_deps_headers),
            target->_deps_headers_num[source_i], fho);
    fclose(fho);

    MACE_FREE(obj_file);
}

/// @brief Read .ho file and put all read headers
///        in _deps_headers
void mace_Target_Read_ho(Target *target, int source_i) {
    // Only read if .ho file was not created.
    if (target->_deps_headers_num[source_i] > 0)
        return;

    target->_deps_headers_num[source_i] = 0;

    // read .ho file. It should exist.
    char *obj_file_flag = target->_argv_objects[source_i];

    /* obj_file_flag should start with "-o" */
    if ((obj_file_flag[0] != '-') || (obj_file_flag[1] != 'o')) {
        /* error? */
        fprintf(stderr, "obj_file_flag '%s' missing the -o flag.\n", obj_file_flag);
        exit(1);
    }
    int oflagl = 2;
    size_t obj_len = strlen(obj_file_flag);
    char *obj_file = calloc(obj_len - oflagl + 5, sizeof(*obj_file));
    MACE_MEMCHECK(obj_file);
    strncpy(obj_file, obj_file_flag + oflagl, obj_len - oflagl);

    char *dot        = strrchr(obj_file,  '.'); /* last dot in path */
    size_t ext = dot - obj_file;

    /* Check if .ho exists */
    memcpy(obj_file + ext + 1, "ho", 2);
    obj_file[ext + 3] = '\0';
    FILE *fho = fopen(obj_file, "rb");
    if (fho == NULL) {
        /* .ho file does not exist: no dependencies */ 
        return;
    }

    /* Get total number of bytes in file */
    fseek(fho, 0L, SEEK_END);
    int bytesize = ftell(fho);
    fseek(fho, 0L, SEEK_SET);

    /* Realloc _deps_headers */
    target->_deps_headers_num[source_i] = bytesize / sizeof(**target->_deps_headers);
    target->_deps_headers_len[source_i] = target->_deps_headers_num[source_i];
    MACE_FREE(target->_deps_headers[source_i]);

    /* Read all bytes into _deps_headers */
    target->_deps_headers[source_i] = calloc(1, bytesize);
    MACE_MEMCHECK(target->_deps_headers[source_i]);
    fread(target->_deps_headers[source_i], bytesize, 1, fho);
    fclose(fho);
    MACE_FREE(obj_file);
}

/// @brief Save header order dependencies to 
///        .ho .d should exist.
void mace_Target_Parse_Objdeps(Target *target) {
    /* Loop over all _argv_sources */
    for (int i = 0; i < target->_argc_sources; i++) {
        mace_Target_Parse_Objdep(target, i);
        mace_Target_Read_ho(target, i);
    }
}

/// @brief Alloc stuff in preparation for user 
///        to set targets, configs, etc.
void mace_pre_user(Mace_Args *args) {
    mace_post_build(NULL);

    /* --- 1. Set switches --- */
    if (args != NULL) {
        silent         = args->silent;
        dry_run        = args->dry_run;
        verbose        = dry_run ? true : args->debug;
        build_all      = args->build_all;
    }

    /* --- 2. Record cwd --- */
    if (getcwd(cwd, MACE_CWD_BUFFERSIZE) == NULL) {
        fprintf(stderr, "getcwd() error %d: '%s'\n", errno, strerror(errno));
        exit(1);
    }

    /* --- 3. Reserved target names --- */
    int i   = MACE_ALL_ORDER + MACE_RESERVED_TARGETS_NUM;
    mace_reserved_targets[i]    = mace_hash(MACE_ALL);

    /* --- 4. Memory allocation --- */
    target_len      = MACE_DEFAULT_TARGET_LEN;
    config_len      = MACE_DEFAULT_TARGET_LEN;
    object_len      = MACE_DEFAULT_OBJECT_LEN;
    build_order_num = 0;

    object      = calloc(object_len, sizeof(*object));
    targets     = calloc(target_len, sizeof(*targets));
    configs     = calloc(config_len, sizeof(*configs));
    build_order = calloc(target_len, sizeof(*build_order));

    /* --- 5. Default output folders --- */
    mace_set_build_dir("build");
    mace_set_obj_dir("obj");
}

/// @brief Prepare for build after user added
///        targets, configs, etc.
void mace_post_user(Mace_Args *args) {
    //   1- Moves to user set dir if not NULL.
    //   2- Checks that at least one target exists,
    //   3- Checks that there are no circular dependency.
    //   4- Parse configs to set flags.
    //   5- Computes user_target order with priority:
    //      a- input argument
    //      b- config
    //      c- macefile
    //   6- Computes default target order from default target_hash.
    //   7- Allocs queue for processes.
    //   8- Overrides compiler with priority:
    //      a- input argument
    //      b- config
    //      c- macefile
    //   9- Overrides archiver with priority:
    //      a- input argument
    //      b- config
    //      c- macefile
    //   10- Checks that compiler and archiver are set.

    /* 1. Move to args->dir */
    if ((args != NULL) && (args->dir != NULL)) {
        mace_chdir(args->dir);
    }

    /* 2. Check that a target exists */
    if ((targets == NULL) || (target_num <= 0)) {
        fprintf(stderr, "No targets to compile. Exiting.\n");
        exit(1);
    }

    /* 3. Check for circular dependency */
    if (mace_circular_deps(targets, target_num)) {
        fprintf(stderr, "Circular dependency in linked library detected. Exiting\n");
        exit(1);
    }
    
    if (targets == NULL) {
        return;
    }

    if (target_num == 0) {
        return;
    }

    /* 4. Parsing configs */
    mace_parse_configs();

    /* 5. Computes default target order from default target_hash */
    mace_default_target_order();
    mace_default_config_order();

    /* 6. Check which target should be compiled */
    mace_user_target_set(   args->user_target_hash,
                            args->user_target);
    mace_target_resolve();

    /* 7. Check which config should be compiled */
    mace_user_config_set(   args->user_config_hash,
                            args->user_config);
    assert(mace_target < target_num);
    mace_config_resolve(&targets[mace_target]);
    Config *config = &configs[mace_config];

    /* 8. Process queue alloc */
    assert(args->jobs >= 1);
    assert(pqueue == NULL);
    plen = args->jobs;
    pqueue = calloc(plen, sizeof(*pqueue));
    MACE_MEMCHECK(pqueue);

    /* 8.b Override compiler with config */
    if (config->cc != NULL) {
        mace_set_compiler(config->cc);
    }

    /* 8.c Override compiler with input arguments */
    if (args->cc != NULL) {
        mace_set_compiler(args->cc);
    }

    /* 9.b Override archiver with config */
    if (config->ar != NULL) {
        mace_set_archiver(config->ar);
    }

    /* 9.c Override archiver with input arguments */
    if (args->ar != NULL) {
        mace_set_archiver(args->ar);
    }

    /* 9. Check that compiler and archiver are set */
    if (cc == NULL) {
        fprintf(stderr, "Compiler not set. Exiting.\n");
        exit(1);
    }
    if (ar == NULL) {
        fprintf(stderr, "Archiver not set. Exiting.\n");
        exit(1);
    }
}

/// @brief Free everything, cause all targets were built.
void mace_post_build(Mace_Args *args) {
    Mace_Args_Free(args);
    for (int i = 0; i < target_num; i++) {
        mace_Target_Free(&targets[i]);
    }
    MACE_FREE(targets);

    for (int i = 0; i < config_num; i++) {
        mace_Config_Free(&configs[i]);
    }
    MACE_FREE(configs);
    MACE_FREE(pqueue);
    MACE_FREE(object);
    MACE_FREE(obj_dir);
    MACE_FREE(build_dir);
    MACE_FREE(build_order);

    target_num      = 0;
    config_num      = 0;
    object_len      = 0;
    build_order_num = 0;
}

/// @brief Realloc _deps_links to bigger
///        if num close to len
void mace_Target_Deps_Grow(Target *target) {
    if (target->_deps_links_len <= target->_deps_links_num) {
        target->_deps_links_len *= 2;
        size_t bytesize = target->_deps_links_len * sizeof(*target->_deps_links);
        target->_deps_links = realloc(target->_deps_links, bytesize);
    }
}

void mace_Target_Deps_Add(Target *target, u64 target_hash) {
    if (!mace_Target_hasDep(target, target_hash)) {
        mace_Target_Deps_Grow(target);
        target->_deps_links[target->_deps_links_num++] = target_hash;
    }
}

void mace_Target_Deps_Hash(Target *target) {
    /* --- Preliminaries --- */
    if ((target->links == NULL) && (target->dependencies == NULL))
        return;

    /* --- Alloc space for deps --- */
    target->_deps_links_num =  0;
    target->_deps_links_len = 16;
    MACE_FREE(target->_deps_links);
    target->_deps_links = malloc(target->_deps_links_len * sizeof(*target->_deps_links));
    MACE_MEMCHECK(target->_deps_links);

    /* --- Add links to _deps_links --- */
    do {
        if (target->links == NULL)
            break;

        /* --- Copy links into modifiable buffer --- */
        char *buffer = mace_str_buffer(target->links);

        /* --- Split links into tokens, --- */
        char *token = strtok(buffer, mace_separator);
        /* --- Hash tokens into _deps_links --- */
        do {
            mace_Target_Deps_Add(target, mace_hash(token));
            token = strtok(NULL, mace_separator);
        } while (token != NULL);
        MACE_FREE(buffer);
    } while (false);

    /* --- Add dependencies to _deps_links --- */
    do {
        if (target->dependencies == NULL)
            break;

        /* --- Copy links into modifiable buffer --- */
        char *buffer = mace_str_buffer(target->dependencies);

        /* --- Split links into tokens, --- */
        char *token = strtok(buffer, mace_separator);

        /* --- Hash tokens into _deps_links --- */
        do {
            mace_Target_Deps_Add(target, mace_hash(token));
            token = strtok(NULL, mace_separator);
        } while (token != NULL);
        MACE_FREE(buffer);
    } while (false);
}

/******************* checksums ******************/
#define MACE_SRC_FOLDER_STR_LEN 4
#define MACE_INCLUDE_FOLDER_STR_LEN 8
#define MACE_CHECKSUM_EXTENSION_STR_LEN 5
#define MACE_SEPARATOR_STR_LEN 1
/// @brief Compute sha1dc checksum of file.
char *mace_checksum_filename(char *file, int mode) {
    // Files should be .c or .h
    if (obj_dir == NULL) {
        assert(0);
        return(NULL);
    }
    /* last dot in path      */
    char *dot        = strrchr(file, '.');
    /* last slash in path    */
    char *slash      = strrchr(file, '/');
    if (dot == NULL) {
        fprintf(stderr, "Could not find extension in filename.\n");
        exit(1);
    }

    /* File length of just file without extension */
    int dot_i   = (int)(dot - file);
    int slash_i = (slash == NULL) ? 0 : (int)(slash - file + 1);
    assert(dot_i > slash_i);
    size_t file_len     = dot_i - slash_i;
    size_t obj_dir_len  = strlen(obj_dir);

    /* Alloc new file */
    size_t checksum_len  = (file_len + MACE_SEPARATOR_STR_LEN + MACE_CHECKSUM_EXTENSION_STR_LEN) + obj_dir_len + 1;
    if (mode == MACE_CHECKSUM_MODE_SRC) {
        checksum_len += MACE_SRC_FOLDER_STR_LEN;
    } else if (mode == MACE_CHECKSUM_MODE_INCLUDE) {
        checksum_len += MACE_INCLUDE_FOLDER_STR_LEN;
    }

    char *sha1 = calloc(checksum_len, sizeof(*sha1));
    MACE_MEMCHECK(sha1);

    strncpy(sha1, obj_dir, obj_dir_len);
    size_t total = obj_dir_len;

    /* Add slash to obj_dir if not present */
    if (sha1[obj_dir_len - 1] != '/') {
        memcpy(sha1 + total, "/", MACE_SEPARATOR_STR_LEN);
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
    strncpy(sha1 + total, file + slash_i, file_len);
    total += file_len;

    /* Add extension */
    memcpy(sha1 + total, ".sha1", MACE_CHECKSUM_EXTENSION_STR_LEN);
    return (sha1);
}

/// @brief Check if two shadc1 checksums are equal
inline b32 mace_sha1dc_cmp(u8 hash1[SHA1_LEN], u8 hash2[SHA1_LEN]) {
    return (memcmp(hash1, hash2, SHA1_LEN) == 0);
}

/// @brief Check for collision in sha1dc
///        checksum between input file and hash
void mace_sha1dc(char *file, u8 hash[SHA1_LEN]) {
    if (file == NULL) {
        assert(0);
        return;
    }
    size_t size;
    int foundcollision;

    /* - open file - */
    FILE *fd = fopen(file, "rb");
    if (fd == NULL) {
        fprintf(stderr, "cannot open file: %s\n", file);
        exit(1);
    }

    /* - compute checksum - */
    SHA1_CTX ctx2;
    SHA1DCInit(&ctx2);
    char buffer[USHRT_MAX + 1];
    while (true) {
        size = fread(buffer, 1, (USHRT_MAX + 1), fd);
        SHA1DCUpdate(&ctx2, buffer, (unsigned)(size));
        if (size != (USHRT_MAX + 1))
            break;
    }
    if (ferror(fd)) {
        fprintf(stderr, "file read error: %s\n", file);
        exit(1);
    }
    if (!feof(fd)) {
        fprintf(stderr, "not end of file?: %s\n", file);
        exit(1);
    }

    /* - check for collision - */
    foundcollision = SHA1DCFinal(hash, &ctx2);

    // TODO: Any way to solve collision? 
    if (foundcollision) {
        fprintf(stderr, "sha1dc: collision detected");
        exit(1);
    }

    fclose(fd);
}

/************** argument parsing **************/
/* list of parg options to be parsed, with usage */
static struct parg_opt longopts[] = {
    // {NULL,          PARG_NOARG,  0,  0,  NULL,   "Debug options:"},
    {"ar",          PARG_REQARG, 0, 'a', NULL,   "Override archiver"},
    {"always-make", PARG_NOARG,  0, 'B', NULL,   "Build all targets without condition"},
    {"directory",   PARG_REQARG, 0, 'C', "DIR",  "Move to directory before anything else"},
    {"cc",          PARG_REQARG, 0, 'c', "CC",   "Override C compiler"},
    {"debug",       PARG_NOARG,  0, 'd', NULL,   "Print debug info"},
    {"config",      PARG_REQARG, 0, 'g', "NAME", "Name of config"},
    {"help",        PARG_NOARG,  0, 'h', NULL,   "Display help and exit"},
    {"jobs",        PARG_REQARG, 0, 'j', "INT",  "Allow N jobs at once"},
    {"dry-run",     PARG_NOARG,  0, 'n', NULL,   "Don't build, just echo commands"},
    {"silent",      PARG_NOARG,  0, 's', NULL,   "Don't echo commands"},
    {"version",     PARG_NOARG,  0, 'v', NULL,   "Display version and exit"},
    {NULL,          PARG_NOARG,  0,  0,  NULL,   "Convenience executable options:"},
    {"file",        PARG_REQARG, 0, 'f', "FILE", "Specify input macefile. Defaults to macefile.c"},
};

Mace_Args Mace_Args_default = {
    .user_target        = NULL,
    .user_target_hash   = 0,
    .user_config        = NULL,
    .user_config_hash   = 0,
    .jobs               = MACE_JOBS_DEFAULT,
    .cc                 = NULL,
    .ar                 = NULL,
    .macefile           = NULL,
    .dir                = NULL,
    .debug              = false,
    .silent             = false,
    .dry_run            = false,
    .build_all          = false,
};

/// @brief Compare user flag input arguments
///        to enviroment variables
Mace_Args mace_combine_args_env(Mace_Args user, Mace_Args env) {
        Mace_Args out;
        /* Args Hierarchy: User > Env > Defaults.
        **  - Use user values only if non-default. */

        b32 _user_target       = (user.user_target      != Mace_Args_default.user_target);
        b32 _macefile          = (user.macefile         != Mace_Args_default.macefile);
        b32 _user_config       = (user.user_config      != Mace_Args_default.user_config);
        b32 _dir               = (user.dir              != Mace_Args_default.dir);
        b32 _cc                = (user.cc               != Mace_Args_default.cc);
        b32 _ar                = (user.ar               != Mace_Args_default.cc);
        b32 _user_target_hash  = (user.user_target_hash != Mace_Args_default.user_target_hash);
        b32 _user_config_hash  = (user.user_config_hash != Mace_Args_default.user_config_hash);
        b32 _jobs              = (user.jobs             >= 1);
        b32 _debug             = (user.debug            != Mace_Args_default.debug);
        b32 _silent            = (user.silent           != Mace_Args_default.silent);
        b32 _dry_run           = (user.dry_run          != Mace_Args_default.dry_run);
        b32 _build_all         = (user.build_all        != Mace_Args_default.build_all);

        out.user_target      = _user_target      ? user.user_target      : env.user_target;
        out.macefile         = _macefile         ? user.macefile         : env.macefile;
        out.user_config      = _user_config      ? user.user_config      : env.user_config;
        out.dir              = _dir              ? user.dir              : env.dir;
        out.cc               = _cc               ? user.cc               : env.cc;
        out.ar               = _ar               ? user.ar               : env.ar;
        out.user_target_hash = _user_target_hash ? user.user_target_hash : env.user_target_hash;
        out.user_config_hash = _user_config_hash ? user.user_config_hash : env.user_config_hash;
        out.jobs             = _jobs             ? user.jobs             : env.jobs;
        out.debug            = _debug            ? user.debug            : env.debug;
        out.silent           = _silent           ? user.silent           : env.silent;
        out.dry_run          = _dry_run          ? user.dry_run          : env.dry_run;
        out.build_all        = _build_all        ? user.build_all        : env.build_all;
        return (out);
    }

/// @brief Parse MACEFLAGS environment variable
Mace_Args mace_parse_env(void) {
    char *env_args = getenv("MACEFLAGS");
    if (env_args != NULL) {
        int argc = 1, len = 8;
        char *tmp = env_args;
        /* Count number of spaces, split into argv */
        while ((tmp = strstr(tmp, " "))) {
            argc++;
            tmp++;
        }
        char **argv = calloc(len, sizeof(*argv));
        argv = mace_argv_flags(&len, &argc, argv, env_args, NULL, false, mace_separator);
        argc++;
        Mace_Args out = mace_parse_args(argc, argv);
        mace_argv_free(argv, argc);
        return (out);
    }

    return (Mace_Args_default);
}

/// @brief Parse builder/mace convenience
///        executable input args using parg
Mace_Args mace_parse_args(int argc, char *argv[]) {
    Mace_Args out_args      = Mace_Args_default;
    struct parg_state ps    = parg_state_default;
    int longindex;
    int c;
    size_t len;

    if (argc <= 1) {
        return (out_args);
    }

    while ((c = parg_getopt_long(&ps, argc, argv, 
                                "a:Bc:C:df:g:hj:no:sv",
                                longopts, &longindex)) != -1) {
        switch (c) {
            case 1:
                len = strlen(ps.optarg);
                out_args.user_target = calloc(len + 1, sizeof(*out_args.user_target));
                strncpy(out_args.user_target, ps.optarg, len);
                out_args.user_target_hash = mace_hash(ps.optarg);
                break;
            case 'a':
                len = strlen(ps.optarg);
                out_args.ar = calloc(len + 1, sizeof(*out_args.dir));
                strncpy(out_args.ar, ps.optarg, len);
                break;
            case 'B':
                out_args.build_all = true;
                break;
            case 'C':
                len = strlen(ps.optarg);
                out_args.dir = calloc(len + 1, sizeof(*out_args.dir));
                strncpy(out_args.dir, ps.optarg, len);
                break;
            case 'c':
                len = strlen(ps.optarg);
                out_args.cc = calloc(len + 1, sizeof(*out_args.dir));
                strncpy(out_args.cc, ps.optarg, len);
                break;
            case 'd':
                out_args.debug = true;
                break;
            case 'f': {
                len = strlen(ps.optarg);
                out_args.macefile = calloc(len + 1, sizeof(*out_args.macefile));
                strncpy(out_args.macefile, ps.optarg, len);
                break;
            }
            case 'g':
                len = strlen(ps.optarg);
                out_args.user_config = calloc(len + 1, sizeof(*out_args.user_config));
                strncpy(out_args.user_config, ps.optarg, len);
                out_args.user_config_hash = mace_hash(ps.optarg);
                break;
            case 'h':
                mace_parg_usage(argv[0], longopts);
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
                } else if (ps.optopt == 'f') {
                    printf("option -f/--file requires an argument\n");
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

void Mace_Args_Free(Mace_Args *args) {
    /* Skip if NULL */
    if (args == NULL){
        return;
    }

    MACE_FREE(args->macefile);
    MACE_FREE(args->user_target);
    MACE_FREE(args->dir);
    MACE_FREE(args->cc);
    MACE_FREE(args->ar);
}

/******************* main *******************/

#ifndef MACE_OVERRIDE_MAIN
/// @brief mace.h implementation of main.
/// To have control when builder executable runs.
///     1- Run "mace" function, getting user info:
///       - Compiler, Targets, Configs, Directories
///     2- Builds target dependency graph
///     3- Determines which targets need to be recompiled
///     4- Build the targets
int main(int argc, char *argv[]) {
    /* --- Parse user arguments --- */
    Mace_Args args      = mace_parse_args(argc, argv);
    Mace_Args args_env  = mace_parse_env();
    args = mace_combine_args_env(args, args_env);

    /* --- Pre-user ops --- 
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

    return(0);
}
#endif /* MACE_OVERRIDE_MAIN */

/*----------------------------------------------*/
/*               MACE SOURCE END                */
/*----------------------------------------------*/
