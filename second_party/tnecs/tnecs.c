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
** tnecs: Tiny C99 Entity-Component-System (ECS) library.
**      
*/

#include "tnecs.h"

/* --- Early return on fail --- */
#define TNECS_CHECK(check) do {\
        if (!check) { \
            printf("tnecs: failed " #check "\n"); \
            return(0); \
        } \
    } while (0)

/* --- PRIVATE DECLARATIONS --- */
typedef unsigned char tnecs_byte;

enum TNECS_PRIVATE {
    TNECS_E_0LEN    = 128,
    TNECS_Ph_0LEN   =   4,
    TNECS_Pi_0LEN   =   4,
    TNECS_C_0LEN    =   8,
    TNECS_S_0LEN    =  16,
    TNECS_A_0LEN    =  16,
    TNECS_ARR_GROW  =   2
};

/* --- WORLD FUNCTIONS --- */
static int _tnecs_breath_C(     tnecs_Cs    *Cs);
static int _tnecs_breath_As(    tnecs_As    *byA);
static int _tnecs_breath_Es(    tnecs_Es    *Es);
static int _tnecs_breath_Ss(    tnecs_Ss    *Ss);
static int _tnecs_breath_Phs(   tnecs_Phs   *byPh);
static int _tnecs_breath_Pis(   tnecs_Pis   *Pis);

static int _tnecs_finale_Ss(    tnecs_Ss    *Ss);
static int _tnecs_finale_Es(    tnecs_Es    *Es);
static int _tnecs_finale_As(    tnecs_As    *byA);
static int _tnecs_finale_Phs(   tnecs_Phs   *byPh);
static int _tnecs_finale_Pis(   tnecs_Pis   *Pis);

/* --- REGISTRATION  --- */
static size_t _tnecs_register_A( tnecs_W *w, size_t num_c,
                                 tnecs_C a);

/* --- SET BIT COUNTING --- */
static size_t setBits_KnR(tnecs_C flags);

/* --- "DYNAMIC" ARRAYS --- */
static void *tnecs_arrdel(  void *arr,  size_t elem,
                            size_t len, size_t bytesize);
static void *tnecs_realloc( void *ptr,  size_t olen,
                            size_t len, size_t bytesize);

#ifndef NDEBUG
static int tnecs_grow_ran(tnecs_W *w);
#endif /* NDEBUG */

static int tnecs_grow_E(        tnecs_W *w);
static int tnecs_grow_S(        tnecs_W *w);
static int tnecs_grow_A(        tnecs_W *w);
static int tnecs_grow_Pi(       tnecs_W *w);
static int tnecs_grow_Ph(       tnecs_W *w, tnecs_Pi    pi);
static int tnecs_grow_byA(      tnecs_W *w, size_t      A_id);
static int tnecs_grow_Es_open(  tnecs_W *w);
static int tnecs_grow_S_byPh(   tnecs_Phs   *byPh,
                                tnecs_Ph     Ph);
static int tnecs_grow_C_array(  tnecs_W     *w, 
                                tnecs_C_arr  *C_arr, 
                                size_t tID, size_t C_O);

/* --- UTILITIES --- */
static size_t tnecs_C_O_byA(    const tnecs_W *const w,
                                size_t C_id, tnecs_C arch);
static size_t tnecs_C_O_byAid(  const tnecs_W *const w, 
                                size_t C_id, size_t A_id);

/* --- COMPONENT ARRAY --- */
static int tnecs_C_arr_new( tnecs_W *w, size_t num,
                            tnecs_C a);
static int tnecs_C_arr_init(tnecs_W *w, tnecs_C_arr *arr,
                            size_t C_id);

/* --- BY ARCHETYPE --- */
static int tnecs_EsbyA_add( tnecs_W *w, tnecs_E e,
                            tnecs_C nt);
static int tnecs_EsbyA_del( tnecs_W *w, tnecs_E e,
                            tnecs_C ot);
static int tnecs_EsbyA_migrate( tnecs_W *w, tnecs_E e,
                                tnecs_C ot, tnecs_C nt);

/* --- COMPONENT --- */
static int tnecs_C_add( tnecs_W *w,     tnecs_C flag);
static int tnecs_C_del( tnecs_W *w,     tnecs_E ent,
                        tnecs_C of);
static int tnecs_C_init(tnecs_W *w,     tnecs_E ent,
                        tnecs_C of);
static int tnecs_C_free(tnecs_W *w,     tnecs_E ent,
                        tnecs_C of);
static int tnecs_C_run( tnecs_W *w,     tnecs_E    ent,
                        tnecs_C  of,    tnecs_free_f *f);
static int tnecs_C_copy(tnecs_W *w,     tnecs_E ent, 
                        tnecs_C  of,    tnecs_C nf);
static int tnecs_C_migrate( tnecs_W *w,     tnecs_E ent,
                            tnecs_C  of,    tnecs_C nf);

/************* W FUNCTIONS ***************/
int tnecs_genesis(tnecs_W **W) {
    if (*W != NULL) 
        TNECS_CHECK(tnecs_finale(W));

    *W = calloc(1, sizeof(tnecs_W));
    TNECS_CHECK(*W);

    /* Allocate W members */
    TNECS_CHECK(_tnecs_breath_Ss(   &((*W)->Ss)));
    TNECS_CHECK(_tnecs_breath_Es(   &((*W)->Es)));
    TNECS_CHECK(_tnecs_breath_Pis(  &((*W)->Pis)));
    TNECS_CHECK(_tnecs_breath_As(   &((*W)->byA)));
    TNECS_CHECK(_tnecs_breath_C(    &((*W)->Cs)));
    return (1);
}

int tnecs_finale(tnecs_W **W) {
    TNECS_CHECK(_tnecs_finale_Pis( &((*W)->Pis)));
    TNECS_CHECK(_tnecs_finale_Ss(  &((*W)->Ss)));
    TNECS_CHECK(_tnecs_finale_Es(  &((*W)->Es)));
    TNECS_CHECK(_tnecs_finale_As(  &((*W)->byA)));
    free(*W);

    *W = NULL;
    return (1);
}

int _tnecs_breath_C(tnecs_Cs *Cs) {
    Cs->num                   = TNECS_NULLSHIFT;
    Cs->bytesizes[TNECS_NULL] = TNECS_NULL;
    return (1);
}

int _tnecs_breath_Es(tnecs_Es *Es) {
    /* Variables */
    Es->num         = TNECS_NULLSHIFT;
    Es->len         = TNECS_E_0LEN;
    Es->open.len    = TNECS_E_0LEN;
    Es->open.num    = 0;

    /* Allocs */
    Es->id          = calloc(Es->len, sizeof(*Es->id));
    Es->Os          = calloc(Es->len, sizeof(*Es->Os));
    Es->open.arr    = calloc(Es->len, sizeof(tnecs_E));
    Es->As          = calloc(Es->len, sizeof(*Es->As));
    TNECS_CHECK(Es->id);
    TNECS_CHECK(Es->Os);
    TNECS_CHECK(Es->open.arr);
    TNECS_CHECK(Es->As);
    return (1);
}

int _tnecs_breath_Pis(tnecs_Pis *Pis) {
    Pis->len  = TNECS_Pi_0LEN;
    Pis->num  = TNECS_NULLSHIFT;

    Pis->byPh = calloc(Pis->len, sizeof(*Pis->byPh));
    TNECS_CHECK(Pis->byPh);
    _tnecs_breath_Phs(&Pis->byPh[TNECS_NULL]);
    return (1);
}

int _tnecs_breath_Phs(tnecs_Phs *byPh) {
    byPh->len = TNECS_Ph_0LEN;
    byPh->num = TNECS_NULLSHIFT;

    byPh->Ss        = calloc(byPh->len, sizeof(*byPh->Ss));
    byPh->Ss_id     = calloc(byPh->len, sizeof(*byPh->Ss_id));
    byPh->num_Ss    = calloc(byPh->len, sizeof(*byPh->num_Ss));
    byPh->len_Ss    = calloc(byPh->len, sizeof(*byPh->len_Ss));
    TNECS_CHECK(byPh->Ss);
    TNECS_CHECK(byPh->Ss_id);
    TNECS_CHECK(byPh->num_Ss);
    TNECS_CHECK(byPh->len_Ss);

    /* Alloc & check for Es_byPh elements */
    for (size_t i = 0; i < byPh->len; i++) {
        byPh->Ss[i]     = calloc(byPh->len, sizeof(**byPh->Ss));
        byPh->Ss_id[i]  = calloc(byPh->len, sizeof(**byPh->Ss_id));
        TNECS_CHECK(byPh->Ss[i]);
        TNECS_CHECK(byPh->Ss_id[i]);

        byPh->num_Ss[i] = 0;
        byPh->len_Ss[i] = byPh->len;
    }
    return (1);
}

int _tnecs_breath_Ss(tnecs_Ss *Ss) {
    /* Variables */
    Ss->len        = TNECS_S_0LEN;
    Ss->num        = TNECS_NULLSHIFT;
#ifndef NDEBUG
    Ss->to_run.len = TNECS_S_0LEN;
    Ss->ran.len    = TNECS_S_0LEN;
#endif /* NDEBUG */

    /* Allocs */
    Ss->Phs = calloc(Ss->len, sizeof(*Ss->Phs));
    Ss->Os  = calloc(Ss->len, sizeof(*Ss->Os));
    Ss->Pi  = calloc(Ss->len, sizeof(*Ss->Pi));
    Ss->Ex  = calloc(Ss->len, sizeof(*Ss->Ex));
    Ss->As  = calloc(Ss->len, sizeof(*Ss->As));
#ifndef NDEBUG
    Ss->ran.arr     = calloc(Ss->ran.len,    sizeof(tnecs_S_f));
    Ss->to_run.arr  = calloc(Ss->to_run.len, sizeof(tnecs_S_f));
#endif /* NDEBUG */
    TNECS_CHECK(Ss->Phs);
    TNECS_CHECK(Ss->Os);
    TNECS_CHECK(Ss->Pi);
    TNECS_CHECK(Ss->Ex);
    TNECS_CHECK(Ss->As);
#ifndef NDEBUG
    TNECS_CHECK(Ss->ran.arr);
    TNECS_CHECK(Ss->to_run.arr);
#endif /* NDEBUG */

    return (1);
}

int _tnecs_breath_As(tnecs_As *byA) {
    /* Variables */
    byA->num = TNECS_NULLSHIFT;
    byA->len = TNECS_A_0LEN;

    /* Allocs */
    byA->A          = calloc(byA->len, sizeof(*byA->A));
    byA->Es         = calloc(byA->len, sizeof(*byA->Es));
    byA->Cs         = calloc(byA->len, sizeof(*byA->Cs));
    byA->len_Es     = calloc(byA->len, sizeof(*byA->len_Es));
    byA->num_Es     = calloc(byA->len, sizeof(*byA->num_Es));
    byA->subA       = calloc(byA->len, sizeof(*byA->subA));
    byA->Cs_id      = calloc(byA->len, sizeof(*byA->Cs_id));
    byA->num_Cs     = calloc(byA->len, sizeof(*byA->num_Cs));
    byA->Cs_O       = calloc(byA->len, sizeof(*byA->Cs_O));
    byA->num_A_ids  = calloc(byA->len, sizeof(*byA->num_A_ids));
    TNECS_CHECK(byA->A);
    TNECS_CHECK(byA->Es);
    TNECS_CHECK(byA->Cs);
    TNECS_CHECK(byA->subA);
    TNECS_CHECK(byA->len_Es);
    TNECS_CHECK(byA->num_Es);
    TNECS_CHECK(byA->Cs_id);
    TNECS_CHECK(byA->num_Cs);
    TNECS_CHECK(byA->Cs_O);
    TNECS_CHECK(byA->num_A_ids);

    /* Alloc & check for id_byA elements */
    for (size_t i = 0; i < byA->len; i++) {
        byA->subA[i] = calloc(TNECS_C_CAP, sizeof(**byA->subA));
        byA->Es[i]   = calloc(TNECS_E_0LEN, sizeof(**byA->Es));
        TNECS_CHECK(byA->subA[i]);
        TNECS_CHECK(byA->Es[i]);

        byA->num_Es[i] = 0;
        byA->len_Es[i] = TNECS_E_0LEN;
    }
    return (1);
}

static int _tnecs_finale_Phs(tnecs_Phs *byPh) {
    for (size_t i = 0; i < byPh->len; i++) {
        if (byPh->Ss != NULL)
            free(byPh->Ss[i]);
        if (byPh->Ss_id != NULL)
            free(byPh->Ss_id[i]);
    }
    free(byPh->Ss);
    free(byPh->Ss_id);
    free(byPh->len_Ss);
    free(byPh->num_Ss);

    return(1);
}

static int _tnecs_finale_Ss(tnecs_Ss *Ss) {
    free(Ss->Os);
    free(Ss->Phs);
    free(Ss->Pi);
    free(Ss->Ex);
    free(Ss->As);
    #ifndef NDEBUG
    free(Ss->to_run.arr);
    free(Ss->ran.arr);
    #endif /* NDEBUG */

    return(1);
}

static int _tnecs_finale_Es(tnecs_Es *Es) {
    free(Es->id);
    free(Es->Os);
    free(Es->open.arr);
    free(Es->As);
    
    return(1);
}

static int _tnecs_finale_Pis(tnecs_Pis *Pis) {
    for (size_t i = 0; i < Pis->len; i++) {
        _tnecs_finale_Phs(&Pis->byPh[i]);
    }
    free(Pis->byPh);
    
    return(1);
}

static int _tnecs_finale_As(tnecs_As *byA) {
    for (size_t i = 0; i < byA->len; i++) {
        if (byA->Es != NULL)
            free(byA->Es[i]);
        if (byA->Cs_id != NULL)
            free(byA->Cs_id[i]);
        if (byA->Cs_O != NULL)
            free(byA->Cs_O[i]);
        if (byA->subA != NULL)
            free(byA->subA[i]);
        if (byA->Cs != NULL) {
            for (size_t j = 0; j < byA->num_Cs[i]; j++) {
                free(byA->Cs[i][j].Cs);
            }
            free(byA->Cs[i]);
        }
    }

    free(byA->A);
    free(byA->Es);
    free(byA->Cs);
    free(byA->len_Es);
    free(byA->num_Es);
    free(byA->subA);
    free(byA->Cs_id);
    free(byA->num_Cs);
    free(byA->Cs_O);
    free(byA->num_A_ids);

    return(1);
}

/********************* STEPPING ********************/
int tnecs_step(tnecs_W *W, tnecs_ns dt, void *data) {
    for (size_t p = 0; p < W->Pis.num; p++) {
        TNECS_CHECK(tnecs_step_Pi(W, dt, data, p));
    }
    return (1);
}

int tnecs_step_Pi(  tnecs_W     *w,     tnecs_ns     dt,
                    void        *data,  tnecs_Pi     pi) {
    #ifndef NDEBUG
    w->Ss.to_run.num   = 0;
    w->Ss.ran.num      = 0;
    #endif /* NDEBUG */

    tnecs_Phs *byPh = TNECS_Pi_GET(w, pi);
    for (size_t ph = 0; ph < byPh->num; ph++) {
        TNECS_CHECK(tnecs_step_Pi_Ph(w, dt, data, pi, ph));
    }
    return(1);
}

int tnecs_step_Pi_Ph(   tnecs_W  *w,    tnecs_ns  dt,
                        void     *data, tnecs_Pi  pi,
                        tnecs_Ph  ph) {
    tnecs_Phs *byPh = TNECS_Pi_GET(w, pi);
    size_t num = byPh->num_Ss[ph];
    for (size_t S_O = 0; S_O < num; S_O++) {
        size_t S_id = byPh->Ss_id[ph][S_O];
        TNECS_CHECK(tnecs_S_run(w, S_id, dt, data));
    }

    return(1);
}

/************* SYSTEM FUNCTIONS ***************/
int tnecs_custom_S_run( tnecs_W     *W, tnecs_S_f    S,
                        tnecs_C      A, tnecs_ns     dt, 
                        void        *data) {
    /* Building the Ss input */
    tnecs_In input = {.world = W, .dt = dt, .data = data};
    size_t A_id = tnecs_A_id(W, A);
    if (A_id == TNECS_NULL) {
        printf("tnecs: Input archetype is unknown.\n");
        return (0);
    }

    /* Running the exclusive custom system */
    input.E_A_id = A_id;
    input.num_Es = W->byA.num_Es[input.E_A_id];
    S(&input);

    /* Running the non-exclusive/inclusive custom system */
    size_t num_subA = W->byA.num_A_ids[A_id];
    for (size_t subA_O = 0; subA_O < num_subA; subA_O++) {
        input.E_A_id = W->byA.subA[A_id][subA_O];
        input.num_Es = W->byA.num_Es[input.E_A_id];
        S(&input);
    }
    return (1);
}

int tnecs_S_run(tnecs_W *W, size_t S_id,
                tnecs_ns dt, void *data) {
    /* Building the Ss input */
    tnecs_In input  = {.world = W, .dt = dt, .data = data};
    size_t S_O      = W->Ss.Os[S_id];
    tnecs_Ph Ph     = W->Ss.Phs[S_id];
    tnecs_Pi Pi     = W->Ss.Pi[S_id];
    size_t S_A_id   = tnecs_A_id(W, W->Ss.As[S_id]);

    input.E_A_id    = S_A_id;
    input.num_Es    = W->byA.num_Es[input.E_A_id];
    tnecs_Phs *byPh = TNECS_Pi_GET(W, Pi);

    tnecs_S_f S = byPh->Ss[Ph][S_O];
#ifndef NDEBUG
    while (W->Ss.to_run.num >= (W->Ss.to_run.len - 1)) {
        TNECS_CHECK(tnecs_grow_ran(W));
    }

    tnecs_S_f *S_arr;
    size_t S_num;
    S_num           = W->Ss.to_run.num++;
    S_arr           = W->Ss.to_run.arr;
    S_arr[S_num]    = S;
#endif /* NDEBUG */

    /* - Running the exclusive Ss in current phase - */
    if (input.num_Es > 0) {
        /* Skip running system if no Es! */
    #ifndef NDEBUG
        S_num           = W->Ss.ran.num++;
        S_arr           = W->Ss.ran.arr;
        S_arr[S_num]    = S;
    #endif /* NDEBUG */
        S(&input);
    }

    if (W->Ss.Ex[S_id])
        return (1);

    /* - Running the inclusive Ss in current phase - */
    size_t num_subA = W->byA.num_A_ids[S_A_id];
    for (size_t subA_O = 0; subA_O < num_subA; subA_O++) {
        input.E_A_id    = W->byA.subA[S_A_id][subA_O];
        input.num_Es    = W->byA.num_Es[input.E_A_id];
        tnecs_S_f S     = byPh->Ss[Ph][S_O];

    #ifndef NDEBUG
        S_num           = W->Ss.to_run.num++;
        S_arr           = W->Ss.to_run.arr;
        S_arr[S_num]    = S;

        while (W->Ss.to_run.num >= (W->Ss.to_run.len - 1)) {
            TNECS_CHECK(tnecs_grow_ran(W));
        }
    #endif /* NDEBUG */

        if (input.num_Es <= 0) {
            continue;
        }

        #ifndef NDEBUG
            S_num           = W->Ss.ran.num++;
            S_arr           = W->Ss.ran.arr;
            S_arr[S_num]    = S;
        #endif /* NDEBUG */

        S(&input);
    }
    return (1);
}

/*************** REGISTRATION ***************/
size_t tnecs_register_S(tnecs_W     *W,     tnecs_S_f  S,
                        tnecs_Pi     Pi,    tnecs_Ph   Ph,
                        int          isEx,  size_t     num_Cs,
                        tnecs_C      A) {
    /* Check if phase exist */
    if (!TNECS_Pi_VALID(W, Pi)) {
        printf("tnecs: System pipeline '%lld' is invalid.\n", Pi);
        return (TNECS_NULL);
    }
    if (!TNECS_Ph_VALID(W, Pi, Ph)) {
        printf("tnecs: System phase '%lld' is invalid (pipeline '%lld').\n", Ph, Pi);
        return (TNECS_NULL);
    }

    /* Compute new id */
    size_t S_id = W->Ss.num++;

    /* Realloc Ss if too many */
    if (W->Ss.num >= W->Ss.len) {
        TNECS_CHECK(tnecs_grow_S(W));
    }

    /* Realloc Ss_byPh if too many */
    tnecs_Phs *byPh = TNECS_Pi_GET(W, Pi);
    if (byPh->num_Ss[Ph] >= byPh->len_Ss[Ph]) {
        TNECS_CHECK(tnecs_grow_S_byPh(byPh, Ph));
    }

    /* -- Actual registration -- */
    /* Note: phase is exclusive to pipeline */
    W->Ss.Phs[S_id] = Ph;
    W->Ss.Pi[S_id]  = Pi;
    W->Ss.Ex[S_id]  = isEx;
    W->Ss.As[S_id]  = A;

    /* System order */
    size_t S_O              = byPh->num_Ss[Ph]++;
    W->Ss.Os[S_id]          = S_O;
    byPh->Ss[Ph][S_O]       = S;
    byPh->Ss_id[Ph][S_O]    = S_id;
    TNECS_CHECK(_tnecs_register_A(W, num_Cs, A));
    return (S_id);
}

tnecs_C tnecs_register_C(   tnecs_W     *W,
                            size_t       bytesize,
                            tnecs_free_f finit,
                            tnecs_free_f ffree) {
    /* Checks */
    if (bytesize <= 0) {
        printf("tnecs: Component should have >0 bytesize.\n");
        return (TNECS_NULL);
    }
    if (W->Cs.num >= TNECS_C_CAP) {
        printf("tnecs: Component capacity reached.\n");
        return (TNECS_NULL);
    }

    /* Registering */
    tnecs_C new_C_id    = W->Cs.num++;
    tnecs_C new_C_flag  = TNECS_C_ID2T(new_C_id);
    W->Cs.bytesizes[new_C_id]   = bytesize;
    W->Cs.ffree[new_C_id]       = ffree;
    W->Cs.finit[new_C_id]       = finit;
    TNECS_CHECK(_tnecs_register_A(W, 1, new_C_flag));
    return (new_C_id);
}

size_t _tnecs_register_A(   tnecs_W    *W, size_t      num_Cs,
                            tnecs_C     A_new) {
    /* 0- Check if archetype exists, return */
    for (size_t i = 0 ; i < W->byA.num; i++) {
        if (A_new == W->byA.A[i]) {
            return (i);
        }
    }

    /* 1- Add new byA.Cs at [tID] */
    if ((W->byA.num + 1) >= W->byA.len)
        TNECS_CHECK(tnecs_grow_A(W));
    W->byA.A[W->byA.num++] = A_new;
    size_t tID = tnecs_A_id(W, A_new);
    assert(tID == (W->byA.num - 1));
    W->byA.num_Cs[tID] = num_Cs;

    /* 2- Add arrays to byA.Cs[tID] for each component */
    TNECS_CHECK(tnecs_C_arr_new(W, num_Cs, A_new));

    /* 3- Add all Cs to byA.Cs_id */
    tnecs_C A_reduced   = A_new, A_added = 0;
    size_t bytesize1    = sizeof(**W->byA.Cs_id);
    size_t bytesize2    = sizeof(**W->byA.Cs_O);
    W->byA.Cs_id[tID]   = calloc(num_Cs,      bytesize1);
    W->byA.Cs_O[tID]    = calloc(TNECS_C_CAP, bytesize2);
    TNECS_CHECK(W->byA.Cs_id[tID]);
    TNECS_CHECK(W->byA.Cs_O[tID]);

    size_t C_O_byA = 0;
    while (A_reduced) {
        A_reduced &= (A_reduced - 1);

        tnecs_C C_T_toadd = (A_reduced + A_added) ^ A_new;
        A_added += C_T_toadd;
        assert(C_T_toadd > 0);
        tnecs_C C_id_toadd = TNECS_C_T2ID(C_T_toadd);

        W->byA.Cs_id[tID][C_O_byA]      = C_id_toadd;
        W->byA.Cs_O[tID][C_id_toadd]    = C_O_byA++;
    }

    /* 4- Find every subtype of every archertype. 
    **  a. Check if ALL archetypes are subtypes
    **  b. If subtype: to list of subtypes byA.subA */
    for (size_t i = 1; i < W->byA.num; i++) {
        W->byA.num_A_ids[i] = 0;
        for (size_t j = 1; j < W->byA.num; j++) {
            if (i == j)
                continue;

            if (!TNECS_A_IS_subA(W->byA.A[i], W->byA.A[j]))
                continue;

            /* j is an archetype of i */
            W->byA.subA[i][W->byA.num_A_ids[i]++] = j;
        }
    }

    return (tID);
}

size_t tnecs_register_Pi(tnecs_W *W) {
    tnecs_Pi Pi = W->Pis.num++;
    while (Pi >= W->Pis.len) {
        TNECS_CHECK(tnecs_grow_Pi(W));
    }
    tnecs_Phs *byPh = TNECS_Pi_GET(W, Pi);
    _tnecs_breath_Phs(byPh);

    return (Pi);
}

size_t tnecs_register_Ph(tnecs_W *W, tnecs_Pi Pi) {
    if (!TNECS_Pi_VALID(W, Pi)) {
        printf("tnecs: Pi '%lld' is invalid for new phase.\n", Pi);
        return (TNECS_NULL);
    }

    tnecs_Phs *byPh   = TNECS_Pi_GET(W, Pi);
    tnecs_Ph Ph       = byPh->num++;
    while (Ph >= byPh->len) {
        TNECS_CHECK(tnecs_grow_Ph(W, Pi));
    }
    return (Ph);
}

/**************** ENTITY MANIPULATION **************/
tnecs_E tnecs_E_create(tnecs_W *W) {
    tnecs_E out = TNECS_NULL;

    /* Check if an open entity exists */
    tnecs_E *arr = W->Es.open.arr;
    while ((out == TNECS_NULL) &&
           (W->Es.open.num > 0) && 
           (W->Es.open.num < TNECS_E_CAP)
          ) {
        out = arr[--W->Es.open.num];
        arr[W->Es.open.num] = TNECS_NULL;
    }

    /* If no open entity existed, create one */
    if (out == TNECS_NULL) {
        do {
            if (W->Es.num >= W->Es.len) {
                if (!tnecs_grow_E(W)) {
                    printf("tnecs: Could not allocate more memory for Es.\n");
                    return (TNECS_NULL);
                }
            }
            out = W->Es.num++;
        } while (TNECS_E_EXISTS(W, out));
    }
    assert(out != TNECS_NULL);

    /* Set entity and checks  */
    W->Es.id[out] = out;
    TNECS_CHECK(tnecs_EsbyA_add(W, out, TNECS_NULL));
    assert(W->Es.id[out]                                          == out);
    assert(W->byA.Es[TNECS_NULL][W->Es.Os[out]]  == out);
    return (out);
}

tnecs_E tnecs_E_create_wC(tnecs_W *W, size_t argnum, ...) {
    /* Get archetype of all vararg Cs ids */
    va_list ap;
    va_start(ap, argnum);
    tnecs_C A = 0;
    for (size_t i = 0; i < argnum; i++) {
        tnecs_C C_id = va_arg(ap, tnecs_C);
        A += TNECS_C_ID2T(C_id);
    }
    va_end(ap);

    /* Create entity with all Cs */
    tnecs_E new_E = tnecs_E_create(W);
    if (new_E == TNECS_NULL) {
        printf("tnecs: could not create new entity\n");
        return (TNECS_NULL);
    }
    TNECS_CHECK(tnecs_E_add_C(W, new_E, A, 1));

#ifndef NDEBUG
    size_t tID      = tnecs_A_id(W, A);
    size_t order    = W->Es.Os[new_E];
    assert(W->byA.Es[tID][order]    == new_E);
    assert(W->Es.id[new_E]          == new_E);
#endif /* NDEBUG */

    return (new_E);
}

int tnecs_E_open_find(tnecs_W *W) {
    /* Adds all null Es to open list */
    for (tnecs_E i = TNECS_NULLSHIFT; i < W->Es.num; i++) {
        if (TNECS_E_EXISTS(W, i))
            continue; /* Skip if entity exists */

        if (tnecs_E_isOpen(W, i))
            continue; /* Skip if already in open list */

        TNECS_CHECK(tnecs_grow_Es_open(W));
        tnecs_E *arr = W->Es.open.arr;
        arr[W->Es.open.num++] = i;
    }
    return (1);
};

int tnecs_E_open_flush(tnecs_W *W) {
    /* Get rid of all Es in Es_open */
    W->Es.open.num = 0;
    return (1);
}

tnecs_E tnecs_E_isOpen(tnecs_W *W, tnecs_E E) {
    if (E <= TNECS_NULL) {
        return (0);
    }

    const tnecs_E * const open_arr = W->Es.open.arr;

    for (tnecs_E i = TNECS_NULLSHIFT; i < W->Es.open.num; i++) {
        if (open_arr[i] == E) {
            return (1);
        }
    }
    return (0);
}

tnecs_E tnecs_E_destroy(tnecs_W *W, tnecs_E E) {
    if (E <= TNECS_NULL) {
        return (1);
    }

    if (!TNECS_E_EXISTS(W, E)) {
        W->Es.id[E] = TNECS_NULL;
        W->Es.Os[E] = TNECS_NULL;
        W->Es.As[E] = TNECS_NULL;
        return (1);
    }

    /* Preliminaries */
    tnecs_C A = W->Es.As[E];

    /* Delete Cs */
    TNECS_CHECK(tnecs_C_free(W, E, A));
    TNECS_CHECK(tnecs_C_del(W, E, A));

#ifndef NDEBUG
    size_t E_O = W->Es.Os[E];
    size_t tID = tnecs_A_id(W, A);
    assert(W->byA.num_Es[tID] > TNECS_NULL);
    assert(W->byA.len_Es[tID] >= E_O);
    assert(W->byA.num_Es[tID] > TNECS_NULL);
#endif /* NDEBUG */

    /* Delete EsbyA */
    TNECS_CHECK(tnecs_EsbyA_del(W, E, A));

    /* Delete entity */
    W->Es.id[E] = TNECS_NULL;

    /* Note: reuse_Es used to add to Es_open, so that
    ** user can call tnecs_E_open_find to reuse Es manually. */
    if (W->reuse_Es) {
        /* Add deleted entity to open Es */
        TNECS_CHECK(tnecs_grow_Es_open(W));
        tnecs_E *arr = W->Es.open.arr;
        arr[W->Es.open.num++] = E;
    }
    assert(!TNECS_E_EXISTS(W, E));
    assert(W->Es.Os[E]      == TNECS_NULL);
    assert(W->Es.As[E]      == TNECS_NULL);
    assert(W->Es.Os[E_O]    != E);
    return (1);
}

void tnecs_W_reuse_E(tnecs_W *W, int toggle) {
    W->reuse_Es = toggle;
}

/*****************************************************/
/***************** TNECS INTERNALS *******************/
/*****************************************************/
tnecs_E tnecs_E_add_C(  tnecs_W *W,         tnecs_E  E,
                        tnecs_C  A_toadd,   int      isNew) {
    if (A_toadd <= 0) {
        return (TNECS_NULL);
    }

    if (!TNECS_E_EXISTS(W, E)) {
        return (TNECS_NULL);
    }

    tnecs_C A_old = W->Es.As[E];

    if (TNECS_A_HAS_T(A_toadd, A_old)) {
        return (E);
    }

    tnecs_C A_new = A_toadd + A_old;
    assert(A_new != A_old);
    if (isNew)
        TNECS_CHECK(_tnecs_register_A(  W, setBits_KnR(A_new), 
                                        A_new));

    TNECS_CHECK(tnecs_C_migrate(    W, E, A_old, A_new));
    TNECS_CHECK(tnecs_EsbyA_migrate(W, E, A_old, A_new));
    TNECS_CHECK(tnecs_C_init(       W, E, A_toadd));

#ifndef NDEBUG
    size_t tID_new = tnecs_A_id(W, A_new);
    size_t new_O = W->byA.num_Es[tID_new] - 1;
    assert(W->Es.As[E]                  == A_new);
    assert(W->byA.Es[tID_new][new_O]    == E);
    assert(W->Es.Os[E]                  == new_O);
#endif /* NDEBUG */
    return (W->Es.id[E]);
}

tnecs_E tnecs_E_rm_C(   tnecs_W *W, tnecs_E  E,
                        tnecs_C  A) {
    /* Get new archetype. 
    ** Since it is a archetype, just need to substract. */
    tnecs_C A_old = W->Es.As[E];
    tnecs_C A_new = A_old - A;

    /* Free removed Cs. */
    TNECS_CHECK(tnecs_C_free(W, E, A));
    if (A_new != TNECS_NULL) {
        /* Migrate remaining Cs to new A array. */
        TNECS_CHECK(_tnecs_register_A(  W, setBits_KnR(A_new),
                                        A_new));
        TNECS_CHECK(tnecs_C_migrate(W, E, A_old, A_new));
    } else {
        /* No remaining component, delete everything. */
        TNECS_CHECK(tnecs_C_del(W, E, A_old));
    }
    /* Migrate E to new byA array. */
    TNECS_CHECK(tnecs_EsbyA_migrate(W, E, A_old, A_new));
    assert(A_new == W->Es.As[E]);
    return (1);
}

void *tnecs_get_C(  tnecs_W *W, tnecs_E  E,
                    tnecs_C  C_id) {
    if (!TNECS_E_EXISTS(W, E))
        return (NULL);

    tnecs_C C_flag  = TNECS_C_ID2T(C_id);
    tnecs_C E_A     = TNECS_E_A(W, E);
    /* If entity has component, get output it.  */
    /* If not output NULL. */
    if (!TNECS_A_HAS_T(C_flag, E_A))
        return (NULL);

    size_t tID = tnecs_A_id(W, E_A);
    assert(tID > 0);
    size_t C_O = tnecs_C_O_byA(W, C_id, E_A);
    assert(C_O <= W->byA.num_Cs[tID]);
    size_t E_O = W->Es.Os[E];
    size_t bytesize = W->Cs.bytesizes[C_id];

    tnecs_C_arr *C_array = &W->byA.Cs[tID][C_O];
    assert(C_array != NULL);
    tnecs_byte *temp_C_bytesptr = (tnecs_byte *)(C_array->Cs);
    void *out = temp_C_bytesptr + (bytesize * E_O);

    return (out);
}

int tnecs_EsbyA_add(tnecs_W *W, tnecs_E E, tnecs_C A_new) {
    size_t tID_new = tnecs_A_id(W, A_new);
    if ((W->byA.num_Es[tID_new] + 1) >= W->byA.len_Es[tID_new]) {
        TNECS_CHECK(tnecs_grow_byA(W, tID_new));
    }
    size_t new_O = W->byA.num_Es[tID_new]++;
    W->Es.Os[E] = new_O;
    W->Es.As[E] = A_new;
    W->byA.Es[tID_new][new_O]  = E;
    return (1);
}

int tnecs_EsbyA_del(tnecs_W *W, tnecs_E E, tnecs_C A_old) {
    if (!TNECS_E_EXISTS(W, E))
        return (1);

    if (E >= W->Es.len)
        return (1);

    size_t A_old_id = tnecs_A_id(W, A_old);
    size_t old_num  = W->byA.num_Es[A_old_id];
    if (old_num <= 0)
        return (1);

    size_t E_O_old = W->Es.Os[E];
    assert(A_old == W->Es.As[E]);

    assert(E_O_old < W->byA.len_Es[A_old_id]);
    assert(W->byA.Es[A_old_id][E_O_old] == E);

    tnecs_E top_E = W->byA.Es[A_old_id][old_num - 1];

    /* Cs scrambles -> EsbyA too */
    TNECS_CHECK(tnecs_arrdel(W->byA.Es[A_old_id], E_O_old, 
                             old_num, sizeof(**W->byA.Es)));

    if (top_E != E) {
        W->Es.Os[top_E] = E_O_old;
        assert(W->byA.Es[A_old_id][E_O_old] == top_E);
    }

    W->Es.Os[E] = TNECS_NULL;
    W->Es.As[E] = TNECS_NULL;

    --W->byA.num_Es[A_old_id];
    return (1);
}

int tnecs_EsbyA_migrate(tnecs_W    *W, tnecs_E E,
                        tnecs_C A_old, tnecs_C A_new) {
    /* Migrate Es into correct byA array */
    TNECS_CHECK(tnecs_EsbyA_del(W, E, A_old));
    assert(W->Es.As[E]  == TNECS_NULL);
    assert(W->Es.Os[E]  == TNECS_NULL);
    TNECS_CHECK(tnecs_EsbyA_add(W, E, A_new));

#ifndef NDEBUG
    size_t tID_new  = tnecs_A_id(W, A_new);
    size_t O_new    = W->Es.Os[E];
    assert(W->Es.As[E]                  == A_new);
    assert(W->byA.num_Es[tID_new] - 1   == O_new);
    assert(W->byA.Es[tID_new][O_new]    == E);
#endif /* NDEBUG */
    return (1);
}

int tnecs_C_add(tnecs_W *W, tnecs_C A) {
    /* Check if need to grow component array after adding new component */
    size_t tID          = tnecs_A_id(W, A);
    size_t new_C_num = W->byA.num_Cs[tID];
#ifndef NDEBUG
    size_t E_O = W->byA.num_Es[tID];
#endif /* NDEBUG */

    for (size_t C_O = 0; C_O < new_C_num; C_O++) {
        /* Take component array of current A_id */
        tnecs_C_arr *C_arr = &W->byA.Cs[tID][C_O];
        /* check if it need to grow after adding new component */
        assert(E_O == C_arr->num);

        if (++C_arr->num >= C_arr->len)
            TNECS_CHECK(tnecs_grow_C_array(W, C_arr, tID, C_O));
    }

    return (1);
}

int tnecs_C_copy(tnecs_W *W,    tnecs_E E,
                tnecs_C old_A,  tnecs_C new_A) {
    /* Copy Cs from old O to top of new_A C_arr */
    if (old_A == new_A)
        return (1);

    size_t old_tID      = tnecs_A_id(W, old_A);
    size_t new_tID      = tnecs_A_id(W, new_A);
    size_t old_E_O      = W->Es.Os[E];
    size_t new_E_O      = W->byA.num_Es[new_tID];
    size_t num_C_new    = W->byA.num_Cs[new_tID];
    size_t num_C_old    = W->byA.num_Cs[old_tID];

#ifndef NDEBUG
    /* Sanity check: entity order is the same in new Cs array */
    for (int i = 0; i < num_C_new; ++i) {
        size_t num = W->byA.Cs[new_tID][i].num;
        assert((num - 1) == new_E_O);
    }
#endif /* NDEBUG */

    size_t       old_C_id,   new_C_id,  C_bytesize;
    tnecs_C_arr *old_arr,   *new_array;
    tnecs_byte  *old_C,     *new_C;
    tnecs_byte  *old_C_arr, *new_C_arr;

    for (size_t old_C_O = 0; old_C_O < num_C_old; old_C_O++) {
        old_C_id = W->byA.Cs_id[old_tID][old_C_O];
        for (size_t new_C_O = 0; new_C_O < num_C_new; new_C_O++) {
            new_C_id = W->byA.Cs_id[new_tID][new_C_O];
            if (old_C_id != new_C_id)
                continue;

            new_array = &W->byA.Cs[new_tID][new_C_O];
            old_arr = &W->byA.Cs[old_tID][old_C_O];
            assert(old_arr->type == new_array->type);
            assert(old_arr != new_array);

            C_bytesize = W->Cs.bytesizes[old_C_id];
            assert(C_bytesize > 0);

            old_C_arr = (tnecs_byte *)(old_arr->Cs);
            assert(old_C != NULL);

            old_C = (old_C_arr + (C_bytesize * old_E_O));
            assert(old_C != NULL);

            new_C_arr = (tnecs_byte *)(new_array->Cs);
            assert(new_C_arr != NULL);

            new_C = (new_C_arr + (C_bytesize * new_E_O));
            assert(new_C != NULL);
            assert(new_C != old_C);

#ifndef NDEBUG
            const void *const out = memmove(new_C, old_C, C_bytesize);
            assert(out == new_C);
#else
            memmove(new_C, old_C, C_bytesize);
#endif /* NDEBUG */
            break;
        }
    }
    return (1);
}

int tnecs_C_run(tnecs_W *W, tnecs_E          E,
                tnecs_C  A, tnecs_init_f    *funcs) {
    size_t tID      = tnecs_A_id(W, A);
    size_t C_num    = W->byA.num_Cs[tID];
    for (size_t C_O = 0; C_O < C_num; C_O++) {
        size_t C_id = W->byA.Cs_id[tID][C_O];
        tnecs_init_f func = funcs[C_id]; 
        if (func == NULL) {
            continue;
        }
        void *comp = tnecs_get_C(W, E, C_id);
        assert(comp != NULL);
        func(comp);
    }
    return(1);
}

int tnecs_C_init(tnecs_W *W, tnecs_E E, tnecs_C A) {
    /* Init ALL entity's Cs in archetype */
    return(tnecs_C_run(W, E, A, W->Cs.finit));
}

int tnecs_C_free(tnecs_W *W, tnecs_E E, tnecs_C A) {
    /* Free ALL entity's Cs in archetype */
    return(tnecs_C_run(W, E, A, W->Cs.ffree));
}

int tnecs_C_del(tnecs_W *W, tnecs_E E, tnecs_C old_A) {
    /* Delete ALL Cs from CsbyA at old entity order */
    size_t old_tID      = tnecs_A_id(W, old_A);
    size_t O_old        = W->Es.Os[E];
    size_t old_C_num    = W->byA.num_Cs[old_tID];
    for (size_t C_O = 0; C_O < old_C_num; C_O++) {
        size_t      C_id        = W->byA.Cs_id[old_tID][C_O];
        tnecs_C_arr *old_arr    = &W->byA.Cs[old_tID][C_O];
        tnecs_byte  *C          = old_arr->Cs;
        assert(C != NULL);

        /* Scramble Cs too */
        size_t C_by       = W->Cs.bytesizes[C_id];
        size_t new_C_num  = W->byA.num_Es[old_tID];
        const tnecs_byte *scramble;
        scramble = tnecs_arrdel(C, O_old, new_C_num, C_by);
        TNECS_CHECK(scramble);

        old_arr->num--;
    }
    return (1);
}

int tnecs_C_migrate(tnecs_W *W,     tnecs_E E,
                    tnecs_C  old_A, tnecs_C new_A) {
    if (old_A != W->Es.As[E]) {
        return (0);
    }
    TNECS_CHECK(tnecs_C_add(W,  new_A));
    if (old_A > TNECS_NULL) {
        TNECS_CHECK(tnecs_C_copy(W, E, old_A, new_A));
        TNECS_CHECK(tnecs_C_del( W, E, old_A));
    }
    return (1);
}

int tnecs_C_arr_new(tnecs_W *W, size_t num_Cs, tnecs_C A) {
    tnecs_C_arr *C_arr = calloc(num_Cs, sizeof(tnecs_C_arr));
    TNECS_CHECK(C_arr);

    tnecs_C A_reduced = A, A_added = 0;
    tnecs_C tID     = tnecs_A_id(W, A);
    size_t id_toadd = 0, num_flags = 0;

    while (A_reduced) {
        A_reduced &= (A_reduced - 1);
        tnecs_C type_toadd = (A_reduced + A_added) ^ A;
        assert(type_toadd > 0);
        id_toadd = TNECS_C_T2ID(type_toadd);
        assert(id_toadd > 0);
        assert(id_toadd < W->Cs.num);
        TNECS_CHECK(tnecs_C_arr_init(W, &C_arr[num_flags], id_toadd));
        num_flags++;
        A_added += type_toadd;
    }
    W->byA.Cs[tID] = C_arr;
    assert(id_toadd < W->Cs.num);
    return ((A_added == A) && (num_flags == num_Cs));
}

int tnecs_C_arr_init(   tnecs_W *W, tnecs_C_arr  *C_arr,
                        size_t   C_id) {
    assert(C_id > 0);
    assert(C_id < W->Cs.num);
    tnecs_C in_type = TNECS_C_ID2T(C_id);
    assert(in_type <= TNECS_C_ID2T(W->Cs.num));

    size_t bytesize = W->Cs.bytesizes[C_id];
    assert(bytesize > 0);

    C_arr->type  = in_type;
    C_arr->num   = 0;
    C_arr->len   = TNECS_C_0LEN;
    C_arr->Cs    = calloc(TNECS_C_0LEN, bytesize);
    TNECS_CHECK(C_arr->Cs);
    return (1);
}

/*********** UTILITY FUNCTIONS/MACROS **************/
size_t tnecs_C_O_byA(   const tnecs_W *const W,
                        size_t C_id, tnecs_C flag) {
    tnecs_C tID = tnecs_A_id(W, flag);
    return (tnecs_C_O_byAid(W, C_id, tID));
}

size_t tnecs_C_O_byAid( const tnecs_W *const W,
                        size_t C_id, size_t tID) {
    for (size_t i = 0; i < W->byA.num_Cs[tID]; i++) {
        if (W->byA.Cs_id[tID][i] == C_id) {
            return(i);
        }
    }
    return (TNECS_C_CAP);
}

tnecs_C tnecs_C_ids2A(size_t argnum, ...) {
    tnecs_C out = 0;
    va_list ap;
    va_start(ap, argnum);
    for (size_t i = 0; i < argnum; i++) {
        size_t id = va_arg(ap, size_t);
        out += TNECS_C_ID2T(id);
    }
    va_end(ap);
    return (out);
}

tnecs_C tnecs_A_id(const tnecs_W *const W, tnecs_C A) {
    for (size_t i = 0; i < W->byA.num; i++) {
        if (A == W->byA.A[i]) {
            return(i);
        }
    }
    return (0);
}

/***************** "DYNAMIC" ARRAYS ******************/
void *tnecs_realloc(void   *ptr,  size_t olen,
                    size_t  nlen, size_t elem_bytesize) {
    if (!ptr)
        return (NULL);
    void *realloced = calloc(nlen, elem_bytesize);
    TNECS_CHECK(realloced);
    if (realloced == NULL) {
        printf("tnecs: failed allocation realloced\n");
        return(NULL);
    }
    size_t elen = (nlen > olen ? olen : nlen);
    memcpy(realloced, ptr, elen * elem_bytesize);
    free(ptr);
    return (realloced);
}

void *tnecs_arrdel(void *arr,  size_t elem, 
                   size_t len, size_t bytesize) {
    /* Scrambles by copying from [len - 1] to [elem] */
    tnecs_byte *ptr = arr;
    if (elem != (len - 1))
        memmove(ptr + (elem * bytesize),
                ptr + ((len - 1) * bytesize), bytesize);

    memset(ptr + ((len - 1) * bytesize), TNECS_NULL, bytesize);
    return (arr);
}

#ifndef NDEBUG
int tnecs_grow_ran(tnecs_W *W) {
    /* Realloc Ss ran if too many */
    size_t olen         = W->Ss.ran.len;
    size_t nlen         = olen * TNECS_ARR_GROW;
    W->Ss.ran.len       = nlen;
    W->Ss.to_run.len    = nlen;
    size_t bytesize     = sizeof(tnecs_S_f);

    W->Ss.ran.arr       = tnecs_realloc(W->Ss.ran.arr, 
                                        olen, nlen, bytesize);
    W->Ss.to_run.arr    = tnecs_realloc(W->Ss.to_run.arr, 
                                        olen, nlen, bytesize);
    TNECS_CHECK(W->Ss.ran.arr);
    TNECS_CHECK(W->Ss.to_run.arr);
    return (1);
}
#endif /* NDEBUG */

int tnecs_grow_Es_open(tnecs_W *W) {
    /* Realloc Es_open if too many */
    if ((W->Es.open.num + 1) >= W->Es.open.len) {
        size_t olen     = W->Es.open.len;
        size_t nlen     = olen * TNECS_ARR_GROW;
        size_t bytesize = sizeof(tnecs_E);
        W->Es.open.len  = nlen;

        W->Es.open.arr  = tnecs_realloc(W->Es.open.arr, 
                                        olen, nlen, bytesize);
        TNECS_CHECK(W->Es.open.arr);
    }
    return (1);
}

int tnecs_grow_C_array( tnecs_W *W, tnecs_C_arr *C_arr,
                        size_t tID, size_t      C_O) {
    size_t olen = C_arr->len;
    size_t nlen = olen * TNECS_ARR_GROW;
    C_arr->len  = nlen;
    size_t C_id  = W->byA.Cs_id[tID][C_O];

    size_t bytesize = W->Cs.bytesizes[C_id];
    C_arr->Cs = tnecs_realloc(C_arr->Cs, olen, nlen, bytesize);
    TNECS_CHECK(C_arr->Cs);
    return (1);
}

int tnecs_grow_E(tnecs_W *W) {
    size_t olen = W->Es.len;
    size_t nlen = W->Es.len * TNECS_ARR_GROW;
    W->Es.len = nlen;
    if (nlen >= TNECS_E_CAP) {
        printf("tnecs: Es cap reached\n");
        return (TNECS_NULL);
    }

    W->Es.id    = tnecs_realloc(W->Es.id, olen, nlen,
                                sizeof(*W->Es.id));
    TNECS_CHECK(W->Es.id);
    W->Es.Os    = tnecs_realloc(W->Es.Os, olen, nlen,
                                sizeof(*W->Es.Os));
    TNECS_CHECK(W->Es.Os);
    W->Es.As    = tnecs_realloc(W->Es.As, olen, nlen,
                                sizeof(*W->Es.As));
    TNECS_CHECK(W->Es.As);

    return (1);
}

int tnecs_grow_S(tnecs_W *W) {
    size_t olen = W->Ss.len;
    size_t nlen = olen * TNECS_ARR_GROW;
    assert(olen > 0);
    W->Ss.len   = nlen;

    W->Ss.Phs   = tnecs_realloc(W->Ss.Phs, olen, nlen,
                                sizeof(*W->Ss.Phs));
    TNECS_CHECK(W->Ss.Phs);
    W->Ss.Os    = tnecs_realloc(W->Ss.Os, olen, nlen,
                                sizeof(*W->Ss.Os));
    TNECS_CHECK(W->Ss.Os);
    W->Ss.Ex    = tnecs_realloc(W->Ss.Ex, olen, nlen,
                                sizeof(*W->Ss.Ex));
    TNECS_CHECK(W->Ss.Pi);
    W->Ss.Pi    = tnecs_realloc(W->Ss.Pi, olen, nlen,
                                sizeof(*W->Ss.Pi));
    TNECS_CHECK(W->Ss.Ex);
    W->Ss.As    = tnecs_realloc(W->Ss.As, olen, nlen,
                                sizeof(*W->Ss.As));
    TNECS_CHECK(W->Ss.As);

    return (1);
}

int tnecs_grow_A(tnecs_W *W) {
    size_t olen = W->byA.len;
    size_t nlen = olen * TNECS_ARR_GROW;
    W->byA.len  = nlen;

    W->byA.A       = tnecs_realloc(W->byA.A, olen, nlen, 
                                    sizeof(*W->byA.A));
    W->byA.Es       = tnecs_realloc(W->byA.Es, olen, nlen,
                                    sizeof(*W->byA.Es));
    W->byA.num_Es   = tnecs_realloc(W->byA.num_Es, olen, nlen,
                                    sizeof(*W->byA.num_Es));
    W->byA.len_Es   = tnecs_realloc(W->byA.len_Es, olen, nlen,
                                    sizeof(*W->byA.len_Es));
    W->byA.subA     = tnecs_realloc(W->byA.subA, olen, nlen,
                                    sizeof(*W->byA.subA));
    W->byA.Cs_id    = tnecs_realloc(W->byA.Cs_id, olen, nlen,
                                    sizeof(*W->byA.Cs_id));
    W->byA.num_Cs   = tnecs_realloc(W->byA.num_Cs, olen, nlen,
                                    sizeof(*W->byA.num_Cs));
    W->byA.Cs_O     = tnecs_realloc(W->byA.Cs_O, olen, nlen,
                                    sizeof(*W->byA.Cs_O));
    W->byA.num_A_ids = tnecs_realloc(W->byA.num_A_ids, olen, nlen,
                                    sizeof(*W->byA.num_A_ids));
    W->byA.Cs        = tnecs_realloc(W->byA.Cs, olen, nlen,
                                    sizeof(*W->byA.Cs));

    TNECS_CHECK(W->byA.A);
    TNECS_CHECK(W->byA.Es);
    TNECS_CHECK(W->byA.num_Es);
    TNECS_CHECK(W->byA.subA);
    TNECS_CHECK(W->byA.len_Es);
    TNECS_CHECK(W->byA.Cs_id);
    TNECS_CHECK(W->byA.num_Cs);
    TNECS_CHECK(W->byA.Cs_O);
    TNECS_CHECK(W->byA.num_A_ids);
    TNECS_CHECK(W->byA.Cs);

    for (size_t i = olen; i < W->byA.len; i++) {
        W->byA.Es[i]    = calloc(TNECS_E_0LEN, sizeof(**W->byA.Es));
        W->byA.subA[i]  = calloc(TNECS_C_CAP, sizeof(**W->byA.subA));
        TNECS_CHECK(W->byA.Es[i]);
        TNECS_CHECK(W->byA.subA[i]);

        W->byA.len_Es[i] = TNECS_E_0LEN;
        W->byA.num_Es[i] = 0;
    }
    return (1);
}

int tnecs_grow_Pi(tnecs_W *W) {
    size_t olen = W->Pis.len;
    size_t nlen = olen * TNECS_ARR_GROW;
    W->Pis.len = nlen;
    if (nlen >= TNECS_Pi_CAP) {
        printf("tnecs: Pis cap reached\n");
        return (TNECS_NULL);
    }

    W->Pis.byPh = tnecs_realloc(W->Pis.byPh, olen, nlen,
                                sizeof(*W->Pis.byPh));
    TNECS_CHECK(W->Pis.byPh);

    return (1);
}

int tnecs_grow_Ph(tnecs_W *W, tnecs_Pi Pi) {
    tnecs_Phs *byPh = TNECS_Pi_GET(W, Pi);
    size_t olen = byPh->len;
    size_t nlen = olen * TNECS_ARR_GROW;
    byPh->len = nlen;
    if (nlen >= TNECS_Ph_CAP) {
        printf("tnecs: Phs cap reached\n");
        return (TNECS_NULL);
    }

    byPh->Ss      = tnecs_realloc(  byPh->Ss, olen, nlen,
                                    sizeof(*byPh->Ss));
    byPh->Ss_id   = tnecs_realloc(  byPh->Ss_id, olen, nlen,
                                    sizeof(*byPh->Ss_id));
    byPh->len_Ss  = tnecs_realloc(  byPh->len_Ss, olen, nlen,
                                    sizeof(*byPh->len_Ss));
    byPh->num_Ss  = tnecs_realloc(  byPh->num_Ss, olen, nlen,
                                    sizeof(*byPh->num_Ss));
    TNECS_CHECK(byPh->Ss);
    TNECS_CHECK(byPh->Ss_id);
    TNECS_CHECK(byPh->len_Ss);
    TNECS_CHECK(byPh->num_Ss);

    for (size_t i = olen; i < byPh->len; i++) {
        size_t bytesize1 = sizeof(**byPh->Ss);
        size_t bytesize2 = sizeof(**byPh->Ss_id);

        byPh->Ss[i]     = calloc(TNECS_Ph_0LEN, bytesize1);
        byPh->Ss_id[i]  = calloc(TNECS_Ph_0LEN, bytesize2);
        TNECS_CHECK(byPh->Ss[i]);
        TNECS_CHECK(byPh->Ss_id[i]);

        byPh->len_Ss[i] = TNECS_Ph_0LEN;
        byPh->num_Ss[i] = 0;
    }
    return (1);
}

int tnecs_grow_S_byPh(tnecs_Phs *byPh, tnecs_Ph Ph) {
    size_t olen         = byPh->len_Ss[Ph];
    size_t nlen         = olen * TNECS_ARR_GROW;
    byPh->len_Ss[Ph]    = nlen;
    size_t bs           = sizeof(**byPh->Ss);
    size_t bsid         = sizeof(**byPh->Ss_id);

    tnecs_S_f *Ss   = byPh->Ss[Ph];
    size_t *S_id    = byPh->Ss_id[Ph];
    byPh->Ss[Ph]    = tnecs_realloc(Ss, olen, nlen, bs);
    byPh->Ss_id[Ph] = tnecs_realloc(S_id, olen, nlen, bsid);
    TNECS_CHECK(byPh->Ss[Ph]);
    TNECS_CHECK(byPh->Ss_id[Ph]);
    return (1);
}

int tnecs_grow_byA(tnecs_W *W, size_t tID) {
    size_t olen = W->byA.len_Es[tID];
    size_t nlen = olen * TNECS_ARR_GROW;

    assert(olen > 0);
    W->byA.len_Es[tID] = nlen;

    size_t bytesize = sizeof(*W->byA.Es[tID]);
    tnecs_E *ptr    = W->byA.Es[tID];
    W->byA.Es[tID]  = tnecs_realloc(ptr, olen, nlen, bytesize);
    TNECS_CHECK(W->byA.Es[tID]);
    return (1);
}

/* -- tnecs_in -- */
int tnecs_In_data(tnecs_In *in, void *data) {
    in->data = data;
    return (1);
}

/*************** SET BIT COUNTING *******************/
size_t setBits_KnR(tnecs_C flags) {
    /* Credits: Kernighan&Ritchie in 'C Programming Language' */
    size_t count = 0;
    while (flags) {
        flags &= (flags - 1);
        count++;
    }
    return (count);
}

void *tnecs_C_array(tnecs_W *W, const size_t C_id,
                    const size_t tID) {
    if ((C_id == TNECS_NULL) || (tID == TNECS_NULL))
        return (NULL);

    if (C_id >= W->Cs.num)
        return (NULL);

    tnecs_C_arr *carr = W->byA.Cs[tID];
    size_t       C_O  = W->byA.Cs_O[tID][C_id];

    return (carr[C_O].Cs);
}
