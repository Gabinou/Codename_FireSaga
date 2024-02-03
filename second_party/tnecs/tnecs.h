#ifndef __TNECS_H__
#define __TNECS_H__
/* Tiny C99 Entity-Component-System (ECS) library.
* tnecs.h
*
* Copyright (C) Gabriel Taillon, 2023
*
* ECSs are an alternative way to organize data and functions to
* Object-Oriented programming (OOP).
* OOP:
* - Objects/Classes contain data and methods.
* - Methods act on objects.
* - Children classes inherit methods and data structure from parents.
* ECS:
* - Components are purely data.
* - Any number of components can be attached to an entity.
* - Entities are acted upon by systems.
*
* In tnecs, an entity is an ```uint64_t``` index.
* A component is user-defined ```struct```.
* A system is a user-defined ```function```.
* All live inside the ```world```.
*
* The systems iterate over the entities that have a user-defined set of components,
* inclusively or exclusively, in phases.
* Phases are user-defined ```uint8_t```.
* System execution order is first-come first-served by default.
* Systems are inclusive by default, meaning that they run over entities with
* additional components to the system's.
*/

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#ifndef log2 // tcc SUCKS and DOES NOT define log2
    #define log2(x)  (x > 0 ? (log(x)/log(2.0f)) : -INFINITY)
#endif

#ifdef __cplusplus
extern "C" {
#endif
/****************************** DEBUGGING ************************************/
#define TNECS_DEBUG_A // TNECS_DEBUG_ASSERT are ignored if undefined
#ifdef TNECS_DEBUG_A
#define TNECS_DEBUG_ASSERT(in) do {assert(in);}while(0)
#else
#define TNECS_DEBUG_ASSERT(...) (void)0
#endif

// #define TNECS_DEBUG_P // TNECS_DEBUG_PRINTF are ignored if undefined
#ifdef TNECS_DEBUG_P
#define TNECS_DEBUG_PRINTF(...) do {printf(__VA_ARGS__);}while(0)
#else
#define TNECS_DEBUG_PRINTF(...) (void)0
#endif

/********************** 0.1 MICROSECOND RESOLUTION CLOCK **********************/
//  Modified from: https://gist.github.com/ForeverZer0/0a4f80fc02b96e19380ebb7a3debbee5
#if defined(__linux)
#  define MICROSECOND_CLOCK
#  define HAVE_POSIX_TIMER
#  include <time.h>
#  ifdef CLOCK_MONOTONIC
#     define CLOCKID CLOCK_MONOTONIC
#  else
#     define CLOCKID CLOCK_REALTIME
#  endif
#elif defined(__APPLE__)
#  define MICROSECOND_CLOCK
#  define HAVE_MACH_TIMER
#  include <mach/mach_time.h>
#elif defined(_WIN32)
#  define MICROSECOND_CLOCK
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

extern uint64_t  tnecs_get_ns();
extern double    tnecs_get_us();

/**************************** TYPE DEFINITIONS *******************************/
typedef uint64_t        tnecs_entity;     // simple 64 bit integer
typedef uint64_t        tnecs_component;  // 64 bit flags -> MAX 63 components
typedef uint64_t        tnecs_hash;
typedef uint8_t         tnecs_phase;
typedef uint64_t        tnecs_ns;
typedef unsigned char   tnecs_byte;

typedef struct tnecs_World             tnecs_world;
typedef struct tnecs_System_Input      tnecs_system_input;
typedef struct tnecs_Components_Array  tnecs_component_array;

typedef void (*tnecs_system_ptr)(struct tnecs_System_Input *);

/***************************** CONSTANT DEFINITIONS **************************/
enum TNECS {
    TNECS_NULL =                   0, // entity, component, system, typeflag: 0 is NULL
    TNECS_NULLSHIFT =              1,
    TNECS_COMPONENT_CAP =          64,
    TNECS_OPEN_IDS_BUFFER =        128,
    TNECS_INITIAL_ENTITY_LEN =     128,
    TNECS_INITIAL_PHASE_LEN =      8,
    TNECS_INITIAL_COMPONENT_LEN =  8,
    TNECS_INITIAL_SYSTEM_LEN =     16,
    TNECS_ARRAY_GROWTH_FACTOR =    2 // in general 2 or 1.5
};

/************************** UTILITY MACROS ***********************************/
#define TNECS_STRINGIFY(x) #x
#define TNECS_DONOTHING(x) x
#define TNECS_CONCATENATE( arg1, arg2) TNECS_CONCATENATE1(arg1, arg2)
#define TNECS_CONCATENATE1(arg1, arg2) TNECS_CONCATENATE2(arg1, arg2)
#define TNECS_CONCATENATE2(arg1, arg2) arg1##arg2
#define TNECS_TYPEFLAG_HAS_TYPE(typeflag, type) ((typeflag & type) > 0)
#define TNECS_TYPEFLAG_IS_ARCHETYPE(typeflag1, typeflag2) ((typeflag1 & typeflag2) == typeflag1) // checks if typeflag2 is a archetype of typeflag1

/******************* HACKY DISTRIBUTION FOR VARIADIC MACROS ******************/
// Distribution as in algebra: a(x+b) -> ax + ab

// TNECS_VAR_EACH_ARGN(__VA_ARGS__) counts the number of args
//      -> up to 63, if elements in TNECS_VAR_ARGN and TNECS_VAR_VARG_SEQ exist
#define TNECS_VAR_EACH_ARGN(...) TNECS_VAR_EACH_ARGN_(__VA_ARGS__, TNECS_VAR_VARG_SEQ())
#define TNECS_VAR_EACH_ARGN_(...) TNECS_VAR_ARGN(__VA_ARGS__)
#define TNECS_VAR_ARGN(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define TNECS_VAR_VARG_SEQ() 8, 7, 6, 5, 4, 3, 2, 1, 0

// TNECS_VARMACRO_FOREACH_XXXX(foo, __VA_ARGS__) applies foo to each __VA_ARGS__, PLUS
//      -> _SCOMMA variant stringifies and puts commas around each (except last)
//      -> _COMMA puts commas around each (except last)
//      up to 63 args if all TNECS_FOREACH_XXXX_N exist
#define TNECS_FOREACH_COMMA_1(macro, x)       macro(x)
#define TNECS_FOREACH_COMMA_2(macro, x, ...)  macro(x),  TNECS_FOREACH_COMMA_1(macro, __VA_ARGS__)
#define TNECS_FOREACH_COMMA_3(macro, x, ...)  macro(x),  TNECS_FOREACH_COMMA_2(macro, __VA_ARGS__)
#define TNECS_FOREACH_COMMA_4(macro, x, ...)  macro(x),  TNECS_FOREACH_COMMA_3(macro,  __VA_ARGS__)
#define TNECS_FOREACH_COMMA_5(macro, x, ...)  macro(x),  TNECS_FOREACH_COMMA_4(macro,  __VA_ARGS__)
#define TNECS_FOREACH_COMMA_6(macro, x, ...)  macro(x),  TNECS_FOREACH_COMMA_5(macro,  __VA_ARGS__)
#define TNECS_FOREACH_COMMA_7(macro, x, ...)  macro(x),  TNECS_FOREACH_COMMA_6(macro,  __VA_ARGS__)
#define TNECS_FOREACH_COMMA_8(macro, x, ...)  macro(x),  TNECS_FOREACH_COMMA_7(macro,  __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_1(macro, x)      macro(#x)
#define TNECS_FOREACH_SCOMMA_2(macro, x, ...) macro(#x), TNECS_FOREACH_SCOMMA_1(macro, __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_3(macro, x, ...) macro(#x), TNECS_FOREACH_SCOMMA_2(macro, __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_4(macro, x, ...) macro(#x), TNECS_FOREACH_SCOMMA_3(macro,  __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_5(macro, x, ...) macro(#x), TNECS_FOREACH_SCOMMA_4(macro,  __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_6(macro, x, ...) macro(#x), TNECS_FOREACH_SCOMMA_5(macro,  __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_7(macro, x, ...) macro(#x), TNECS_FOREACH_SCOMMA_6(macro,  __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_8(macro, x, ...) macro(#x), TNECS_FOREACH_SCOMMA_7(macro,  __VA_ARGS__)

#define TNECS_VARMACRO_FOREACH_COMMA_(N, macro, ...) TNECS_CONCATENATE(TNECS_FOREACH_COMMA_, N)(macro, __VA_ARGS__)
#define TNECS_VARMACRO_FOREACH_COMMA(macro, ...) TNECS_VARMACRO_FOREACH_COMMA_(TNECS_VAR_EACH_ARGN(__VA_ARGS__), macro, __VA_ARGS__)

#define TNECS_VARMACRO_FOREACH_SCOMMA_(N, macro, ...) TNECS_CONCATENATE(TNECS_FOREACH_SCOMMA_, N)(macro, __VA_ARGS__)
#define TNECS_VARMACRO_FOREACH_SCOMMA(macro, ...) TNECS_VARMACRO_FOREACH_SCOMMA_(TNECS_VAR_EACH_ARGN(__VA_ARGS__), macro, __VA_ARGS__)

/***************************** STRUCTS DEFINITIONS ***************************/
struct tnecs_World {
    tnecs_entity     *entities; // (entities[entity_id] == entity_id) unless deleted
    tnecs_component *typeflags;                       // [typeflag_id]
    tnecs_component *entity_typeflags;                // [entity_id]
    tnecs_component *system_typeflags;                // [system_id]
    tnecs_phase      *system_phases;                  // [system_id]
    bool               *system_exclusive;               // [system_id]
    tnecs_phase      *phases;                         // [phase_id]
    size_t             *system_orders;                  // [system_id]
    size_t            component_bytesizes[TNECS_COMPONENT_CAP]; // [component_id]
    tnecs_hash      component_hashes[TNECS_COMPONENT_CAP];    // [component_id]
    char *component_names[TNECS_COMPONENT_CAP];         // [component_id]
    tnecs_hash       *system_hashes;                  // [system_id]
    char              **system_names;                   // [system_id]

    //_bytype arrays are exclusive
    size_t             **archetype_id_bytype;           // [typeflag_id][typeflag_id_order]
    size_t              *num_archetype_ids;             // [typeflag_id]
    struct tnecs_Components_Array **components_bytype;  // [typeflag_id][component_order_bytype]
    tnecs_entity     **entities_bytype;               // [typeflag_id][entity_order_bytype]
    tnecs_component **components_idbytype;            // [typeflag_id][component_order_bytype]
    tnecs_component **components_flagbytype;          // [typeflag_id][component_order_bytype]
    size_t             **components_orderbytype;        // [typeflag_id][component_id]
    size_t             **systems_idbyphase;             // [phase_id][system_order]
    tnecs_system_ptr   **systems_byphase;               // [phase_id][system_id]
    tnecs_system_ptr   *systems_torun;                  // [torun_order] debug
    size_t            num_systems_torun;
    size_t            len_systems_torun;

    size_t            len_entities;                        // len is allocated size
    size_t            len_typeflags;                       // len is allocated size
    size_t            len_systems;                         // len is allocated size
    size_t            len_phases;                          // len is allocated size
    size_t            num_components;                      // num is active elements
    size_t            num_typeflags;                       // num is active elements
    size_t            num_systems;                         // num is active elements
    size_t            num_phases;                          // num is active elements
    size_t   *entity_orders;                       // [entity_id]
    size_t   *num_components_bytype;               // [typeflag_id]
    size_t   *len_entities_bytype;                 // [typeflag_id]
    size_t   *num_entities_bytype;                 // [typeflag_id]
    size_t   *len_systems_byphase;                 // [phase_id]
    size_t   *num_systems_byphase;                 // [phase_id]

    size_t            num_entities_open;
    size_t            len_entities_open;
    tnecs_entity    entity_next;
    tnecs_entity     *entities_open;

    tnecs_ns previous_time;
};

struct tnecs_System_Input {
    // Note: Systems run over entity_order_bytype for entity_order_bytype
    struct tnecs_World   *world;
    tnecs_ns     deltat;
    tnecs_component   system_typeflag;
    size_t              num_entities;
    size_t              entity_typeflag_id;
    void                 *user_data;
};

struct tnecs_Components_Array {
    tnecs_component   type;
    size_t              num_components;
    size_t              len_components;
    void                *
    components;                                        // [entity_order_bytype]
};

/**************************** WORLD FUNCTIONS ********************************/
struct tnecs_World *tnecs_world_genesis();
void tnecs_world_destroy(struct tnecs_World             *w);
void tnecs_world_step(struct tnecs_World                *w, tnecs_ns deltat);
void tnecs_world_step_wdata(struct tnecs_World *w, tnecs_ns deltat,
                            void *data);

/**************************** SYSTEM FUNCTIONS ********************************/
void tnecs_system_run(struct tnecs_World *w, size_t id, void *data);
void tnecs_system_run_dt(struct tnecs_World *w, size_t id, tnecs_ns deltat,
                         void *data);
void tnecs_systems_byphase_run(struct tnecs_World *w, tnecs_phase phase_id,
                               void *data);
void tnecs_systems_byphase_run_dt(struct tnecs_World *w, tnecs_phase phase_id,
                                  tnecs_ns deltat, void *data);
void tnecs_custom_system_run(struct tnecs_World *w, tnecs_system_ptr c,
                             tnecs_component ar,
                             tnecs_ns deltat, void *data);

/***************************** REGISTRATION **********************************/
tnecs_component tnecs_register_component(struct tnecs_World *w,
                                         const char *name, size_t b);

size_t tnecs_register_system(struct tnecs_World *w, const char *name,
                             void (* system)(struct tnecs_System_Input *), tnecs_phase run_phase,
                             bool isExclusive, size_t component_num, tnecs_component component_typeflag);
size_t tnecs_register_phase(struct tnecs_World *w, tnecs_phase phase);


#define TNECS_REGISTER_SYSTEM(world, pfunc, ...) tnecs_register_system(world, #pfunc, &pfunc, 0, 0, TNECS_VAR_EACH_ARGN(__VA_ARGS__), tnecs_component_names2typeflag(world, TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__)))
#define TNECS_REGISTER_SYSTEM_wPHASE(world, pfunc, phase, ...) tnecs_register_system(world, #pfunc, &pfunc, phase, 0,TNECS_VAR_EACH_ARGN(__VA_ARGS__), tnecs_component_names2typeflag(world, TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__)))
#define TNECS_REGISTER_SYSTEM_wEXCL(world, pfunc, excl, ...) tnecs_register_system(world, #pfunc, &pfunc, 0, excl, TNECS_VAR_EACH_ARGN(__VA_ARGS__), tnecs_component_names2typeflag(world, TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__)))
#define TNECS_REGISTER_SYSTEM_wPHASE_wEXCL(world, pfunc, phase, excl, ...) tnecs_register_system(world, #pfunc, &pfunc, phase, excl,TNECS_VAR_EACH_ARGN(__VA_ARGS__), tnecs_component_names2typeflag(world, TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__)))
#define TNECS_REGISTER_SYSTEM_wEXCL_wPHASE(world, pfunc, excl, phase, ...) tnecs_register_system(world, #pfunc, &pfunc, phase, excl,TNECS_VAR_EACH_ARGN(__VA_ARGS__), tnecs_component_names2typeflag(world, TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__)))

#define TNECS_REGISTER_COMPONENT(world, name) tnecs_register_component(world, #name, sizeof(name))

/***************************** ENTITY MANIPULATION ***************************/
/* -- Public -- */
tnecs_entity tnecs_entity_create(struct tnecs_World *w);
tnecs_entity tnecs_entity_create_wID(struct tnecs_World *w, tnecs_entity entity);
tnecs_entity tnecs_entities_create(struct tnecs_World *w, size_t num);
tnecs_entity tnecs_entities_create_wID(struct tnecs_World *w, size_t num,
                                       tnecs_entity *ents);
tnecs_entity tnecs_entity_create_wcomponents(struct tnecs_World *w, size_t argnum, ...);

tnecs_entity tnecs_entity_destroy(struct tnecs_World *w, tnecs_entity entity);

// Overloaded macros: -> TNECS_ENTITY_CREATE, TNECS_ENTITIES_CREATE, TNECS_ADD_COMPONENT
#define TNECS_ENTITY_CREATE(...) TNECS_CHOOSE_ENTITY_CREATE(__VA_ARGS__, TNECS_ENTITY_CREATE2, TNECS_ENTITY_CREATE1)(__VA_ARGS__)
#define TNECS_CHOOSE_ENTITY_CREATE(_1,_2,NAME,...) NAME
#define TNECS_ENTITY_CREATE1(world) tnecs_entity_create(world)
#define TNECS_ENTITY_CREATE2(world, index) tnecs_entity_create_wID(world, index)
#define TNECS_ENTITY_EXISTS(world, index) (world->entities[index] > TNECS_NULL)

#define TNECS_ENTITIES_CREATE(...) TNECS_CHOOSE_ENTITIES_CREATE(__VA_ARGS__, TNECS_ENTITIES_CREATE3, TNECS_ENTITIES_CREATE2)(__VA_ARGS__)
#define TNECS_CHOOSE_ENTITIES_CREATE(_1,_2,_3,NAME,...) NAME
#define TNECS_ENTITIES_CREATE2(world, num) tnecs_entities_create(world, num)
#define TNECS_ENTITIES_CREATE3(world, num, indices) tnecs_entities_create_wID(world, num, indices)

#define TNECS_ENTITY_CREATE_wCOMPONENTS(world, ...) tnecs_entity_create_wcomponents(world, TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_SCOMMA(TNECS_HASH, __VA_ARGS__))
#define TNECS_ENTITY_TYPEFLAG(world, entity) world->entity_typeflags[entity]
#define TNECS_ENTITY_HASCOMPONENT(world, entity, name) ((world->entity_typeflags[entity] &tnecs_component_names2typeflag(world, 1, #name)) > 0)

#define TNECS_ADD_COMPONENT(...) TNECS_CHOOSE_ADD_COMPONENT(__VA_ARGS__, TNECS_ADD_COMPONENT4, TNECS_ADD_COMPONENT3)(__VA_ARGS__)
#define TNECS_CHOOSE_ADD_COMPONENT(_1,_2,_3,_4,NAME,...) NAME
#define TNECS_ADD_COMPONENT3(world, entity_id, component) tnecs_entity_add_components(world, entity_id, 1, tnecs_component_names2typeflag(world, 1, #component), true)
#define TNECS_ADD_COMPONENT4(world, entity_id, component, isnewtype) tnecs_entity_add_components(world, entity_id, 1, tnecs_component_names2typeflag(world, 1, #component), isnewtype)

#define TNECS_ADD_COMPONENTS(world, entity_id, isnewtype, ...) tnecs_entity_add_components(world, entity_id, TNECS_VAR_EACH_ARGN(__VA_ARGS__), tnecs_component_names2typeflag(world, TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__)), isnewtype)

#define TNECS_REMOVE_COMPONENTS(world, entity_id, ...) tnecs_entity_remove_components(world, entity_id, TNECS_VAR_EACH_ARGN(__VA_ARGS__), tnecs_component_names2typeflag(world, TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__)))

#define TNECS_GET_COMPONENT(world, entity_id, name) tnecs_entity_get_component(world, entity_id, tnecs_component_name2id(world, #name))

/*****************************************************************************/
/***************************** TNECS INTERNALS *******************************/
/*****************************************************************************/
tnecs_entity tnecs_entity_add_components(struct tnecs_World *w, tnecs_entity entity,
                                         size_t num_components, tnecs_component typeflag, bool isNew);
void tnecs_entity_remove_components(struct tnecs_World *w,
                                    tnecs_entity entity, size_t num_components, tnecs_component typeflag);
void *tnecs_entity_get_component(struct tnecs_World *w, tnecs_entity entity,
                                 tnecs_component component_id);

size_t tnecs_entitiesbytype_add(struct tnecs_World *w, tnecs_entity entity,
                                tnecs_component new_type);
size_t tnecs_entitiesbytype_del(struct tnecs_World *w, tnecs_entity entity,
                                tnecs_component old_type);
size_t tnecs_entitiesbytype_migrate(struct tnecs_World *w, tnecs_entity entity,
                                    tnecs_component old_type, tnecs_component new_type);

bool tnecs_component_add(struct tnecs_World *w, tnecs_component flag);
bool tnecs_component_copy(struct tnecs_World *w, tnecs_entity entity,
                          tnecs_component old_flag, tnecs_component new_flag);
bool tnecs_component_del(struct tnecs_World *w, tnecs_entity entity,
                         tnecs_component old_flag);
bool tnecs_component_migrate(struct tnecs_World *w, tnecs_entity entity,
                             tnecs_component old_flag, tnecs_component new_flag);

bool tnecs_component_array_new(struct tnecs_World *w, size_t num_components,
                               tnecs_component typeflag);
void tnecs_component_array_init(struct tnecs_World *w,
                                struct tnecs_Components_Array *array, size_t component_id);

bool tnecs_system_order_switch(struct tnecs_World *w, tnecs_phase phase_id,
                               size_t order1, size_t order2);

/************************ UTILITY FUNCTIONS/MACROS ***************************/
size_t tnecs_component_name2id(struct tnecs_World *w,
                               const char *name);
size_t tnecs_component_hash2id(struct tnecs_World *w, tnecs_hash hash);
size_t tnecs_component_order_bytype(struct tnecs_World *w, size_t component_id,
                                    tnecs_component typeflag);
size_t tnecs_component_order_bytypeid(struct tnecs_World *w, size_t component_id,
                                      size_t typeflag_id);
tnecs_component tnecs_component_names2typeflag(struct tnecs_World *w, size_t argnum, ...);
tnecs_component tnecs_component_ids2typeflag(size_t argnum, ...);
tnecs_component tnecs_component_hash2type(struct tnecs_World *w, tnecs_hash hash);

size_t tnecs_system_name2id(struct tnecs_World *w,
                            const char *name);
size_t tnecs_system_hash2id(struct tnecs_World *w, tnecs_hash hash);
tnecs_component tnecs_system_name2typeflag(struct tnecs_World *w,
                                           const char *name);

size_t tnecs_typeflagid(struct tnecs_World *w, tnecs_component typeflag);

#define TNECS_COMPONENT_HASH2ID(world, hash) tnecs_component_hash2id(world, hash)
#define TNECS_COMPONENT_HASH2TYPE(world, hash) tnecs_component_hash2type(world, hash)
#define TNECS_COMPONENT_NAME2TYPE(world, name) tnecs_component_names2typeflag(world, 1, #name)
#define TNECS_COMPONENT_NAMES2TYPEFLAG(world, ...) tnecs_component_names2typeflag(world, TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__))
#define TNECS_COMPONENT_NAMES2TYPEFLAGID(world, ...) tnecs_typeflagid(world, tnecs_component_names2typeflag(world, TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__)))
#define TNECS_COMPONENT_ID2TYPE(id) (1 << (id - TNECS_NULLSHIFT))
#define TNECS_COMPONENT_IDS2TYPEFLAG(...) tnecs_component_ids2typeflag(TNECS_VAR_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_DONOTHING, __VA_ARGS__))
#define TNECS_COMPONENT_NAME2ID(world, name) tnecs_component_name2id(world, #name)
#define TNECS_COMPONENT_TYPE2ID(type) (type >=1 ? (tnecs_component)(log2(type) + 1.1f): 0) // casting to int floors
#define TNECS_COMPONENTS_LIST(input, component_name) (input->world->components_bytype[input->entity_typeflag_id][input->world->components_orderbytype[input->entity_typeflag_id][tnecs_component_name2id(input->world, #component_name)]].components)

#define TNECS_SYSTEM_ID2TYPEFLAG(world, id) world->system_typeflags[id]
#define TNECS_SYSTEM_NAME2ID(world, name) tnecs_system_name2id(world, #name)
#define TNECS_SYSTEM_NAME2TYPEFLAG(world, name) tnecs_system_name2typeflag(world, #name)
#define TNECS_SYSTEM_NAME2TYPEFLAGID(world, name) tnecs_typeflagid(world, tnecs_system_name2typeflag(world, #name))

#define TNECS_TYPEFLAGID(world, typeflag) tnecs_typeflagid(world, typeflag)

/***************************** "DYNAMIC" ARRAYS ******************************/
void *tnecs_realloc(void *ptr, size_t old_len, size_t new_len, size_t elem_bytesize);
void *tnecs_arrdel(void *arr, size_t elem, size_t len, size_t bytesize);
void *tnecs_arrdel_scramble(void *arr, size_t elem, size_t len, size_t bytesize);

bool tnecs_growArray_bytype(struct tnecs_World *w, size_t typeflag_id);
bool tnecs_growArray_entity(struct tnecs_World *w);
bool tnecs_growArray_system(struct tnecs_World *w);
bool tnecs_growArray_typeflag(struct tnecs_World *w);
bool tnecs_growArray_phase(struct tnecs_World *w);

/****************************** STRING HASHING *******************************/
uint64_t tnecs_hash_djb2(const char *str);  // slightly faster
uint64_t tnecs_hash_sdbm(const char *str);
uint64_t tnecs_hash_combine(uint64_t h1, uint64_t h2);
#define TNECS_HASH(name) tnecs_hash_djb2(name)

/****************************** SET BIT COUNTING *****************************/
size_t setBits_KnR_uint64_t(uint64_t flags);

#ifdef __cplusplus
}
#endif
#endif // __TNECS_H__