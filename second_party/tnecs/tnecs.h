#ifndef __TNECS_H__
#define __TNECS_H__
/* Tiny C99 Entity-Component-System (ECS) library.
* tnecs.h
*
* Copyright (C) Gabriel Taillon, 2023-2025
*
* In tnecs, an entity is an ```ull``` index.
* A component is user-defined ```struct```.
* A system is a user-defined ```function```.
* All live inside the ```world```.
*
* The systems iterate over the entities that have a user-defined set of components,
* inclusively or exclusively, in phases.
* Phases are user-defined ```uint8_t```.
* System execution order in a phases is first-come first-served by default.
* Systems are inclusive by default, they run for all compatible archetypes.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#ifndef log2 /* for tcc */
    #define log2(x) (log(x) * 1.44269504088896340736) /* log(x) / log(2) */
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
typedef unsigned long long int  u64;                // 64 bit int
typedef unsigned long long int  tnecs_entity;       // 64 bit int
typedef unsigned long long int  tnecs_component;    // 64 bit flag
typedef unsigned long int       tnecs_phase;
typedef unsigned long long int  tnecs_ns;
typedef int                     b32;
typedef unsigned char           tnecs_byte;

/*** Forward declarations ***/
typedef struct tnecs_system_input tnecs_system_input;

/*** Function pointer ***/
typedef void (*tnecs_system_ptr)(struct tnecs_system_input *);

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
#define TNECS_ARCHETYPE_HAS_TYPE(archetype, type) ((archetype & type) > 0)
#define TNECS_ARCHETYPE_IS_SUBTYPE(archetype1, archetype2) ((archetype1 & archetype2) == archetype1)

/********** HACKY DISTRIBUTION FOR VARIADIC MACROS **********/
// Distribution as in algebra: a(x+b) -> ax + ab

// TNECS_VAR_EACH_ARGN(__VA_ARGS__) counts the number of args
//  - up to 63, if TNECS_VAR_ARGN and TNECS_VAR_VARG_SEQ exist
#define TNECS_VAR_EACH_ARGN(...) TNECS_VAR_EACH_ARGN_(__VA_ARGS__, TNECS_VAR_VARG_SEQ())
#define TNECS_VAR_EACH_ARGN_(...) TNECS_VAR_ARGN(__VA_ARGS__)
#define TNECS_VAR_ARGN(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define TNECS_VAR_VARG_SEQ() 8, 7, 6, 5, 4, 3, 2, 1, 0

// TNECS_VARMACRO_COMMA(__VA_ARGS__) puts commas around each arg, except last.
//  - up to 63 args if all TNECS_COMMA_N exist
#define TNECS_VARMACRO_COMMA(...) TNECS_VARMACRO_COMMA_(TNECS_VAR_EACH_ARGN(__VA_ARGS__), __VA_ARGS__)
#define TNECS_VARMACRO_COMMA_(N, ...) TNECS_CONCATENATE(TNECS_COMMA_, N)(__VA_ARGS__)

#define TNECS_COMMA_1(x)      x
#define TNECS_COMMA_2(x, ...) x,  TNECS_COMMA_1(__VA_ARGS__)
#define TNECS_COMMA_3(x, ...) x,  TNECS_COMMA_2(__VA_ARGS__)
#define TNECS_COMMA_4(x, ...) x,  TNECS_COMMA_3(__VA_ARGS__)
#define TNECS_COMMA_5(x, ...) x,  TNECS_COMMA_4(__VA_ARGS__)
#define TNECS_COMMA_6(x, ...) x,  TNECS_COMMA_5(__VA_ARGS__)
#define TNECS_COMMA_7(x, ...) x,  TNECS_COMMA_6(__VA_ARGS__)
#define TNECS_COMMA_8(x, ...) x,  TNECS_COMMA_7(__VA_ARGS__)

/********************* STRUCTS DEFINITIONS ******************/
typedef struct tnecs_carr { /* 1D array of components */
    tnecs_component  type;
    size_t           num;
    size_t           len;
    void            *components;      /* [entity_order_bytype] */
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
    // entities.num has slightly different meaning:
    // - Some entities might get deleted, but entities.num won't change.
    // - If reuse_entities is true, they are added to entities_open, and reused.
    // - If reuse_entities is false, entities do not get added to entities_open.
    //      - Call tnecs_entities_open_reuse to add open entities to
    //        entities_open and slate them for reuse.
    size_t num;
    size_t len;

    tnecs_entity    *id;            // [entity_id]
    size_t          *orders;        // [entity_id]
    tnecs_component *archetypes;    // [entity_id]
} tnecs_entities;

typedef struct tnecs_system {
    size_t num;
    size_t len;

    tnecs_phase     *phases;        // [system_id]
    size_t          *orders;        // [system_id]
    b32             *exclusive;     // [system_id]
    tnecs_component *archetypes;    // [system_id]
} tnecs_system;

typedef struct tnecs_archetype {
    size_t num;
    size_t len;

    tnecs_component   *id;                  // [archetype_id]
    size_t            *num_components;      // [archetype_id]
    size_t            *len_entities;        // [archetype_id]
    size_t            *num_entities;        // [archetype_id]
    size_t            *num_archetype_ids;   // [archetype_id]
    size_t           **archetype_id;        // [archetype_id][archetype_id_order]
    tnecs_entity     **entities;            // [archetype_id][entity_order_bytype]
    size_t           **components_order;    // [archetype_id][component_id]
    tnecs_component  **components_id;       // [archetype_id][component_order_bytype]

    tnecs_carr       **components;          // [archetype_id][component_order_bytype]
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

    b32 reuse_entities;
} tnecs_world;

struct tnecs_system_input {
    tnecs_world     *world;
    tnecs_ns         deltat;
    tnecs_component  system_archetype;
    size_t           num_entities;
    size_t           entity_archetype_id;
    void            *data;
};

/******************** WORLD FUNCTIONS ***********************/
b32 tnecs_world_genesis(tnecs_world **w);
b32 tnecs_world_destroy(tnecs_world **w);

b32 tnecs_world_step(      tnecs_world *w,                     tnecs_ns deltat, void *data);
b32 tnecs_world_step_phase(tnecs_world *w, tnecs_phase  phase, tnecs_ns deltat, void *data);

/********************* SYSTEM FUNCTIONS ********************/
b32 tnecs_system_run(       tnecs_world *w,     size_t           id, tnecs_ns deltat, void *data);
b32 tnecs_custom_system_run(tnecs_world *w,     tnecs_system_ptr c,
                            tnecs_component ar, tnecs_ns         dt, void *data);

/*********************** REGISTRATION ***********************/
size_t          tnecs_register_phase(    tnecs_world    *w,    tnecs_phase         phase);
size_t          tnecs_register_system(   tnecs_world    *w,    tnecs_system_ptr    system, 
                                         tnecs_phase     p,    b32                 isExclusive,
                                         size_t          num,  tnecs_component     archetype);
tnecs_component tnecs_register_component(tnecs_world    *w,    size_t              b);

#define TNECS_REGISTER_SYSTEM(world, pfunc, phase, excl, ...) tnecs_register_system(world, &pfunc, phase, excl, TNECS_VAR_EACH_ARGN(__VA_ARGS__), tnecs_component_ids2archetype(TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_COMMA(__VA_ARGS__)))

#define TNECS_REGISTER_COMPONENT(world, name) tnecs_register_component(world, sizeof(name))

/******************* ENTITY MANIPULATION ********************/
b32             tnecs_entity_isOpen(    tnecs_world *w, tnecs_entity ent);
tnecs_entity    tnecs_entity_create(    tnecs_world *w);
b32             tnecs_entity_destroy(   tnecs_world *w, tnecs_entity ent);
tnecs_entity    tnecs_entity_create_wcomponents(tnecs_world *w, size_t argnum, ...);

tnecs_entity    tnecs_entities_create(    tnecs_world *w, size_t num);
b32             tnecs_entities_open_reuse(tnecs_world *w);
b32             tnecs_entities_open_flush(tnecs_world *w);

#define TNECS_PHASE_VALID(      world, index) ((index == TNECS_NULL) || (world->byphase.id[index] == index))
#define TNECS_ENTITY_CREATE_wCOMPONENTS(world, ...) tnecs_entity_create_wcomponents(world, TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_COMMA(__VA_ARGS__))
#define TNECS_ENTITY_EXISTS(      world, index) ((index != TNECS_NULL) && (world->entities.id[index] == index))
#define TNECS_ENTITY_ARCHETYPE(   world, entity) world->entities.archetypes[entity]
#define TNECS_ENTITY_HASCOMPONENT(world, entity, cID) ((world->entities.archetypes[entity] & tnecs_component_ids2archetype(1, cID)) > 0)

#define TNECS_ADD_COMPONENT(...) TNECS_CHOOSE_ADD_COMPONENT(__VA_ARGS__, TNECS_ADD_COMPONENT4, TNECS_ADD_COMPONENT3)(__VA_ARGS__)
#define TNECS_CHOOSE_ADD_COMPONENT(_1,_2,_3,_4,NAME,...) NAME
#define TNECS_ADD_COMPONENT3(world, entity_id, cID) tnecs_entity_add_components(world, entity_id, tnecs_component_ids2archetype(1, cID), true)
#define TNECS_ADD_COMPONENT4(world, entity_id, cID, isnewtype) tnecs_entity_add_components(world, entity_id, tnecs_component_ids2archetype(1, cID), isnewtype)

#define TNECS_ADD_COMPONENTS(world, entity_id, isnewtype, ...) tnecs_entity_add_components(world, entity_id, tnecs_component_ids2archetype(TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_COMMA(__VA_ARGS__)), isnewtype)

#define TNECS_REMOVE_COMPONENTS(world, entity_id, ...) tnecs_entity_remove_components(world, entity_id, tnecs_component_ids2archetype(TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_COMMA(__VA_ARGS__)))
/************************************************************/
/********************** TNECS INTERNALS *********************/
/************************************************************/
void *tnecs_get_component(tnecs_world *w, tnecs_entity eID, tnecs_component cID);

tnecs_entity    tnecs_entity_add_components(   tnecs_world *w, tnecs_entity eID, tnecs_component archetype, b32 isNew);
b32             tnecs_entity_remove_components(tnecs_world *w, tnecs_entity eID, tnecs_component archetype);

b32 tnecs_entitiesbytype_add(    tnecs_world *w, tnecs_entity e, tnecs_component nt);
b32 tnecs_entitiesbytype_del(    tnecs_world *w, tnecs_entity e, tnecs_component ot);
b32 tnecs_entitiesbytype_migrate(tnecs_world *w, tnecs_entity e, tnecs_component ot, tnecs_component nt);

b32 tnecs_component_add(    tnecs_world     *w,     tnecs_component flag);

b32 tnecs_component_del(    tnecs_world     *w,     tnecs_entity    ent,
                            tnecs_component  of);
b32 tnecs_component_copy(   tnecs_world     *w,     tnecs_entity    ent,
                            tnecs_component  of,    tnecs_component nf);
b32 tnecs_component_migrate(tnecs_world     *w,     tnecs_entity    ent,
                            tnecs_component  of,    tnecs_component nf);

b32 tnecs_component_del(    tnecs_world     *w,     tnecs_entity    ent,
                            tnecs_component  of);
b32 tnecs_component_copy(   tnecs_world     *w,     tnecs_entity    ent,
                            tnecs_component  of,    tnecs_component nf);
b32 tnecs_component_migrate(tnecs_world     *w,     tnecs_entity    ent,
                            tnecs_component  of,    tnecs_component nf);

b32 tnecs_carr_new( tnecs_world *w, size_t         num_,   tnecs_component a);
b32 tnecs_carr_init(tnecs_world *w, tnecs_carr   *array,  size_t          cID);

b32 tnecs_system_order_switch(tnecs_world   *w, tnecs_phase phase,
                              size_t        o1, size_t      o2);

/************************** UTILITIES ***********************/
size_t tnecs_archetypeid(               tnecs_world *w, tnecs_component arch);
size_t tnecs_component_order_bytype(    tnecs_world *w, size_t          cID, tnecs_component arch);
size_t tnecs_component_order_bytypeid(  tnecs_world *w, size_t          cID, size_t          aID);

tnecs_component tnecs_component_ids2archetype(size_t argnum, ...);

#define TNECS_COMPONENT_ID2TYPE(id) (1 << (id - TNECS_NULLSHIFT))
#define TNECS_COMPONENT_TYPE2ID(type) (type >= 1 ? (tnecs_component)(log2(type) + 1.1f) : 0) // casting to int floors
#define TNECS_COMPONENT_IDS2ARCHETYPE(...) tnecs_component_ids2archetype(TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_COMMA(__VA_ARGS__))
#define TNECS_COMPONENT_IDS2ARCHETYPEID(world, ...) tnecs_archetypeid(world, tnecs_component_ids2archetype(TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_COMMA(__VA_ARGS__)))

#define TNECS_SYSTEM_ID2ARCHETYPE(world, id) world->systems.archetypes[id]

#define TNECS_COMPONENTS_LIST(input, cID)  tnecs_carr_component_array(input->world, cID, input->entity_archetype_id)
void *tnecs_carr_component_array(tnecs_world *world, const size_t cID, const size_t tID);

/********************** "DYNAMIC" ARRAYS ********************/
void *tnecs_arrdel( void *arr, size_t elem,     size_t len,     size_t bytesize);
void *tnecs_realloc(void *ptr, size_t old_len,  size_t new_len, size_t elem_bytesize);

b32 tnecs_grow_phase(           tnecs_world *w);
b32 tnecs_grow_torun(           tnecs_world *w);
b32 tnecs_grow_bytype(          tnecs_world *w,     size_t aID);
b32 tnecs_grow_entity(          tnecs_world *w);
b32 tnecs_grow_system(          tnecs_world *w);
b32 tnecs_grow_archetype(       tnecs_world *w);
b32 tnecs_grow_entities_open(   tnecs_world *w);
b32 tnecs_grow_system_byphase(  tnecs_world *w,     tnecs_phase  phase);
b32 tnecs_grow_component_array( tnecs_world *w,     tnecs_carr *comp_arr, 
                                size_t      tID,    size_t       corder);

/********************* SET BIT COUNTING *********************/
size_t setBits_KnR_u64(u64 flags);

#endif /* __TNECS_H__ */
