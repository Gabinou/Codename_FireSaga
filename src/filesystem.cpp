
#include "filesystem.hpp"
#include "game.hpp"
// #ifndef STB_SPRINTF_IMPLEMENTATION //Why no need?
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h" 
//#endif /* STB_SPRINTF_IMPLEMENTATION */

#if defined(_WIN32)
#include <windows.h>
int mkdir(char* path, int mode)
{
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

void log(void* userdata, int category, SDL_LogPriority priority, const char* message) {
    FILE * logf = fopen(LOGFILE, "a");
    fprintf(logf, message);
    fprintf(logf, "\n");
    fclose(logf);
    printf(message);
    printf("\n");
}

int init(char *argvZero, char* baseDir, char *assetsPath) {
    char output[DEFAULT::MAXPATH];
    int mkdirResult;
    const char * pathSep = PHYSFS_getDirSeparator();
    char saveDir[DEFAULT::MAXPATH] = "save";

    PHYSFS_init(argvZero);
    PHYSFS_permitSymbolicLinks(1);

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
    if (!PHYSFS_mount(output, NULL, 1))
    {
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

void loadFileToMemory(const char *name, unsigned char **mem, size_t *len, bool addnull) {
    PHYSFS_File *physfs_file = PHYSFS_openRead(name);
    if (physfs_file == NULL)
    {
        return;
    }
    PHYSFS_uint32 length = PHYSFS_fileLength(physfs_file);
    if (len != NULL)
    {
        *len = length;
    }
    if (addnull)
    {
        *mem = (unsigned char *) malloc(length + 1);
        (*mem)[length] = 0;
    }
    else
    {
        *mem = (unsigned char*) malloc(length);
    }
    PHYSFS_readBytes(physfs_file, *mem, length);
    PHYSFS_close(physfs_file);
}


void deinit() {
    PHYSFS_deinit();
}

void freeMemory(unsigned char **mem) {
    free(*mem);
    *mem = NULL;
}
}

SDL_Surface * ZIP_loadSurface(const char *filename, bool noBlend = true, bool noAlpha = false)
{
    SDL_Surface* loadedImage = NULL; //Temporary storage for image
    SDL_Surface* optimizedImage = NULL;//optimized image to be used

    unsigned char *data;
    unsigned char *fileIn = NULL;
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
        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(in_renderer, textsurface);

        if (texture == NULL) {
            SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(textsurface); //Get rid of old surface
    }
    return (texture);
}

void writeText(SDL_Renderer * in_renderer, int in_fontsize, int in_position[2], float in_sizefactor[2], std::string in_text, SDL_Color in_color, TTF_Font * in_font) {
    std::string text = "FPS";
    SDL_Texture * texture = textToTexture(in_renderer, text, in_color, in_font);
    SDL_Rect srcrect, destrect; // background always first?
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
    PHYSFS_writeBytes(in_fp, longbuffer, printer.CStrSize());
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
    ptemp = in_pStats->FirstChildElement("hand");
    ptemp->QueryUnsignedText(&buffint);
    in_stats->hand[0] = (unsigned char)buffint;
    ptemp = in_pStats->LastChildElement("hand");
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

    tinyxml2::XMLElement * ppc_death = in_pNarrative->FirstChildElement("Death_PC");
    tinyxml2::XMLElement * pnpc_death = in_pNarrative->FirstChildElement("Death_NPC");
    tinyxml2::XMLElement * precruited = in_pNarrative->FirstChildElement("Recruited");
    tinyxml2::XMLElement * pUnit;

    tinyxml2::XMLElement * pchapter = in_pNarrative->FirstChildElement("Chapter");
    pchapter->QueryUnsignedText(&buffint);
    in_state->chapter = buffint;

    pUnit = ppc_death->FirstChildElement("Unit");
    readXML_narrativeUnits(pUnit, in_state->pc_death);
    pUnit = pnpc_death->FirstChildElement("Unit");
    readXML_narrativeUnits(pUnit, in_state->npc_death);
    pUnit = precruited->FirstChildElement("Unit");
    readXML_narrativeUnits(pUnit, in_state->recruited);
}

void writeXML_narrative(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pNarrative, Narrative * in_state) {
    tinyxml2::XMLElement * ppc_death = in_doc->NewElement("Death_PC");
    tinyxml2::XMLElement * pnpc_death = in_doc->NewElement("Death_NPC");
    tinyxml2::XMLElement * precruited = in_doc->NewElement("Recruited");
    tinyxml2::XMLElement * pchapter = in_doc->NewElement("Chapter");
    in_pNarrative->InsertEndChild(pchapter);
    in_pNarrative->InsertEndChild(ppc_death);
    in_pNarrative->InsertEndChild(pnpc_death);
    in_pNarrative->InsertEndChild(precruited);

    pchapter->SetText(in_state->chapter);

    tinyxml2::XMLElement * ptemp1;
    tinyxml2::XMLElement * ptemp2;
    tinyxml2::XMLElement * ptemp3;
    bool buffbool;
    std::string name;
    char buffer[DEFAULT::BUFFER_SIZE];

    for (unsigned int i = UNIT::NAME::ERWIN; i < UNIT::NAME::PC_END; i++) {
        name = unitNames[i];
        buffbool = in_state->pc_death[i - UNIT::NAME::ERWIN];
        ptemp1 = in_doc->NewElement("Unit");
        ptemp2 = in_doc->NewElement("Died");
        ptemp3 = in_doc->NewElement("Name");
        ppc_death->InsertEndChild(ptemp1);
        ptemp1->InsertEndChild(ptemp3);
        ptemp1->InsertEndChild(ptemp2);
        ptemp2->SetText(buffbool);
        ptemp3->SetText(name.c_str());
        ptemp1->SetAttribute("id", i);

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

    for (unsigned int i = UNIT::NAME::ZINEDAN; i < UNIT::NAME::NPC_END; i++) {
        buffbool = in_state->npc_death[i - UNIT::NAME::ZINEDAN];
        stbsp_sprintf(buffer, "NPC%d", i);
        ptemp1 = in_doc->NewElement("Unit");
        ptemp2 = in_doc->NewElement("Died");
        ptemp3 = in_doc->NewElement("Name");
        pnpc_death->InsertEndChild(ptemp1);
        ptemp1->InsertEndChild(ptemp3);
        ptemp1->InsertEndChild(ptemp2);
        ptemp2->SetText(buffbool);
        ptemp3->SetText(buffer);
        ptemp1->SetAttribute("id", i);
    }
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
    tinyxml2::XMLElement * phand1 = in_doc->NewElement("hand");
    tinyxml2::XMLElement * phand2 = in_doc->NewElement("hand");
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

void writeXML_items(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pItems, Inventory_item * in_items, int size) {
    tinyxml2::XMLElement * pItem;
    tinyxml2::XMLElement * pUsed;
    tinyxml2::XMLElement * pwpnName;
    char buffer[DEFAULT::BUFFER_SIZE];
    for (int i = 0; i < size; i++) {
        pItem = in_doc->NewElement("Item");
        in_pItems->InsertEndChild(pItem);
        pItem->SetAttribute("id", in_items[i].id);
        pUsed = in_doc->NewElement("Used");
        stbsp_sprintf(buffer, "%d", in_items[i].used);
        pUsed->SetText(buffer);
        pItem->InsertEndChild(pUsed);
        pwpnName = in_doc->NewElement("Name");
        if (in_items[i].id > 0) {
            pwpnName->SetText(all_weapons[in_items[i].id].getName().c_str());
        } else {
            pwpnName->SetText("Empty");
        }
        pItem->InsertFirstChild(pwpnName);
    }
}

void readXML_items(tinyxml2::XMLElement * in_pItems, Inventory_item * in_items) {
    tinyxml2::XMLElement * pItem = in_pItems->FirstChildElement("Item");
    tinyxml2::XMLElement * pUsed;
    int buffint;
    int i = 0;
    while (pItem) {
        pUsed = pItem->FirstChildElement("Used");
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


int parseXML(const char * filename, tinyxml2::XMLDocument * in_doc) {
    PHYSFS_file * fp;
    fp = PHYSFS_openRead(filename);
    if (!fp) {
        SDL_Log("Failed to open %s for xml parsing.", filename);
        return(-1);
    }
    unsigned int filelen = PHYSFS_fileLength(fp);
    char filebuffer[filelen];
    PHYSFS_readBytes(fp, filebuffer, filelen);
    PHYSFS_close(fp);
    if (in_doc->Parse(filebuffer, filelen) != 0) {
        SDL_Log("Failed to parseXML %s", filename);
        return(-1);
    }
    return(0);
}



