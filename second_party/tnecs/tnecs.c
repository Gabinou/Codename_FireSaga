/********************************* C99 ***************************************/
/* Clock is POSIX, not C99 */
#ifndef _POSIX_C_SOURCE
    #define _POSIX_C_SOURCE 199309L
#endif /* _POSIX_C_SOURCE */

#include "tnecs.h"

/************************* PRIVATE DECLARATIONS ******************************/
/* --- WORLD FUNCTIONS --- */
static b32 _tnecs_world_breath_entities(struct tnecs_World     *w);
static b32 _tnecs_world_breath_components(struct tnecs_World   *w);
static b32 _tnecs_world_breath_systems(struct tnecs_World      *w);
static b32 _tnecs_world_breath_typeflags(struct tnecs_World    *w);

/* --- SYSTEM FUNCTIONS --- */
static void _tnecs_system_torun_realloc(struct tnecs_World *world);

/* --- REGISTRATION  --- */
static size_t _tnecs_register_typeflag(struct tnecs_World *w, size_t num_components,
                                       tnecs_component typeflag);

/********************** 0.1 MICROSECOND RESOLUTION CLOCK **********************/
uint64_t tnecs_get_ns() {
    static uint64_t is_init = 0;
    #if defined(__APPLE__)
    static mach_timebase_info_data_t info;
    if (0 == is_init) {
        mach_timebase_info(&info);
        is_init = 1;
    }
    uint64_t now;
    now = mach_absolute_time();
    now *= info.numer;
    now /= info.denom;
    return now;
    #elif defined(__linux)
    static struct timespec linux_rate;
    if (0 == is_init) {
        clock_getres(CLOCKID, &linux_rate);
        is_init = 1;
    }
    uint64_t now;
    struct timespec spec;
    clock_gettime(CLOCKID, &spec);
    now = spec.tv_sec * 1.0e9 + spec.tv_nsec;
    return now;
    #elif defined(_WIN32)
    static LARGE_INTEGER win_frequency;
    if (0 == is_init) {
        QueryPerformanceFrequency(&win_frequency);
        is_init = 1;
    }
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (uint64_t)((1e9 * now.QuadPart) / win_frequency.QuadPart);
    #endif
}
#ifdef MICROSECOND_CLOCK
double tnecs_get_us() {
    return (tnecs_get_ns() / 1e3);
}
#else
#  define FAILSAFE_CLOCK
#  define tnecs_get_us() (((double)clock())/CLOCKS_PER_SEC*1e6) // [us]
#  define tnecs_get_ns() (((double)clock())/CLOCKS_PER_SEC*1e9) // [ns]
#endif

/**************************** WORLD FUNCTIONS ********************************/
struct tnecs_World *tnecs_world_genesis() {
    struct tnecs_World *tnecs_world = calloc(sizeof(struct tnecs_World), 1);
    TNECS_DEBUG_ASSERT(tnecs_world);
    b32 success = 1;
    success &= _tnecs_world_breath_entities(tnecs_world);
    success &= _tnecs_world_breath_typeflags(tnecs_world);
    success &= _tnecs_world_breath_systems(tnecs_world);
    success &= _tnecs_world_breath_components(tnecs_world);
    TNECS_DEBUG_ASSERT(success);
    return (tnecs_world);
}

void tnecs_world_destroy(struct tnecs_World *world) {
    for (size_t i = 0; i < world->len_phases; i++) {
        free(world->systems_byphase[i]);
        free(world->systems_idbyphase[i]);
    }
    for (size_t i = 0; i < world->len_typeflags; i++) {
        free(world->entities_bytype[i]);
        free(world->components_idbytype[i]);
        free(world->components_flagbytype[i]);
        free(world->components_orderbytype[i]);
        free(world->archetype_id_bytype[i]);
        for (size_t j = 0; j < world->num_components_bytype[i]; j++) {
            free(world->components_bytype[i][j].components);
        }
        free(world->components_bytype[i]);
    }
    for (size_t i = 0; i < world->num_components; i++) {
        free(world->component_names[i]);
    }
    for (size_t i = 0; i < world->num_systems; i++) {
        free(world->system_names[i]);
    }
    free(world->components_bytype);
    free(world->components_idbytype);
    free(world->components_flagbytype);
    free(world->components_orderbytype);
    free(world->entities_bytype);
    free(world->entity_orders);
    free(world->entities);
    free(world->entities_open);
    free(world->entity_typeflags);
    free(world->len_entities_bytype);
    free(world->len_systems_byphase);
    free(world->num_entities_bytype);
    free(world->num_systems_byphase);
    free(world->num_archetype_ids);
    free(world->num_components_bytype);
    free(world->phases);
    free(world->archetype_id_bytype);
    free(world->systems_byphase);
    free(world->system_orders);
    free(world->system_exclusive);
    free(world->systems_torun);
    free(world->systems_idbyphase);
    free(world->system_typeflags);
    free(world->system_phases);
    free(world->system_hashes);
    free(world->system_names);
    free(world->typeflags);
    free(world);
}

void tnecs_world_step(tnecs_World *world, tnecs_ns deltat, void *data) {
    world->num_systems_torun = 0;
    for (size_t phase = 0; phase < world->num_phases; phase++)
        tnecs_world_step_phase(world, phase, deltat, data);
}

void tnecs_world_step_phase(tnecs_World *world,  tnecs_phase  phase,
                            tnecs_ns     deltat, void        *data) {
    if (phase != world->phases[phase]) {
        printf("Invalid phase '%d' \n", phase);
        exit(1);        
    }

    for (size_t sorder = 0; sorder < world->num_systems_byphase[phase]; sorder++) {
        size_t system_id = world->systems_idbyphase[phase][sorder];
        tnecs_system_run(world, system_id, deltat, data);
    }
}

b32 _tnecs_world_breath_entities(struct tnecs_World *world) {
    b32 success = 1;
    /* Variables */
    world->entity_next =        TNECS_NULLSHIFT;
    world->len_entities =       TNECS_INITIAL_ENTITY_LEN;
    world->len_entities_open =  TNECS_INITIAL_ENTITY_LEN;
    world->num_entities_open =  0;

    /* Allocs */
    world->entities            = calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(*world->entities));
    world->entities_open       = calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(*world->entities_open));
    world->entity_orders       = calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(*world->entity_orders));
    world->entities_bytype     = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*world->entities_bytype));
    world->entity_typeflags    = calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(*world->entity_typeflags));
    world->len_entities_bytype = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*world->len_entities_bytype));
    world->num_entities_bytype = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*world->num_entities_bytype));

    /* Check allocs (same number as allocs) */
    success &= (world->entities             !=  NULL);
    success &= (world->entities_open        !=  NULL);
    success &= (world->entity_orders        !=  NULL);
    success &= (world->entities_bytype      !=  NULL);
    success &= (world->entity_typeflags     !=  NULL);
    success &= (world->len_entities_bytype  !=  NULL);
    success &= (world->num_entities_bytype  !=  NULL);

    /* Alloc & check for entities_bytype elements */
    for (size_t i = 0; i < TNECS_INITIAL_SYSTEM_LEN; i++) {
        world->entities_bytype[i] =     calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(**world->entities_bytype));
        success &= (world->entities_bytype[i] != NULL);
        world->num_entities_bytype[i] = 0;
        world->len_entities_bytype[i] = TNECS_INITIAL_ENTITY_LEN;
    }
    return (success);
}

b32 _tnecs_world_breath_components(struct tnecs_World *world) {
    /* Variables */
    b32 success = 1;
    world->num_components =               TNECS_NULLSHIFT;
    world->component_hashes[TNECS_NULL] = TNECS_NULL;

    /* Allocs */
    int syslen = TNECS_INITIAL_SYSTEM_LEN, namelen = 5;
    world->components_bytype =            calloc(syslen, sizeof(*world->components_bytype));
    world->num_components_bytype =        calloc(syslen, sizeof(*world->num_components_bytype));
    world->components_idbytype =          calloc(syslen, sizeof(*world->components_idbytype));
    world->components_flagbytype =        calloc(syslen, sizeof(*world->components_flagbytype));
    world->components_orderbytype =       calloc(syslen, sizeof(*world->components_orderbytype));
    world->component_names[TNECS_NULL] =  malloc(namelen);

    /* Check allocs (same number as allocs) */
    success &= (world->components_bytype       !=  NULL);
    success &= (world->num_components_bytype   !=  NULL);
    success &= (world->components_idbytype     !=  NULL);
    success &= (world->components_flagbytype   !=  NULL);
    success &= (world->components_orderbytype  !=  NULL);
    success &= (world->component_names         !=  NULL);

    /* Set name of first component */
    strncpy(world->component_names[TNECS_NULL], "NULL\0", namelen);
    return (success);
}

b32 _tnecs_world_breath_systems(struct tnecs_World *world) {
    /* Variables */
    b32 success = 1;
    world->len_systems = TNECS_INITIAL_SYSTEM_LEN;
    world->len_systems_torun = TNECS_INITIAL_SYSTEM_LEN;
    world->num_systems = TNECS_NULLSHIFT;
    world->len_phases = TNECS_INITIAL_PHASE_LEN;
    world->num_phases = TNECS_NULLSHIFT;

    /* Allocs */
    int namelen = 5;
    world->phases =                   calloc(world->len_phases, sizeof(*world->phases));
    world->system_names =             calloc(world->len_systems, sizeof(*world->system_names));
    world->systems_torun =            calloc(world->len_systems_torun, sizeof(tnecs_system_ptr));
    world->system_hashes =            calloc(world->len_systems, sizeof(*world->system_hashes));
    world->system_phases =            calloc(world->len_systems, sizeof(*world->system_phases));
    world->system_orders =            calloc(world->len_systems, sizeof(*world->system_orders));
    world->systems_byphase =          calloc(world->len_phases, sizeof(*world->systems_byphase));
    world->system_typeflags =         calloc(world->len_systems, sizeof(*world->system_typeflags));
    world->system_exclusive =         calloc(world->len_systems, sizeof(*world->system_exclusive));
    world->systems_idbyphase =        calloc(world->len_phases, sizeof(*world->systems_idbyphase));
    world->len_systems_byphase =      calloc(world->len_phases, sizeof(*world->len_systems_byphase));
    world->num_systems_byphase =      calloc(world->len_phases, sizeof(*world->num_systems_byphase));
    world->system_names[TNECS_NULL] = malloc(namelen);

    /* Check allocs (same number as allocs) */
    success &= (world->phases                   != NULL);
    success &= (world->system_names             != NULL);
    success &= (world->systems_torun            != NULL);
    success &= (world->system_hashes            != NULL);
    success &= (world->system_phases            != NULL);
    success &= (world->system_orders            != NULL);
    success &= (world->systems_byphase          != NULL);
    success &= (world->system_typeflags         != NULL);
    success &= (world->system_exclusive         != NULL);
    success &= (world->systems_idbyphase        != NULL);
    success &= (world->len_systems_byphase      != NULL);
    success &= (world->num_systems_byphase      != NULL);
    success &= (world->system_names[TNECS_NULL] != NULL);

    /* Alloc & check for entities_byphase elements */
    for (size_t i = 0; i < world->len_phases; i++) {
        world->systems_byphase[i] =     calloc(world->len_phases, sizeof(**world->systems_byphase));
        world->systems_idbyphase[i] =   calloc(world->len_phases, sizeof(**world->systems_idbyphase));
        success &= world->systems_byphase[i]    != NULL;
        success &= world->systems_idbyphase[i]  != NULL;
        world->num_systems_byphase[i] = 0;
        world->len_systems_byphase[i] = world->len_phases;
    }

    /* Set name of first system */
    strncpy(world->system_names[TNECS_NULL], "NULL\0", namelen);
    return (success);
}

b32 _tnecs_world_breath_typeflags(struct tnecs_World *world) {

    /* Variables */
    b32 success = 1;
    world->num_typeflags = TNECS_NULLSHIFT;
    world->len_typeflags = TNECS_INITIAL_SYSTEM_LEN;

    /* Allocs */
    world->typeflags =           calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*world->typeflags));
    world->archetype_id_bytype = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*world->archetype_id_bytype));
    world->num_archetype_ids =   calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*world->num_archetype_ids));

    /* Check allocs (same number as allocs) */
    success &= (world->typeflags != NULL);
    success &= (world->archetype_id_bytype != NULL);
    success &= (world->num_archetype_ids != NULL);

    /* Alloc & check for id_bytype elements */
    for (size_t i = 0; i < TNECS_INITIAL_SYSTEM_LEN; i++) {
        world->archetype_id_bytype[i] = calloc(TNECS_COMPONENT_CAP, sizeof(**world->archetype_id_bytype));
        success &= (world->archetype_id_bytype[i] != NULL);
    }
    return (success);
}

/**************************** SYSTEM FUNCTIONS ********************************/
void tnecs_custom_system_run(struct tnecs_World *world, tnecs_system_ptr custom_system,
                             tnecs_component archetype, tnecs_ns deltat, void *data) {
    /* Building the systems input */
    struct tnecs_System_Input input = {.world = world, .deltat = deltat, .data = data};
    size_t tID = tnecs_typeflagid(world, archetype);
    TNECS_DEBUG_ASSERT(tID != TNECS_NULL);

    /* Running the exclusive custom system */
    input.entity_typeflag_id =  tID;
    input.num_entities =        world->num_entities_bytype[input.entity_typeflag_id];
    custom_system(&input);

    /* Running the non-exclusive/inclusive custom system */
    for (size_t tsub = 0; tsub < world->num_archetype_ids[tID]; tsub++) {
        input.entity_typeflag_id =  world->archetype_id_bytype[tID][tsub];
        input.num_entities =        world->num_entities_bytype[input.entity_typeflag_id];
        custom_system(&input);
    }
}

void _tnecs_system_torun_realloc(struct tnecs_World *world) {
    /* Realloc systems_to_run if too many */
    if (world->num_systems_torun >= (world->len_systems_torun - 1)) {
        size_t old_len =            world->len_systems_torun;
        size_t new_len =            old_len * TNECS_ARRAY_GROWTH_FACTOR;
        world->len_systems_torun =  new_len;
        size_t bytesize =           sizeof(*world->systems_torun);

        world->systems_torun = tnecs_realloc(world->systems_torun, old_len, new_len, bytesize);
    }
}

void tnecs_system_run(tnecs_World *world, size_t in_system_id,
                      tnecs_ns     deltat, void *data) {
    /* Building the systems input */
    struct tnecs_System_Input input = {.world = world, .deltat = deltat, .data = data};
    size_t sorder               = world->system_orders[in_system_id];
    tnecs_phase phase        = world->system_phases[in_system_id];
    size_t system_typeflag_id   = tnecs_typeflagid(world, world->system_typeflags[in_system_id]);

    input.entity_typeflag_id    = system_typeflag_id;
    input.num_entities          = world->num_entities_bytype[input.entity_typeflag_id];

    /* Running the exclusive systems in current phase */
    _tnecs_system_torun_realloc(world);
    tnecs_system_ptr system =              world->systems_byphase[phase][sorder];
    int system_num =                       world->num_systems_torun++;
    world->systems_torun[system_num] =     world->systems_byphase[phase][sorder];
    system(&input);

    if (world->system_exclusive[in_system_id])
        return;

    /* Running the inclusive systems in current phase */
    for (size_t tsub = 0; tsub < world->num_archetype_ids[system_typeflag_id]; tsub++) {
        input.entity_typeflag_id =         world->archetype_id_bytype[system_typeflag_id][tsub];
        input.num_entities =               world->num_entities_bytype[input.entity_typeflag_id];

        _tnecs_system_torun_realloc(world);
        tnecs_system_ptr system =          world->systems_byphase[phase][sorder];
        int system_num =                   world->num_systems_torun++;
        world->systems_torun[system_num] = system;
        system(&input);
    }
}

/***************************** REGISTRATION **********************************/
size_t tnecs_register_system(struct tnecs_World *world, const char *name,
                             void (* in_system)(struct tnecs_System_Input *), tnecs_phase phase,
                             b32 isExclusive, size_t num_components, tnecs_component components_typeflag) {
    /* Compute new id */
    size_t system_id = world->num_systems++;

    /* Realloc systems if too many */
    if (world->num_systems >= world->len_systems)
        tnecs_growArray_system(world);

    /* Realloc systems_byphase if too many */
    if (world->num_systems_byphase[phase] >= world->len_systems_byphase[phase]) {
        size_t olen =                        world->len_systems_byphase[phase];
        size_t nlen =                        olen * TNECS_ARRAY_GROWTH_FACTOR;
        world->len_systems_byphase[phase] =  nlen;
        size_t bs =                          sizeof(**world->systems_byphase);
        size_t bsid =                        sizeof(**world->systems_idbyphase);

        tnecs_system_ptr *systems =          world->systems_byphase[phase];
        size_t *system_id =                  world->systems_idbyphase[phase];
        world->systems_byphase[phase] =      tnecs_realloc(systems, olen, nlen, bs);
        world->systems_idbyphase[phase] =    tnecs_realloc(system_id, olen, nlen, bsid);
    }

    /* -- Actual registration -- */
    /* Saving name and hash */
    world->system_names[system_id] =    malloc(strlen(name) + 1);
    tnecs_hash hash =                 tnecs_hash_djb2(name);
    strncpy(world->system_names[system_id], name, strlen(name) + 1);

    /* Register new phase if didn't exist */
    if (!world->phases[phase])
        tnecs_register_phase(world, phase);

    world->system_exclusive[system_id] =   isExclusive;
    world->system_phases[system_id] =      phase;
    world->system_hashes[system_id] =      hash;
    world->system_typeflags[system_id] =   components_typeflag;

    /* System order */
    size_t system_order =                               world->num_systems_byphase[phase]++;
    world->system_orders[system_id] =                   system_order;
    world->systems_byphase[phase][system_order] =    in_system;
    world->systems_idbyphase[phase][system_order] =  system_id;
    _tnecs_register_typeflag(world, num_components, components_typeflag);
    return (system_id);
}

tnecs_component tnecs_register_component(struct tnecs_World *world,
                                         const char *name,
                                         size_t bytesize) {
    /* Checks */
    TNECS_DEBUG_ASSERT(world->num_components < TNECS_COMPONENT_CAP);
    TNECS_DEBUG_ASSERT(bytesize > 0);

    /* Registering */
    tnecs_component new_component_id =            world->num_components++;
    world->component_hashes[new_component_id] =     tnecs_hash_djb2(name);
    tnecs_component new_component_flag =          TNECS_COMPONENT_ID2TYPE(new_component_id);
    world->component_bytesizes[new_component_id] =  bytesize;

    /* Setting component name */
    world->component_names[new_component_id] =      malloc(strlen(name) + 1);
    strncpy(world->component_names[new_component_id], name, strlen(name) + 1);
    _tnecs_register_typeflag(world, 1, new_component_flag);
    return (new_component_id);
}

size_t _tnecs_register_typeflag(struct tnecs_World *world, size_t num_components,
                                tnecs_component typeflag_new) {

    // 0- Check if typeflag exists, return
    size_t tID = 0;
    for (size_t i = 0 ; i < world->num_typeflags; i++) {
        if (typeflag_new == world->typeflags[i]) {
            tID = i;
            break;
        }
    }
    if (tID)
        return (tID);

    // 1- Add new components_bytype at [tID]
    if ((world->num_typeflags + 1) >= world->len_typeflags)
        tnecs_growArray_typeflag(world);
    world->typeflags[world->num_typeflags++] = typeflag_new;
    tID = tnecs_typeflagid(world, typeflag_new);
    TNECS_DEBUG_ASSERT(tID == (world->num_typeflags - 1));
    world->num_components_bytype[tID] = num_components;

    // 2- Add arrays to components_bytype[tID] for each component
    tnecs_component_array_new(world, num_components, typeflag_new);

    // 3- Add all components to components_idbytype and components_flagbytype
    tnecs_component component_id_toadd, component_type_toadd;
    tnecs_component typeflag_reduced = typeflag_new, typeflag_added = 0;
    size_t bytesize1 =  sizeof(**world->components_idbytype);
    size_t bytesize2 =  sizeof(**world->components_flagbytype);
    size_t bytesize3 =  sizeof(**world->components_orderbytype);
    world->components_idbytype[tID] =     calloc(num_components,      bytesize1);
    world->components_flagbytype[tID] =   calloc(num_components,      bytesize2);
    world->components_orderbytype[tID] =  calloc(TNECS_COMPONENT_CAP, bytesize3);
    size_t i = 0;
    while (typeflag_reduced) {
        typeflag_reduced &= (typeflag_reduced - 1);

        component_type_toadd = (typeflag_reduced + typeflag_added) ^ typeflag_new;
        typeflag_added +=      component_type_toadd;
        component_id_toadd =   TNECS_COMPONENT_TYPE2ID(component_type_toadd);

        world->components_idbytype[tID][i] =   component_id_toadd;
        world->components_flagbytype[tID][i] = component_type_toadd;

        world->components_orderbytype[tID][component_id_toadd] = i++;
    }

    // 4- Check archetypes.
    for (size_t i = 1 ; i < world->num_typeflags; i++) {
        world->num_archetype_ids[i] = 0;
        for (size_t j = 1 ; j < (world->num_typeflags); j++) {
            if (i == j)
                continue;

            if (!TNECS_TYPEFLAG_IS_ARCHETYPE(world->typeflags[i], world->typeflags[j]))
                continue;

            // j is an archetype of i
            world->archetype_id_bytype[i][world->num_archetype_ids[i]++] = j;
        }
    }

    return (tID);
}

size_t tnecs_register_phase(struct tnecs_World *world, tnecs_phase phase) {
    while (phase >= world->len_phases)
        tnecs_growArray_phase(world);
    world->phases[phase] = phase;
    world->num_phases = (phase >= world->num_phases) ? (phase + 1) : world->num_phases;
    return (phase);
}

/***************************** ENTITY MANIPULATION ***************************/
tnecs_entity tnecs_entity_create(struct tnecs_World *world) {
    tnecs_entity out = TNECS_NULL;
    
    if (world->reuse_entities) {
        /* Check if an open entity exists */
        while ((out == TNECS_NULL) && (world->num_entities_open > 0)) {
            out = world->entities_open[--world->num_entities_open];
            world->entities_open[world->num_entities_open] = TNECS_NULL;
        }
    }

    /* If no open entity existed, create one */
    if (out == TNECS_NULL) {
        do {
            if (world->entity_next >= world->len_entities)
                tnecs_growArray_entity(world);
        } while (world->entities[out = world->entity_next++] != TNECS_NULL);
    }
    TNECS_DEBUG_ASSERT(out != TNECS_NULL);

    /* Set entity and checks  */
    world->entities[out] = out;
    tnecs_entitiesbytype_add(world, out, TNECS_NULL);
    TNECS_DEBUG_ASSERT(world->entities[out] == out);
    TNECS_DEBUG_ASSERT(world->entities_bytype[TNECS_NULL][world->entity_orders[out]] == out);
    return (out);
}

tnecs_entity tnecs_entity_create_wID(struct tnecs_World *world, tnecs_entity entity) {
    // TODO: What to do if entity existed before?
    tnecs_entity out = 0;
    while (entity >= world->len_entities)
        tnecs_growArray_entity(world);

    if ((!world->entities[entity]) & (entity > 0)) {
        out = world->entities[entity] = entity;
        tnecs_entitiesbytype_add(world, out, TNECS_NULL);
    }
    return (out);
}

tnecs_entity tnecs_entities_create(struct tnecs_World *world, size_t num) {
    for (int i = 0; i < num; i++)
        TNECS_DEBUG_ASSERT(tnecs_entity_create(world) > 0);
    return (num);
}

tnecs_entity tnecs_entities_create_wID(struct tnecs_World *world, size_t num,
                                       tnecs_entity *ents) {
    for (int i = 0; i < num; i++)
        TNECS_DEBUG_ASSERT(tnecs_entity_create_wID(world, ents[i]) > 0);
    return (num);
}

tnecs_entity tnecs_entity_create_wcomponents(struct tnecs_World *world, size_t argnum, ...) {
    /* Get typeflag of all vararg components */
    va_list ap;
    va_start(ap, argnum);
    tnecs_component typeflag = 0;
    for (size_t i = 0; i < argnum; i++) {
        tnecs_hash hash =  va_arg(ap, tnecs_hash);
        typeflag +=          tnecs_component_hash2type(world, hash);
    }
    va_end(ap);

    /* Create entity with all components */
    tnecs_entity new_entity = tnecs_entity_create(world);
    tnecs_entity_add_components(world, new_entity, argnum, typeflag, true);

    /* Check */
    size_t tID =       TNECS_TYPEFLAGID(world, typeflag);
    size_t order =     world->entity_orders[new_entity];
    TNECS_DEBUG_ASSERT(world->entities_bytype[tID][order] == new_entity);
    TNECS_DEBUG_ASSERT(world->entities[new_entity] == new_entity);
    return (new_entity);
}

tnecs_entity tnecs_entity_destroy(struct tnecs_World *world, tnecs_entity entity) {
    TNECS_DEBUG_ASSERT(world->entities[entity]);
    TNECS_DEBUG_ASSERT(entity > TNECS_NULL);
    /* Preliminaries */
    tnecs_component typeflag =  world->entity_typeflags[entity];
    size_t tID =                  TNECS_TYPEFLAGID(world, typeflag);
    size_t entity_order =         world->entity_orders[entity];
    TNECS_DEBUG_ASSERT(world->num_entities_bytype[tID] > TNECS_NULL);
    /* Delete components */
    tnecs_component_del(world, entity, typeflag);
    TNECS_DEBUG_ASSERT(world->len_entities_bytype[tID] >= entity_order);
    TNECS_DEBUG_ASSERT(world->num_entities_bytype[tID] > TNECS_NULL);
    /* Delete entitiesbytype */
    tnecs_entitiesbytype_del(world, entity, typeflag);

    /* Realloc entities_open if too many */
    world->entities[entity] =          TNECS_NULL;
    world->entity_orders[entity] =     TNECS_NULL;
    world->entity_typeflags[entity] =  TNECS_NULL;
    if ((world->num_entities_open + 1) >= world->len_entities_open) {
        size_t old_len =            world->len_entities_open;
        size_t new_len =            old_len * TNECS_ARRAY_GROWTH_FACTOR;
        size_t bytesize =           sizeof(*world->entities_open);
        world->len_entities_open =  new_len;

        world->entities_open = tnecs_realloc(world->entities_open, old_len, new_len, bytesize);
    }

    /* Add deleted entity to open entities */
    world->entities_open[world->num_entities_open++] = entity;
    TNECS_DEBUG_ASSERT(world->entities[entity]            == TNECS_NULL);
    TNECS_DEBUG_ASSERT(world->entity_typeflags[entity]    == TNECS_NULL);
    TNECS_DEBUG_ASSERT(world->entity_orders[entity]       == TNECS_NULL);
    TNECS_DEBUG_ASSERT(world->entity_orders[entity_order] != entity);
    return (world->entities[entity] == TNECS_NULL);
}

/*****************************************************************************/
/***************************** TNECS INTERNALS *******************************/
/*****************************************************************************/
tnecs_entity tnecs_entity_add_components(struct tnecs_World *world, tnecs_entity entity,
                                         size_t num_components_toadd, tnecs_component typeflag_toadd, b32 isNew) {

    TNECS_DEBUG_ASSERT(num_components_toadd > 0);
    TNECS_DEBUG_ASSERT(typeflag_toadd > 0);
    tnecs_component typeflag_old = world->entity_typeflags[entity];
    TNECS_DEBUG_ASSERT(!(typeflag_toadd & typeflag_old));
    tnecs_component typeflag_new = typeflag_toadd + typeflag_old;
    TNECS_DEBUG_ASSERT(typeflag_new != typeflag_old);
    if (isNew)
        _tnecs_register_typeflag(world, setBits_KnR_uint64_t(typeflag_new), typeflag_new);

    size_t tID_new = tnecs_typeflagid(world, typeflag_new);

    tnecs_component_migrate(world,      entity, typeflag_old, typeflag_new);
    tnecs_entitiesbytype_migrate(world, entity, typeflag_old, typeflag_new);

    size_t new_order = world->num_entities_bytype[tID_new] - 1;
    TNECS_DEBUG_ASSERT(world->entity_typeflags[entity]                    == typeflag_new);
    TNECS_DEBUG_ASSERT(world->entities_bytype[tID_new][new_order] == entity);
    TNECS_DEBUG_ASSERT(world->entity_orders[entity]                       == new_order);
    return (world->entities[entity]);
}

void tnecs_entity_remove_components(struct tnecs_World *world, tnecs_entity entity,
                                    size_t num_components, tnecs_component typeflag) {
    /* Get new typeflag. Since it is a typeflag, just need to substract. */
    tnecs_component typeflag_old = world->entity_typeflags[entity];
    tnecs_component typeflag_new = typeflag_old - typeflag;

    if (typeflag_new != TNECS_NULL) {
        /* Migrate remaining components to new typeflag array. */
        _tnecs_register_typeflag(world, setBits_KnR_uint64_t(typeflag_new), typeflag_new);
        tnecs_component_migrate(world, entity, typeflag_old, typeflag_new);
    } else {
        /* No remaining component, delete everything. */
        tnecs_component_del(world, entity, typeflag_old);
    }
    /* Migrate entity to new bytype array. */
    tnecs_entitiesbytype_migrate(world, entity, typeflag_old, typeflag_new);
    TNECS_DEBUG_ASSERT(typeflag_new == world->entity_typeflags[entity]);
}


void *tnecs_entity_get_component(struct tnecs_World *world, tnecs_entity eID,
                                 tnecs_component cID) {

    tnecs_component component_flag =   TNECS_COMPONENT_ID2TYPE(cID);
    tnecs_component entity_typeflag =  TNECS_ENTITY_TYPEFLAG(world, eID);
    void *out = NULL;
    // If entity has component, get output it. If not output NULL.
    if ((component_flag & entity_typeflag) == 0)
        return (out);

    size_t tID = tnecs_typeflagid(world, entity_typeflag);
    size_t component_order = tnecs_component_order_bytype(world, cID, entity_typeflag);
    TNECS_DEBUG_ASSERT(component_order <= world->num_components_bytype[tID]);
    size_t entity_order = world->entity_orders[eID];
    size_t bytesize = world->component_bytesizes[cID];
    struct tnecs_Components_Array *comp_array;
    comp_array = &world->components_bytype[tID][component_order];
    tnecs_byte *temp_component_bytesptr = (tnecs_byte *)(comp_array->components);
    out = (temp_component_bytesptr + (bytesize * entity_order));
    return (out);
}

size_t tnecs_entitiesbytype_add(struct tnecs_World *world, tnecs_entity entity,
                                tnecs_component typeflag_new) {

    size_t tID_new = tnecs_typeflagid(world, typeflag_new);
    if ((world->num_entities_bytype[tID_new] + 1) >= world->len_entities_bytype[tID_new]) {
        tnecs_growArray_bytype(world, tID_new);
    }
    int new_order =                               world->num_entities_bytype[tID_new]++;
    world->entity_orders[entity] =                new_order;
    world->entity_typeflags[entity] =             typeflag_new;
    world->entities_bytype[tID_new][new_order] =  entity;
    return (world->entity_orders[entity]);
}

size_t tnecs_entitiesbytype_del(struct tnecs_World *world, tnecs_entity entity,
                                tnecs_component typeflag_old) {

    TNECS_DEBUG_ASSERT(entity);
    TNECS_DEBUG_ASSERT(world->entities[entity] == entity);
    TNECS_DEBUG_ASSERT(entity < world->len_entities);
    size_t typeflag_old_id = tnecs_typeflagid(world, typeflag_old);
    size_t old_num = world->num_entities_bytype[typeflag_old_id];
    size_t entity_order_old = world->entity_orders[entity];
    TNECS_DEBUG_ASSERT(old_num > 0);
    TNECS_DEBUG_ASSERT(entity_order_old < world->len_entities_bytype[typeflag_old_id]);
    TNECS_DEBUG_ASSERT(world->entities_bytype[typeflag_old_id][entity_order_old] == entity);

    tnecs_entity top_entity = world->entities_bytype[typeflag_old_id][old_num - 1];

    /* components scrambles -> entitiesbytype too */
    tnecs_arrdel_scramble(world->entities_bytype[typeflag_old_id], entity_order_old, old_num,
                          sizeof(**world->entities_bytype));

    if (top_entity != entity) {
        world->entity_orders[top_entity] = entity_order_old;
        TNECS_DEBUG_ASSERT(world->entities_bytype[typeflag_old_id][entity_order_old] == top_entity);
    }

    world->entity_orders[entity]    = TNECS_NULL;
    world->entity_typeflags[entity] = TNECS_NULL;

    --world->num_entities_bytype[typeflag_old_id];
    return (true);
}

size_t tnecs_entitiesbytype_migrate(struct tnecs_World *world, tnecs_entity entity,
                                    tnecs_component typeflag_old, tnecs_component typeflag_new) {
    /* Migrate entities into correct bytype array */
    tnecs_entitiesbytype_del(world, entity, typeflag_old);
    TNECS_DEBUG_ASSERT(world->entity_typeflags[entity]  == TNECS_NULL);
    TNECS_DEBUG_ASSERT(world->entity_orders[entity]     == TNECS_NULL);
    tnecs_entitiesbytype_add(world, entity, typeflag_new);

    /* Checks */
    size_t tID_new = tnecs_typeflagid(world, typeflag_new);
    size_t order_new = world->entity_orders[entity];
    TNECS_DEBUG_ASSERT(world->entity_typeflags[entity]            == typeflag_new);
    TNECS_DEBUG_ASSERT(world->num_entities_bytype[tID_new] - 1    == order_new);
    TNECS_DEBUG_ASSERT(world->entities_bytype[tID_new][order_new] == entity);
    return (world->entity_orders[entity]);
}

b32 tnecs_component_add(struct tnecs_World *world, tnecs_component typeflag) {
    /* Check if need to grow component array after adding new component */
    size_t tID = tnecs_typeflagid(world, typeflag);
    size_t new_comp_num = world->num_components_bytype[tID];
    size_t new_order = world->num_entities_bytype[tID];
    struct tnecs_Components_Array *current_array;

    b32 togrow = 0, success = 1;
    for (size_t corder = 0; corder < new_comp_num; corder++) {
        // Take component array of current typeflag_id
        current_array = &world->components_bytype[tID][corder];
        // check if it need to grow after adding new component
        TNECS_DEBUG_ASSERT(new_order == current_array->num_components);
        togrow |= (++current_array->num_components >= current_array->len_components);
    }
    if (togrow)
        success &= tnecs_growArray_bytype(world, tID);
    return (success);

}

b32 tnecs_component_copy(struct tnecs_World *world, tnecs_entity entity,
                          tnecs_component old_typeflag, tnecs_component new_typeflag) {
    /* Copy components from old order unto top of new type component array */

    TNECS_DEBUG_ASSERT(old_typeflag != TNECS_NULL);
    b32 success = 1;
    size_t old_tID =            tnecs_typeflagid(world, old_typeflag);
    size_t new_tID =            tnecs_typeflagid(world, new_typeflag);
    size_t old_entity_order =   world->entity_orders[entity];
    size_t new_entity_order =   world->num_entities_bytype[new_tID];

    int num_comp_new =          world->num_components_bytype[new_tID];
    int num_comp_old =          world->num_components_bytype[old_tID];

    for (int i = 0; i < num_comp_new; ++i) {
        int num = world->components_bytype[new_tID][i].num_components;
        TNECS_DEBUG_ASSERT((num - 1) == new_entity_order);
    }
    TNECS_DEBUG_ASSERT(old_typeflag != TNECS_NULL);
    TNECS_DEBUG_ASSERT(old_typeflag != new_typeflag);
    size_t old_component_id, new_component_id, component_bytesize;
    struct tnecs_Components_Array *old_array,  *new_array;
    tnecs_byte *old_component_ptr,        *new_component_ptr;
    tnecs_byte *old_component_bytesptr,   *new_component_bytesptr;
    for (size_t old_corder = 0; old_corder < num_comp_old; old_corder++) {
        old_component_id = world->components_idbytype[old_tID][old_corder];
        for (size_t new_corder = 0; new_corder < num_comp_new; new_corder++) {
            new_component_id = world->components_idbytype[new_tID][new_corder];
            if (old_component_id != new_component_id)
                continue;

            TNECS_DEBUG_ASSERT(old_component_id == new_component_id);
            new_array = &world->components_bytype[new_tID][new_corder];
            old_array = &world->components_bytype[old_tID][old_corder];
            TNECS_DEBUG_ASSERT(old_array->type == new_array->type);
            TNECS_DEBUG_ASSERT(old_array != new_array);
            component_bytesize = world->component_bytesizes[old_component_id];
            TNECS_DEBUG_ASSERT(component_bytesize > 0);
            old_component_bytesptr = (tnecs_byte *)(old_array->components);
            success &= (old_component_bytesptr != NULL);
            old_component_ptr = (old_component_bytesptr + (component_bytesize * old_entity_order));
            success &= (old_component_ptr != NULL);
            new_component_bytesptr = (tnecs_byte *)(new_array->components);
            success &= (new_component_bytesptr != NULL);
            new_component_ptr = (new_component_bytesptr + (component_bytesize * new_entity_order));
            success &= (new_component_ptr != NULL);
            success &= (new_component_ptr != old_component_ptr);
            void *out =  memcpy(new_component_ptr, old_component_ptr, component_bytesize);
            TNECS_DEBUG_ASSERT(out == new_component_ptr);
            break;
        }
    }
    return (success);
}

b32 tnecs_component_del(struct tnecs_World *world, tnecs_entity entity,
                         tnecs_component old_typeflag) {
    /* Delete ALL components from componentsbytype at old entity order */
    b32 success = 1;
    size_t old_tID      = tnecs_typeflagid(world, old_typeflag);
    size_t order_old    = world->entity_orders[entity];
    size_t old_comp_num = world->num_components_bytype[old_tID];
    for (size_t corder = 0; corder < old_comp_num; corder++) {
        size_t current_component_id = world->components_idbytype[old_tID][corder];
        tnecs_component_array *old_array = &world->components_bytype[old_tID][corder];
        tnecs_byte *comp_ptr =             old_array->components;
        TNECS_DEBUG_ASSERT(comp_ptr != NULL);

        /* Scramble components too */
        size_t comp_by =              world->component_bytesizes[current_component_id];
        size_t new_comp_num =         world->num_entities_bytype[old_tID];
        tnecs_byte *scramble = tnecs_arrdel_scramble(comp_ptr, order_old, new_comp_num, comp_by);
        success &= (comp_ptr == scramble);
        old_array->num_components--;
    }
    return (success);
}

b32 tnecs_component_migrate(struct tnecs_World *world, tnecs_entity entity,
                             tnecs_component old_typeflag, tnecs_component new_typeflag) {
    TNECS_DEBUG_ASSERT(old_typeflag == world->entity_typeflags[entity]);
    b32 success = 1;
    success     &= tnecs_component_add(world,  new_typeflag);
    if (old_typeflag > TNECS_NULL) {
        success &= tnecs_component_copy(world, entity, old_typeflag, new_typeflag);
        success &= tnecs_component_del(world, entity, old_typeflag);
    }
    return (success);
}

b32 tnecs_component_array_new(struct tnecs_World *world, size_t num_components,
                               tnecs_component typeflag) {
    struct tnecs_Components_Array *temp_comparray;
    temp_comparray = calloc(num_components, sizeof(struct tnecs_Components_Array));

    tnecs_component typeflag_reduced = typeflag, typeflag_added = 0, type_toadd;
    tnecs_component tID = tnecs_typeflagid(world, typeflag);
    size_t id_toadd, num_flags = 0;

    while (typeflag_reduced) {
        typeflag_reduced &= (typeflag_reduced - 1);
        type_toadd = (typeflag_reduced + typeflag_added) ^ typeflag;
        id_toadd = TNECS_COMPONENT_TYPE2ID(type_toadd);
        TNECS_DEBUG_ASSERT(id_toadd > 0);
        TNECS_DEBUG_ASSERT(id_toadd < world->num_components);
        tnecs_component_array_init(world, &temp_comparray[num_flags], id_toadd);
        num_flags++;
        typeflag_added += type_toadd;
    }
    world->components_bytype[tID] = temp_comparray;
    TNECS_DEBUG_ASSERT(id_toadd < world->num_components);
    return ((typeflag_added == typeflag) && (num_flags == num_components));
}

void tnecs_component_array_init(struct tnecs_World *world, struct tnecs_Components_Array *in_array,
                                size_t cID) {
    TNECS_DEBUG_ASSERT(cID > 0);
    TNECS_DEBUG_ASSERT(cID < world->num_components);
    tnecs_component in_type = TNECS_COMPONENT_ID2TYPE(cID);
    TNECS_DEBUG_ASSERT(in_type <= (1 << world->num_components));
    size_t bytesize = world->component_bytesizes[cID];
    TNECS_DEBUG_ASSERT(bytesize > 0);

    in_array->type = in_type;
    in_array->num_components = 0;
    in_array->len_components = TNECS_INITIAL_ENTITY_LEN;
    in_array->components = calloc(TNECS_INITIAL_ENTITY_LEN, bytesize);
}

b32 tnecs_system_order_switch(struct tnecs_World *world, tnecs_phase phase,
                               size_t order1, size_t order2) {
    void (* systems_temp)(struct tnecs_System_Input *);
    TNECS_DEBUG_ASSERT(world->num_phases > phase);
    TNECS_DEBUG_ASSERT(world->phases[phase]);
    TNECS_DEBUG_ASSERT(world->num_systems_byphase[phase] > order1);
    TNECS_DEBUG_ASSERT(world->num_systems_byphase[phase] > order2);
    TNECS_DEBUG_ASSERT(world->systems_byphase[phase][order1]);
    TNECS_DEBUG_ASSERT(world->systems_byphase[phase][order2]);
    systems_temp =                             world->systems_byphase[phase][order1];
    world->systems_byphase[phase][order1] = world->systems_byphase[phase][order2];
    world->systems_byphase[phase][order2] = systems_temp;
    b32 out1 = (world->systems_byphase[phase][order1] != NULL);
    b32 out2 = (world->systems_byphase[phase][order2] != NULL);
    return (out1 && out2);
}

/************************ UTILITY FUNCTIONS/MACROS ***************************/
size_t tnecs_component_name2id(struct tnecs_World *world,
                               const char *name) {
    return (tnecs_component_hash2id(world, tnecs_hash_djb2(name)));
}

size_t tnecs_component_hash2id(struct tnecs_World *world, tnecs_hash hash) {
    size_t out;
    for (size_t i = 0; i < world->num_components; i++) {
        if (world->component_hashes[i] == hash) {
            out = i;
            break;
        }
    }
    return (out);
}

size_t tnecs_component_order_bytype(struct tnecs_World *world, size_t cID, tnecs_component flag) {
    tnecs_component tID = tnecs_typeflagid(world, flag);
    return (tnecs_component_order_bytypeid(world, cID, tID));
}

size_t tnecs_component_order_bytypeid(struct tnecs_World *world, size_t cID, size_t tID) {
    size_t order = TNECS_COMPONENT_CAP;
    for (size_t i = 0; i < world->num_components_bytype[tID]; i++) {
        if (world->components_idbytype[tID][i] == cID) {
            order = i;
            break;
        }
    }
    return (order);
}

tnecs_component tnecs_component_names2typeflag(struct tnecs_World *world, size_t argnum, ...) {
    va_list ap;
    tnecs_component typeflag = 0;
    va_start(ap, argnum);
    for (size_t i = 0; i < argnum; i++) {
        typeflag += world->typeflags[tnecs_component_name2id(world, va_arg(ap, const char *))];
    }
    va_end(ap);
    return (typeflag);
}

void tnecs_component_names_print(struct tnecs_World *world, tnecs_entity entity) {
    tnecs_component typeflag = world->entity_typeflags[entity];
    size_t tID               = tnecs_typeflagid(world, typeflag);
    size_t comp_num          = world->num_components_bytype[tID];
    printf("Entity %d: ", entity);
    for (size_t corder = 0; corder < comp_num; corder++) {
        size_t component_id = world->components_idbytype[tID][corder];
        printf("%s, ", world->component_names[component_id]);
    }
    printf("\n");
}


tnecs_component tnecs_component_ids2typeflag(size_t argnum, ...) {
    tnecs_component out = 0;
    va_list ap;
    va_start(ap, argnum);
    for (size_t i = 0; i < argnum; i++)
        out += TNECS_COMPONENT_ID2TYPE(va_arg(ap, size_t));
    va_end(ap);
    return (out);
}

tnecs_component tnecs_component_hash2type(struct tnecs_World *world, tnecs_hash hash) {
    return (TNECS_COMPONENT_ID2TYPE(tnecs_component_hash2id(world, hash)));
}

size_t tnecs_system_name2id(struct tnecs_World *world, const char *name) {
    tnecs_hash hash = tnecs_hash_djb2(name);
    size_t found = 0;
    for (size_t i = 0; i < world->num_systems; i++) {
        if (world->system_hashes[i] == hash) {
            found = i;
            break;
        }
    }
    return (found);
}

tnecs_component tnecs_system_name2typeflag(struct tnecs_World *world,
                                           const char *name) {
    size_t id = tnecs_system_name2id(world, name);
    return (world->system_typeflags[id]);
}

size_t tnecs_typeflagid(struct tnecs_World *world, tnecs_component typeflag) {
    size_t id = 0;
    for (size_t i = 0; i < world->num_typeflags; i++) {
        if (typeflag == world->typeflags[i]) {
            id = i;
            break;
        }
    }
    return (id);
}

/***************************** "DYNAMIC" ARRAYS ******************************/
void *tnecs_realloc(void *ptr, size_t old_len, size_t new_len, size_t elem_bytesize) {
    TNECS_DEBUG_ASSERT(ptr);
    TNECS_DEBUG_ASSERT(new_len > old_len);
    assert(new_len > old_len);
    void *temp = (void *)calloc(new_len, elem_bytesize);
    TNECS_DEBUG_ASSERT(temp);
    memcpy(temp, ptr, old_len * elem_bytesize);
    free(ptr);
    return (temp);
}

void *tnecs_arrdel(void *arr, size_t elem, size_t len, size_t bytesize) {
    void *out;
    tnecs_byte *bytes = arr;
    if (elem < (len - 1)) {
        tnecs_byte *dst = bytes + (elem * bytesize);
        tnecs_byte *src = bytes + ((elem + 1) * bytesize);
        out = memmove(dst, src, bytesize * (len - elem - 1));
    } else
        out = memset(bytes + (elem * bytesize), TNECS_NULL, bytesize);
    return (out);
}

void *tnecs_arrdel_scramble(void *arr, size_t elem, size_t len, size_t bytesize) {
    tnecs_byte *bytes = arr;
    if (elem != (len - 1))
        memmove(bytes + (elem * bytesize), bytes + ((len - 1) * bytesize), bytesize);

    memset(bytes + ((len - 1) * bytesize), TNECS_NULL, bytesize);
    return (arr);
}

b32 tnecs_growArray_entity(struct tnecs_World *world) {
    size_t olen = world->len_entities;
    size_t nlen = world->len_entities * TNECS_ARRAY_GROWTH_FACTOR;
    world->len_entities = nlen;

    size_t bytesize1 = sizeof(*world->entities);
    size_t bytesize2 = sizeof(*world->entity_orders);
    size_t bytesize3 = sizeof(*world->entity_typeflags);

    world->entities =          tnecs_realloc(world->entities,         olen, nlen, bytesize1);
    world->entity_orders =     tnecs_realloc(world->entity_orders,    olen, nlen, bytesize2);
    world->entity_typeflags =  tnecs_realloc(world->entity_typeflags, olen, nlen, bytesize3);

    b32 success = 1;
    success &= (world->entity_typeflags  != NULL);
    success &= (world->entity_orders     != NULL);
    success &= (world->entities          != NULL);
    return (success);
}

b32 tnecs_growArray_system(struct tnecs_World *world) {
    size_t olen = world->len_systems;
    size_t olen_torun = world->len_systems_torun;
    size_t nlen = olen * TNECS_ARRAY_GROWTH_FACTOR;
    size_t nlen_torun = olen_torun * TNECS_ARRAY_GROWTH_FACTOR;
    TNECS_DEBUG_ASSERT(olen > 0);
    world->len_systems = nlen;
    world->len_systems_torun = nlen_torun;

    size_t bytesize1 = sizeof(*world->systems_torun);
    size_t bytesize2 = sizeof(*world->system_names);
    size_t bytesize3 = sizeof(*world->system_phases);
    size_t bytesize4 = sizeof(*world->system_orders);
    size_t bytesize5 = sizeof(*world->system_hashes);
    size_t bytesize6 = sizeof(*world->system_exclusive);
    size_t bytesize7 = sizeof(*world->system_typeflags);

    world->systems_torun =    tnecs_realloc(world->systems_torun,    olen_torun, nlen_torun, bytesize1);
    world->system_names =     tnecs_realloc(world->system_names,     olen, nlen, bytesize2);
    world->system_phases =    tnecs_realloc(world->system_phases,    olen, nlen, bytesize3);
    world->system_orders =    tnecs_realloc(world->system_orders,    olen, nlen, bytesize4);
    world->system_hashes =    tnecs_realloc(world->system_hashes,    olen, nlen, bytesize5);
    world->system_exclusive = tnecs_realloc(world->system_exclusive, olen, nlen, bytesize6);
    world->system_typeflags = tnecs_realloc(world->system_typeflags, olen, nlen, bytesize7);

    b32 success = 1;
    success &= (world->system_names      != NULL);
    success &= (world->systems_torun     != NULL);
    success &= (world->system_phases     != NULL);
    success &= (world->system_orders     != NULL);
    success &= (world->system_hashes     != NULL);
    success &= (world->system_exclusive  != NULL);
    success &= (world->system_typeflags  != NULL);

    return (success);
}

b32 tnecs_growArray_typeflag(struct tnecs_World *world) {
    size_t olen = world->len_typeflags;
    size_t nlen = olen * TNECS_ARRAY_GROWTH_FACTOR;
    world->len_typeflags = nlen;

    size_t bsize1 =  sizeof(*world->typeflags);
    size_t bsize2 =  sizeof(*world->entities_bytype);
    size_t bsize3 =  sizeof(*world->components_bytype);
    size_t bsize4 =  sizeof(*world->num_archetype_ids);
    size_t bsize5 =  sizeof(*world->num_entities_bytype);
    size_t bsize6 =  sizeof(*world->len_entities_bytype);
    size_t bsize7 =  sizeof(*world->components_idbytype);
    size_t bsize8 =  sizeof(*world->archetype_id_bytype);
    size_t bsize9 =  sizeof(*world->num_components_bytype);
    size_t bsize10 = sizeof(*world->components_flagbytype);
    size_t bsize11 = sizeof(*world->components_orderbytype);

    world->typeflags =               tnecs_realloc(world->typeflags,              olen, nlen, bsize1);
    world->entities_bytype =         tnecs_realloc(world->entities_bytype,        olen, nlen, bsize2);
    world->components_bytype =       tnecs_realloc(world->components_bytype,      olen, nlen, bsize3);
    world->num_archetype_ids =       tnecs_realloc(world->num_archetype_ids,      olen, nlen, bsize4);
    world->num_entities_bytype =     tnecs_realloc(world->num_entities_bytype,    olen, nlen, bsize5);
    world->len_entities_bytype =     tnecs_realloc(world->len_entities_bytype,    olen, nlen, bsize6);
    world->components_idbytype =     tnecs_realloc(world->components_idbytype,    olen, nlen, bsize7);
    world->archetype_id_bytype =     tnecs_realloc(world->archetype_id_bytype,    olen, nlen, bsize8);
    world->num_components_bytype =   tnecs_realloc(world->num_components_bytype,  olen, nlen, bsize9);
    world->components_flagbytype =   tnecs_realloc(world->components_flagbytype,  olen, nlen, bsize10);
    world->components_orderbytype =  tnecs_realloc(world->components_orderbytype, olen, nlen, bsize11);

    b32 success = 1;
    success &= (world->typeflags               != NULL);
    success &= (world->components_bytype       != NULL);
    success &= (world->num_components_bytype   != NULL);
    success &= (world->entities_bytype         != NULL);
    success &= (world->num_entities_bytype     != NULL);
    success &= (world->num_archetype_ids       != NULL);
    success &= (world->len_entities_bytype     != NULL);
    success &= (world->components_idbytype     != NULL);
    success &= (world->components_flagbytype   != NULL);
    success &= (world->components_orderbytype  != NULL);
    success &= (world->archetype_id_bytype     != NULL);


    for (size_t i = olen; i < world->len_typeflags; i++) {
        world->entities_bytype[i] = calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(**world->entities_bytype));
        world->archetype_id_bytype[i] = calloc(TNECS_COMPONENT_CAP, sizeof(**world->archetype_id_bytype));

        success &= (world->entities_bytype[i] != NULL);
        success &= (world->archetype_id_bytype[i] != NULL);

        world->len_entities_bytype[i] = TNECS_INITIAL_ENTITY_LEN;
        world->num_entities_bytype[i] = 0;
    }
    return (success);
}

b32 tnecs_growArray_phase(struct tnecs_World *world) {
    size_t olen = world->len_phases;
    size_t nlen = olen * TNECS_ARRAY_GROWTH_FACTOR;
    world->len_phases = nlen;

    size_t bsize1 = sizeof(*world->phases);
    size_t bsize2 = sizeof(*world->systems_byphase);
    size_t bsize3 = sizeof(*world->systems_idbyphase);
    size_t bsize4 = sizeof(*world->len_systems_byphase);
    size_t bsize5 = sizeof(*world->num_systems_byphase);

    world->phases =              tnecs_realloc(world->phases,              olen, nlen, bsize1);
    world->systems_byphase =     tnecs_realloc(world->systems_byphase,     olen, nlen, bsize2);
    world->systems_idbyphase =   tnecs_realloc(world->systems_idbyphase,   olen, nlen, bsize3);
    world->len_systems_byphase = tnecs_realloc(world->len_systems_byphase, olen, nlen, bsize4);
    world->num_systems_byphase = tnecs_realloc(world->num_systems_byphase, olen, nlen, bsize5);

    b32 success = 1;
    success &= (world->systems_byphase      != NULL);
    success &= (world->systems_idbyphase    != NULL);
    success &= (world->phases               != NULL);
    success &= (world->len_systems_byphase  != NULL);
    success &= (world->num_systems_byphase  != NULL);

    for (size_t i = olen; i < world->len_phases; i++) {
        size_t bysize1 = sizeof(**world->systems_byphase);
        size_t bysize2 = sizeof(**world->systems_idbyphase);

        world->systems_byphase[i] =   calloc(TNECS_INITIAL_PHASE_LEN, bysize1);
        world->systems_idbyphase[i] = calloc(TNECS_INITIAL_PHASE_LEN, bysize2);

        success &= (world->systems_byphase[i]   != NULL);
        success &= (world->systems_idbyphase[i] != NULL);

        world->len_systems_byphase[i] = TNECS_INITIAL_PHASE_LEN;
        world->num_systems_byphase[i] = 0;
    }
    return (success);
}

b32 tnecs_growArray_bytype(struct tnecs_World *world, size_t tID) {
    b32 success = 1;
    size_t olen = world->len_entities_bytype[tID];
    size_t nlen = olen * TNECS_ARRAY_GROWTH_FACTOR;
    TNECS_DEBUG_ASSERT(olen > 0);
    world->len_entities_bytype[tID] = nlen;

    size_t bsize =                  sizeof(*world->entities_bytype[tID]);
    tnecs_entity *ptr =           world->entities_bytype[tID];
    world->entities_bytype[tID] =   tnecs_realloc(ptr, olen, nlen, bsize);

    success &= (world->entities_bytype[tID] != NULL);
    size_t new_comp_num = world->num_components_bytype[tID];
    tnecs_component_array *current_array;
    for (size_t corder = 0; corder < new_comp_num; corder++) {
        current_array =     &world->components_bytype[tID][corder];
        size_t old_len =    current_array->len_components;
        size_t new_len =    old_len * TNECS_ARRAY_GROWTH_FACTOR;
        current_array->len_components = new_len;

        size_t cID = world->components_idbytype[tID][corder];

        size_t bsize = world->component_bytesizes[cID];
        current_array->components = tnecs_realloc(current_array->components, old_len, new_len, bsize);
        success &= (current_array->components != NULL);
    }
    return (success);
}

/****************************** STRING HASHING *******************************/
uint64_t tnecs_hash_djb2(const char *str) {
    /* djb2 hashing algorithm by Dan Bernstein.
    * Description: This algorithm (k=33) was first reported by dan bernstein many
    * years ago in comp.lang.c. Another version of this algorithm (now favored by bernstein)
    * uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33
    * (why it works better than many other constants, prime or not) has never been adequately explained.
    * [1] https://stackoverflow.com/questions/7666509/hash-function-for-string
    * [2] http://www.cse.yorku.ca/~oz/hash.html */
    uint64_t hash = 5381;
    int32_t str_char;
    while ((str_char = *str++))
        hash = ((hash << 5) + hash) + str_char; /* hash * 33 + c */
    return (hash);
}

uint64_t tnecs_hash_sdbm(const char *str) {
    /* sdbm hashing algorithm by Dan Bernstein.
    * Description: This algorithm was created for sdbm (a public-domain
    * reimplementation of ndbm) database library. It was found to do
    * well in scrambling bits, causing better distribution of the
    * keys and fewer splits. It also happens to be a good general hashing
    * function with good distribution. The actual function is
    *hash(i) = hash(i - 1) * 65599 + str[i]; what is included below
    * is the faster version used in gawk. [* there is even a faster,
    * duff-device version] the magic constant 65599 was picked out of
    * thin air while experimenting with different constants, and turns
    * out to be a prime. this is one of the algorithms used in
    * berkeley db (see sleepycat) and elsewhere.
    * [1] https://stackoverflow.com/questions/7666509/hash-function-for-string
    * [2] http://www.cse.yorku.ca/~oz/hash.html */

    uint64_t hash = 0;
    uint32_t str_char;
    while ((str_char = *str++)) {
        hash = str_char + (hash << 6) + (hash << 16) - hash;
    }
    return (hash);
}

uint64_t tnecs_hash_combine(uint64_t h1, uint64_t h2) {
    /* SotA: need to combine couple hashes into 1. Max 4-5? */
    /* -> Order of combination should not matter -> + or XOR  */
    /* -> Should be simple and fast -> + or XOR */
    return (h1 ^ h2); // XOR ignores order
}

/****************************** SET BIT COUNTING *****************************/
size_t setBits_KnR_uint64_t(uint64_t in_flags) {
    // Credits to Kernighan and Ritchie in the C Programming Language
    size_t count = 0;
    while (in_flags) {
        in_flags &= (in_flags - 1);
        count++;
    }
    return (count);
}
