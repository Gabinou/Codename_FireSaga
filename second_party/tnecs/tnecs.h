#ifndef __TNECS_H__
#define __TNECS_H__
/*
**  Copyright (C) Gabriel Taillon 2023-2025
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
**  tnecs: Tiny C99 Entity-Component-System (ECS) library.
**      
**  The simplest possible C99 ECS library, 
**  only with the minimum necessary features. 
**
**  Abbreviations:
**      - E:    Entity      - Pi:   Pipeline
**      - C:    Component   - Ph:   Phase
**      - S:    System      - W:    World
**      - O:    Order       - Ex:   Exclusive
**      - A:    Archetype  i.e. ull w/ many bits set
**      - T:    Type       i.e. ull w/ single bit set 
**      Notes: 
**          1- wC   means "with Components" 
**          2- byA  means "by Archetype" 
**          3- Plural forms i.e. "Cs"
**  Glossary:
**      id: Unique identifier, always 1++.
**          1. Self is NOT id: C,  S,  T,  A
**          2. Self is id:     E, Ph, Pi 
**      Order: index of thing, normally in other context
**          - E_O_byA: order of entity, in archetype array.
*/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#ifndef log2 /* for tcc: log2(x) = log(x) / log(2) */
    #define log2(x) (log(x) * 1.44269504088896340736)
#endif

/* --- TYPEDEFS --- */
typedef unsigned long long int tnecs_ns;
typedef unsigned long long int tnecs_Ph;
typedef unsigned long long int tnecs_E;
typedef unsigned long long int tnecs_Pi;
typedef unsigned long long int tnecs_C;

/* -- Forward declarations -- */
struct tnecs_In;

/* --- CONSTANTS --- */
enum TNECS_PUBLIC {
    TNECS_NULL           =         0,
    TNECS_NULLSHIFT      =         1,
    TNECS_E_CAP          = 100000000,
    TNECS_Pi_CAP         =        64,
    TNECS_Ph_CAP         =        64,
    TNECS_C_CAP          =        64
};


/* -- Functions -- */
typedef void (*tnecs_S_f)   (struct tnecs_In *);
typedef void (*tnecs_free_f)(void *);
typedef void (*tnecs_init_f)(void *);

/* -- struct -- */
typedef struct tnecs_C_arr { /* 1D array of Cs */
    size_t   num;
    size_t   len;

    tnecs_C  type;
    void    *Cs; /* [E_O_byA] */
} tnecs_C_arr;

typedef struct tnecs_arr {
    size_t   num;
    size_t   len;

    void    *arr;
} tnecs_arr;

typedef struct tnecs_Phs {
    /* phase == id == 1++ */
    size_t num;
    size_t len;

    size_t       *len_Ss; /* [Ph]       */
    size_t       *num_Ss; /* [Ph]       */
    size_t      **Ss_id;  /* [Ph][S_O]  */
    tnecs_S_f   **Ss;     /* [Ph][S_O]  */
} tnecs_Phs;

typedef struct tnecs_Pis {
    size_t num;
    size_t len;

    tnecs_Phs *byPh;   /* [Pi] */
} tnecs_Pis;

typedef struct tnecs_Es {
    /* 1. .num doesn't change even if Es get deleted
    ** 2. if reuse_Es: add deleted Es to Es_open
    **      - Call tnecs_E_open_find to add Es with
    **        id[ent] == false to Es_open. */
    size_t num;
    size_t len;

    tnecs_E     *id; /* [E] */
    size_t      *Os; /* [E] */
    tnecs_C     *As; /* [E] */
    tnecs_arr  open;
} tnecs_Es;

typedef struct tnecs_Ss {
    size_t num;
    size_t len;

    tnecs_Ph    *Phs;   /* [S_id] */
    size_t      *Os;    /* [S_id] */
    int         *Ex;    /* [S_id] */
    tnecs_C     *As;    /* [S_id] */
    tnecs_Pi    *Pi;    /* [S_id] */
#ifndef NDEBUG
    /* Systems maybe run in current pipeline */
    tnecs_arr to_run;
    /* Systems ran, if num_Es > 0 */
    tnecs_arr ran;
#endif /* NDEBUG */
} tnecs_Ss;

typedef struct tnecs_As {
    size_t num;
    size_t len;

    tnecs_C      *A;            /* [A_id]   */
    size_t       *num_Cs;       /* [A_id]   */
    size_t       *len_Es;       /* [A_id]   */
    size_t       *num_Es;       /* [A_id]   */
    size_t       *num_A_ids;    /* [A_id]   */

    size_t      **subA;     /* [A_id][subA_O]   */
    tnecs_E     **Es;       /* [A_id][E_O_byA]  */
    size_t      **Cs_O;     /* [A_id][C_id]     */
    tnecs_C     **Cs_id;    /* [A_id][C_O_byA]  */
    tnecs_C_arr **Cs;       /* [A_id][C_O_byA]  */
} tnecs_As;

typedef struct tnecs_Cs {
    size_t          num;
    size_t          bytesizes[TNECS_C_CAP]; /* [C_id] */
    tnecs_init_f    finit[TNECS_C_CAP];     /* [C_id] */
    tnecs_free_f    ffree[TNECS_C_CAP];     /* [C_id] */
} tnecs_Cs;

typedef struct tnecs_W {
    tnecs_Ss    Ss;
    tnecs_Es    Es;
    tnecs_As    byA;
    tnecs_Pis   Pis;
    tnecs_Cs    Cs;
    int reuse_Es;
} tnecs_W;

typedef struct tnecs_In {
    tnecs_W *world;
    tnecs_ns dt;
    tnecs_C  S_A;
    size_t   num_Es;
    size_t   E_A_id;
    void    *data;
} tnecs_In;

/* --- UTILITY MACROS --- */
#define TNECS_CONCAT( arg1, arg2) TNECS_CONCAT1(arg1, arg2)
#define TNECS_CONCAT1(arg1, arg2) TNECS_CONCAT2(arg1, arg2)
#define TNECS_CONCAT2(arg1, arg2) arg1##arg2

#define TNECS_A_HAS_T(A, T)         ((A & T) > 0)
#define TNECS_A_IS_subA(A1, A2)     ((A1 & A2) == A1)

/* -- Hacky distribution for variadic macros -- */
/* Distribution as in algebra: a(x + b) -> ax + ab */

/* TNECS_ARGN(__VA_ARGS__) counts the number of args,
**  _TNECS_SEQ pushes the args so correct N is output.
**  Example: 
**      1. TNECS_ARGN(x, y, z)
**      2. _TNECS_ARGN_(x, y, z, _TNECS_SEQ())
**      3. _TNECS_SEQCUT(x, y, z, 8, 7, 6, 5, 4, 3, 2, 1, 0)
**         i.e.         _1,_2,_3,_4,_5,_6,_7,_8, N, ...  
**      4. 3 is output
**  - up to 63 args, if _TNECS_SEQCUT and TNECS_VARG_SEQ exist */
#define  TNECS_ARGN(...) _TNECS_ARGN( __VA_ARGS__, _TNECS_SEQ())
#define _TNECS_ARGN(...) _TNECS_SEQCUT(__VA_ARGS__)
#define _TNECS_SEQCUT(_1, _2, _3, _4, _5, _6, _7, _8,  N, ...) N
#define _TNECS_SEQ()   8,  7,  6,  5,  4,  3,  2,  1,  0

/* TNECS_COMMA puts commas after each arg except last. 
**  - up to 63 args, if all TNECS_COMMA_N exist */
#define TNECS_COMMA(...) \
    TNECS_COMMA_(TNECS_ARGN(__VA_ARGS__), __VA_ARGS__)
#define TNECS_COMMA_(N, ...) \
    TNECS_CONCAT(TNECS_COMMA_, N)(__VA_ARGS__)

#define TNECS_COMMA_1(x)      x
#define TNECS_COMMA_2(x, ...) x, TNECS_COMMA_1(__VA_ARGS__)
#define TNECS_COMMA_3(x, ...) x, TNECS_COMMA_2(__VA_ARGS__)
#define TNECS_COMMA_4(x, ...) x, TNECS_COMMA_3(__VA_ARGS__)
#define TNECS_COMMA_5(x, ...) x, TNECS_COMMA_4(__VA_ARGS__)
#define TNECS_COMMA_6(x, ...) x, TNECS_COMMA_5(__VA_ARGS__)
#define TNECS_COMMA_7(x, ...) x, TNECS_COMMA_6(__VA_ARGS__)
#define TNECS_COMMA_8(x, ...) x, TNECS_COMMA_7(__VA_ARGS__)

/* --- WORLD --- */
int tnecs_genesis(tnecs_W **w);
int tnecs_finale(tnecs_W **w);

/* Toggle entity reuse i.e. deleted entity in queue */
void tnsecs_reuse_E( tnecs_W *w, int toggle);

/* Run all systems, by pipelines and phases */
int tnecs_step( tnecs_W *w,     tnecs_ns dt, 
                void    *data);

/* --- PIPELINES --- */
/* Run all systems in pipeline, by phases */
int tnecs_step_Pi(  tnecs_W *w,     tnecs_ns dt,
                    void    *data,  tnecs_Pi pi);

/* Run all systems in pipeline & phase combo */
int tnecs_step_Pi_Ph(   tnecs_W     *w, tnecs_ns dt,
                        void        *d, tnecs_Pi pi,
                        tnecs_Ph     ph);

/* --- PIPELINE --- */
#define TNECS_Pi_VALID(W, Pi) (Pi < W->Pis.num)
#define TNECS_Pi_GET(W, Pi) &W->Pis.byPh[(Pi)]

/* --- SYSTEM --- */
int tnecs_S_run(tnecs_W *w,     size_t   id,
                tnecs_ns dt,    void    *data);

int tnecs_custom_S_run( tnecs_W *w,     tnecs_S_f     s,    
                        tnecs_C  a,     tnecs_ns    dt,    
                        void    *data);
/* -- tnecs_in -- */
int tnecs_In_data(tnecs_In *in, void *data);

/* --- REGISTRATION --- */
/* Phases start at 1, increment every call. */
size_t tnecs_register_Ph(   tnecs_W *w, tnecs_Pi pi);
/* Pipelines start at 1, increment every call. */
size_t tnecs_register_Pi(   tnecs_W *w);

size_t tnecs_register_S(tnecs_W     *w,     tnecs_S_f    s,
                        tnecs_Pi     pi,    tnecs_Ph    ph,    
                        int         excl,   size_t      num, 
                        tnecs_C     arch);

#define TNECS_REGISTER_S(W, func, Pi, Ph, excl, ...) \
    tnecs_register_S(\
        W, func, Pi, Ph, excl, \
        TNECS_ARGN(__VA_ARGS__), \
        tnecs_C_ids2A(\
            TNECS_ARGN(__VA_ARGS__), \
            TNECS_COMMA(__VA_ARGS__)\
        )\
    )

/* Component ID start at 1, increment every call. */
tnecs_C tnecs_register_C(   tnecs_W         *w,
                            size_t           b,
                            tnecs_free_f    ffree,  
                            tnecs_init_f    finit);

#define TNECS_REGISTER_C(W, name, init, free) \
    tnecs_register_C(W, sizeof(name), init, free)

/* --- ENTITY --- */
tnecs_E tnecs_E_isOpen(     tnecs_W *w, tnecs_E ent);
tnecs_E tnecs_E_create(     tnecs_W *w);
tnecs_E tnecs_E_destroy(    tnecs_W *w, tnecs_E ent);
tnecs_E tnecs_E_create_wC(  tnecs_W *w, size_t argnum, ...);

tnecs_E tnecs_E_add_C(  tnecs_W *w, tnecs_E E,
                        tnecs_C  A, int     isNew);
tnecs_E tnecs_E_rm_C(   tnecs_W *w, tnecs_E E, 
                        tnecs_C  A);

int tnecs_E_open_find(  tnecs_W *w);
int tnecs_E_open_flush( tnecs_W *w);

#define TNECS_E_CREATE_wC(W, ...) \
    tnecs_E_create_wC(  W, TNECS_ARGN(__VA_ARGS__), \
                        TNECS_COMMA(__VA_ARGS__))
#define TNECS_E_EXISTS(w, i) \
    ((i != TNECS_NULL) && (w->Es.id[i] == i))
#define TNECS_E_A(w, e) w->Es.As[e]

/* --- COMPONENT --- */
void *tnecs_get_C(tnecs_W *w, tnecs_E E, tnecs_C C_id);

#define TNECS_E_HAS_C(w, e, C_id) \
        ((w->Es.As[e] & tnecs_C_ids2A(1, C_id)) > 0)
#define TNECS_ADD_C(...) \
    TNECS_CHOOSE_ADD_C(\
        __VA_ARGS__, TNECS_ADD_C4, TNECS_ADD_C3 \
    )(__VA_ARGS__)
#define TNECS_CHOOSE_ADD_C(_1, _2, _3, _4, NAME, ...) NAME
#define TNECS_ADD_C3(W, E_id, C_id) \
    tnecs_E_add_C(W, E_id, tnecs_C_ids2A(1, C_id), 1)
#define TNECS_ADD_C4(W, E_id, C_id, isnewT) \
    tnecs_E_add_C(W, E_id, tnecs_C_ids2A(1, C_id), isnewT)
#define TNECS_ADD_Cs(W, E_id, isnewT, ...) \
    tnecs_E_add_C(\
        W, E_id, \
        tnecs_C_ids2A(\
            TNECS_ARGN(__VA_ARGS__), \
            TNECS_COMMA(__VA_ARGS__)\
        ), \
        isnewT\
    )
#define TNECS_RM_C(W, E_id, ...) \
    tnecs_E_rm_C(\
        W, E_id, \
        tnecs_C_ids2A(\
            TNECS_ARGN(__VA_ARGS__), \
            TNECS_COMMA(__VA_ARGS__)\
        )\
    )

/* --- COMPONENT ARRAY --- */
void *tnecs_C_array(tnecs_W     *w, const size_t     C_id,
                    const size_t tID);

#define TNECS_C_ARRAY(in, C_id) \
    tnecs_C_array(in->world, C_id, in->E_A_id)

/* --- ARCHETYPES --- */
tnecs_C tnecs_C_ids2A(size_t argnum, ...);
tnecs_C tnecs_A_id(const tnecs_W *const w, tnecs_C arch);

#define TNECS_C_ID2T(id) ( \
        ((id >= TNECS_NULLSHIFT) && (id < TNECS_C_CAP)) ? \
        (1ULL << (id - TNECS_NULLSHIFT)) : 0ULL)
#define TNECS_C_T2ID(T) \
    (T >= 1 ? (tnecs_C)(log2(T) + 1.1f) : 0ULL) 
#define TNECS_C_IDS2A(...) \
    tnecs_C_ids2A(  TNECS_ARGN(__VA_ARGS__), \
                    TNECS_COMMA(__VA_ARGS__))
#define TNECS_C_IDS2AID(W, ...) \
    tnecs_A_id(W, TNECS_C_IDS2A(__VA_ARGS__))

/* --- SYSTEM --- */
#define TNECS_S_ID2A(W, id) W->Ss.As[id]

/* --- PHASE --- */
#define TNECS_Ph_VALID(W, Pi, Ph) (Ph < W->Pis.byPh[Pi].num)

#endif /* __TNECS_H__ */
