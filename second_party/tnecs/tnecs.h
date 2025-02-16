#ifndef __TNECS_H__
#define __TNECS_H__
/* tnecs: Tiny C99 Entity-Component-System (ECS) library.
*
* The simplest possible C99 ECS library, 
* only with the minimum necessary features. 
*
* Copyright (C) Gabriel Taillon, 2023-2025
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#ifndef log2 /* for tcc: log2(x) = log(x) / log(2) */
    #define log2(x) (log(x) * 1.44269504088896340736)
#endif

/************************ DEBUGGING *************************/
#define TNECS_CHECK_ALLOC(name) do {\
        if (name == NULL) { \
            printf("tnecs: failed allocation " #name "\n"); \
            return(0); \
        } \
    } while (0)

#define TNECS_CHECK_CALL(call) do {\
        if (!call) { \
            printf("tnecs: failed function call " #call "\n"); \
            return(0); \
        } \
    } while (0)

/********************* TYPE DEFINITIONS *********************/
typedef unsigned long long int  tnecs_entity;
typedef unsigned long long int  tnecs_component;
typedef unsigned long long int  tnecs_phase;
typedef unsigned long long int  tnecs_ns;

struct tnecs_input; /* Forward declaration */
typedef void (*tnecs_system_ptr)(struct tnecs_input *);

/******************* CONSTANT DEFINITIONS *******************/
enum TNECS {
    TNECS_NULL                  =         0,
    TNECS_NULLSHIFT             =         1,
    TNECS_INIT_ENTITY_LEN       =       128,
    TNECS_INIT_PHASE_LEN        =         8,
    TNECS_INIT_COMPONENT_LEN    =         8,
    TNECS_INIT_SYSTEM_LEN       =        16,
    TNECS_INIT_ARCHETYPE_LEN    =        16,
    TNECS_COMPONENT_CAP         =        64,
    TNECS_ENTITIES_CAP          = 100000000,
    TNECS_PHASES_CAP            = TNECS_INIT_PHASE_LEN * 8 + 1,
    TNECS_ARRAY_GROWTH_FACTOR   =         2
};

/********************* UTILITY MACROS ***********************/
#define TNECS_CONCATENATE( arg1, arg2) TNECS_CONCATENATE1(arg1, arg2)
#define TNECS_CONCATENATE1(arg1, arg2) TNECS_CONCATENATE2(arg1, arg2)
#define TNECS_CONCATENATE2(arg1, arg2) arg1##arg2
#define TNECS_ARCHETYPE_HAS_TYPE(archetype, type) \
    ((archetype & type) > 0)
#define TNECS_ARCHETYPE_IS_SUBTYPE(archetype1, archetype2) \
    ((archetype1 & archetype2) == archetype1)

/********** HACKY DISTRIBUTION FOR VARIADIC MACROS **********/
// Distribution as in algebra: a(x + b) -> ax + ab

// TNECS_VAR_EACH_ARGN(__VA_ARGS__) counts the number of args
//  - up to 63, if TNECS_VAR_ARGN and TNECS_VAR_VARG_SEQ exist
#define TNECS_VAR_EACH_ARGN(...) \
    TNECS_VAR_EACH_ARGN_(__VA_ARGS__, TNECS_VAR_VARG_SEQ())
#define TNECS_VAR_EACH_ARGN_(...) \
    TNECS_VAR_ARGN(__VA_ARGS__)
#define TNECS_VAR_ARGN(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) \
    N
#define TNECS_VAR_VARG_SEQ() \
    8, 7, 6, 5, 4, 3, 2, 1, 0

// TNECS_VARMACRO_COMMA(__VA_ARGS__) puts commas after each arg,
// except the last.
//  - up to 63 args if all TNECS_COMMA_N exist
#define TNECS_VARMACRO_COMMA(...) \
    TNECS_VARMACRO_COMMA_(\
        TNECS_VAR_EACH_ARGN(__VA_ARGS__), \
        __VA_ARGS__\
    )
#define TNECS_VARMACRO_COMMA_(N, ...) \
    TNECS_CONCATENATE(TNECS_COMMA_, N)(__VA_ARGS__)

#define TNECS_COMMA_1(x)      x
#define TNECS_COMMA_2(x, ...) x, TNECS_COMMA_1(__VA_ARGS__)
#define TNECS_COMMA_3(x, ...) x, TNECS_COMMA_2(__VA_ARGS__)
#define TNECS_COMMA_4(x, ...) x, TNECS_COMMA_3(__VA_ARGS__)
#define TNECS_COMMA_5(x, ...) x, TNECS_COMMA_4(__VA_ARGS__)
#define TNECS_COMMA_6(x, ...) x, TNECS_COMMA_5(__VA_ARGS__)
#define TNECS_COMMA_7(x, ...) x, TNECS_COMMA_6(__VA_ARGS__)
#define TNECS_COMMA_8(x, ...) x, TNECS_COMMA_7(__VA_ARGS__)

/********************* STRUCTS DEFINITIONS ******************/
typedef struct tnecs_carr { /* 1D array of components */
    tnecs_component  type;
    size_t           num;
    size_t           len;
    void            *components; /* [entity_order_bytype] */
} tnecs_carr;

typedef struct tnecs_array {
    void    *arr;
    size_t   num;
    size_t   len;
} tnecs_array;

typedef struct tnecs_phases {
    size_t num;
    size_t len;

    tnecs_phase       *id;          // [phase_id]
    size_t            *len_systems; // [phase_id]
    size_t            *num_systems; // [phase_id]
    size_t           **systems_id;  // [phase_id][system_order]
    tnecs_system_ptr **systems;     // [phase_id][system_id]
} tnecs_phases;

typedef struct tnecs_entities {
    // - .num doesn't change even if entities get deleted
    // - if reuse_entities: add deleted entities to entities_open
    //      - Call tnecs_entities_open_reuse to add entities with
    //        id[ent] == false entities_open.
    size_t num;
    size_t len;

    tnecs_entity    *id;            // [entity_id] -> eID
    size_t          *orders;        // [entity_id]
    tnecs_component *archetypes;    // [entity_id]
} tnecs_entities;

typedef struct tnecs_system {
    size_t num;
    size_t len;

    tnecs_phase     *phases;        // [system_id]
    size_t          *orders;        // [system_id]
    int             *exclusive;     // [system_id]
    tnecs_component *archetypes;    // [system_id]
} tnecs_system;

typedef struct tnecs_archetype {
    size_t num;
    size_t len;

    tnecs_component   *id;                  // [archetype_id] -> aID
    size_t            *num_components;      // [archetype_id]
    size_t            *len_entities;        // [archetype_id]
    size_t            *num_entities;        // [archetype_id]
    size_t            *num_archetype_ids;   // [archetype_id]

    size_t          **archetype_id;     // [aID][archetype_id_order]
    tnecs_entity    **entities;         // [aID][entity_order_bytype]
    size_t          **components_order; // [aID][component_id]
    tnecs_component **components_id;    // [aID][component_order_bytype]
    tnecs_carr      **components;       // [aID][component_order_bytype]
} tnecs_archetype;

typedef struct tnecs_components {
    size_t           num;
    size_t           bytesizes[TNECS_COMPONENT_CAP];  // [cID]
} tnecs_components;

typedef struct tnecs_world {
    tnecs_phases        byphase;
    tnecs_system        systems;
    tnecs_entities      entities;
    tnecs_archetype     bytype;
    tnecs_components    components;
    tnecs_array         entities_open;
    tnecs_array         systems_torun;

    int reuse_entities;
} tnecs_world;

typedef struct tnecs_input {
    tnecs_world     *world;
    tnecs_ns         deltat;
    tnecs_component  system_archetype;
    size_t           num_entities;
    size_t           entity_archetype_id;
    void            *data;
} tnecs_input;

/******************** WORLD ***********************/
int tnecs_world_genesis(tnecs_world **w);
int tnecs_world_destroy(tnecs_world **w);

void tnecs_world_toggle_reuse(tnecs_world *w, int toggle);

int tnecs_world_step(
    tnecs_world *w, tnecs_ns deltat, void *data);
int tnecs_world_step_phase(
    tnecs_world *w, tnecs_ns deltat, void *data, tnecs_phase phase);

/********************* SYSTEM ********************/
int tnecs_system_run(
    tnecs_world *w, size_t id, tnecs_ns deltat, void *data);
int tnecs_custom_system_run(
    tnecs_world *w,     tnecs_system_ptr c,    
    tnecs_component ar, tnecs_ns         dt,    void *data);

/*********************** REGISTRATION ***********************/
size_t tnecs_register_phase(
    tnecs_world    *w,    tnecs_phase         phase);
size_t tnecs_register_system(
    tnecs_world    *w,    tnecs_system_ptr    system,
    tnecs_phase     p,    int                 isExclusive,
    size_t          num,  tnecs_component     archetype);
tnecs_component tnecs_register_component(
    tnecs_world    *w,    size_t b);

#define TNECS_REGISTER_SYSTEM(world, pfunc, phase, excl, ...) \
    tnecs_register_system(\
        world, &pfunc, phase, excl, \
        TNECS_VAR_EACH_ARGN(__VA_ARGS__), \
        tnecs_component_ids2archetype(\
            TNECS_VAR_EACH_ARGN(__VA_ARGS__), \
            TNECS_VARMACRO_COMMA(__VA_ARGS__)\
        )\
    )
#define TNECS_REGISTER_COMPONENT(world, name) \
    tnecs_register_component(world, sizeof(name))

/********************** ENTITY ***********************/
tnecs_entity tnecs_entity_isOpen( 
    tnecs_world *w, tnecs_entity ent);
tnecs_entity tnecs_entity_create( 
    tnecs_world *w);
tnecs_entity tnecs_entity_destroy(
    tnecs_world *w, tnecs_entity ent);
tnecs_entity tnecs_entity_create_wcomponents(
    tnecs_world *w, size_t argnum, ...);

tnecs_entity tnecs_entity_add_components(
    tnecs_world     *w,         tnecs_entity    eID,
    tnecs_component  archetype, int             isNew);
tnecs_entity tnecs_entity_remove_components(
    tnecs_world *w, tnecs_entity eID, tnecs_component archetype);

int tnecs_entities_open_reuse(tnecs_world *w);
int tnecs_entities_open_flush(tnecs_world *w);

#define TNECS_ENTITY_CREATE_wCOMPONENTS(world, ...) \
    tnecs_entity_create_wcomponents(\
        world, \
        TNECS_VAR_EACH_ARGN(__VA_ARGS__), \
        TNECS_VARMACRO_COMMA(__VA_ARGS__)\
    )
#define TNECS_ENTITY_EXISTS(world, index) \
    ((index != TNECS_NULL) && (world->entities.id[index] == index))
#define TNECS_ENTITY_ARCHETYPE(world, entity) \
    world->entities.archetypes[entity]

/********************** COMPONENT ***********************/
void *tnecs_get_component(
    tnecs_world *w, tnecs_entity eID, tnecs_component cID);

#define TNECS_ENTITY_HASCOMPONENT(world, entity, cID) \
    (\
        (\
            world->entities.archetypes[entity] & \
            tnecs_component_ids2archetype(1, cID)\
        ) > 0\
    )
#define TNECS_ADD_COMPONENT(...) \
    TNECS_CHOOSE_ADD_COMPONENT(\
        __VA_ARGS__, \
        TNECS_ADD_COMPONENT4, \
        TNECS_ADD_COMPONENT3\
    )(__VA_ARGS__)
#define TNECS_CHOOSE_ADD_COMPONENT(_1,_2,_3,_4,NAME,...) \
    NAME
#define TNECS_ADD_COMPONENT3(world, entity_id, cID) \
    tnecs_entity_add_components(\
        world, \
        entity_id, \
        tnecs_component_ids2archetype(1, cID), \
        true\
    )
#define TNECS_ADD_COMPONENT4(world, entity_id, cID, isnewtype) \
    tnecs_entity_add_components(\
        world, \
        entity_id, \
        tnecs_component_ids2archetype(1, cID), \
        isnewtype\
    )
#define TNECS_ADD_COMPONENTS(world, entity_id, isnewtype, ...) \
    tnecs_entity_add_components(\
        world, \
        entity_id, \
        tnecs_component_ids2archetype(\
            TNECS_VAR_EACH_ARGN(__VA_ARGS__), \
            TNECS_VARMACRO_COMMA(__VA_ARGS__)\
        ), \
        isnewtype\
    )
#define TNECS_REMOVE_COMPONENTS(world, entity_id, ...) \
    tnecs_entity_remove_components(\
        world, \
        entity_id, \
        tnecs_component_ids2archetype(\
            TNECS_VAR_EACH_ARGN(__VA_ARGS__), \
            TNECS_VARMACRO_COMMA(__VA_ARGS__)\
        )\
    )

/******************* COMPONENT ARRAY ***********************/
void *tnecs_component_array(
        tnecs_world *w, const size_t cID, const size_t tID);

#define TNECS_COMPONENT_ARRAY(input, cID) \
    tnecs_component_array(\
        input->world, \
        cID, \
        input->entity_archetype_id\
    )

/******************* ARCHETYPES ***********************/
tnecs_component tnecs_component_ids2archetype(
    size_t argnum, ...);
tnecs_component tnecs_archetypeid(
    tnecs_world *w, tnecs_component arch);

#define TNECS_COMPONENT_ID2TYPE(id) \
    (1 << (id - TNECS_NULLSHIFT))
#define TNECS_COMPONENT_TYPE2ID(type) \
    (type >= 1 ? (tnecs_component)(log2(type) + 1.1f) : 0) 
#define TNECS_COMPONENT_IDS2ARCHETYPE(...) \
    tnecs_component_ids2archetype(\
        TNECS_VAR_EACH_ARGN(__VA_ARGS__), \
        TNECS_VARMACRO_COMMA(__VA_ARGS__)\
    )
#define TNECS_COMPONENT_IDS2ARCHETYPEID(world, ...) \
    tnecs_archetypeid(\
        world, \
        TNECS_COMPONENT_IDS2ARCHETYPE(__VA_ARGS__)\
    )

/************************** SYSTEM ***********************/
int tnecs_system_order_switch(
    tnecs_world *w, tnecs_phase phase, size_t o1, size_t o2);

#define TNECS_SYSTEM_ID2ARCHETYPE(world, id) \
    world->systems.archetypes[id]

/************************** PHASE ***********************/
#define TNECS_PHASE_VALID(world, index) \
    ((index == TNECS_NULL) || (world->byphase.id[index] == index))

#endif /* __TNECS_H__ */
