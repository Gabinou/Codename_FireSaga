#ifndef __TNECS_H__
#define __TNECS_H__

// Tiny C99 Entity-Component-System (ECS) library.

// Originally created for use in a game I am developping using C99: https://gitlab.com/Gabinou/firesagamaker. Title pending.

// ECSs are an alternative way to organize data and functions to Object-Oriented programming (OOP).
// * OOP: Objects/Classes contain data and methods.
// Methods act on objects.
// Children classes inherit methods and data structure from parents.
// * ECS: Components are purely data.
// Any number of components can be attached to an entity.
// Entities are acted upon by systems.

// In tnecs, an entity is an ```uint64_t``` index.
// A component is user-defined ```struct```.
// A system is a user-defined ```function```.
// All live inside the ```world```.

// The systems iterate exclusively over the entities that have exactly the user-defined set of components, in phases.
// Phases are user-defined ```size_t``` and system execution order is first-come first-served by default.

/* Un-viral MIT License
*
* Copyright (c) 2021 Average Bear Games, Made by Gabriel Taillon
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE. */

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

extern uint64_t get_ns();
extern double get_us();

/**************************** TYPE DEFINITIONS *******************************/
typedef uint64_t tnecs_entity_t;     // simple 64 bit integer
typedef uint64_t tnecs_component_t;  // 64 bit flags -> MAX 63 components
typedef uint16_t tnecs_system_t;
typedef uint64_t tnecs_hash_t;
typedef uint8_t tnecs_phase_t;
typedef uint64_t tnecs_time_ns_t;
typedef unsigned char tnecs_byte_t;
typedef unsigned char tnecs_str_t;
typedef struct tnecs_World tnecs_world_t;
typedef struct tnecs_System_Input tnecs_system_input_t;
typedef struct tnecs_Components_Array tnecs_component_array_t;

/***************************** CONSTANT DEFINITIONS **************************/
#define TNECS_NULL 0 // entity, component, system, typeflag: 0 is NULL
#define TNECS_NULLSHIFT 1
#define TNECS_COMPONENT_CAP 64
#define TNECS_OPEN_IDS_BUFFER 128
#define TNECS_INITIAL_ENTITY_LEN 128
#define TNECS_INITIAL_PHASE_LEN 8
#define TNECS_INITIAL_COMPONENT_LEN 8
#define TNECS_INITIAL_SYSTEM_LEN 16
#define TNECS_ARRAY_GROWTH_FACTOR 2 // in general 2 or 1.5

/************************** UTILITY MACROS ***********************************/
#define TNECS_STRINGIFY(x) #x
#define TNECS_NULLMACRO(x) x
#define TNECS_CONCATENATE(arg1, arg2) TNECS_CONCATENATE1(arg1, arg2)
#define TNECS_CONCATENATE1(arg1, arg2) TNECS_CONCATENATE2(arg1, arg2)
#define TNECS_CONCATENATE2(arg1, arg2) arg1##arg2
#define TNECS_IS_SUBTYPE(typeflag, type) ((type & typeflag) > 0)

/******************* HACKY DISTRIBUTION FOR VARIADIC MACROS ******************/
// Distribution as in algebra: a(x+b) = ax + ab

// TNECS_VARMACRO_EACH_ARGN(__VA_ARGS__) counts the number of args
//      -> up to 63, if elements in TNECS_VARMACRO_ARGN and TNECS_VARMACRO_VARG_SEQ exist
#define TNECS_VARMACRO_EACH_ARGN(...) TNECS_VARMACRO_EACH_ARGN_(__VA_ARGS__, TNECS_VARMACRO_VARG_SEQ())
#define TNECS_VARMACRO_EACH_ARGN_(...) TNECS_VARMACRO_ARGN(__VA_ARGS__)
#define TNECS_VARMACRO_ARGN(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define TNECS_VARMACRO_VARG_SEQ() 8, 7, 6, 5, 4, 3, 2, 1, 0

// TNECS_VARMACRO_FOREACH_XXXX(foo, __VA_ARGS__) applies foo to each __VA_ARGS__, PLUS
//      -> _SUM variant puts + after each (except last)
//      -> _SCOMMA variant stringifies and puts commas around each (except last)
//      -> _COMMA puts commas around each (except last)
//      -> _NEWLINE makes newline for each (except last)
//      up to 8 input args. Theoretically up to 63, if all TNECS_FOREACH_XXXX_N exist
#define TNECS_FOREACH_1(macro, x) macro(x)
#define TNECS_FOREACH_COMMA_1(macro, x) macro(x)
#define TNECS_FOREACH_S1(macro, x) macro(#x)
#define TNECS_FOREACH_COMMA_2(macro, x, ...) macro(x), TNECS_FOREACH_1(macro, __VA_ARGS__)
#define TNECS_FOREACH_COMMA_3(macro, x, ...) macro(x), TNECS_FOREACH_COMMA_2(macro, __VA_ARGS__)
#define TNECS_FOREACH_COMMA_4(macro, x, ...) macro(x), TNECS_FOREACH_COMMA_3(macro,  __VA_ARGS__)
#define TNECS_FOREACH_COMMA_5(macro, x, ...) macro(x), TNECS_FOREACH_COMMA_4(macro,  __VA_ARGS__)
#define TNECS_FOREACH_COMMA_6(macro, x, ...) macro(x), TNECS_FOREACH_COMMA_5(macro,  __VA_ARGS__)
#define TNECS_FOREACH_COMMA_7(macro, x, ...) macro(x), TNECS_FOREACH_COMMA_6(macro,  __VA_ARGS__)
#define TNECS_FOREACH_COMMA_8(macro, x, ...) macro(x), TNECS_FOREACH_COMMA_7(macro,  __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_1(macro, x) macro(#x)
#define TNECS_FOREACH_SCOMMA_2(macro, x, ...) macro(#x), TNECS_FOREACH_S1(macro, __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_3(macro, x, ...) macro(#x), TNECS_FOREACH_SCOMMA_2(macro, __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_4(macro, x, ...) macro(#x), TNECS_FOREACH_SCOMMA_3(macro,  __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_5(macro, x, ...) macro(#x), TNECS_FOREACH_SCOMMA_4(macro,  __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_6(macro, x, ...) macro(#x), TNECS_FOREACH_SCOMMA_5(macro,  __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_7(macro, x, ...) macro(#x), TNECS_FOREACH_SCOMMA_6(macro,  __VA_ARGS__)
#define TNECS_FOREACH_SCOMMA_8(macro, x, ...) macro(#x),TNECS_FOREACH_SCOMMA_7(macro,  __VA_ARGS__)

#define TNECS_VARMACRO_FOREACH_COMMA_(N, macro, ...) TNECS_CONCATENATE(TNECS_FOREACH_COMMA_, N)(macro, __VA_ARGS__)
#define TNECS_VARMACRO_FOREACH_COMMA(macro, ...) TNECS_VARMACRO_FOREACH_COMMA_(TNECS_VARMACRO_EACH_ARGN(__VA_ARGS__), macro, __VA_ARGS__)

#define TNECS_VARMACRO_FOREACH_SCOMMA_(N, macro, ...) TNECS_CONCATENATE(TNECS_FOREACH_SCOMMA_, N)(macro, __VA_ARGS__)
#define TNECS_VARMACRO_FOREACH_SCOMMA(macro, ...) TNECS_VARMACRO_FOREACH_SCOMMA_(TNECS_VARMACRO_EACH_ARGN(__VA_ARGS__), macro, __VA_ARGS__)

/***************************** STRUCTS DEFINITIONS ***************************/
struct tnecs_World {
    tnecs_entity_t * entities; // (entities[entity_id] == entity_id) unless deleted
    tnecs_component_t * typeflags;                            // [typeflag_id]
    tnecs_component_t * entity_typeflags;                     // [entity_id]
    tnecs_component_t * system_typeflags;                     // [system_id]
    tnecs_phase_t * system_phases;                            // [system_id]
    tnecs_phase_t * phases;                                   // [phase_id]
    size_t * system_orders;                                   // [system_id]
    size_t component_bytesizes[TNECS_COMPONENT_CAP];          // [component_id]
    tnecs_hash_t component_hashes[TNECS_COMPONENT_CAP];       // [component_id]
    tnecs_hash_t * system_hashes;                             // [system_id]
    char ** component_names;

    // bytype arrays are exclusive -> entities unique in components_bytype
    struct tnecs_Components_Array ** components_bytype;       // [typeflag_id][component_order_bytype]
    tnecs_entity_t ** entities_bytype;                        // [typeflag_id][entity_order_bytype]
    tnecs_component_t ** components_idbytype;                 // [typeflag_id][component_order_bytype]
    tnecs_component_t ** components_flagbytype;               // [typeflag_id][component_order_bytype]
    size_t ** components_orderbytype;                         // [typeflag_id][component_id]
    size_t ** systems_idbyphase;                              // [phase_id][system_order]
    void (* ** systems_byphase)(struct tnecs_System_Input *); // [phase_id][system_id]
    void (** systems_torun)(struct tnecs_System_Input *);     // [torun_order] debug
    size_t num_systems_torun;

    // len is allocated size
    // num is active elements in array
    size_t len_entities, len_typeflags, len_systems, len_phases;
    size_t num_components, num_typeflags, num_systems, num_phases;
    size_t * entity_orders;                                   // [entity_id]
    size_t * num_components_bytype;                           // [typeflag_id]
    size_t * len_entities_bytype, * num_entities_bytype;      // [typeflag_id]
    size_t * len_systems_byphase, * num_systems_byphase;      // [phase_id]

    size_t num_entities_open, len_entities_open;
    tnecs_entity_t entity_next, *entities_open;

    tnecs_time_ns_t previous_time;
};

struct tnecs_System_Input {
    struct tnecs_World * world;
    size_t num_entities;
    tnecs_time_ns_t deltat;
    tnecs_component_t typeflag_id;
};

struct tnecs_Components_Array {
    tnecs_component_t type;
    size_t num_components, len_components;
    void * components;  // [entity_order_bytype]
};

/**************************** WORLD FUNCTIONS ********************************/
struct tnecs_World * tnecs_world_genesis();
void tnecs_world_destroy(struct tnecs_World * in_world);
void tnecs_world_step(struct tnecs_World * in_world, tnecs_time_ns_t in_deltat);
void tnecs_world_breath_entities(struct tnecs_World * in_world);
void tnecs_world_breath_components(struct tnecs_World * in_world);
void tnecs_world_breath_systems(struct tnecs_World * in_world);
void tnecs_world_breath_typeflags(struct tnecs_World * in_world);

/***************************** REGISTRATION **********************************/
void tnecs_register_component(struct tnecs_World * in_world, const char * in_name, size_t in_bytesize);
void tnecs_register_system(struct tnecs_World * in_world, tnecs_hash_t in_hash, void (* in_system)(struct tnecs_System_Input *), tnecs_phase_t in_run_phase, size_t component_num, tnecs_component_t component_typeflag);
size_t tnecs_register_typeflag(struct tnecs_World * in_wormakld, size_t num_components, tnecs_component_t typeflag);
size_t tnecs_register_phase(struct tnecs_World * in_world, tnecs_phase_t in_phase);

#define TNECS_REGISTER_SYSTEM(world, pfunc, ...) tnecs_register_system(world, TNECS_HASH(#pfunc), &pfunc, 0,  TNECS_VARMACRO_EACH_ARGN(__VA_ARGS__), tnecs_component_names2typeflag(world, TNECS_VARMACRO_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__)))
#define TNECS_REGISTER_SYSTEM_WPHASE(world, pfunc, phase, ...) tnecs_register_system(world, TNECS_HASH(#pfunc), &pfunc, phase, TNECS_VARMACRO_EACH_ARGN(__VA_ARGS__), tnecs_component_names2typeflag(world, TNECS_VARMACRO_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__)))
#define TNECS_REGISTER_COMPONENT(world, name) tnecs_register_component(world, #name, sizeof(name))

/***************************** ENTITY MANIPULATION ***************************/
tnecs_entity_t tnecs_entity_create(struct tnecs_World * in_world);
tnecs_entity_t tnecs_entity_create_windex(struct tnecs_World * in_world, tnecs_entity_t in_entity);
void tnecs_entities_create(struct tnecs_World * in_world, size_t num);
void tnecs_entities_create_windices(struct tnecs_World * in_world, size_t num, tnecs_entity_t * in_entities);
tnecs_entity_t tnecs_entity_create_wcomponents(struct tnecs_World * in_world, size_t argnum, ...);

void tnecs_entity_destroy(struct tnecs_World * in_world, tnecs_entity_t in_entity);

#define TNECS_CHOOSE_ENTITY_CREATE(_1,_2,NAME,...) NAME
#define TNECS_ENTITY_CREATE(...) TNECS_CHOOSE_ENTITY_CREATE(__VA_ARGS__, TNECS_ENTITY_CREATE2, TNECS_ENTITY_CREATE1)(__VA_ARGS__)
#define TNECS_ENTITY_CREATE1(world) tnecs_entity_create(world)
#define TNECS_ENTITY_CREATE2(world, index) tnecs_entity_create_windex(world, index)

#define TNECS_CHOOSE_ENTITIES_CREATE(_1,_2,_3,NAME,...) NAME
#define TNECS_ENTITIES_CREATE(...) TNECS_CHOOSE_ENTITIES_CREATE(__VA_ARGS__, TNECS_ENTITIES_CREATE3, TNECS_ENTITIES_CREATE2)(__VA_ARGS__)
#define TNECS_ENTITIES_CREATE2(world, num) tnecs_entities_create(world, num)
#define TNECS_ENTITIES_CREATE3(world, num, indices) tnecs_entities_create_windices(world, num, indices)

#define TNECS_ENTITY_CREATE_WCOMPONENTS(world, ...) tnecs_entity_create_wcomponents(world, TNECS_VARMACRO_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_SCOMMA(TNECS_HASH, __VA_ARGS__))
#define TNECS_ENTITY_TYPEFLAG(world, entity) world->entity_typeflags[entity]
#define TNECS_ENTITY_HASCOMPONENT(world, entity, name) ((world->entity_typeflags[entity] &tnecs_component_names2typeflag(world, 1, #name)) > 0)

// TNECS_ADD_COMPONENT is overloaded 3/4 inputs
//      skip checks if 4th input is true
#define TNECS_CHOOSE_ADD_COMPONENT(_1,_2,_3,_4,NAME,...) NAME
#define TNECS_ADD_COMPONENT(...) TNECS_CHOOSE_ADD_COMPONENT(__VA_ARGS__, TNECS_ADD_COMPONENT4, TNECS_ADD_COMPONENT3)(__VA_ARGS__)
#define TNECS_ADD_COMPONENT3(world, entity_id, component) tnecs_entity_add_components(world, entity_id, 1, tnecs_component_names2typeflag(world, 1, #component), true)
#define TNECS_ADD_COMPONENT4(world, entity_id, component, isnewtype) tnecs_entity_add_components(world, entity_id, 1, tnecs_component_names2typeflag(world, 1, #component), isnewtype)

#define TNECS_ADD_COMPONENTS(world, entity_id, isnewtype, ...) tnecs_entity_add_components(world, entity_id, TNECS_VARMACRO_EACH_ARGN(__VA_ARGS__), tnecs_component_names2typeflag(world, TNECS_VARMACRO_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__)), isnewtype)

#define TNECS_GET_COMPONENT(world, entity_id, name) (name *)tnecs_entity_get_component(world, entity_id, tnecs_component_name2id(world, #name))

/*****************************************************************************/
/***************************** TNECS INTERNALS *******************************/
/*****************************************************************************/
void tnecs_entity_add_components(struct tnecs_World * in_world, tnecs_entity_t in_entity, size_t num_components, tnecs_component_t typeflag, bool isNew);
void * tnecs_entity_get_component(struct tnecs_World * in_world, tnecs_entity_t in_entity, tnecs_component_t in_component_id);

size_t tnecs_entitiesbytype_add(struct tnecs_World * in_world, tnecs_entity_t in_entity, tnecs_component_t new_type);
void tnecs_entitiesbytype_del(struct tnecs_World * in_world, tnecs_entity_t in_entity, tnecs_component_t old_type);
size_t tnecs_entitiesbytype_migrate(struct tnecs_World * in_world, tnecs_entity_t in_entity, tnecs_component_t old_type, tnecs_component_t new_type);

void tnecs_component_add(struct tnecs_World * in_world, tnecs_component_t in_flag);
void tnecs_component_copy(struct tnecs_World * in_world, tnecs_entity_t in_entity, tnecs_component_t old_flag, tnecs_component_t new_flag);
void tnecs_component_del(struct tnecs_World * in_world, tnecs_entity_t in_entity, tnecs_component_t old_flag);
bool tnecs_component_migrate(struct tnecs_World * in_world, tnecs_entity_t in_entity, size_t entity_order_new, tnecs_component_t new_flag);

void tnecs_component_array_new(struct tnecs_World * in_world, size_t num_components, tnecs_component_t typeflag);
void tnecs_component_array_init(struct tnecs_World * in_world, struct tnecs_Components_Array * in_array, size_t in_component_id);

void tnecs_system_order_switch(struct tnecs_World * in_world, tnecs_phase_t in_phase_id, size_t order1, size_t order2);

/************************ UTILITY FUNCTIONS/MACROS ***************************/
size_t tnecs_component_name2id(struct tnecs_World * in_world, const tnecs_str_t * in_name);
size_t tnecs_component_hash2id(struct tnecs_World * in_world, tnecs_hash_t in_hash);
size_t tnecs_component_order_bytype(struct tnecs_World * in_world, size_t in_component_id, tnecs_component_t in_typeflag);
size_t tnecs_component_order_bytypeid(struct tnecs_World * in_world, size_t in_component_id, size_t in_typeflag_id);
tnecs_component_t tnecs_component_names2typeflag(struct tnecs_World * in_world, size_t argnum, ...);
tnecs_component_t tnecs_component_ids2typeflag(size_t argnum, ...);
tnecs_component_t tnecs_component_hash2type(struct tnecs_World * in_world, tnecs_hash_t in_hash);

size_t tnecs_system_name2id(struct tnecs_World * in_world, const tnecs_str_t * in_name);
size_t tnecs_system_hash2id(struct tnecs_World * in_world, tnecs_hash_t in_hash);
tnecs_component_t tnecs_system_name2typeflag(struct tnecs_World * in_world, const tnecs_str_t * in_name);

size_t tnecs_typeflagid(struct tnecs_World * in_world, tnecs_component_t in_typeflag);

#define TNECS_COMPONENT_HASH2ID(world, hash) tnecs_component_hash2id(world, hash)
#define TNECS_COMPONENT_HASH2TYPE(world, hash) tnecs_component_hash2type(world, hash)
#define TNECS_COMPONENT_TYPE(world, name) tnecs_component_names2typeflag(world, 1, #name)
#define TNECS_COMPONENT_NAMES2TYPEFLAG(world, ...) tnecs_component_names2typeflag(world, TNECS_VARMACRO_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_STRINGIFY, __VA_ARGS__))
#define TNECS_COMPONENT_IDS2TYPEFLAG(...) tnecs_component_ids2typeflag(TNECS_VARMACRO_EACH_ARGN(__VA_ARGS__), TNECS_VARMACRO_FOREACH_COMMA(TNECS_NULLMACRO, __VA_ARGS__))
#define TNECS_COMPONENT_NAME2ID(world, name) tnecs_component_name2id(world, #name)
#define TNECS_COMPONENT_ID2TYPE(id) (1 << (id - TNECS_NULLSHIFT))
#define TNECS_COMPONENT_TYPE2ID(type) (type >=1 ? (tnecs_component_t)(log2(type) + 1.1f): 0) // casting to int floors
#define TNECS_COMPONENTS_LIST(input, component_name) (struct component_name *) (input->world->components_bytype[input->typeflag_id][input->world->components_orderbytype[input->typeflag_id][tnecs_component_name2id(input->world, #component_name)]].components)

#define TNECS_SYSTEM_ID(world, name) tnecs_system_id(world, #name)
#define TNECS_SYSTEM_ID2TYPEFLAG(world, id) world->system_typeflags[id]
#define TNECS_SYSTEM_TYPEFLAG(world, name) tnecs_system_name2typeflag(world, #name)

#define TNECS_TYPEFLAGID(world, typeflag) tnecs_typeflagid(world, typeflag)

/***************************** "DYNAMIC" ARRAYS ******************************/
void * tnecs_realloc(void * ptr, size_t old_len, size_t new_len, size_t elem_bytesize);
void * tnecs_arrdel(void * arr, size_t elem, size_t len, size_t bytesize);
void * tnecs_arrdel_scramble(void * arr, size_t elem, size_t len, size_t bytesize);

void tnecs_growArray_entity(struct tnecs_World * in_world);
void tnecs_growArray_system(struct tnecs_World * in_world);
void tnecs_growArray_typeflag(struct tnecs_World * in_world);
void tnecs_growArray_phase(struct tnecs_World * in_world);

/****************************** STRING HASHING *******************************/
uint64_t tnecs_hash_djb2(const unsigned char * str); // slightly faster
uint64_t tnecs_hash_sdbm(const unsigned char * str);
#define TNECS_HASH(name) tnecs_hash_djb2(name)

/****************************** SET BIT COUNTING *****************************/
size_t setBits_KnR_uint64_t(uint64_t in_flags);

#ifdef __cplusplus
}
#endif

#endif // __TNECS_H__