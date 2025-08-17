#ifndef RELOAD_H
#define RELOAD_H

#include "events.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Unit;
struct Game;
struct Convoy;

/* --- Reload --- */
typedef void (* entity_reload_f)(void *struct_ptr);

/* -- Reload all Es using specific reload function -- */
void Reload_Entities(          struct Game *sota, entity_reload_f func, size_t flag,  char *c);
void Reload_Entities_Archetype(struct Game *sota, entity_reload_f func,  char *c);

/* -- Reload funcs -- */
void Reload_JSON(    void *struct_ptr);
void Reload_Menu(    void *struct_ptr);
void Reload_Popup(   void *struct_ptr);
void Reload_MapHpBar(void *struct_ptr);

#endif /* RELOAD_H */
