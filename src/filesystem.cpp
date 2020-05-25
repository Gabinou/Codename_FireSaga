
#include "filesystem.hpp"
#include "game.hpp"
// #ifndef STB_SPRINTF_IMPLEMENTATION //Why no need?
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
//#endif /* STB_SPRINTF_IMPLEMENTATION */

#if defined(_WIN32)
#include <windows.h>
int mkdir(char * path, int mode) {
    WCHAR utf16_path[DEFAULT::MAXPATH];
    MultiByteToWideChar(CP_UTF8, 0, path, -1, utf16_path, DEFAULT::MAXPATH);
    return CreateDirectoryW(utf16_path, NULL);
}
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__HAIKU__)
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#endif

namespace FILESYSTEM {

void log(void * userdata, int category, SDL_LogPriority priority, const char * message) {
    FILE * logf = fopen(LOGFILE, "a");
    fprintf(logf, message);
    fprintf(logf, "\n");
    fclose(logf);
    printf(message);
    printf("\n");
}

int init(char * argvZero, char * baseDir, char * assetsPath) {
    char output[DEFAULT::MAXPATH];
    int mkdirResult;
    const char * pathSep = PHYSFS_getDirSeparator();
    char saveDir[DEFAULT::MAXPATH] = "save";

    PHYSFS_init(argvZero);
    PHYSFS_permitSymbolicLinks(1);

    PHYSFS_setSaneConfig("AvgBear", "CodenameFiresaga", "bsa", 0, 0);
    // bsar: bear strategic archive

    /* Determine the OS user directory */
    if (baseDir && strlen(baseDir) > 0) {
        strcpy(output, baseDir);

        /* We later append to this path and assume it ends in a slash */
        if (strcmp(std::string(1, output[strlen(output) - 1]).c_str(), pathSep) != 0) {
            strcat(output, pathSep);
        }
    }

    /* Mount our base user directory */
    PHYSFS_mount(output, NULL, 1);
    PHYSFS_setWriteDir(output);
    SDL_Log("Base directory: %s\n", output);

    /* Create save directory */
    strcpy(saveDir, output);
    strcat(saveDir, "saves");
    strcat(saveDir, PHYSFS_getDirSeparator());
    // mkdir(saveDir, 0777);
    SDL_Log("Save directory: %s\n", saveDir);

    if (assetsPath) {
        strcpy(output, assetsPath);
        strcat(output, "\\assets.binou");
    } else {
        strcpy(output, PHYSFS_getBaseDir());
        strcat(output, "\\assets.binou");
    }

    SDL_Log("Path to assets: %s\n", output);

    if (!PHYSFS_mount(output, NULL, 1)) {
        SDL_Log("Missing assets.binou\n");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing assets.binou", "Missing assets.binou", NULL);
        return 0;
    }

    strcpy(output, baseDir);
    strcat(output, "\\gamecontrollerdb.txt");
    SDL_Log("Path to gamecontrollerdb: %s\n", output);

    if (SDL_GameControllerAddMappingsFromFile(output) < 0) {
        SDL_Log("gamecontrollerdb.txt not found!\n");
    }

    return 1;
}

void loadFileToMemory(const char * name, unsigned char ** mem, size_t * len, bool addnull) {
    SDL_Log("Loading %s to memory", name);
    PHYSFS_File * physfs_file = PHYSFS_openRead(name);

    if (physfs_file == NULL) {
        return;
    }

    PHYSFS_uint32 length = PHYSFS_fileLength(physfs_file);

    if (len != NULL) {
        *len = length;
    }

    if (addnull) {
        *mem = (unsigned char *) malloc(length + 1);
        (*mem)[length] = 0;
    } else {
        *mem = (unsigned char *) malloc(length);
    }

    PHYSFS_readBytes(physfs_file, *mem, length);
    PHYSFS_close(physfs_file);
}


void deinit() {
    PHYSFS_deinit();
}

void freeMemory(unsigned char ** mem) {
    free(*mem);
    *mem = NULL;
}
}

SDL_Surface * ZIP_loadSurface(const char * filename, bool noBlend = true, bool noAlpha = false) {
    SDL_Surface * loadedImage = NULL; //Temporary storage for image
    SDL_Surface * optimizedImage = NULL; //optimized image to be used

    unsigned char * data;
    unsigned char * fileIn = NULL;
    unsigned int width, height;
    size_t length = 0;

    FILESYSTEM::loadFileToMemory(filename, &fileIn, &length);

    if (noAlpha) {
        lodepng_decode24(&data, &width, &height, fileIn, length);
    } else {
        lodepng_decode32(&data, &width, &height, fileIn, length);
    }

    FILESYSTEM::freeMemory(&fileIn);

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
        return NULL;
    }
}

SDL_Texture * loadTexture(SDL_Renderer * in_renderer, const char * filename, const bool ZIP) {
    SDL_Surface * tempsurface;
    SDL_Log("LoadTexture: %s\n", filename);

    if (ZIP) {
        tempsurface = ZIP_loadSurface(filename); // How fast is this?
    } else {
        tempsurface = IMG_Load(filename); // Not that fast.
    }

    if (tempsurface == NULL) {
        SDL_Log("loadTexture. IMG_Load error: %s\n", IMG_GetError());
    }

    SDL_Texture * texture = SDL_CreateTextureFromSurface(in_renderer, tempsurface);

    if (texture == NULL) {
        SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
    }

    return (texture);
}

SDL_Texture * textToTexture(SDL_Renderer * in_renderer, std::string textureText, SDL_Color textColor, TTF_Font * in_font) {
    SDL_Surface * textsurface = TTF_RenderText_Blended(in_font, textureText.c_str(), textColor); // Blended is better than Solid
    // TTF_RenderText_Shaded shades the whole destrect background.

    SDL_Texture * texture;

    if (textsurface == NULL) {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {

        texture = SDL_CreateTextureFromSurface(in_renderer, textsurface);

        if (texture == NULL) {
            SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }

        SDL_FreeSurface(textsurface);
    }

    return (texture);
}

void writeText(SDL_Renderer * in_renderer, int in_fontsize, int in_position[2], float in_sizefactor[2], std::string in_text, SDL_Color in_color, TTF_Font * in_font) {
    std::string text = "FPS";
    SDL_Texture * texture = textToTexture(in_renderer, text, in_color, in_font);
    SDL_Rect srcrect, destrect;
    srcrect.x = srcrect.y = 0;
    destrect.x = in_position[0];
    destrect.y = in_position[1];
    srcrect.h = in_fontsize;
    srcrect.w = text.length() * in_fontsize;
    destrect.h = (int)in_fontsize * in_sizefactor[0];
    destrect.w = (int)text.length() * in_fontsize * in_sizefactor[1];
    SDL_RenderCopy(in_renderer, texture, &srcrect, &destrect);
    SDL_RenderPresent(in_renderer);
}

void printXMLDoc(PHYSFS_file * in_fp, tinyxml2::XMLDocument * in_doc) {
    tinyxml2::XMLPrinter printer;
    in_doc->Print(&printer);
    char longbuffer[printer.CStrSize()];
    stbsp_sprintf(longbuffer, printer.CStr());

    if (!PHYSFS_setBuffer(in_fp, printer.CStrSize())) {
        SDL_Log("PHYSFS_setBuffer failed");
    } else {
        PHYSFS_writeBytes(in_fp, longbuffer, printer.CStrSize());
    }
}

void readXML_stats(tinyxml2::XMLElement * in_pStats, Weapon_stats * in_stats) {
    tinyxml2::XMLElement * ptemp = in_pStats->FirstChildElement("Pmight");
    unsigned int buffint;
    ptemp->QueryUnsignedText(&buffint);
    in_stats->Pmight = (unsigned char)buffint;

    ptemp = in_pStats->FirstChildElement("Mmight");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->Mmight = (unsigned char)buffint;

    ptemp = in_pStats->FirstChildElement("hit");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->combat.hit = (unsigned char)buffint;
    ptemp = in_pStats->FirstChildElement("dodge");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->combat.dodge = (unsigned char)buffint;
    ptemp = in_pStats->FirstChildElement("crit");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->combat.crit = (unsigned char)buffint;
    ptemp = in_pStats->FirstChildElement("favor");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->combat.favor = (unsigned char)buffint;
    ptemp = in_pStats->FirstChildElement("wgt");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->wgt = (unsigned char)buffint;
    ptemp = in_pStats->FirstChildElement("uses");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->uses = (unsigned char)buffint;
    ptemp = in_pStats->FirstChildElement("prof");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->prof = (unsigned char)buffint;
    ptemp = in_pStats->FirstChildElement("minrange");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->range[0] = (unsigned char)buffint;
    ptemp = in_pStats->FirstChildElement("maxrange");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->range[1] = (unsigned char)buffint;
    ptemp = in_pStats->FirstChildElement("minhand");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->hand[0] = (unsigned char)buffint;
    ptemp = in_pStats->FirstChildElement("maxhand");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->hand[1] = (unsigned char)buffint;
    ptemp = in_pStats->FirstChildElement("dmg_type");
    ptemp->QueryBoolText(&in_stats->dmg_type);
    ptemp = in_pStats->FirstChildElement("price");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->price = (unsigned short int)buffint;
    ptemp = in_pStats->FirstChildElement("heal");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->heal = (unsigned char)buffint;
}

void readXML_stats(tinyxml2::XMLElement * in_pStats, Unit_stats * in_stats) {
    tinyxml2::XMLElement * ptemp = in_pStats->FirstChildElement("hp");
    unsigned int buffint;
    ptemp->QueryUnsignedText(&buffint);
    in_stats->hp = (unsigned char)buffint;

    ptemp = in_pStats->FirstChildElement("str");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->str = (unsigned char)buffint;

    ptemp = in_pStats->FirstChildElement("mag");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->mag = (unsigned char)buffint;

    ptemp = in_pStats->FirstChildElement("agi");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->agi = (unsigned char)buffint;

    ptemp = in_pStats->FirstChildElement("dex");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->dex = (unsigned char)buffint;

    ptemp = in_pStats->FirstChildElement("luck");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->luck = (unsigned char)buffint;

    ptemp = in_pStats->FirstChildElement("def");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->def = (unsigned char)buffint;

    ptemp = in_pStats->FirstChildElement("res");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->res = (unsigned char)buffint;

    ptemp = in_pStats->FirstChildElement("con");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->con = (unsigned char)buffint;

    ptemp = in_pStats->FirstChildElement("move");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->move = (unsigned char)buffint;

    ptemp = in_pStats->FirstChildElement("prof");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->prof = (unsigned char)buffint;
}

void readXML_narrativeUnits(tinyxml2::XMLElement * in_pUnit, bool in_statearr[]) {
    unsigned int buffint;
    bool buffbool;
    short unsigned int id;
    tinyxml2::XMLElement * ptemp;

    while (in_pUnit) {
        id = (unsigned short int)ptemp->IntAttribute("id");
        ptemp = in_pUnit->FirstChildElement("Died");
        ptemp->QueryBoolText(&buffbool);
        in_statearr[id] = buffbool;
        in_pUnit = in_pUnit->NextSiblingElement("Unit");
    }
}

void readXML_narrative(tinyxml2::XMLElement * in_pNarrative, Narrative * in_state) {
    unsigned int buffint;
    bool buffbool;

    tinyxml2::XMLElement * ppc_death = in_pNarrative->FirstChildElement("Death");
    tinyxml2::XMLElement * precruited = in_pNarrative->FirstChildElement("Recruited");
    tinyxml2::XMLElement * pUnit;

    tinyxml2::XMLElement * pchapter = in_pNarrative->FirstChildElement("Chapter");
    pchapter->QueryUnsignedText(&buffint);
    in_state->chapter = buffint;

    pUnit = ppc_death->FirstChildElement("Unit");
    readXML_narrativeUnits(pUnit, in_state->death);
    pUnit = precruited->FirstChildElement("Unit");
    readXML_narrativeUnits(pUnit, in_state->recruited);
}

void readJSON_narrative(cJSON * in_json, Narrative * in_state) {
    cJSON * jnarrative = cJSON_GetObjectItem(in_json, "Narrative");
    cJSON * jchapter = cJSON_GetObjectItem(jnarrative, "Chapter");
    cJSON * jdeaths = cJSON_GetObjectItem(jnarrative, "Deaths");
    cJSON * jrecruited = cJSON_GetObjectItem(jnarrative, "Recruited");
    in_state->chapter = cJSON_GetNumberValue(jchapter);
    cJSON * jdeath;
    cJSON * jdied;
    short int i = UNIT::NAME::ERWIN;

    cJSON * junit = cJSON_GetObjectItem(jdeaths, "Unit");

    while (junit != NULL) {
        in_state->death[i] = cJSON_IsTrue(jdied);
        junit = junit->next;
        i++;
    }

    i = UNIT::NAME::ERWIN;
    junit = cJSON_GetObjectItem(jrecruited, "Unit");

    while (junit != NULL) {
        in_state->recruited[i] = cJSON_IsTrue(jdied);
        junit = junit->next;
        i++;
    }
}

void writeJSON_narrative(cJSON * in_json, Narrative * in_state) {
    cJSON * jnarrative = cJSON_CreateObject();
    cJSON_AddItemToObject(in_json, "Narrative", jnarrative);
    cJSON * jdeath = cJSON_CreateObject();
    cJSON * jrecruited = cJSON_CreateObject();
    cJSON * jisdead;
    cJSON * jisrecruited;
    cJSON * jname;
    cJSON * junit1;
    cJSON * junit2;
    cJSON * jid;
    cJSON * jchapter = cJSON_CreateNumber(in_state->chapter);
    cJSON_AddItemToObject(jnarrative, "Chapter", jchapter);
    cJSON_AddItemToObject(jnarrative, "Deaths", jdeath);
    cJSON_AddItemToObject(jnarrative, "Recruited", jrecruited);

    bool buffbool;
    std::string name;
    char buffer[DEFAULT::BUFFER_SIZE];

    for (unsigned int i = UNIT::NAME::ERWIN; i < UNIT::NAME::NPC_END; i++) {
        name = unitNames[i];

        if (i < UNIT::NAME::PC_END) {
            buffbool = in_state->recruited[i - UNIT::NAME::ERWIN];
            jid = cJSON_CreateNumber(i);
            jname = cJSON_CreateString(name.c_str());
            junit2 = cJSON_CreateObject();
            jisrecruited = cJSON_CreateBool(buffbool);
            cJSON_AddItemToObject(jrecruited, "Unit", junit2);
            cJSON_AddItemToObject(junit2, "id", jid);
            cJSON_AddItemToObject(junit2, "Name", jname);
            cJSON_AddItemToObject(junit2, "Recruited", jisrecruited);
        }

        buffbool = in_state->death[i - UNIT::NAME::ERWIN];
        junit1 = cJSON_CreateObject();
        jid = cJSON_CreateNumber(i);
        jisdead = cJSON_CreateBool(buffbool);
        jname = cJSON_CreateString(name.c_str());
        cJSON_AddItemToObject(jdeath, "Unit", junit1);
        cJSON_AddItemToObject(junit1, "id", jid);
        cJSON_AddItemToObject(junit1, "Name", jname);
        cJSON_AddItemToObject(junit1, "Died", jisdead);
    }
}

void writeXML_narrative(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pNarrative, Narrative * in_state) {
    tinyxml2::XMLElement * pdeath = in_doc->NewElement("Death");
    tinyxml2::XMLElement * precruited = in_doc->NewElement("Recruited");
    tinyxml2::XMLElement * pchapter = in_doc->NewElement("Chapter");
    in_pNarrative->InsertEndChild(pchapter);
    in_pNarrative->InsertEndChild(pdeath);
    in_pNarrative->InsertEndChild(precruited);

    pchapter->SetText(in_state->chapter);

    tinyxml2::XMLElement * ptemp1;
    tinyxml2::XMLElement * ptemp2;
    tinyxml2::XMLElement * ptemp3;
    bool buffbool;
    std::string name;
    char buffer[DEFAULT::BUFFER_SIZE];

    for (unsigned int i = UNIT::NAME::ERWIN; i < UNIT::NAME::NPC_END; i++) {
        name = unitNames[i];
        buffbool = in_state->death[i - UNIT::NAME::ERWIN];
        ptemp1 = in_doc->NewElement("Unit");
        ptemp2 = in_doc->NewElement("Died");
        ptemp3 = in_doc->NewElement("Name");
        pdeath->InsertEndChild(ptemp1);
        ptemp1->InsertEndChild(ptemp3);
        ptemp1->InsertEndChild(ptemp2);
        ptemp2->SetText(buffbool);
        ptemp3->SetText(name.c_str());
        ptemp1->SetAttribute("id", i);

        if (i < UNIT::NAME::PC_END) {
            buffbool = in_state->recruited[i - UNIT::NAME::ERWIN];
            ptemp1 = in_doc->NewElement("Unit");
            ptemp2 = in_doc->NewElement("Recruited");
            ptemp3 = in_doc->NewElement("Name");
            precruited->InsertEndChild(ptemp1);
            ptemp1->InsertEndChild(ptemp3);
            ptemp1->InsertEndChild(ptemp2);
            ptemp2->SetText(buffbool);
            ptemp3->SetText(name.c_str());
            ptemp1->SetAttribute("id", i);
        }
    }
}

void readJSON_mvtcost(cJSON * in_jcost, Movement_cost * in_cost) {
    cJSON * jfoot_slow = cJSON_GetObjectItem(in_jcost, "foot_slow");
    cJSON * jfoot_fast = cJSON_GetObjectItem(in_jcost, "foot_fast");
    cJSON * jmages = cJSON_GetObjectItem(in_jcost, "mages");
    cJSON * jriders_slow = cJSON_GetObjectItem(in_jcost, "riders_slow");
    cJSON * jriders_fast = cJSON_GetObjectItem(in_jcost, "riders_fast");
    cJSON * jfliers = cJSON_GetObjectItem(in_jcost, "fliers");
    cJSON * jarmors = cJSON_GetObjectItem(in_jcost, "armors");
    cJSON * jpirates = cJSON_GetObjectItem(in_jcost, "pirates");
    cJSON * jbandits = cJSON_GetObjectItem(in_jcost, "bandits");

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

std::vector<std::vector<short int>> readJSON_tilemap(cJSON * in_jtilemap) {
    std::vector<std::vector<short int>> out;
    std::vector<short int> row_temp;
    short int rownum = 0;
    char rowname[8];
    cJSON * jrow;
    cJSON * jnum;
    jrow = cJSON_GetObjectItem(in_jtilemap, "row 0");

    while (jrow != NULL) {
        row_temp.clear();
        cJSON_ArrayForEach(jnum, jrow) {
            row_temp.push_back(cJSON_GetNumberValue(jnum));
        }
        out.push_back(row_temp);
        rownum++;
        stbsp_sprintf(rowname, "row %d", rownum);
        // SDL_Log("rowname %s", rowname);
        jrow = cJSON_GetObjectItem(in_jtilemap, rowname);
    }

    return (out);
}

void writeJSON_tilemap(cJSON * in_jtilemap, std::vector<std::vector<short int>> in_tilemap) {
    cJSON * jrow;
    cJSON * jnum;
    char rowname[8];

    jrow = cJSON_CreateArray();

    for (int row = 0; row < in_tilemap.size(); row++) {// This loop cache friendly.
        jrow = cJSON_CreateArray();

        for (int col = 0; col < in_tilemap[row].size(); col++) {
            jnum = cJSON_CreateNumber(in_tilemap[row][col]);
            cJSON_AddItemToArray(jrow, jnum);
        }

        stbsp_sprintf(rowname, "row %d", row);
        cJSON_AddItemToObject(in_jtilemap, rowname, jrow);
    }
}

void writeJSON_mvtcost(cJSON * in_jcost, Movement_cost * in_cost) {
    cJSON * jfoot_slow = cJSON_CreateNumber(in_cost->foot_slow);
    cJSON * jfoot_fast = cJSON_CreateNumber(in_cost->foot_fast);
    cJSON * jmages = cJSON_CreateNumber(in_cost->mages);
    cJSON * jriders_slow = cJSON_CreateNumber(in_cost->riders_slow);
    cJSON * jriders_fast = cJSON_CreateNumber(in_cost->riders_fast);
    cJSON * jfliers = cJSON_CreateNumber(in_cost->fliers);
    cJSON * jarmors = cJSON_CreateNumber(in_cost->armors);
    cJSON * jpirates = cJSON_CreateNumber(in_cost->pirates);
    cJSON * jbandits = cJSON_CreateNumber(in_cost->bandits);
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

void readJSON_stats(cJSON * in_jstats, Tile_stats * in_stats) {
    cJSON * jdodge = cJSON_GetObjectItem(in_jstats, "Dodge");
    cJSON * jPprot = cJSON_GetObjectItem(in_jstats, "Pprot");
    cJSON * jMprot = cJSON_GetObjectItem(in_jstats, "Mprot");
    cJSON * jHeal = cJSON_GetObjectItem(in_jstats, "Heal");

    in_stats->dodge = cJSON_GetNumberValue(jdodge);
    in_stats->Pprot = cJSON_GetNumberValue(jPprot);
    in_stats->Mprot = cJSON_GetNumberValue(jMprot);
    in_stats->heal = cJSON_GetNumberValue(jHeal);
}

void readJSON_arrival(cJSON * in_jarrival, Map_arrival * in_arrival) {
    cJSON * jid = cJSON_GetObjectItem(in_jarrival, "id");
    cJSON * jarmy = cJSON_GetObjectItem(in_jarrival, "army");
    cJSON * jlevelups = cJSON_GetObjectItem(in_jarrival, "levelups");
    cJSON * jturn = cJSON_GetObjectItem(in_jarrival, "turn");
    cJSON * jposition = cJSON_GetObjectItem(in_jarrival, "position");
    cJSON * jrow = cJSON_GetObjectItem(jposition, "row");
    cJSON * jcol = cJSON_GetObjectItem(jposition, "col");

    in_arrival->turn = cJSON_GetNumberValue(jturn);
    in_arrival->id = cJSON_GetNumberValue(jid);
    in_arrival->army = cJSON_GetNumberValue(jarmy);
    in_arrival->levelups = cJSON_GetNumberValue(jlevelups);
    in_arrival->position.x = cJSON_GetNumberValue(jrow);
    in_arrival->position.y = cJSON_GetNumberValue(jcol);
}

void writeJSON_arrival(cJSON * in_jarrival, Map_arrival * in_arrival) {
    cJSON * jid = cJSON_CreateNumber(in_arrival->id);
    cJSON * jturn = cJSON_CreateNumber(in_arrival->turn);
    cJSON * jarmy = cJSON_CreateNumber(in_arrival->army);
    cJSON * jlevelups = cJSON_CreateNumber(in_arrival->levelups);
    cJSON * jposition = cJSON_CreateObject();
    cJSON * jrow = cJSON_CreateNumber(in_arrival->position.x);
    cJSON * jcol = cJSON_CreateNumber(in_arrival->position.y);

    cJSON_AddItemToObject(jposition, "row", jrow);
    cJSON_AddItemToObject(jposition, "col", jcol);
    cJSON_AddItemToObject(in_jarrival, "id", jid);
    cJSON_AddItemToObject(in_jarrival, "army", jarmy);
    cJSON_AddItemToObject(in_jarrival, "levelups", jlevelups);
    cJSON_AddItemToObject(in_jarrival, "turn", jturn);
    cJSON_AddItemToObject(in_jarrival, "Position", jposition);
}


void writeJSON_stats(cJSON * in_jstats, Tile_stats * in_stats) {
    cJSON * jdodge = cJSON_CreateNumber(in_stats->dodge);
    cJSON * jPprot = cJSON_CreateNumber(in_stats->Pprot);
    cJSON * jMprot = cJSON_CreateNumber(in_stats->Mprot);
    cJSON * jheal = cJSON_CreateNumber(in_stats->heal);
    cJSON_AddItemToObject(in_jstats, "Dodge", jdodge);
    cJSON_AddItemToObject(in_jstats, "Pprot", jPprot);
    cJSON_AddItemToObject(in_jstats, "Mprot", jMprot);
    cJSON_AddItemToObject(in_jstats, "Heal", jheal);
}

void writeJSON_stats(cJSON * in_jstats, Unit_stats * in_stats) {
    cJSON * php = NULL;
    cJSON * pstr = NULL;
    cJSON * pmag = NULL;
    cJSON * pagi = NULL;
    cJSON * pdex = NULL;
    cJSON * pluck = NULL;
    cJSON * pdef = NULL;
    cJSON * pres = NULL;
    cJSON * pcon = NULL;
    cJSON * pmove = NULL;
    cJSON * pprof = NULL;
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

void writeJSON_stats(cJSON * in_jstats, Weapon_stats * in_stats) {
    cJSON * pPmight = NULL;
    cJSON * pMmight = NULL;
    cJSON * phit = NULL;
    cJSON * pdodge = NULL;
    cJSON * pcrit = NULL;
    cJSON * pfavor = NULL;
    cJSON * pwgt = NULL;
    cJSON * puses =  NULL;
    cJSON * pprof =  NULL;
    cJSON * pminrange =  NULL;
    cJSON * pmaxrange =  NULL;
    cJSON * pminhand =  NULL;
    cJSON * pmaxhand =  NULL;
    cJSON * pdmg_type =  NULL;
    cJSON * pprice =  NULL;
    cJSON * pheal =  NULL;
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
    pprice = cJSON_CreateNumber(in_stats->price);
    pheal = cJSON_CreateNumber(in_stats->heal);
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
    cJSON_AddItemToObject(in_jstats, "price", pprice);
    cJSON_AddItemToObject(in_jstats, "heal", pheal);
}

void writeXML_stats(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pStats, Weapon_stats * in_stats) {
    // Pmight, Mmight, hit, dodge, crit, favor, wgt, uses, prof, range, hand, dmg_type, cost
    tinyxml2::XMLElement * pPmight = in_doc->NewElement("Pmight");
    tinyxml2::XMLElement * pMmight = in_doc->NewElement("Mmight");
    tinyxml2::XMLElement * phit = in_doc->NewElement("hit");
    tinyxml2::XMLElement * pdodge = in_doc->NewElement("dodge");
    tinyxml2::XMLElement * pcrit = in_doc->NewElement("crit");
    tinyxml2::XMLElement * pfavor = in_doc->NewElement("favor");
    tinyxml2::XMLElement * pwgt = in_doc->NewElement("wgt");
    tinyxml2::XMLElement * puses = in_doc->NewElement("uses");
    tinyxml2::XMLElement * pprof = in_doc->NewElement("prof");
    tinyxml2::XMLElement * pminrange = in_doc->NewElement("minrange");
    tinyxml2::XMLElement * pmaxrange = in_doc->NewElement("maxrange");
    tinyxml2::XMLElement * phand1 = in_doc->NewElement("minhand");
    tinyxml2::XMLElement * phand2 = in_doc->NewElement("maxhand");
    tinyxml2::XMLElement * pdmg_type = in_doc->NewElement("dmg_type");
    tinyxml2::XMLElement * pprice = in_doc->NewElement("price");
    tinyxml2::XMLElement * pheal = in_doc->NewElement("heal");
    in_pStats->InsertEndChild(pPmight);
    in_pStats->InsertEndChild(pMmight);
    in_pStats->InsertEndChild(phit);
    in_pStats->InsertEndChild(pdodge);
    in_pStats->InsertEndChild(pcrit);
    in_pStats->InsertEndChild(pfavor);
    in_pStats->InsertEndChild(pwgt);
    in_pStats->InsertEndChild(puses);
    in_pStats->InsertEndChild(pprof);
    in_pStats->InsertEndChild(pminrange);
    in_pStats->InsertEndChild(pmaxrange);
    in_pStats->InsertEndChild(phand1);
    in_pStats->InsertEndChild(phand2);
    in_pStats->InsertEndChild(pdmg_type);
    in_pStats->InsertEndChild(pprice);
    in_pStats->InsertEndChild(pheal);
    pPmight->SetText(in_stats->Pmight);
    pMmight->SetText(in_stats->Mmight);
    phit->SetText(in_stats->combat.hit);
    pdodge->SetText(in_stats->combat.dodge);
    pcrit->SetText(in_stats->combat.crit);
    pfavor->SetText(in_stats->combat.favor);
    pwgt->SetText(in_stats->wgt);
    puses->SetText(in_stats->uses);
    pprof->SetText(in_stats->prof);
    pminrange->SetText(in_stats->range[0]);
    pmaxrange->SetText(in_stats->range[1]);
    phand1->SetText(in_stats->hand[0]);
    phand2->SetText(in_stats->hand[1]);
    pdmg_type->SetText(in_stats->dmg_type);
    pprice->SetText(in_stats->price);
    pdmg_type->SetAttribute("eg", "isMagic");
    pheal->SetText(in_stats->heal);
}

void readXML_arrival(tinyxml2::XMLElement * in_pArrival, Map_arrival * in_arrival) {
    unsigned int buffint;

    tinyxml2::XMLElement * pTurn = in_pArrival->FirstChildElement("Turn");
    tinyxml2::XMLElement * plevelups = in_pArrival->FirstChildElement("levelups");
    tinyxml2::XMLElement * pPosition = in_pArrival->FirstChildElement("Position");

    pTurn->QueryUnsignedText(&buffint);
    in_arrival->turn = buffint;
    in_arrival->position.x = (short int)pPosition->IntAttribute("row");
    in_arrival->position.y = (short int)pPosition->IntAttribute("col");
    plevelups->QueryUnsignedText(&buffint);
    in_arrival->levelups = buffint;
    in_arrival->id = (short int)in_pArrival->IntAttribute("unitid");
}

void writeXML_arrival(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pArrival, Map_arrival * in_arrival) {
    tinyxml2::XMLElement * pTurn = in_doc->NewElement("Turn");
    tinyxml2::XMLElement * pLevels = in_doc->NewElement("levelups");
    tinyxml2::XMLElement * pPosition = in_doc->NewElement("Position");
    tinyxml2::XMLElement * pRow = in_doc->NewElement("Row");
    tinyxml2::XMLElement * pCol = in_doc->NewElement("Col");
    in_pArrival->InsertEndChild(pTurn);
    in_pArrival->InsertEndChild(pLevels);
    in_pArrival->InsertEndChild(pPosition);
    pPosition->InsertEndChild(pRow);
    pPosition->InsertEndChild(pCol);
    in_pArrival->SetAttribute("unitid", in_arrival->id);
    pTurn->SetText(in_arrival->turn);
    pLevels->SetText(in_arrival->levelups);
    pPosition->SetAttribute("row", in_arrival->position.x);
    pPosition->SetAttribute("col", in_arrival->position.y);
    pRow->SetText(in_arrival->position.x);
    pCol->SetText(in_arrival->position.y);
}

void writeXML_stats(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pStats, Unit_stats * in_stats) {
    tinyxml2::XMLElement * php = in_doc->NewElement("hp");
    tinyxml2::XMLElement * pstr = in_doc->NewElement("str");
    tinyxml2::XMLElement * pmag = in_doc->NewElement("mag");
    tinyxml2::XMLElement * pagi = in_doc->NewElement("agi");
    tinyxml2::XMLElement * pdex = in_doc->NewElement("dex");
    tinyxml2::XMLElement * pluck = in_doc->NewElement("luck");
    tinyxml2::XMLElement * pdef = in_doc->NewElement("def");
    tinyxml2::XMLElement * pres = in_doc->NewElement("res");
    tinyxml2::XMLElement * pcon = in_doc->NewElement("con");
    tinyxml2::XMLElement * pmove = in_doc->NewElement("move");
    tinyxml2::XMLElement * pprof = in_doc->NewElement("prof");
    in_pStats->InsertEndChild(php);
    in_pStats->InsertEndChild(pstr);
    in_pStats->InsertEndChild(pmag);
    in_pStats->InsertEndChild(pagi);
    in_pStats->InsertEndChild(pdex);
    in_pStats->InsertEndChild(pluck);
    in_pStats->InsertEndChild(pdef);
    in_pStats->InsertEndChild(pres);
    in_pStats->InsertEndChild(pcon);
    in_pStats->InsertEndChild(pmove);
    in_pStats->InsertEndChild(pprof);
    php->SetText(in_stats->hp);
    pstr->SetText(in_stats->str);
    pmag->SetText(in_stats->mag);
    pagi->SetText(in_stats->agi);
    pdex->SetText(in_stats->dex);
    pluck->SetText(in_stats->luck);
    pdef->SetText(in_stats->def);
    pres->SetText(in_stats->res);
    pcon->SetText(in_stats->con);
    pmove->SetText(in_stats->move);
    pprof->SetText(in_stats->prof);
}

void readJSON_item(cJSON * in_jitem, Inventory_item * in_item) {
    cJSON * jinfused = cJSON_GetObjectItem(in_jitem, "Infused");
    cJSON * jid = cJSON_GetObjectItem(in_jitem, "id");
    cJSON * jused = cJSON_GetObjectItem(in_jitem, "used");

    in_item->id = cJSON_GetNumberValue(jid);
    in_item->used = cJSON_GetNumberValue(jused);
    in_item->infused = cJSON_GetNumberValue(jinfused);
}

void readJSON_stats(cJSON * in_jstats, Weapon_stats * in_stats) {
    cJSON * jPmight = cJSON_GetObjectItem(in_jstats, "Pmight");
    cJSON * jMmight = cJSON_GetObjectItem(in_jstats, "Mmight");
    cJSON * jhit = cJSON_GetObjectItem(in_jstats, "hit");
    cJSON * jdodge = cJSON_GetObjectItem(in_jstats, "dodge");
    cJSON * jcrit = cJSON_GetObjectItem(in_jstats, "crit");
    cJSON * jfavor = cJSON_GetObjectItem(in_jstats, "favor");
    cJSON * jwgt = cJSON_GetObjectItem(in_jstats, "wgt");
    cJSON * juses = cJSON_GetObjectItem(in_jstats, "uses");
    cJSON * jprof = cJSON_GetObjectItem(in_jstats, "prof");
    cJSON * jminrange = cJSON_GetObjectItem(in_jstats, "minrange");
    cJSON * jmaxrange = cJSON_GetObjectItem(in_jstats, "maxrange");
    cJSON * jminhand = cJSON_GetObjectItem(in_jstats, "minhand");
    cJSON * jmaxhand = cJSON_GetObjectItem(in_jstats, "maxhand");
    cJSON * jdmg_type = cJSON_GetObjectItem(in_jstats, "dmg_type");
    cJSON * jprice = cJSON_GetObjectItem(in_jstats, "price");
    cJSON * jheal = cJSON_GetObjectItem(in_jstats, "heal");

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
    in_stats->price = cJSON_GetNumberValue(jprice);
    in_stats->heal = cJSON_GetNumberValue(jheal);
}

void readJSON_stats(cJSON * in_jstats, Unit_stats * in_stats) {
    cJSON * jhp = cJSON_GetObjectItem(in_jstats, "hp");
    cJSON * jstr = cJSON_GetObjectItem(in_jstats, "str");
    cJSON * jmag = cJSON_GetObjectItem(in_jstats, "mag");
    cJSON * jagi = cJSON_GetObjectItem(in_jstats, "agi");
    cJSON * jdex = cJSON_GetObjectItem(in_jstats, "dex");
    cJSON * jluck = cJSON_GetObjectItem(in_jstats, "luck");
    cJSON * jdef = cJSON_GetObjectItem(in_jstats, "def");
    cJSON * jres = cJSON_GetObjectItem(in_jstats, "res");
    cJSON * jcon = cJSON_GetObjectItem(in_jstats, "con");
    cJSON * jmove = cJSON_GetObjectItem(in_jstats, "move");
    cJSON * jprof = cJSON_GetObjectItem(in_jstats, "prof");
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

void writeJSON_items(cJSON * in_jitems, std::vector<Inventory_item> in_items) {
    writeJSON_items(in_jitems, in_items.data(), in_items.size());
}

void writeJSON_items(cJSON * in_jitems, Inventory_item * in_items, int size) {
    cJSON * jitem = NULL;
    cJSON * jused = NULL;
    cJSON * jid = NULL;
    cJSON * jwpnname = NULL;
    cJSON * jinfused = NULL;
    char buffer[DEFAULT::BUFFER_SIZE];

    for (int i = 0; i < size; i++) {
        jitem = cJSON_CreateObject();
        jid = cJSON_CreateNumber(in_items[i].id);
        jused = cJSON_CreateNumber(in_items[i].used);

        if (in_items[i].infused > 0) {
            jinfused = cJSON_CreateNumber(in_items[i].infused);
        } else {
            jinfused = cJSON_CreateNumber(-1);
        }

        cJSON_AddItemToObject(jitem, "Infused", jinfused);
        cJSON_AddItemToObject(jitem, "id", jid);
        cJSON_AddItemToObject(jitem, "used", jused);
        cJSON_AddItemToObject(in_jitems, "Item", jitem);
    }
}

void writeXML_items(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pItems, std::vector<Inventory_item> in_items) {
    writeXML_items(in_doc, in_pItems, in_items.data(), in_items.size());
}

void writeXML_items(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pItems, Inventory_item * in_items, int size) {
    tinyxml2::XMLElement * pItem;
    tinyxml2::XMLElement * pUsed;
    tinyxml2::XMLElement * pInfused;
    char buffer[DEFAULT::BUFFER_SIZE];

    for (int i = 0; i < size; i++) {
        pItem = in_doc->NewElement("Item");
        in_pItems->InsertEndChild(pItem);
        pItem->SetAttribute("id", in_items[i].id);
        pUsed = in_doc->NewElement("Used");
        stbsp_sprintf(buffer, "%d", in_items[i].used);
        pUsed->SetText(buffer);
        pItem->InsertEndChild(pUsed);

        if (in_items[i].infused > 0) {
            pInfused = in_doc->NewElement("Infused");
            pItem->InsertEndChild(pInfused);
            pInfused->SetText(in_items[i].infused);
        }
    }
}

void readXML_items(tinyxml2::XMLElement * in_pItems, std::vector<Inventory_item> * equipment) {
    tinyxml2::XMLElement * pItem = in_pItems->FirstChildElement("Item");
    tinyxml2::XMLElement * pInfused;
    tinyxml2::XMLElement * pUsed;
    int buffint;
    Inventory_item temp;
    equipment->clear();

    while (pItem) {
        pUsed = pItem->FirstChildElement("Used");
        pInfused = pItem->FirstChildElement("Infused");

        if (pInfused) {
            pInfused->QueryIntText(&buffint);
            temp.infused = buffint;
        }

        temp.id = pItem->IntAttribute("id");
        pUsed->QueryIntText(&buffint);
        temp.used = buffint;
        equipment->push_back(temp);
        pItem = pItem->NextSiblingElement("Item");
    }
}

void readXML_items(tinyxml2::XMLElement * in_pItems, Inventory_item * in_items) {
    tinyxml2::XMLElement * pItem = in_pItems->FirstChildElement("Item");
    tinyxml2::XMLElement * pUsed;
    tinyxml2::XMLElement * pInfused;
    int buffint;
    int i = 0;

    while (pItem) {
        pUsed = pItem->FirstChildElement("Used");
        pInfused = pItem->FirstChildElement("Infused");

        if (pInfused) {
            pInfused->QueryIntText(&buffint);
            in_items[i].infused = buffint;
        }

        in_items[i].id = pItem->IntAttribute("id");
        pUsed->QueryIntText(&buffint);
        in_items[i].used = buffint;
        pItem = pItem->NextSiblingElement("Item");
        i++;
    }
}

void writeXML_mvtcost(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pCost, Movement_cost * in_cost) {
    tinyxml2::XMLElement * pfoot_slow = in_doc->NewElement("foot_slow");
    tinyxml2::XMLElement * pfoot_fast = in_doc->NewElement("foot_fast");
    tinyxml2::XMLElement * pmages = in_doc->NewElement("mages");
    tinyxml2::XMLElement * priders_slow = in_doc->NewElement("riders_slow");
    tinyxml2::XMLElement * priders_fast = in_doc->NewElement("riders_fast");
    tinyxml2::XMLElement * pfliers = in_doc->NewElement("fliers");
    tinyxml2::XMLElement * parmors = in_doc->NewElement("armors");
    tinyxml2::XMLElement * ppirates = in_doc->NewElement("pirates");
    tinyxml2::XMLElement * pbandits = in_doc->NewElement("bandits");
    in_pCost->InsertEndChild(pfoot_slow);
    in_pCost->InsertEndChild(pfoot_fast);
    in_pCost->InsertEndChild(pmages);
    in_pCost->InsertEndChild(priders_slow);
    in_pCost->InsertEndChild(priders_fast);
    in_pCost->InsertEndChild(pfliers);
    in_pCost->InsertEndChild(parmors);
    in_pCost->InsertEndChild(ppirates);
    in_pCost->InsertEndChild(pbandits);
    pfoot_slow->SetText(in_cost->foot_slow);
    pfoot_fast->SetText(in_cost->foot_fast);
    pmages->SetText(in_cost->mages);
    priders_slow->SetText(in_cost->riders_slow);
    priders_fast->SetText(in_cost->riders_fast);
    pfliers->SetText(in_cost->fliers);
    parmors->SetText(in_cost->armors);
    ppirates->SetText(in_cost->pirates);
    pbandits->SetText(in_cost->bandits);
}

void readXML_mvtcost(tinyxml2::XMLElement * in_pCost, Movement_cost * in_cost) {
    int buffint;

    tinyxml2::XMLElement * pfoot_slow = in_pCost->FirstChildElement("foot_slow");
    tinyxml2::XMLElement * pfoot_fast = in_pCost->FirstChildElement("foot_fast");
    tinyxml2::XMLElement * pmages = in_pCost->FirstChildElement("mages");
    tinyxml2::XMLElement * priders_slow = in_pCost->FirstChildElement("riders_slow");
    tinyxml2::XMLElement * priders_fast = in_pCost->FirstChildElement("riders_fast");
    tinyxml2::XMLElement * pfliers = in_pCost->FirstChildElement("fliers");
    tinyxml2::XMLElement * parmors = in_pCost->FirstChildElement("armors");
    tinyxml2::XMLElement * ppirates = in_pCost->FirstChildElement("pirates");
    tinyxml2::XMLElement * pbandits = in_pCost->FirstChildElement("bandits");

    pfoot_slow->QueryIntText(&buffint);
    in_cost->foot_slow = (unsigned char)buffint;
    pfoot_fast->QueryIntText(&buffint);
    in_cost->foot_fast = (unsigned char)buffint;

    pmages->QueryIntText(&buffint);
    in_cost->mages = (unsigned char)buffint;
    priders_slow->QueryIntText(&buffint);
    in_cost->riders_slow = (unsigned char)buffint;
    priders_fast->QueryIntText(&buffint);
    in_cost->riders_fast = (unsigned char)buffint;
    pfliers->QueryIntText(&buffint);
    in_cost->fliers = (unsigned char)buffint;
    parmors->QueryIntText(&buffint);
    in_cost->armors = (unsigned char)buffint;
    ppirates->QueryIntText(&buffint);
    in_cost->pirates = (unsigned char)buffint;
    pbandits->QueryIntText(&buffint);
    in_cost->bandits = (unsigned char)buffint;

}

void readXML_stats(tinyxml2::XMLElement * in_pStats, Tile_stats * in_stats) {
    int buffint;
    tinyxml2::XMLElement * pdodge = in_pStats->FirstChildElement("dodge");
    tinyxml2::XMLElement * pPprot = in_pStats->FirstChildElement("Pprot");
    tinyxml2::XMLElement * pMprot = in_pStats->FirstChildElement("Mprot");
    tinyxml2::XMLElement * pheal = in_pStats->FirstChildElement("heal");

    pdodge->QueryIntText(&buffint);
    in_stats->dodge = (char)buffint;
    pPprot->QueryIntText(&buffint);
    in_stats->Pprot = (char)buffint;
    pMprot->QueryIntText(&buffint);
    in_stats->Mprot = (char)buffint;
    pheal->QueryIntText(&buffint);
    in_stats->heal = (char)buffint;
}

void writeXML_stats(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pStats, Tile_stats * in_stats) {
    tinyxml2::XMLElement * pdodge = in_doc->NewElement("dodge");
    tinyxml2::XMLElement * pPprot = in_doc->NewElement("Pprot");
    tinyxml2::XMLElement * pMprot = in_doc->NewElement("Mprot");
    tinyxml2::XMLElement * pheal = in_doc->NewElement("heal");
    in_pStats->InsertEndChild(pdodge);
    in_pStats->InsertEndChild(pPprot);
    in_pStats->InsertEndChild(pMprot);
    in_pStats->InsertEndChild(pheal);
    pdodge->SetText(in_stats->dodge);
    pPprot->SetText(in_stats->Pprot);
    pMprot->SetText(in_stats->Mprot);
    pheal->SetText(in_stats->heal);
}

void printJSON(PHYSFS_file * in_fp, cJSON * in_json) {
    char * buffer = NULL;
    buffer = cJSON_Print(in_json);
    unsigned long int length = strlen(buffer);

    if (!PHYSFS_setBuffer(in_fp, length)) {
        SDL_Log("PHYSFS_setBuffer failed");
    } else {
        PHYSFS_writeBytes(in_fp, buffer, length);
    }
}


cJSON * parseJSON(const char * filename) {
    SDL_Log("Parsing JSON file %s", filename);
    PHYSFS_file * fp;
    fp = PHYSFS_openRead(filename);
    cJSON * json = NULL;

    if (!fp) {
        SDL_Log("Failed to open %s for JSON parsing.", filename);
    }

    unsigned int filelen = PHYSFS_fileLength(fp);
    // SDL_Log("filelen: %d", filelen);
    char filebuffer[filelen];
    PHYSFS_readBytes(fp, filebuffer, filelen);
    PHYSFS_close(fp);

    json = cJSON_ParseWithLength(filebuffer, filelen);
    // SDL_Log("in_json: %d", (in_json == NULL));
    return (json);
}


int parseXML(const char * filename, tinyxml2::XMLDocument * in_doc) {
    PHYSFS_file * fp;
    fp = PHYSFS_openRead(filename);

    if (!fp) {
        SDL_Log("Failed to open %s for XML parsing.", filename);
        return (-1);
    }

    unsigned int filelen = PHYSFS_fileLength(fp);
    char filebuffer[filelen];
    PHYSFS_readBytes(fp, filebuffer, filelen);
    PHYSFS_close(fp);

    if (in_doc->Parse(filebuffer, filelen) != 0) {
        SDL_Log("Failed to parse XML %s", filename);
        return (-1);
    }

    return (0);
}

void JSON_IO::readJSON(const char * filename) {
    SDL_Log("readJSON file: %s", filename);
    cJSON * json = parseJSON(filename);

    if (json != NULL) {
        if (JSONelement != "") {
            cJSON * jelement = cJSON_GetObjectItem(json, JSONelement.c_str());

            if (jelement == NULL) {
                SDL_Log("Cannot get JSON element %s"), JSONelement.c_str();
            } else {
                readJSON(jelement);
            }
        } else {
            SDL_Log("JSON element not set");
        }
    } else {
        SDL_Log("Cannot parse JSON file %s", filename);

    }
}

void JSON_IO::readJSON(cJSON * in_json) {

}

void XML_IO::readXML(const char * filename) {
    // SDL_Log("readXML file: %s", filename);
    tinyxml2::XMLDocument xmlDoc;
    parseXML(filename, &xmlDoc);
    tinyxml2::XMLElement * pEle = xmlDoc.FirstChildElement(XMLelement.c_str());

    if (!pEle) {
        SDL_Log("Cannot get %s element", XMLelement.c_str());
    } else {
        readXML(pEle);
    }
}

void XML_IO::readXML(tinyxml2::XMLElement * in_pEle) {

}

void XML_IO::writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pEle) {

}

void JSON_IO::writeJSON(cJSON * in_json) {

}

void JSON_IO::writeJSON(const char * filename, const bool append) {
    SDL_Log("Writing Json to %s:", filename);

    PHYSFS_file * fp = NULL;
    cJSON * json = cJSON_CreateObject();
    SDL_Log("JSON element: %s", JSONelement.c_str());

    if (JSONelement !=  "") {
        cJSON * jelement = cJSON_CreateObject();

        // SDL_Log("json==NULL%d", json == NULL);
        if (append) {
            fp = PHYSFS_openAppend(filename);
        } else {
            fp = PHYSFS_openWrite(filename);
        }

        if (!fp) {
            SDL_Log("Could not open %s for writing\n", filename);
        } else {
            writeJSON(jelement);
            cJSON_AddItemToObject(json, JSONelement.c_str(), jelement);

            printJSON(fp, json);
            PHYSFS_close(fp);
            cJSON_Delete(json);
        }
    }   else {
        SDL_Log("JSON element not set");
    }
}


void XML_IO::writeXML(const char * filename, const bool append) {
    // SDL_Log("writeXML %s to: %s\n", element.c_str(), filename);
    PHYSFS_file * fp;
    tinyxml2::XMLDocument xmlDoc;

    if (append) {
        fp = PHYSFS_openAppend(filename);
    } else {
        fp = PHYSFS_openWrite(filename);
        xmlDoc.InsertFirstChild(xmlDoc.NewDeclaration());
    }

    if (!fp) {
        SDL_Log("Could not open %s for %s writing\n", filename, XMLelement.c_str());
    } else {
        tinyxml2::XMLElement * pEle = xmlDoc.NewElement(XMLelement.c_str());
        xmlDoc.InsertEndChild(pEle);
        writeXML(&xmlDoc, pEle);
        printXMLDoc(fp, &xmlDoc);
        PHYSFS_close(fp);
    }
}

void XML_IO::setXMLElement(std::string in_Element) {
    XMLelement = in_Element;
}

std::string XML_IO::getXMLElement() {
    return (XMLelement);
}

void JSON_IO::setJSONElement(std::string in_Element) {
    JSONelement = in_Element;
}

std::string JSON_IO::getJSONElement() {
    return (JSONelement);
}

void path_removefolder(char * path) {
    path[strlen(path) - 1] = '\0';
    char * folder = strrchr(path, '\\') + 1;
    path[strlen(path) - strlen(folder)] = '\0';
}

char * path_topfolder(char * path) {
    char * temp;
    strcpy(temp, path);
    temp[strlen(temp) - 1] = '\0';
    char * folder = strrchr(temp, '\\') + 1;
    return (folder);
}

const char * fextension(const char * filename) {
    const char * dot = strrchr(filename, '.');

    if (!dot || dot == filename) { return ""; }

    return dot + 1;
}
