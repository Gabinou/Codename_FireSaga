
#include "filesystem.h"
#include "stb_ds.h"

void filesystem_log(void * userdata, int category, SDL_LogPriority priority, const char * message) {
    FILE * logf = fopen(LOGFILE, "a");
    fprintf(logf, message);
    fprintf(logf, "\n");
    fclose(logf);
    printf("%s", message);
    printf("\n");
}

int filesystem_init(char * argvZero, char * baseDir, char * assetsPath) {
    char output[DEFAULT_MAXPATH] = "";
    int mkdirResult;
    // const char * separator = PHYSFS_getDirSeparator();
    char temp[DEFAULT_MAXPATH] = "";
    PHYSFS_init(argvZero);
    PHYSFS_permitSymbolicLinks(1);
    PHYSFS_setSaneConfig("AvgBear", "CodenameFiresaga", "bsa", 0, 0);
    // bsa: bear strategic archive
    /* Determine the OS user directory */
    if (output && strlen(baseDir) > 0) {
        strcpy(output, baseDir);
        /* We later append to this path and assume it ends in a slash */
        if (output[strlen(output) - 1] != DIR_SEPARATOR[0]) {
            strcat(output, DIR_SEPARATOR);
        }
    }
    SDL_Log("Mounting %s", output);
    if (!PHYSFS_mount(output, NULL, 1)) {
        SDL_Log("Could not mount %s", output);
        exit(ERROR_PHYSFSCannotMount);
    };
    PHYSFS_setWriteDir(output);
    SDL_Log("Base directory: %s\n", output);

    /* Create save directory */
    strcpy(temp, output);
    strcat(temp, "saves"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };
    temp[0] = '\0';

    strcpy(temp, output);
    strcat(temp, "maps"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };

    strcpy(temp, output);
    strcat(temp, "build"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "tiles"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "items"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "units"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "assets");
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "assets"DIR_SEPARATOR);
    strcat(temp, "Tiles"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };
    temp[0] = '\0';

    // SDL_Log("Save directory: %s\n", saveDir);
    if (assetsPath) {
        strcpy(output, assetsPath);
        strcat(output, DIR_SEPARATOR);
        strcat(output, "assets.binou");
    } else {
        strcpy(output, PHYSFS_getBaseDir());
        strcat(output, DIR_SEPARATOR);
        strcat(output, "assets.binou");
    }
    SDL_Log("SDL_GetBasePath() %s\n", SDL_GetBasePath());
    SDL_Log("Path to assets: %s\n", output);
    if (!PHYSFS_mount(output, NULL, 1)) {
        if (!PHYSFS_mount("../assets.binou", NULL, 1)) {
            SDL_Log("Missing assets.binou: PHYSFS_ERROR: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing assets.binou", "Missing assets.binou", NULL);
            exit(ERROR_MissingAssets);
        }
    }
    strcpy(output, baseDir);
    // strcat(output, separator);
    strcat(output, DIR_SEPARATOR"gamecontrollerdb.txt");
    SDL_Log("Path to gamecontrollerdb: %s\n", output);
    if (SDL_GameControllerAddMappingsFromFile(output) < 0) {
        SDL_Log("gamecontrollerdb.txt not found!\n");
    }
    return 1;
}

void loadFileToMemory(const char * name, uint8_t ** mem, size_t * len, bool addnull) {
    SDL_Log("Loading %s to memory", name);
    PHYSFS_File * physfs_file = PHYSFS_openRead(name);
    SDL_assert(physfs_file != NULL);
    PHYSFS_uint32 length = PHYSFS_fileLength(physfs_file);
    if (len != NULL) {
        *len = length;
    }
    if (addnull) {
        *mem = (uint8_t *) SDL_malloc(length + 1);
        (*mem)[length] = 0;
    } else {
        *mem = (uint8_t *) SDL_malloc(length);
    }
    PHYSFS_readBytes(physfs_file, *mem, length);
    PHYSFS_close(physfs_file);
}


void deinit() {
    PHYSFS_deinit();
}

struct SDL_Surface * ZIP_loadSurface(const char * filename, bool noBlend, bool noAlpha) {
    struct SDL_Surface * loadedImage = NULL; //Temporary storage for image
    struct SDL_Surface * optimizedImage = NULL; //optimized image to be used
    uint8_t * data;
    uint8_t * fileIn = NULL;
    uint32_t width, height;
    size_t length = 0;
    loadFileToMemory(filename, &fileIn, &length, false);
    if (noAlpha) {
        lodepng_decode24(&data, &width, &height, fileIn, length);
    } else {
        lodepng_decode32(&data, &width, &height, fileIn, length);
    }
    free(fileIn);
    fileIn = NULL;
    loadedImage = SDL_CreateRGBSurfaceFrom(
                      data,
                      width,
                      height,
                      noAlpha ? 24 : 32,
                      width * (noAlpha ? 3 : 4),
                      0x000000FF,
                      0x0000FF00,
                      0x00FF0000,
                      noAlpha ? 0x00000000 : 0xFF000000
                  );
    if (loadedImage != NULL) {
        optimizedImage = SDL_ConvertSurfaceFormat(
                             loadedImage,
                             SDL_PIXELFORMAT_ABGR8888,
                             0
                         );
        SDL_FreeSurface(loadedImage);
        free(data);
        if (noBlend) {
            SDL_SetSurfaceBlendMode(optimizedImage, SDL_BLENDMODE_BLEND);
        }
        return optimizedImage;
    } else {
        SDL_Log("Image not found: %s\n", filename);
        exit(ERROR_CannotOpenFile);
    }
}

SDL_Texture * loadTexture(struct SDL_Renderer * in_renderer, const char * filename, const bool ZIP) {
    struct SDL_Surface * tempsurface;
    struct SDL_Texture * texture;
    SDL_Log("LoadTexture: %s\n", filename);
    if (ZIP) {
        tempsurface = ZIP_loadSurface(filename, false, false); // How fast is this?
    } else {
        tempsurface = IMG_Load(filename); // Not that fast.
    }
    if (tempsurface == NULL) {
        SDL_Log("Tile Load error. Surface is NULL: %s\n", IMG_GetError());
    } else {
        texture = SDL_CreateTextureFromSurface(in_renderer, tempsurface);
    }
    if (texture == NULL) {
        SDL_Log("Tile Load error. CreateTextureFromSurface failed: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(tempsurface);
    return (texture);
}

// void writeText(struct SDL_Renderer * in_renderer, int in_fontsize, int in_position[2], float in_sizefactor[2], char in_text[], struct SDL_Color in_color, TTF_Font * in_font) {
//     char text[DEFAULT_BUFFER_SIZE] = "FPS";
//     struct SDL_Texture * texture = textToTexture(in_renderer, text, in_color, in_font);
//     SDL_Rect srcrect, destrect;
//     srcrect.x = srcrect.y = 0;
//     destrect.x = in_position[0];
//     destrect.y = in_position[1];
//     srcrect.h = in_fontsize;
//     // srcrect.w = text.length() * in_fontsize;
//     destrect.h = (int)in_fontsize * in_sizefactor[0];
//     // destrect.w = (int)text.length() * in_fontsize * in_sizefactor[1];
//     SDL_RenderCopy(in_renderer, texture, &srcrect, &destrect);
//     SDL_RenderPresent(in_renderer);
// }

// void writeJSON_lines(struct cJSON * in_jline, struct Dialog_line * in_lines) {
// struct cJSON * jconditions = cJSON_CreateObject();
// struct cJSON * jcondition;
// struct cJSON * jid;
// struct cJSON * jdead;
// struct cJSON * jrecruited;
// for (int8_t i = 0; i < in_line->conditions_num; i++) {
//     jcondition = cJSON_CreateObject();
//     jdead = cJSON_CreateBool(in_line->conditions[i].dead);
//     jrecruited = cJSON_CreateBool(in_line->conditions[i].recruited);
//     jid = cJSON_CreateNumber(in_line->conditions[i].unitid);
//     cJSON_AddItemToObject(jcondition, "Unit id", jid);
//     cJSON_AddItemToObject(jcondition, "Dead", jdead);
//     cJSON_AddItemToObject(jcondition, "Recruited", jrecruited);
//     cJSON_AddItemToObject(jconditions, "Condition", jcondition);
// }
// cJSON_AddItemToObject(in_jline, "Conditions", jconditions);
// struct cJSON * jspeaker = cJSON_CreateNumber(in_line->speaker);
// cJSON_AddItemToObject(in_jline, "Speaker", jspeaker);
// struct cJSON * jlinestr = cJSON_CreateString(in_line->line);
// cJSON_AddItemToObject(in_jline, "Linestr", jlinestr);
// }

// void readJSON_lines(struct cJSON * in_jlines, struct Dialog_line * in_line) {
//     // SDL_Log("readJSON_lines");
//     struct cJSON * jid;
//     struct cJSON * jline;
//     struct cJSON * jdead;
//     struct cJSON * jrecruited;
//     // struct Condition temp_cond = Condition_default;
//     struct Condition * temp_cond = NULL;
//     struct cJSON * jconditions;
//     struct cJSON * jcondition;
//     uint8_t cond_len = 0;
//     arrfree(in_line->conditions);
//     jline = cJSON_GetObjectItem(in_jlines, "Line");
//     while (jline != NULL) {
//         jconditions = cJSON_GetObjectItem(jline, "Conditions");
//         if (jconditions != NULL) {
//             jcondition = cJSON_GetObjectItem(jconditions, "Condition");
//             while (jcondition != NULL) {
//                 // SDL_Log("in condition.");
//                 jid = cJSON_GetObjectItem(jcondition, "Unit id");
//                 jdead = cJSON_GetObjectItem(jcondition, "Dead");
//                 jrecruited = cJSON_GetObjectItem(jcondition, "Recruited");
//                 arraddn(temp_cond, 1);
//                 cond_len = arrlen(temp_cond);
//                 temp_cond[cond_len - 1].dead = cJSON_IsTrue(jdead);
//                 temp_cond[cond_len - 1].recruited = cJSON_IsTrue(jrecruited);
//                 temp_cond[cond_len - 1].unitid = cJSON_GetNumberValue(jid);
//                 arrput(in_line->conditions, arrlast(temp_cond));
//                 jcondition = jcondition->next;
//             }
//             jconditions = jconditions->next;
//         }
//         struct cJSON * jlinestr = cJSON_GetObjectItem(jline, "Linestr");
//         // SDL_Log("%s", line);
//         arrput(in_line->lines, cJSON_GetStringValue(jlinestr));
//         // SDL_Log("%s", arrlast(in_line->lines));
//         struct cJSON * jspeaker = cJSON_GetObjectItem(jline, "Speaker");
//         in_line->speaker = cJSON_GetNumberValue(jspeaker);
//         jline = jline->next;
//     }
//     // SDL_Log("readJSON_line_out");
// }

// void readJSON_narrative(struct cJSON * in_jnarrative, struct Narrative_Conditions * in_state) {
//     struct cJSON * jchapter = cJSON_GetObjectItem(in_jnarrative, "Chapter");
//     struct cJSON * jdeaths = cJSON_GetObjectItem(in_jnarrative, "Deaths");
//     struct cJSON * jrecruited = cJSON_GetObjectItem(in_jnarrative, "Recruited");
//     in_state->chapter = cJSON_GetNumberValue(jchapter);
//     struct cJSON * jdeath;
//     struct cJSON * jdied;
//     int16_t i = UNIT_NAME_ERWIN;
//     struct cJSON * junit = cJSON_GetObjectItem(jdeaths, "Unit");
//     while (junit != NULL) {
//         in_state->death[i] = cJSON_IsTrue(jdied);
//         junit = junit->next;
//         i++;
//     }
//     i = UNIT_NAME_ERWIN;
//     junit = cJSON_GetObjectItem(jrecruited, "Unit");
//     while (junit != NULL) {
//         in_state->recruited[i] = cJSON_IsTrue(jdied);
//         junit = junit->next;
//         i++;
//     }
// }

// void writeJSON_narrative(struct cJSON * in_jnarrative, struct Narrative_Conditions * in_state) {
//     if (in_jnarrative != NULL) {
//         struct cJSON * jdeath = cJSON_CreateObject();
//         struct cJSON * jrecruited = cJSON_CreateObject();
//         struct cJSON * jisdead;
//         struct cJSON * jisrecruited;
//         struct cJSON * jname;
//         struct cJSON * junit1;
//         struct cJSON * junit2;
//         struct cJSON * jid;
//         struct cJSON * jchapter = cJSON_CreateNumber(in_state->chapter);
//         cJSON_AddItemToObject(in_jnarrative, "Chapter", jchapter);
//         cJSON_AddItemToObject(in_jnarrative, "Deaths", jdeath);
//         cJSON_AddItemToObject(in_jnarrative, "Recruited", jrecruited);
//         bool buffbool;
//         char name[DEFAULT_BUFFER_SIZE];
//         char buffer[DEFAULT_BUFFER_SIZE];
//         for (uint32_t i = UNIT_NAME_ERWIN; i < UNIT_NAME_NPC_END; i++) {
//             // name = unitNames[i];
//             if (i < UNIT_NAME_PC_END) {
//                 buffbool = in_state->recruited[i - UNIT_NAME_ERWIN];
//                 jid = cJSON_CreateNumber(i);
//                 jname = cJSON_CreateString(name);
//                 junit2 = cJSON_CreateObject();
//                 jisrecruited = cJSON_CreateBool(buffbool);
//                 cJSON_AddItemToObject(jrecruited, "Unit", junit2);
//                 cJSON_AddItemToObject(junit2, "id", jid);
//                 cJSON_AddItemToObject(junit2, "Name", jname);
//                 cJSON_AddItemToObject(junit2, "Recruited", jisrecruited);
//             }
//             buffbool = in_state->death[i - UNIT_NAME_ERWIN];
//             junit1 = cJSON_CreateObject();
//             jid = cJSON_CreateNumber(i);
//             jisdead = cJSON_CreateBool(buffbool);
//             jname = cJSON_CreateString(name);
//             cJSON_AddItemToObject(jdeath, "Unit", junit1);
//             cJSON_AddItemToObject(junit1, "id", jid);
//             cJSON_AddItemToObject(junit1, "Name", jname);
//             cJSON_AddItemToObject(junit1, "Died", jisdead);
//         }
//     } else {
//         SDL_Log("in_jnarrative is NULL");
//         exit(ERROR_JSONStructNull);
//     }
// }

void readJSON_mvtcost(struct cJSON * in_jcost, struct Movement_cost * in_cost) {
    SDL_assert(in_cost != NULL);
    struct cJSON * jfoot_slow = cJSON_GetObjectItem(in_jcost, "foot_slow");
    struct cJSON * jfoot_fast = cJSON_GetObjectItem(in_jcost, "foot_fast");
    struct cJSON * jmages = cJSON_GetObjectItem(in_jcost, "mages");
    struct cJSON * jriders_slow = cJSON_GetObjectItem(in_jcost, "riders_slow");
    struct cJSON * jriders_fast = cJSON_GetObjectItem(in_jcost, "riders_fast");
    struct cJSON * jfliers = cJSON_GetObjectItem(in_jcost, "fliers");
    struct cJSON * jarmors = cJSON_GetObjectItem(in_jcost, "armors");
    struct cJSON * jpirates = cJSON_GetObjectItem(in_jcost, "pirates");
    struct cJSON * jbandits = cJSON_GetObjectItem(in_jcost, "bandits");
    in_cost->foot_slow = cJSON_GetNumberValue(jfoot_slow);
    in_cost->foot_fast = cJSON_GetNumberValue(jfoot_fast);
    in_cost->mages = cJSON_GetNumberValue(jmages);
    in_cost->riders_slow = cJSON_GetNumberValue(jriders_slow);
    in_cost->riders_fast = cJSON_GetNumberValue(jriders_fast);
    in_cost->fliers = cJSON_GetNumberValue(jfliers);
    in_cost->armors = cJSON_GetNumberValue(jarmors);
    in_cost->pirates = cJSON_GetNumberValue(jpirates);
    in_cost->bandits = cJSON_GetNumberValue(jbandits);
}

int16_t * readJSON_2DArray(struct cJSON * in_array, int16_t * in_2DArray, uint8_t row_len, uint8_t col_len) {
    SDL_Log("readJSON_2DArray");
    SDL_assert(in_array != NULL);

    if (in_2DArray != NULL) {
        arrfree(in_2DArray);
    }
    arrsetlen(in_2DArray, row_len * col_len);
    char rowname[8];
    struct cJSON * jrow = cJSON_GetObjectItem(in_array, "row 0");
    struct cJSON * jnum;
    uint8_t row = 0;
    uint8_t col = 0;
    while ((jrow != NULL) && (row < row_len)) {
        col = 0;
        cJSON_ArrayForEach(jnum, jrow) {
            if (col < col_len) {
                in_2DArray[(row * col_len + col)] = (int16_t)cJSON_GetNumberValue(jnum);
                col++;
            } else {
                SDL_Log("readJSON_2DArray: wrong number of columns");
                exit(ERROR_ArraySize);
            }
        }
        row++;
        stbsp_sprintf(rowname, "row %d", row);
        jrow = cJSON_GetObjectItem(in_array, rowname);
    }
    return (in_2DArray);
}

void writeJSON_2DArray(struct cJSON * in_array, int16_t * in_2DArray, uint8_t row_len, uint8_t col_len) {
    SDL_Log("writeJSON_2DArray %d %d", row_len, col_len);
    SDL_assert(in_array != NULL);

    struct cJSON * jrow;
    struct cJSON * jnum;
    char rowname[DEFAULT_BUFFER_SIZE / 8];
    for (uint8_t row = 0; row < row_len; row++) {
        jrow = cJSON_CreateArray();
        for (uint8_t col = 0; col < col_len; col++) {
            // SDL_Log("elem %d %d %d", (row * col_len + col), row, col);
            jnum = cJSON_CreateNumber(in_2DArray[(row * col_len + col)]);
            cJSON_AddItemToArray(jrow, jnum);
        }
        stbsp_sprintf(rowname, "row %d", row);
        cJSON_AddItemToObject(in_array, rowname, jrow);
    }
}

void writeJSON_mvtcost(struct cJSON * in_jcost, struct Movement_cost * in_cost) {
    SDL_assert(in_jcost != NULL);
    struct cJSON * jfoot_slow = cJSON_CreateNumber(in_cost->foot_slow);
    struct cJSON * jfoot_fast = cJSON_CreateNumber(in_cost->foot_fast);
    struct cJSON * jmages = cJSON_CreateNumber(in_cost->mages);
    struct cJSON * jriders_slow = cJSON_CreateNumber(in_cost->riders_slow);
    struct cJSON * jriders_fast = cJSON_CreateNumber(in_cost->riders_fast);
    struct cJSON * jfliers = cJSON_CreateNumber(in_cost->fliers);
    struct cJSON * jarmors = cJSON_CreateNumber(in_cost->armors);
    struct cJSON * jpirates = cJSON_CreateNumber(in_cost->pirates);
    struct cJSON * jbandits = cJSON_CreateNumber(in_cost->bandits);
    cJSON_AddItemToObject(in_jcost, "foot_slow", jfoot_slow);
    cJSON_AddItemToObject(in_jcost, "foot_fast", jfoot_fast);
    cJSON_AddItemToObject(in_jcost, "mages", jmages);
    cJSON_AddItemToObject(in_jcost, "fliers", jfliers);
    cJSON_AddItemToObject(in_jcost, "riders_slow", jriders_slow);
    cJSON_AddItemToObject(in_jcost, "riders_fast", jriders_fast);
    cJSON_AddItemToObject(in_jcost, "armors", jarmors);
    cJSON_AddItemToObject(in_jcost, "pirates", jpirates);
    cJSON_AddItemToObject(in_jcost, "bandits", jbandits);
}

void readJSON_Tilestats(struct cJSON * in_jstats, struct Tile_stats * in_stats) {
    SDL_Log("readJSON_Tilestats");
    SDL_assert(in_jstats != NULL);
    struct cJSON * jdodge = cJSON_GetObjectItem(in_jstats, "Dodge");
    struct cJSON * jPprot = cJSON_GetObjectItem(in_jstats, "Pprot");
    struct cJSON * jMprot = cJSON_GetObjectItem(in_jstats, "Mprot");
    struct cJSON * jHeal = cJSON_GetObjectItem(in_jstats, "Heal");
    in_stats->dodge = cJSON_GetNumberValue(jdodge);
    in_stats->Pprot = cJSON_GetNumberValue(jPprot);
    in_stats->Mprot = cJSON_GetNumberValue(jMprot);
    in_stats->heal = cJSON_GetNumberValue(jHeal);
}

void readJSON_arrival(struct cJSON * in_jarrival, struct Map_arrival * in_arrival) {
    SDL_Log("readJSON_arrival");
    SDL_assert(in_jarrival != NULL);
    struct cJSON * jid = cJSON_GetObjectItem(in_jarrival, "id");
    struct cJSON * jarmy = cJSON_GetObjectItem(in_jarrival, "army");
    struct cJSON * jlevelups = cJSON_GetObjectItem(in_jarrival, "levelups");
    struct cJSON * jturn = cJSON_GetObjectItem(in_jarrival, "turn");
    struct cJSON * jposition = cJSON_GetObjectItem(in_jarrival, "position");
    struct cJSON * jrow = cJSON_GetObjectItem(jposition, "row");
    struct cJSON * jcol = cJSON_GetObjectItem(jposition, "col");
    in_arrival->turn = cJSON_GetNumberValue(jturn);
    in_arrival->id = cJSON_GetNumberValue(jid);
    in_arrival->army = cJSON_GetNumberValue(jarmy);
    in_arrival->levelups = cJSON_GetNumberValue(jlevelups);
    in_arrival->position.x = cJSON_GetNumberValue(jrow);
    in_arrival->position.y = cJSON_GetNumberValue(jcol);
}

void writeJSON_arrival(struct cJSON * in_jarrival, struct Map_arrival * in_arrival) {
    SDL_Log("writeJSON_arrival");
    SDL_assert(in_jarrival != NULL);
    struct cJSON * jid = cJSON_CreateNumber(in_arrival->id);
    struct cJSON * jturn = cJSON_CreateNumber(in_arrival->turn);
    struct cJSON * jarmy = cJSON_CreateNumber(in_arrival->army);
    struct cJSON * jlevelups = cJSON_CreateNumber(in_arrival->levelups);
    struct cJSON * jposition = cJSON_CreateObject();
    struct cJSON * jrow = cJSON_CreateNumber(in_arrival->position.x);
    struct cJSON * jcol = cJSON_CreateNumber(in_arrival->position.y);
    cJSON_AddItemToObject(jposition, "row", jrow);
    cJSON_AddItemToObject(jposition, "col", jcol);
    cJSON_AddItemToObject(in_jarrival, "id", jid);
    cJSON_AddItemToObject(in_jarrival, "army", jarmy);
    cJSON_AddItemToObject(in_jarrival, "levelups", jlevelups);
    cJSON_AddItemToObject(in_jarrival, "turn", jturn);
    cJSON_AddItemToObject(in_jarrival, "Position", jposition);
}


void writeJSON_Tilestats(struct cJSON * in_jstats, struct Tile_stats * in_stats) {
    SDL_Log("writeJSON_Tilestats");
    SDL_assert(in_jstats != NULL);
    struct cJSON * jdodge = cJSON_CreateNumber(in_stats->dodge);
    struct cJSON * jPprot = cJSON_CreateNumber(in_stats->Pprot);
    struct cJSON * jMprot = cJSON_CreateNumber(in_stats->Mprot);
    struct cJSON * jheal = cJSON_CreateNumber(in_stats->heal);
    cJSON_AddItemToObject(in_jstats, "Dodge", jdodge);
    cJSON_AddItemToObject(in_jstats, "Pprot", jPprot);
    cJSON_AddItemToObject(in_jstats, "Mprot", jMprot);
    cJSON_AddItemToObject(in_jstats, "Heal", jheal);
}

void writeJSON_Unitstats(struct cJSON * in_jstats, struct Unit_stats * in_stats) {
    SDL_Log("writeJSON_Unitstats");
    SDL_assert(in_jstats != NULL);
    struct cJSON * php = NULL;
    struct cJSON * pstr = NULL;
    struct cJSON * pmag = NULL;
    struct cJSON * pagi = NULL;
    struct cJSON * pdex = NULL;
    struct cJSON * pluck = NULL;
    struct cJSON * pdef = NULL;
    struct cJSON * pres = NULL;
    struct cJSON * pcon = NULL;
    struct cJSON * pmove = NULL;
    struct cJSON * pprof = NULL;
    php = cJSON_CreateNumber(in_stats->hp);
    pstr = cJSON_CreateNumber(in_stats->str);
    pmag = cJSON_CreateNumber(in_stats->mag);
    pagi = cJSON_CreateNumber(in_stats->agi);
    pdex = cJSON_CreateNumber(in_stats->dex);
    pluck = cJSON_CreateNumber(in_stats->luck);
    pdef = cJSON_CreateNumber(in_stats->def);
    pres = cJSON_CreateNumber(in_stats->res);
    pcon = cJSON_CreateNumber(in_stats->con);
    pmove = cJSON_CreateNumber(in_stats->move);
    pprof = cJSON_CreateNumber(in_stats->prof);
    cJSON_AddItemToObject(in_jstats, "hp", php);
    cJSON_AddItemToObject(in_jstats, "str", pstr);
    cJSON_AddItemToObject(in_jstats, "mag", pmag);
    cJSON_AddItemToObject(in_jstats, "agi", pagi);
    cJSON_AddItemToObject(in_jstats, "dex", pdex);
    cJSON_AddItemToObject(in_jstats, "luck", pluck);
    cJSON_AddItemToObject(in_jstats, "def", pdef);
    cJSON_AddItemToObject(in_jstats, "res", pres);
    cJSON_AddItemToObject(in_jstats, "con", pcon);
    cJSON_AddItemToObject(in_jstats, "move", pmove);
    cJSON_AddItemToObject(in_jstats, "prof", pprof);
}

void writeJSON_Wpnstats(struct cJSON * in_jstats, struct Weapon_stats * in_stats) {
    SDL_Log("writeJSON_Wpnstats");
    SDL_assert(in_jstats != NULL);
    struct cJSON * pPmight = NULL;
    struct cJSON * pMmight = NULL;
    struct cJSON * phit = NULL;
    struct cJSON * pdodge = NULL;
    struct cJSON * pcrit = NULL;
    struct cJSON * pfavor = NULL;
    struct cJSON * pwgt = NULL;
    struct cJSON * puses =  NULL;
    struct cJSON * pprof =  NULL;
    struct cJSON * pminrange =  NULL;
    struct cJSON * pmaxrange =  NULL;
    struct cJSON * pminhand =  NULL;
    struct cJSON * pmaxhand =  NULL;
    struct cJSON * pdmg_type =  NULL;
    pPmight = cJSON_CreateNumber(in_stats->Pmight);
    pMmight = cJSON_CreateNumber(in_stats->Mmight);
    phit = cJSON_CreateNumber(in_stats->combat.hit);
    pdodge = cJSON_CreateNumber(in_stats->combat.dodge);
    pcrit = cJSON_CreateNumber(in_stats->combat.crit);
    pfavor = cJSON_CreateNumber(in_stats->combat.favor);
    pwgt = cJSON_CreateNumber(in_stats->wgt);
    puses = cJSON_CreateNumber(in_stats->uses);
    pprof = cJSON_CreateNumber(in_stats->prof);
    pminrange = cJSON_CreateNumber(in_stats->range[0]);
    pmaxrange = cJSON_CreateNumber(in_stats->range[1]);
    pminhand = cJSON_CreateNumber(in_stats->hand[0]);
    pmaxhand = cJSON_CreateNumber(in_stats->hand[1]);
    pdmg_type = cJSON_CreateNumber(in_stats->dmg_type);
    cJSON_AddItemToObject(in_jstats, "Pmight", pPmight);
    cJSON_AddItemToObject(in_jstats, "Mmight", pMmight);
    cJSON_AddItemToObject(in_jstats, "hit", phit);
    cJSON_AddItemToObject(in_jstats, "dodge", pdodge);
    cJSON_AddItemToObject(in_jstats, "crit", pcrit);
    cJSON_AddItemToObject(in_jstats, "favor", pfavor);
    cJSON_AddItemToObject(in_jstats, "wgt", pwgt);
    cJSON_AddItemToObject(in_jstats, "uses", puses);
    cJSON_AddItemToObject(in_jstats, "prof", pprof);
    cJSON_AddItemToObject(in_jstats, "minrange", pminrange);
    cJSON_AddItemToObject(in_jstats, "maxrange", pmaxrange);
    cJSON_AddItemToObject(in_jstats, "minhand", pminhand);
    cJSON_AddItemToObject(in_jstats, "maxhand", pmaxhand);
    cJSON_AddItemToObject(in_jstats, "dmg_type", pdmg_type);
}

void readJSON_item(struct cJSON * in_jitem, struct Inventory_item * in_item) {
    SDL_Log("readJSON_item");
    SDL_assert(in_jitem != NULL);
    struct cJSON * jinfused = cJSON_GetObjectItem(in_jitem, "Infused");
    struct cJSON * jid = cJSON_GetObjectItem(in_jitem, "id");
    struct cJSON * jused = cJSON_GetObjectItem(in_jitem, "used");
    in_item->id = cJSON_GetNumberValue(jid);
    in_item->used = cJSON_GetNumberValue(jused);
    in_item->infused = cJSON_GetNumberValue(jinfused);
}

void writeJSON_item(struct cJSON * in_jitems, struct Inventory_item * in_item) {
    SDL_Log("writeJSON_item");
    SDL_assert(in_jitems != NULL);
    cJSON * jitem = NULL;
    cJSON * jused = NULL;
    cJSON * jid = NULL;
    cJSON * jwpnname = NULL;
    cJSON * jinfused = NULL;
    char buffer[DEFAULT_BUFFER_SIZE];
    jitem = cJSON_CreateObject();
    jid = cJSON_CreateNumber(in_item->id);
    jused = cJSON_CreateNumber(in_item->used);
    if (in_item->infused > 0) {
        jinfused = cJSON_CreateNumber(in_item->infused);
    } else {
        jinfused = cJSON_CreateNumber(-1);
    }
    cJSON_AddItemToObject(jitem, "Infused", jinfused);
    cJSON_AddItemToObject(jitem, "id", jid);
    cJSON_AddItemToObject(jitem, "used", jused);
    cJSON_AddItemToObject(in_jitems, "Item", jitem);
}

void readJSON_Wpnstats(struct cJSON * in_jstats, struct Weapon_stats * in_stats) {
    SDL_Log("readJSON_Wpnstats");
    SDL_assert(in_jstats != NULL);
    struct cJSON * jPmight = cJSON_GetObjectItem(in_jstats, "Pmight");
    struct cJSON * jMmight = cJSON_GetObjectItem(in_jstats, "Mmight");
    struct cJSON * jhit = cJSON_GetObjectItem(in_jstats, "hit");
    struct cJSON * jdodge = cJSON_GetObjectItem(in_jstats, "dodge");
    struct cJSON * jcrit = cJSON_GetObjectItem(in_jstats, "crit");
    struct cJSON * jfavor = cJSON_GetObjectItem(in_jstats, "favor");
    struct cJSON * jwgt = cJSON_GetObjectItem(in_jstats, "wgt");
    struct cJSON * juses = cJSON_GetObjectItem(in_jstats, "uses");
    struct cJSON * jprof = cJSON_GetObjectItem(in_jstats, "prof");
    struct cJSON * jminrange = cJSON_GetObjectItem(in_jstats, "minrange");
    struct cJSON * jmaxrange = cJSON_GetObjectItem(in_jstats, "maxrange");
    struct cJSON * jminhand = cJSON_GetObjectItem(in_jstats, "minhand");
    struct cJSON * jmaxhand = cJSON_GetObjectItem(in_jstats, "maxhand");
    struct cJSON * jdmg_type = cJSON_GetObjectItem(in_jstats, "dmg_type");
    in_stats->Pmight = cJSON_GetNumberValue(jPmight);
    in_stats->Mmight = cJSON_GetNumberValue(jMmight);
    in_stats->combat.hit = cJSON_GetNumberValue(jhit);
    in_stats->combat.dodge = cJSON_GetNumberValue(jdodge);
    in_stats->combat.crit = cJSON_GetNumberValue(jcrit);
    in_stats->combat.favor = cJSON_GetNumberValue(jfavor);
    in_stats->wgt = cJSON_GetNumberValue(jwgt);
    in_stats->uses = cJSON_GetNumberValue(juses);
    in_stats->prof = cJSON_GetNumberValue(jprof);
    in_stats->range[0] = cJSON_GetNumberValue(jminrange);
    in_stats->range[1] = cJSON_GetNumberValue(jmaxrange);
    in_stats->hand[0] = cJSON_GetNumberValue(jminhand);
    in_stats->hand[1] = cJSON_GetNumberValue(jmaxhand);
    in_stats->dmg_type = cJSON_GetNumberValue(jdmg_type);
}

void readJSON_Unitstats(struct cJSON * in_jstats, struct Unit_stats * in_stats) {
    SDL_Log("readJSON_Unitstats");
    SDL_assert(in_jstats != NULL);
    struct cJSON * jhp = cJSON_GetObjectItem(in_jstats, "hp");
    struct cJSON * jstr = cJSON_GetObjectItem(in_jstats, "str");
    struct cJSON * jmag = cJSON_GetObjectItem(in_jstats, "mag");
    struct cJSON * jagi = cJSON_GetObjectItem(in_jstats, "agi");
    struct cJSON * jdex = cJSON_GetObjectItem(in_jstats, "dex");
    struct cJSON * jluck = cJSON_GetObjectItem(in_jstats, "luck");
    struct cJSON * jdef = cJSON_GetObjectItem(in_jstats, "def");
    struct cJSON * jres = cJSON_GetObjectItem(in_jstats, "res");
    struct cJSON * jcon = cJSON_GetObjectItem(in_jstats, "con");
    struct cJSON * jmove = cJSON_GetObjectItem(in_jstats, "move");
    struct cJSON * jprof = cJSON_GetObjectItem(in_jstats, "prof");
    in_stats->hp = cJSON_GetNumberValue(jhp);
    in_stats->str = cJSON_GetNumberValue(jstr);
    in_stats->mag = cJSON_GetNumberValue(jmag);
    in_stats->agi = cJSON_GetNumberValue(jagi);
    in_stats->dex = cJSON_GetNumberValue(jdex);
    in_stats->luck = cJSON_GetNumberValue(jluck);
    in_stats->def = cJSON_GetNumberValue(jdef);
    in_stats->res = cJSON_GetNumberValue(jres);
    in_stats->con = cJSON_GetNumberValue(jcon);
    in_stats->move = cJSON_GetNumberValue(jmove);
    in_stats->prof = cJSON_GetNumberValue(jprof);
}

void printJSON(PHYSFS_file * in_fp, struct cJSON * in_json) {
    char * buffer = NULL;
    buffer = cJSON_Print(in_json);
    uint64_t length = strlen(buffer);
    if (!PHYSFS_setBuffer(in_fp, length)) {
        SDL_Log("PHYSFS_setBuffer failed");
        exit(ERROR_CannotWriteFile);
    } else {
        PHYSFS_writeBytes(in_fp, buffer, length);
    }
}

