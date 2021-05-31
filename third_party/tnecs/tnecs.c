
#include "tnecs.h"

/********************** 0.1 MICROSECOND RESOLUTION CLOCK **********************/
uint64_t get_ns() {
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
extern double get_us();
#else
#  define FAILSAFE_CLOCK
#  define get_us() (((double)clock())/CLOCKS_PER_SEC*1e6) // [us]
#  define get_ns() (((double)clock())/CLOCKS_PER_SEC*1e9) // [ns]
#endif

/**************************** WORLD FUNCTIONS ********************************/
struct tnecs_World * tnecs_world_genesis() {
    TNECS_DEBUG_PRINTF("tnecs_world_genesis\n");

    struct tnecs_World * tnecs_world = (struct tnecs_World *)calloc(sizeof(struct tnecs_World), 1);
    tnecs_world_breath_entities(tnecs_world);
    tnecs_world_breath_typeflags(tnecs_world);
    tnecs_world_breath_systems(tnecs_world);
    tnecs_world_breath_components(tnecs_world);
    return (tnecs_world);
}

void tnecs_world_destroy(struct tnecs_World * in_world) {
    TNECS_DEBUG_PRINTF("tnecs_world_destroy\n");
    for (size_t i = 0; i < in_world->len_phases; i++) {
        free(in_world->systems_byphase[i]);
        free(in_world->systems_idbyphase[i]);
    }
    free(in_world->phases);
    free(in_world->systems_byphase);
    free(in_world->system_orders);
    free(in_world->systems_torun);
    free(in_world->systems_idbyphase);
    for (size_t i = 0; i < in_world->len_typeflags; i++) {
        free(in_world->entities_bytype[i]);
        free(in_world->components_idbytype[i]);
        free(in_world->components_flagbytype[i]);
        free(in_world->components_orderbytype[i]);
        for (size_t j = 0; j < in_world->num_components_bytype[i]; j++) {
            free(in_world->components_bytype[i][j].components);
        }
        free(in_world->components_bytype[i]);
    }
    for (size_t i = 0; i < in_world->num_components; i++) {
        free(in_world->component_names[i]);
    }
    free(in_world->entities_bytype);
    free(in_world->entity_orders);
    free(in_world->entities_open);
    free(in_world->components_bytype);
    free(in_world->components_idbytype);
    free(in_world->components_flagbytype);
    free(in_world->components_orderbytype);
    free(in_world->component_names);
    free(in_world->entities);
    free(in_world->entity_typeflags);

    free(in_world->typeflags);

    free(in_world->system_typeflags);
    free(in_world->system_phases);
    free(in_world->system_hashes);

    free(in_world->num_components_bytype);
    free(in_world->len_entities_bytype);
    free(in_world->num_entities_bytype);

    free(in_world->len_systems_byphase);
    free(in_world->num_systems_byphase);
    free(in_world);
}

void tnecs_world_step(struct tnecs_World * in_world, tnecs_time_ns_t in_deltat) {
    TNECS_DEBUG_PRINTF("tnecs_world_step\n");

    struct tnecs_System_Input current_input;
    current_input.world = in_world;
    size_t system_id, current_phase;
    in_world->num_systems_torun = 0;

    if (!in_deltat) {
        current_input.deltat = get_ns() - in_world->previous_time;
    } else {
        current_input.deltat = in_deltat;
    }
    for (size_t phase_id = 0; phase_id < in_world->num_phases; phase_id++) {
        current_phase = in_world->phases[phase_id];
        if (phase_id == current_phase) {
            for (size_t sorder = 0; sorder < in_world->num_systems_byphase[phase_id]; sorder++) {
                system_id = in_world->systems_idbyphase[phase_id][sorder];
                current_input.typeflag_id = tnecs_typeflagid(in_world, in_world->system_typeflags[system_id]);
                current_input.num_entities = in_world->num_entities_bytype[current_input.typeflag_id];
                in_world->systems_torun[in_world->num_systems_torun++] = in_world->systems_byphase[phase_id][sorder];
                in_world->systems_byphase[phase_id][sorder](&current_input);
            }
        }
    }
    in_world->previous_time = get_ns();
}

void tnecs_world_breath_entities(struct tnecs_World * in_world) {
    TNECS_DEBUG_PRINTF("tnecs_world_breath_entities\n");

    in_world->entities = calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(*in_world->entities));
    in_world->entities_open = calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(*in_world->entities_open));
    in_world->entity_next = TNECS_NULLSHIFT;
    in_world->num_entities_open = 0;
    in_world->len_entities_open = TNECS_INITIAL_ENTITY_LEN;

    in_world->entity_typeflags = calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(*in_world->entity_typeflags));
    in_world->entity_orders = calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(*in_world->entity_orders));
    in_world->len_entities = TNECS_INITIAL_ENTITY_LEN;

    in_world->entities_bytype = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->entities_bytype));
    in_world->len_entities_bytype = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->len_entities_bytype));
    in_world->num_entities_bytype = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->num_entities_bytype));
    for (size_t i = 0; i < TNECS_INITIAL_SYSTEM_LEN; i++) {
        in_world->entities_bytype[i] = calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(**in_world->entities_bytype));
        in_world->num_entities_bytype[i] = 0;
        in_world->len_entities_bytype[i] = TNECS_INITIAL_ENTITY_LEN;
    }
}

void tnecs_world_breath_components(struct tnecs_World * in_world) {
    TNECS_DEBUG_PRINTF("tnecs_world_breath_components\n");

    in_world->component_hashes[TNECS_NULL] = TNECS_NULL;
    in_world->components_bytype = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->components_bytype));
    in_world->component_names = calloc(TNECS_COMPONENT_CAP, sizeof(*in_world->component_names));
    in_world->num_components_bytype = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->num_components_bytype));
    in_world->components_idbytype = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->components_idbytype));
    in_world->components_flagbytype = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->components_flagbytype));
    in_world->components_orderbytype = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->components_orderbytype));
    for (size_t i = 0; i < TNECS_INITIAL_SYSTEM_LEN; i++) {
        in_world->num_components_bytype[i] = 0;
    }
    in_world->num_components = TNECS_NULLSHIFT;
}

void tnecs_world_breath_systems(struct tnecs_World * in_world) {
    TNECS_DEBUG_PRINTF("tnecs_world_breath_systems\n");

    in_world->systems_torun = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->systems_torun));
    in_world->system_typeflags = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->system_typeflags));
    in_world->system_hashes = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->system_hashes));
    in_world->system_phases = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->system_phases));
    in_world->system_orders = calloc(TNECS_INITIAL_SYSTEM_LEN, sizeof(*in_world->system_orders));
    in_world->len_phases = TNECS_INITIAL_PHASE_LEN;
    in_world->num_phases = TNECS_NULLSHIFT;
    in_world->phases = calloc(TNECS_INITIAL_PHASE_LEN, sizeof(*in_world->phases));
    in_world->systems_idbyphase = calloc(TNECS_INITIAL_PHASE_LEN, sizeof(*in_world->systems_idbyphase));
    in_world->len_systems_byphase = calloc(TNECS_INITIAL_PHASE_LEN, sizeof(*in_world->len_systems_byphase));
    in_world->num_systems_byphase = calloc(TNECS_INITIAL_PHASE_LEN, sizeof(*in_world->num_systems_byphase));
    in_world->systems_byphase = calloc(TNECS_INITIAL_PHASE_LEN, sizeof(*in_world->systems_byphase));
    for (size_t i = 0; i < TNECS_INITIAL_PHASE_LEN; i++) {
        in_world->systems_byphase[i] = calloc(TNECS_INITIAL_PHASE_LEN, sizeof(**in_world->systems_byphase));
        in_world->systems_idbyphase[i] = calloc(TNECS_INITIAL_PHASE_LEN, sizeof(**in_world->systems_idbyphase));
        in_world->num_systems_byphase[i] = 0;
        in_world->len_systems_byphase[i] = TNECS_INITIAL_PHASE_LEN;
    }
    in_world->len_systems = TNECS_INITIAL_SYSTEM_LEN;
    in_world->num_systems = TNECS_NULLSHIFT;
}

void tnecs_world_breath_typeflags(struct tnecs_World * in_world) {
    TNECS_DEBUG_PRINTF("tnecs_world_breath_typeflags\n");

    in_world->typeflags = calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(*in_world->typeflags));
    in_world->num_typeflags = TNECS_NULLSHIFT;
    in_world->len_typeflags = TNECS_INITIAL_SYSTEM_LEN;
}

/***************************** REGISTRATION **********************************/
void tnecs_register_system(struct tnecs_World * in_world, tnecs_hash_t in_hash, void (* in_system)(struct tnecs_System_Input *), tnecs_phase_t in_phase, size_t num_components, tnecs_component_t components_typeflag) {
    TNECS_DEBUG_PRINTF("tnecs_register_system\n");

    size_t system_id = in_world->num_systems++;
    if (in_world->num_systems >= in_world->len_systems) { tnecs_growArray_system(in_world); }
    if (!in_world->phases[in_phase]) { tnecs_register_phase(in_world, in_phase); }

    in_world->system_phases[system_id] = in_phase;
    in_world->system_hashes[system_id] = in_hash;
    in_world->system_typeflags[system_id] = components_typeflag;

    size_t system_order = in_world->num_systems_byphase[in_phase]++;
    if (in_world->num_systems_byphase[in_phase] >= in_world->len_systems_byphase[in_phase]) {
        size_t old_len = in_world->len_systems_byphase[in_phase];
        in_world->len_systems_byphase[in_phase] *= TNECS_ARRAY_GROWTH_FACTOR;
        in_world->systems_byphase[in_phase] = tnecs_realloc(in_world->systems_byphase[in_phase], old_len, in_world->len_systems_byphase[in_phase], sizeof(**in_world->systems_byphase));
        in_world->systems_idbyphase[in_phase] = tnecs_realloc(in_world->systems_idbyphase[in_phase], old_len, in_world->len_systems_byphase[in_phase], sizeof(**in_world->systems_idbyphase));
    }

    in_world->system_orders[system_id] = system_order;
    in_world->systems_byphase[in_phase][system_order] = in_system;
    in_world->systems_idbyphase[in_phase][system_order] = system_id;

    tnecs_register_typeflag(in_world, num_components, components_typeflag);
}

void tnecs_register_component(struct tnecs_World * in_world, const char * in_name, size_t in_bytesize) {
    TNECS_DEBUG_PRINTF("tnecs_register_component\n");

    tnecs_hash_t in_hash = tnecs_hash_djb2(in_name);
    if (in_world->num_components < TNECS_COMPONENT_CAP) {
        in_world->component_hashes[in_world->num_components] = in_hash;
        tnecs_component_t new_component_flag = TNECS_COMPONENT_ID2TYPE(in_world->num_components);
        in_world->component_bytesizes[in_world->num_components] = in_bytesize;
        TNECS_DEBUG_ASSERT(in_bytesize > 0);
        in_world->component_names[in_world->num_components] = malloc(strlen(in_name) + 1);
        strncpy(in_world->component_names[in_world->num_components], in_name, strlen(in_name) + 1);
        size_t typeflag_id = tnecs_register_typeflag(in_world, 1, new_component_flag);
        in_world->num_components++;
    } else {
        printf("TNECS ERROR: Cannot register more than 63 components\n");
    }
}

size_t tnecs_register_typeflag(struct tnecs_World * in_world, size_t num_components, tnecs_component_t typeflag_new) {
    TNECS_DEBUG_PRINTF("tnecs_new_typeflag\n");

    size_t typeflag_id = 0;
    for (size_t i = 0 ; i < in_world->num_typeflags; i++) {
        if (typeflag_new == in_world->typeflags[i]) {
            typeflag_id = i;
            break;
        }
    }
    if (!typeflag_id) {
        // 1- Add new components_bytype at [typeflag_id]
        if ((in_world->num_typeflags + 1) >= in_world->len_typeflags) {
            tnecs_growArray_typeflag(in_world);
        }
        in_world->typeflags[in_world->num_typeflags++] = typeflag_new;
        typeflag_id = tnecs_typeflagid(in_world, typeflag_new);
        TNECS_DEBUG_ASSERT(typeflag_id == (in_world->num_typeflags - 1));
        in_world->num_components_bytype[typeflag_id] = num_components;

        // 2- Add arrays to components_bytype[typeflag_id] for each component
        tnecs_component_array_new(in_world, num_components, typeflag_new);

        // 3- Add all components to components_idbytype and components_flagbytype
        tnecs_component_t component_id_toadd, component_type_toadd;
        tnecs_component_t typeflag_reduced = typeflag_new, typeflag_added = 0;
        in_world->components_idbytype[typeflag_id] =  calloc(num_components, sizeof(**in_world->components_idbytype));
        in_world->components_flagbytype[typeflag_id] =  calloc(num_components, sizeof(**in_world->components_flagbytype));
        in_world->components_orderbytype[typeflag_id] =  calloc(TNECS_COMPONENT_CAP, sizeof(**in_world->components_orderbytype));

        size_t i = 0;
        while (typeflag_reduced) {
            typeflag_reduced &= (typeflag_reduced - 1);
            component_type_toadd = (typeflag_reduced + typeflag_added) ^ typeflag_new;
            component_id_toadd = TNECS_COMPONENT_TYPE2ID(component_type_toadd);
            in_world->components_idbytype[typeflag_id][i] = component_id_toadd;
            in_world->components_flagbytype[typeflag_id][i] = component_type_toadd;
            in_world->components_orderbytype[typeflag_id][component_id_toadd] = i;

            typeflag_added += component_type_toadd;
            i++;
        }
    }
    return (typeflag_id);
}

size_t tnecs_register_phase(struct tnecs_World * in_world, tnecs_phase_t in_phase) {
    TNECS_DEBUG_PRINTF("tnecs_register_phase\n");
    while (in_phase >= in_world->len_phases) { tnecs_growArray_phase(in_world); }
    in_world->phases[in_phase] = in_phase;
    in_world->num_phases = (in_phase >= in_world->num_phases) ? (in_phase + 1) : in_world->num_phases;
    return (in_phase);
}

/***************************** ENTITY MANIPULATION ***************************/
tnecs_entity_t tnecs_entity_create(struct tnecs_World * in_world) {
    TNECS_DEBUG_PRINTF("tnecs_entity_create\n");

    tnecs_entity_t out = TNECS_NULL;
    tnecs_component_t component_flag;
    while ((out == TNECS_NULL) && (in_world->num_entities_open > 0)) {
        out = in_world->entities_open[--in_world->num_entities_open];
        in_world->entities_open[in_world->num_entities_open] = TNECS_NULL;
    }
    if (out == TNECS_NULL) {
        do {
            if (in_world->entity_next >= in_world->len_entities) { tnecs_growArray_entity(in_world); }
            out = in_world->entity_next;
        } while (in_world->entities[in_world->entity_next++] != TNECS_NULL);
    }
    TNECS_DEBUG_ASSERT(out != TNECS_NULL);
    in_world->entities[out] = out;
    tnecs_entitiesbytype_add(in_world, out, TNECS_NULL);
    TNECS_DEBUG_ASSERT(in_world->entities[out] == out);

    TNECS_DEBUG_ASSERT(in_world->entities_bytype[TNECS_NULL][in_world->entity_orders[out]] == out);
    return (out);
}

tnecs_entity_t tnecs_entity_create_windex(struct tnecs_World * in_world, tnecs_entity_t in_entity) {
    TNECS_DEBUG_PRINTF("tnecs_entity_create_windex\n");

    tnecs_entity_t out = 0;
    while (in_entity >= in_world->len_entities) { tnecs_growArray_entity(in_world); }
    if (!in_world->entities[in_entity]) {
        out = in_world->entities[in_entity] = in_entity;
        tnecs_entitiesbytype_add(in_world, out, TNECS_NULL);
    }
    return (out);
}

void tnecs_entities_create(struct tnecs_World * in_world, size_t num) {
    TNECS_DEBUG_PRINTF("tnecs_entities_create\n");

    size_t created = 0;
    while (created < num) {created += (tnecs_entity_create(in_world) > 0);}
}

void tnecs_entities_create_windices(struct tnecs_World * in_world, size_t num, tnecs_entity_t * in_entities) {
    TNECS_DEBUG_PRINTF("tnecs_entities_create_windices\n");

    for (size_t i = 0; i < num; i++) {
        tnecs_entity_create_windex(in_world, in_entities[i]);
    }
}

tnecs_entity_t tnecs_entity_create_wcomponents(struct tnecs_World * in_world, size_t argnum, ...) {
    TNECS_DEBUG_PRINTF("tnecs_entity_create_wcomponents \n");

    va_list ap;
    va_start(ap, argnum);
    tnecs_component_t typeflag = 0;
    tnecs_hash_t current_hash;
    for (size_t i = 0; i < argnum; i++) {
        current_hash = va_arg(ap, tnecs_hash_t);
        typeflag += tnecs_component_hash2type(in_world, current_hash);
    }
    va_end(ap);

    tnecs_entity_t new_entity = tnecs_entity_create(in_world);
    tnecs_entity_add_components(in_world, new_entity, argnum, typeflag, true);
    size_t typeflag_id = TNECS_TYPEFLAGID(in_world, typeflag);
    TNECS_DEBUG_ASSERT(in_world->entities_bytype[typeflag_id][in_world->entity_orders[new_entity]] == new_entity);
    TNECS_DEBUG_ASSERT(in_world->entities[new_entity] = new_entity);
    return (new_entity);
}

void tnecs_entity_destroy(struct tnecs_World * in_world, tnecs_entity_t in_entity) {
    TNECS_DEBUG_PRINTF("tnecs_entity_destroy \n");

    TNECS_DEBUG_ASSERT(in_world->entities[in_entity]);
    TNECS_DEBUG_ASSERT(in_entity > 0);
    tnecs_component_t entity_typeflag = in_world->entity_typeflags[in_entity];
    TNECS_DEBUG_ASSERT(entity_typeflag >= 0);
    size_t entity_typeflag_id = TNECS_TYPEFLAGID(in_world, entity_typeflag);
    size_t entity_order = in_world->entity_orders[in_entity];
    TNECS_DEBUG_ASSERT(in_world->num_entities_bytype[entity_typeflag_id] > 0);
    tnecs_component_del(in_world, in_entity, entity_typeflag);

    // Entity removed from entities_bytype, entity_typeflags, entities...
    TNECS_DEBUG_ASSERT(in_world->len_entities_bytype[entity_typeflag_id] >= entity_order);
    TNECS_DEBUG_ASSERT(in_world->num_entities_bytype[entity_typeflag_id] > 0);
    tnecs_entitiesbytype_del(in_world, in_entity, entity_typeflag);
    in_world->entities[in_entity] = TNECS_NULL;
    in_world->entity_typeflags[in_entity] = TNECS_NULL;
    in_world->entity_orders[in_entity] = TNECS_NULL;
    if ((in_world->num_entities_open + 1) >= in_world->len_entities_open) {
        size_t old_len = in_world->len_entities_open;
        in_world->len_entities_open *= TNECS_ARRAY_GROWTH_FACTOR;
        in_world->entities_open = tnecs_realloc(in_world->entities_open, old_len, in_world->len_entities_open, sizeof(*in_world->entities_open));
    }
    in_world->entities_open[in_world->num_entities_open++] = in_entity;

    TNECS_DEBUG_ASSERT(in_world->entities[in_entity] == TNECS_NULL);
    TNECS_DEBUG_ASSERT(in_world->entity_typeflags[in_entity] == TNECS_NULL);
    TNECS_DEBUG_ASSERT(in_world->entity_orders[in_entity] == TNECS_NULL);
    TNECS_DEBUG_ASSERT(in_world->entity_orders[entity_order] != in_entity);
}

/*****************************************************************************/
/***************************** TNECS INTERNALS *******************************/
/*****************************************************************************/
void tnecs_entity_add_components(struct tnecs_World * in_world, tnecs_entity_t in_entity, size_t num_components_toadd, tnecs_component_t typeflag_toadd, bool isNew) {
    TNECS_DEBUG_PRINTF("tnecs_entity_add_components\n");

    tnecs_component_t typeflag_old = in_world->entity_typeflags[in_entity];
    TNECS_DEBUG_ASSERT((typeflag_toadd != typeflag_old));
    tnecs_component_t typeflag_new = typeflag_toadd + typeflag_old;

    // 1- Checks if the new entity_typeflag exists, if not create empty component array
    if (isNew) { tnecs_register_typeflag(in_world, setBits_KnR_uint64_t(typeflag_new), typeflag_new); }

    size_t typeflag_new_id = tnecs_typeflagid(in_world, typeflag_new);
    tnecs_component_migrate(in_world, in_entity, typeflag_old, typeflag_new);
    tnecs_entitiesbytype_migrate(in_world, in_entity, typeflag_old, typeflag_new);
    TNECS_DEBUG_ASSERT(in_world->entity_typeflags[in_entity] == typeflag_new);

    TNECS_DEBUG_ASSERT(in_world->entities_bytype[typeflag_new_id][in_world->num_entities_bytype[typeflag_new_id] - 1 ] == in_entity);
    TNECS_DEBUG_ASSERT(in_world->entity_orders[in_entity] == in_world->num_entities_bytype[typeflag_new_id] - 1);
}

void * tnecs_entity_get_component(struct tnecs_World * in_world, tnecs_entity_t in_entity_id, tnecs_component_t in_component_id) {
    TNECS_DEBUG_PRINTF("tnecs_entity_get_component\n");

    tnecs_component_t component_flag = TNECS_COMPONENT_ID2TYPE(in_component_id);
    tnecs_component_t entity_typeflag = TNECS_ENTITY_TYPEFLAG(in_world, in_entity_id);

    if ((component_flag & entity_typeflag) > 0) {
        size_t typeflag_id = tnecs_typeflagid(in_world, entity_typeflag);
        size_t component_order = tnecs_component_order_bytype(in_world, in_component_id, entity_typeflag);
        TNECS_DEBUG_ASSERT(component_order <= in_world->num_entities_bytype[typeflag_id]);
        size_t entity_order = in_world->entity_orders[in_entity_id];
        size_t bytesize = in_world->component_bytesizes[in_component_id];
        struct tnecs_Components_Array * comp_array = &in_world->components_bytype[typeflag_id][component_order];
        tnecs_byte_t * temp_component_bytesptr = (tnecs_byte_t *)(comp_array->components);
        return (temp_component_bytesptr + (bytesize * entity_order));
    } else {
        return (NULL);
    }
}

size_t tnecs_entitiesbytype_add(struct tnecs_World * in_world, tnecs_entity_t in_entity, tnecs_component_t typeflag_new) {
    TNECS_DEBUG_PRINTF("tnecs_entitiesbytype_add\n");

    size_t typeflag_id_new = tnecs_typeflagid(in_world, typeflag_new);
    if ((in_world->num_entities_bytype[typeflag_id_new] + 1) >= in_world->len_entities_bytype[typeflag_id_new]) {
        size_t old_len = in_world->len_entities_bytype[typeflag_id_new];
        in_world->len_entities_bytype[typeflag_id_new] *= TNECS_ARRAY_GROWTH_FACTOR;
        in_world->entities_bytype[typeflag_id_new] = tnecs_realloc(in_world->entities_bytype[typeflag_id_new], old_len, in_world->len_entities_bytype[typeflag_id_new], sizeof(*in_world->entities_bytype[typeflag_id_new]));
    }
    in_world->entities_bytype[typeflag_id_new][in_world->num_entities_bytype[typeflag_id_new]] = in_entity;
    in_world->entity_typeflags[in_entity] = typeflag_new;
    in_world->entity_orders[in_entity] = in_world->num_entities_bytype[typeflag_id_new]++;
    return (in_world->entity_orders[in_entity]);
}

void tnecs_entitiesbytype_del(struct tnecs_World * in_world, tnecs_entity_t in_entity, tnecs_component_t typeflag_old) {
    TNECS_DEBUG_PRINTF("tnecs_entitiesbytype_del\n");

    TNECS_DEBUG_ASSERT(in_entity);
    TNECS_DEBUG_ASSERT(in_world->entities[in_entity] == in_entity);
    TNECS_DEBUG_ASSERT(in_entity < in_world->len_entities);
    size_t typeflag_old_id = tnecs_typeflagid(in_world, typeflag_old);
    TNECS_DEBUG_ASSERT(in_world->num_entities_bytype[typeflag_old_id] > 0);
    size_t entity_order_old = in_world->entity_orders[in_entity];
    TNECS_DEBUG_ASSERT(in_world->entities_bytype[typeflag_old_id][entity_order_old] == in_entity);
    TNECS_DEBUG_ASSERT(entity_order_old < in_world->len_entities_bytype[typeflag_old_id]);

    TNECS_DEBUG_ASSERT(in_world->entities_bytype[typeflag_old_id][entity_order_old] == in_entity);

    tnecs_entity_t top_entity = in_world->entities_bytype[typeflag_old_id][--in_world->num_entities_bytype[typeflag_old_id]];
    size_t top_entity_order = in_world->entity_orders[top_entity];
    TNECS_DEBUG_ASSERT(in_world->entities_bytype[typeflag_old_id][top_entity_order] == top_entity);

    if (top_entity != in_entity) {
        in_world->entities_bytype[typeflag_old_id][top_entity_order] = TNECS_NULL;
        in_world->entity_orders[top_entity] = entity_order_old;
        in_world->entities_bytype[typeflag_old_id][entity_order_old] = top_entity;
        TNECS_DEBUG_ASSERT(in_world->entities_bytype[typeflag_old_id][in_world->entity_orders[top_entity]] == top_entity);
    }

    in_world->entity_orders[in_entity] = TNECS_NULL;
    in_world->entity_typeflags[in_entity] = TNECS_NULL;
}

size_t tnecs_entitiesbytype_migrate(struct tnecs_World * in_world, tnecs_entity_t in_entity, tnecs_component_t typeflag_old, tnecs_component_t typeflag_new) {
    TNECS_DEBUG_PRINTF("tnecs_entitiesbytype_migrate\n");
    tnecs_entitiesbytype_del(in_world, in_entity, typeflag_old);
    TNECS_DEBUG_ASSERT(in_world->entity_typeflags[in_entity] == TNECS_NULL);
    TNECS_DEBUG_ASSERT(in_world->entity_orders[in_entity] == TNECS_NULL);
    tnecs_entitiesbytype_add(in_world, in_entity, typeflag_new);
    size_t typeflag_new_id = tnecs_typeflagid(in_world, typeflag_new);
    TNECS_DEBUG_ASSERT(in_world->entity_typeflags[in_entity] == typeflag_new);
    TNECS_DEBUG_ASSERT(in_world->entity_orders[in_entity] == in_world->num_entities_bytype[typeflag_new_id] - 1);
    TNECS_DEBUG_ASSERT(in_world->entities_bytype[typeflag_new_id][in_world->entity_orders[in_entity]] == in_entity);

    size_t out = in_world->entity_orders[in_entity];
    return (out);
}

void tnecs_component_add(struct tnecs_World * in_world, tnecs_component_t in_typeflag) {
    TNECS_DEBUG_PRINTF("tnecs_component_add \n");

    struct tnecs_Components_Array * current_array;
    size_t in_typeflag_id = tnecs_typeflagid(in_world, in_typeflag);
    size_t new_component_num = in_world->num_components_bytype[in_typeflag_id];
    size_t current_component_id;

    for (size_t corder = 0; corder < new_component_num; corder++) {
        current_array = &in_world->components_bytype[in_typeflag_id][corder];
        current_component_id = in_world->components_idbytype[in_typeflag_id][corder];

        TNECS_DEBUG_ASSERT(current_array != NULL);
        if (++current_array->num_components >= current_array->len_components) {
            size_t old_len = current_array->len_components;
            size_t bytesize = in_world->component_bytesizes[current_component_id];
            current_array->len_components *= TNECS_ARRAY_GROWTH_FACTOR;
            current_array->components = tnecs_realloc(current_array->components, old_len, current_array->len_components, bytesize);
        }
    }
}

void tnecs_component_copy(struct tnecs_World * in_world, tnecs_entity_t in_entity, tnecs_component_t old_typeflag, tnecs_component_t new_typeflag) {
    TNECS_DEBUG_PRINTF("tnecs_component_copy \n");

    TNECS_DEBUG_ASSERT(old_typeflag != TNECS_NULL);
    size_t old_typeflag_id = tnecs_typeflagid(in_world, old_typeflag);
    size_t new_typeflag_id = tnecs_typeflagid(in_world, new_typeflag);
    size_t old_entity_order = in_world->entity_orders[in_entity];
    size_t new_entity_order = in_world->num_entities_bytype[new_typeflag_id] + 1;
    size_t old_component_id, new_component_id, component_bytesize;
    struct tnecs_Components_Array * old_array, * new_array;
    tnecs_byte_t * old_component_ptr, * new_component_ptr, * temp_component_bytesptr;
    for (size_t old_corder = 0; old_corder < in_world->num_components_bytype[old_typeflag_id]; old_corder++) {
        old_component_id = in_world->components_idbytype[old_typeflag_id][old_corder];
        for (size_t new_corder = 0; new_corder < in_world->num_components_bytype[new_typeflag_id]; new_corder++) {
            new_component_id = in_world->components_idbytype[new_typeflag_id][new_corder];
            if (old_component_id == new_component_id) {
                new_array = &in_world->components_bytype[new_typeflag_id][new_corder];
                old_array = &in_world->components_bytype[old_typeflag_id][old_corder];
                component_bytesize = in_world->component_bytesizes[old_component_id];
                temp_component_bytesptr = (tnecs_byte_t *)(old_array->components);
                TNECS_DEBUG_ASSERT(temp_component_bytesptr != NULL);

                old_component_ptr = (tnecs_byte_t *)(temp_component_bytesptr + (component_bytesize * old_entity_order));

                TNECS_DEBUG_ASSERT(old_component_ptr != NULL);
                temp_component_bytesptr = (tnecs_byte_t *)(new_array->components);
                new_component_ptr = (tnecs_byte_t *)(temp_component_bytesptr + (component_bytesize * new_entity_order));
                TNECS_DEBUG_ASSERT(new_component_ptr != NULL);
                memcpy(new_component_ptr, old_component_ptr, component_bytesize);
                break;
            }
        }
    }

}

void tnecs_component_del(struct tnecs_World * in_world, tnecs_entity_t in_entity, tnecs_component_t old_typeflag) {
    TNECS_DEBUG_PRINTF("tnecs_component_del \n");
    // for input entity, delete ALL components from componentsbytype

    size_t old_typeflag_id = tnecs_typeflagid(in_world, old_typeflag);
    size_t old_component_num = in_world->num_components_bytype[old_typeflag_id];
    size_t entity_order_old = in_world->entity_orders[in_entity];

    size_t component_order_current, current_component_id, component_bytesize;
    tnecs_byte_t * current_component_ptr, * next_component_ptr, * temp_component_ptr;
    for (size_t corder = 0; corder < old_component_num; corder++) {
        current_component_id = in_world->components_idbytype[old_typeflag_id][corder];
        temp_component_ptr = (tnecs_byte_t *)in_world->components_bytype[old_typeflag_id][corder].components;
        TNECS_DEBUG_ASSERT(temp_component_ptr != NULL);
        component_bytesize = in_world->component_bytesizes[current_component_id];

        tnecs_arrdel(temp_component_ptr, entity_order_old, in_world->num_entities_bytype[old_typeflag_id], component_bytesize);
    }
}

bool tnecs_component_migrate(struct tnecs_World * in_world, tnecs_entity_t in_entity, size_t entity_order_new, tnecs_component_t new_typeflag) {
    TNECS_DEBUG_PRINTF("tnecs_component_migrate \n");

    tnecs_component_t old_typeflag = in_world->entity_typeflags[in_entity];
    if (old_typeflag > TNECS_NULL) {
        tnecs_component_add(in_world, new_typeflag);
        tnecs_component_copy(in_world, in_entity, old_typeflag, new_typeflag);
        tnecs_component_del(in_world, in_entity, old_typeflag);
    } else {
        tnecs_component_add(in_world, new_typeflag);
    }
}

void tnecs_component_array_new(struct tnecs_World * in_world, size_t num_components, tnecs_component_t in_typeflag) {
    TNECS_DEBUG_PRINTF("tnecs_component_array_new\n");
    // assumes new typeflag was added on top of world->typeflags

    struct tnecs_Components_Array * temp_comparray = (struct tnecs_Components_Array *)calloc(num_components, sizeof(struct tnecs_Components_Array));
    tnecs_component_t typeflag_reduced = in_typeflag, typeflag_added = 0, type_toadd, typeflag_id = tnecs_typeflagid(in_world, in_typeflag);
    size_t id_toadd, num_flags = 0;
    while (typeflag_reduced) {
        typeflag_reduced &= (typeflag_reduced - 1);
        type_toadd = (typeflag_reduced + typeflag_added) ^ in_typeflag;

        id_toadd = TNECS_COMPONENT_TYPE2ID(type_toadd);
        TNECS_DEBUG_ASSERT(id_toadd > 0);
        tnecs_component_array_init(in_world, &temp_comparray[num_flags], id_toadd);
        num_flags++;
        typeflag_added += type_toadd;
    }
    in_world->components_bytype[typeflag_id] = temp_comparray;

    TNECS_DEBUG_ASSERT(typeflag_added == in_typeflag);
    TNECS_DEBUG_ASSERT(num_flags == num_components);
}

void tnecs_component_array_init(struct tnecs_World * in_world, struct tnecs_Components_Array * in_array, size_t in_component_id) {
    TNECS_DEBUG_PRINTF("tnecs_component_array_init\n");

    TNECS_DEBUG_ASSERT(in_component_id > 0);
    tnecs_component_t in_type = TNECS_COMPONENT_ID2TYPE(in_component_id);
    TNECS_DEBUG_ASSERT(in_type < (1 << in_world->num_components));
    size_t bytesize = in_world->component_bytesizes[in_component_id];
    TNECS_DEBUG_ASSERT(bytesize > 0);
    in_array->type = in_type;
    in_array->num_components = 0;
    in_array->len_components = TNECS_INITIAL_ENTITY_LEN;
    TNECS_DEBUG_ASSERT(in_array->components == NULL);
    in_array->components = calloc(TNECS_INITIAL_ENTITY_LEN, bytesize);
    TNECS_DEBUG_ASSERT(in_array->components != NULL);
}

void tnecs_system_order_switch(struct tnecs_World * in_world, tnecs_phase_t in_phase_id, size_t order1, size_t order2) {
    TNECS_DEBUG_PRINTF("tnecs_system_order_switch\n");

    void (* systems_temp)(struct tnecs_System_Input *);
    TNECS_DEBUG_ASSERT(in_world->num_phases > in_phase_id);
    TNECS_DEBUG_ASSERT(in_world->phases[in_phase_id]);
    TNECS_DEBUG_ASSERT(in_world->num_systems_byphase[in_phase_id] > order1);
    TNECS_DEBUG_ASSERT(in_world->num_systems_byphase[in_phase_id] > order2);
    TNECS_DEBUG_ASSERT(in_world->systems_byphase[in_phase_id][order1]);
    TNECS_DEBUG_ASSERT(in_world->systems_byphase[in_phase_id][order2]);
    systems_temp = in_world->systems_byphase[in_phase_id][order1];
    in_world->systems_byphase[in_phase_id][order1] = in_world->systems_byphase[in_phase_id][order2];
    in_world->systems_byphase[in_phase_id][order2] = systems_temp;
}

/************************ UTILITY FUNCTIONS/MACROS ***************************/
size_t tnecs_component_name2id(struct tnecs_World * in_world, const tnecs_str_t * in_name) {
    TNECS_DEBUG_PRINTF("tnecs_component_name2id\n");

    return (tnecs_component_hash2id(in_world, tnecs_hash_djb2(in_name)));
}

size_t tnecs_component_hash2id(struct tnecs_World * in_world, tnecs_hash_t in_hash) {
    TNECS_DEBUG_PRINTF("tnecs_component_hash2id\n");

    size_t out;
    for (size_t i = 0; i < in_world->num_components; i++) {
        if (in_world->component_hashes[i] == in_hash) {
            out = i;
            break;
        }
    }
    return (out);
}

size_t tnecs_component_order_bytype(struct tnecs_World * in_world, size_t in_component_id, tnecs_component_t in_typeflag) {
    TNECS_DEBUG_PRINTF("tnecs_component_order_bytype\n");

    tnecs_component_t in_typeflag_id = tnecs_typeflagid(in_world, in_typeflag);
    return (tnecs_component_order_bytypeid(in_world, in_component_id, in_typeflag_id));
}

size_t tnecs_component_order_bytypeid(struct tnecs_World * in_world, size_t in_component_id, size_t in_typeflag_id) {
    TNECS_DEBUG_PRINTF("tnecs_component_order_bytypeid\n");

    size_t order = TNECS_COMPONENT_CAP;
    for (size_t i = 0; i < in_world->num_components_bytype[in_typeflag_id]; i++) {
        if (in_world->components_idbytype[in_typeflag_id][i] == in_component_id) {
            order = i;
            break;
        }
    }
    return (order);
}

tnecs_component_t tnecs_component_names2typeflag(struct tnecs_World * in_world, size_t argnum, ...) {
    TNECS_DEBUG_PRINTF("tnecs_component_names2typeflag\n");

    va_list ap;
    tnecs_component_t typeflag = 0;
    va_start(ap, argnum);
    for (size_t i = 0; i < argnum; i++) {
        typeflag += in_world->typeflags[tnecs_component_name2id(in_world, va_arg(ap, const tnecs_str_t *))];
    }
    va_end(ap);
    return (typeflag);
}

tnecs_component_t tnecs_component_ids2typeflag(size_t argnum, ...) {
    TNECS_DEBUG_PRINTF("tnecs_component_ids2typeflag\n");

    tnecs_component_t out = 0;
    va_list ap;
    va_start(ap, argnum);
    for (size_t i = 0; i < argnum; i++) {
        out += TNECS_COMPONENT_ID2TYPE(va_arg(ap, size_t));
    }
    va_end(ap);
    return (out);
}

tnecs_component_t tnecs_component_hash2type(struct tnecs_World * in_world, tnecs_hash_t in_hash) {
    TNECS_DEBUG_PRINTF("tnecs_component_hash2type \n");
    return (TNECS_COMPONENT_ID2TYPE(tnecs_component_hash2id(in_world, in_hash)));
}

size_t tnecs_system_id(struct tnecs_World * in_world, const tnecs_str_t * in_name) {
    TNECS_DEBUG_PRINTF("tnecs_system_id\n");

    tnecs_hash_t in_hash = tnecs_hash_djb2(in_name);
    size_t found = 0;
    for (size_t i = 0; i < in_world->num_systems; i++) {
        if (in_world->system_hashes[i] == in_hash) {
            found = i;
            break;
        }
    }
    return (found);
}

tnecs_component_t tnecs_system_name2typeflag(struct tnecs_World * in_world, const tnecs_str_t * in_name) {
    TNECS_DEBUG_PRINTF("tnecs_system_name2typeflag\n");
    size_t id = tnecs_system_id(in_world, in_name);
    return (in_world->system_typeflags[id]);
}

size_t tnecs_typeflagid(struct tnecs_World * in_world, tnecs_component_t in_typeflag) {
    TNECS_DEBUG_PRINTF("tnecs_typeflagid \n");

    size_t id = 0;
    for (size_t i = 0; i < in_world->num_typeflags; i++) {
        if (in_typeflag == in_world->typeflags[i]) {
            id = i;
            break;
        }
    }
    return (id);
}

/***************************** "DYNAMIC" ARRAYS ******************************/
void * tnecs_realloc(void * ptr, size_t old_len, size_t new_len, size_t elem_bytesize) {
    TNECS_DEBUG_PRINTF("tnecs_realloc\n");

    void * temp = (void *)calloc(new_len, elem_bytesize);
    memcpy(temp, ptr, old_len * elem_bytesize);
    free(ptr);
    return (temp);
}

void * tnecs_arrdel(void * arr, size_t elem, size_t len, size_t bytesize) {
    TNECS_DEBUG_PRINTF("tnecs_arrdel\n");
    void * out;
    if (elem < (len - 1)) {
        out = memcpy(arr + (elem * bytesize), arr + ((elem + 1) * bytesize), bytesize * (len - elem - 1));
    } else {
        out = memset(arr + (elem * bytesize), 0, bytesize);
    }
    return (out);
}

void * tnecs_arrdel_scramble(void * arr, size_t elem, size_t len, size_t bytesize) {
    TNECS_DEBUG_PRINTF("tnecs_arrdel_scramble\n");
    memcpy(arr + (elem * bytesize), arr + ((len - 1) * bytesize), bytesize);
    memset(arr + ((len - 1) * bytesize), 0, bytesize);
    return (arr);
}

void tnecs_growArray_entity(struct tnecs_World * in_world) {
    TNECS_DEBUG_PRINTF("tnecs_growArray_entity\n");

    size_t old_len = in_world->len_entities;
    in_world->len_entities *= TNECS_ARRAY_GROWTH_FACTOR;

    in_world->entity_typeflags = tnecs_realloc(in_world->entity_typeflags, old_len, in_world->len_entities, sizeof(*in_world->entity_typeflags));
    in_world->entity_orders = tnecs_realloc(in_world->entity_orders, old_len, in_world->len_entities, sizeof(*in_world->entity_orders));
    in_world->entities = tnecs_realloc(in_world->entities, old_len, in_world->len_entities, sizeof(*in_world->entities));
}

void tnecs_growArray_system(struct tnecs_World * in_world) {
    TNECS_DEBUG_PRINTF("tnecs_growArray_system\n");

    size_t old_len = in_world->len_systems;
    in_world->len_systems *= TNECS_ARRAY_GROWTH_FACTOR;

    in_world->systems_torun = tnecs_realloc(in_world->systems_torun, old_len, in_world->len_systems, sizeof(*in_world->systems_torun));
    in_world->system_phases = tnecs_realloc(in_world->system_phases, old_len, in_world->len_systems, sizeof(*in_world->system_phases));
    in_world->system_orders = tnecs_realloc(in_world->system_orders, old_len, in_world->len_systems, sizeof(*in_world->system_orders));
    in_world->systems_idbyphase = tnecs_realloc(in_world->systems_idbyphase, old_len, in_world->len_systems, sizeof(*in_world->systems_idbyphase));
    in_world->system_typeflags = tnecs_realloc(in_world->system_typeflags, old_len, in_world->len_systems, sizeof(*in_world->system_typeflags));
    in_world->system_hashes = tnecs_realloc(in_world->system_hashes, old_len, in_world->len_systems, sizeof(*in_world->system_hashes));
}

void tnecs_growArray_typeflag(struct tnecs_World * in_world) {
    TNECS_DEBUG_PRINTF("tnecs_growArray_typeflag\n");

    size_t old_len = in_world->len_typeflags;
    in_world->len_typeflags *= TNECS_ARRAY_GROWTH_FACTOR;

    in_world->typeflags = tnecs_realloc(in_world->typeflags, old_len, in_world->len_typeflags, sizeof(*in_world->typeflags));
    in_world->components_bytype = tnecs_realloc(in_world->components_bytype, old_len, in_world->len_typeflags, sizeof(*in_world->components_bytype));
    in_world->num_components_bytype = tnecs_realloc(in_world->num_components_bytype, old_len, in_world->len_typeflags, sizeof(*in_world->num_components_bytype));
    in_world->entities_bytype = tnecs_realloc(in_world->entities_bytype, old_len, in_world->len_typeflags, sizeof(*in_world->entities_bytype));
    in_world->num_entities_bytype = tnecs_realloc(in_world->num_entities_bytype, old_len, in_world->len_typeflags, sizeof(*in_world->num_entities_bytype));
    in_world->len_entities_bytype = tnecs_realloc(in_world->len_entities_bytype, old_len, in_world->len_typeflags, sizeof(*in_world->len_entities_bytype));
    in_world->components_idbytype = tnecs_realloc(in_world->components_idbytype, old_len, in_world->len_typeflags, sizeof(*in_world->components_idbytype));
    in_world->components_flagbytype = tnecs_realloc(in_world->components_flagbytype, old_len, in_world->len_typeflags, sizeof(*in_world->components_flagbytype));
    in_world->components_orderbytype = tnecs_realloc(in_world->components_orderbytype, old_len, in_world->len_typeflags, sizeof(*in_world->components_orderbytype));
    for (size_t i = old_len; i < in_world->len_typeflags; i++) {
        in_world->entities_bytype[i] = calloc(TNECS_INITIAL_ENTITY_LEN, sizeof(**in_world->entities_bytype));
    }
}

void tnecs_growArray_phase(struct tnecs_World * in_world) {
    TNECS_DEBUG_PRINTF("tnecs_growArray_phase\n");

    size_t old_len = in_world->len_phases;
    in_world->len_phases *= TNECS_ARRAY_GROWTH_FACTOR;
    in_world->systems_byphase = tnecs_realloc(in_world->systems_byphase, old_len, in_world->len_phases, sizeof(*in_world->systems_byphase));
    in_world->systems_idbyphase = tnecs_realloc(in_world->systems_idbyphase, old_len, in_world->len_phases, sizeof(*in_world->systems_idbyphase));
    in_world->phases = tnecs_realloc(in_world->phases, old_len, in_world->len_phases, sizeof(*in_world->phases));
    in_world->len_systems_byphase = tnecs_realloc(in_world->len_systems_byphase, old_len, in_world->len_phases, sizeof(*in_world->len_systems_byphase));
    in_world->num_systems_byphase = tnecs_realloc(in_world->num_systems_byphase, old_len, in_world->len_phases, sizeof(*in_world->num_systems_byphase));
    for (size_t i = old_len; i < in_world->len_phases; i++) {
        in_world->systems_byphase[i] = calloc(TNECS_INITIAL_PHASE_LEN, sizeof(**in_world->systems_byphase));
        in_world->systems_idbyphase[i] = calloc(TNECS_INITIAL_PHASE_LEN, sizeof(**in_world->systems_idbyphase));
        in_world->len_systems_byphase[i] = TNECS_INITIAL_PHASE_LEN;
        in_world->num_systems_byphase[i] = 0;
    }
}


/****************************** STRING HASHING *******************************/
uint64_t tnecs_hash_djb2(const unsigned char * str) {
    /* djb2 hashing algorithm by Dan Bernstein.
    * Description: This algorithm (k=33) was first reported by dan bernstein many
    * years ago in comp.lang.c. Another version of this algorithm (now favored by bernstein)
    * uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33
    * (why it works better than many other constants, prime or not) has never been adequately explained.
    * [1] https://stackoverflow.com/questions/7666509/hash-function-for-string
    * [2] http://www.cse.yorku.ca/~oz/hash.html */
    TNECS_DEBUG_PRINTF("tnecs_hash_djb2\n");

    uint64_t hash = 5381;
    int32_t str_char;
    while (str_char = *str++) {
        hash = ((hash << 5) + hash) + str_char; /* hash * 33 + c */
    }
    return (hash);
}

uint64_t tnecs_hash_sdbm(const unsigned char * str) {
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
    TNECS_DEBUG_PRINTF("tnecs_hash_djb2\n");

    uint64_t hash = 0;
    uint32_t str_char;
    while (str_char = *str++) {
        hash = str_char + (hash << 6) + (hash << 16) - hash;
    }
    return (hash);
}

/****************************** SET BIT COUNTING *****************************/
size_t setBits_KnR_uint64_t(uint64_t in_flags) {
    // Credits to Kernighan and Ritchie in the C Programming Language
    TNECS_DEBUG_PRINTF("setBits_KnR_uint64_t\n");
    size_t count = 0;
    while (in_flags) {
        in_flags &= (in_flags - 1);
        count++;
    }
    return (count);
}