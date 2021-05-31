#ifndef filesystem_H
#define filesystem_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "physfs.h"
#include "platform.h"
#include "cJSON.h"
#include "lodepng.h"
#include "tile.h"
#include "narrative.h"
#include "enums.h"
#include "structs.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"

extern SDL_Renderer * Game_renderer;

#define ASSET_LIB "assets.binou"
#define LOGFILE "log.txt"

extern int32_t filesystem_init(char * argvZero, char * baseDir, char * assetsPath);
extern void loadFileToMemory(const char * name, uint8_t ** mem, size_t * len, bool addnull);
extern void filesystem_log(void * userdata, int32_t category, SDL_LogPriority priority, const char * message);

extern SDL_Texture * loadTexture(struct SDL_Renderer * in_renderer, const char * filename, const bool ZIP);
extern SDL_Texture * textToTexture(struct SDL_Renderer * in_renderer, char textureText[], struct SDL_Color textColor, TTF_Font * in_font);
extern void writeText(struct SDL_Renderer * in_renderer, int32_t in_fontsize, int32_t in_position[2], float in_sizefactor[2], char in_text[], struct SDL_Color in_color, TTF_Font * in_font);

extern void readJSON_mvtcost(struct cJSON * in_jcost, struct Movement_cost * in_cost);
// extern void readJSON_lines(struct cJSON * in_jlines, struct Dialog_line * in_line);
extern void readJSON_Tilestats(struct cJSON * in_jstats, struct Tile_stats * in_stats);
extern void readJSON_Unitstats(struct cJSON * in_jstats, struct Unit_stats * in_stats);
extern void readJSON_Wpnstats(struct cJSON * in_jstats, struct Weapon_stats * in_stats);
extern void readJSON_item(struct cJSON * in_jitem, struct Inventory_item * in_item);

extern int16_t * readJSON_2DArray(struct cJSON * in_jtilemap, int16_t * in_tilemap, uint8_t row_len, uint8_t col_len);
extern void writeJSON_2DArray(struct cJSON * in_jtilemap, int16_t * in_tilemap, uint8_t row_len, uint8_t col_len);

// extern void readJSON_narrative(struct cJSON * in_jnarrative, struct Narrative_Conditions * in_state);
// extern void writeJSON_narrative(struct cJSON * in_jnarrative, struct Narrative_Conditions * in_state);

extern void readJSON_arrival(struct cJSON * in_jarrival, struct Map_arrival * in_arrival);
extern void writeJSON_arrival(struct cJSON * in_jarrival, struct Map_arrival * in_arrival);
// extern void writeJSON_lines(struct cJSON * in_jlines, struct Dialog_line * in_line);

extern void writeJSON_RNseed(struct cJSON * in_jRandom);
extern void readJSON_RNseed(struct cJSON * in_jRandom);

extern void writeJSON_item(cJSON * in_jitems, struct Inventory_item * in_items);
extern void writeJSON_Wpnstats(struct cJSON * in_jstats, struct Weapon_stats * in_stats);
extern void writeJSON_Unitstats(struct cJSON * in_jstats, struct Unit_stats * in_stats);
extern void writeJSON_Tilestats(struct cJSON * in_jstats, struct Tile_stats * in_stats);
extern void writeJSON_mvtcost(struct cJSON * in_jcost, struct Movement_cost * in_cost);
extern void printJSON(PHYSFS_file * in_fp, struct cJSON * in_json);

#endif /* filesystem_H */
