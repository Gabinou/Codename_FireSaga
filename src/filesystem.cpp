
#include "filesystem.hpp"
#include "game.hpp"

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
    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    unsigned char *data;
    unsigned int width, height;

    unsigned char *fileIn = NULL;
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

SDL_Texture * loadTexture(SDL_Renderer * in_renderer, const char * filename, const bool FS) {
    SDL_Surface * tempsurface;
    SDL_Log("LoadTexture: %s\n", filename);
    if (FS) {
        tempsurface = ZIP_loadSurface(filename); // How fast is this?
    } else {
        tempsurface = IMG_Load(filename); // Not that fast.
    }
    if (tempsurface == NULL) {
        SDL_Log("loadTexture. IMG_Load error: %s\n", IMG_GetError());
    }
    SDL_Texture * texture = SDL_CreateTextureFromSurface(in_renderer, tempsurface); // THIS FUNCTION CRASHES.
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

bool dmg_type; // 0 is 1 physical. 1 magic.
unsigned short int price;

void readXML_stats(tinyxml2::XMLElement * in_pStats, Weapon_stats * in_stats) {
    tinyxml2::XMLElement * ptemp = in_pStats->FirstChildElement("Pmight");
    unsigned int bufint;
    ptemp->QueryUnsignedText(&bufint);
    in_stats->Pmight = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("Mmight");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->Mmight = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("hit");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->combat.hit = (unsigned char)bufint;
    ptemp = in_pStats->FirstChildElement("dodge");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->combat.dodge = (unsigned char)bufint;
    ptemp = in_pStats->FirstChildElement("crit");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->combat.crit = (unsigned char)bufint;
    ptemp = in_pStats->FirstChildElement("favor");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->combat.favor = (unsigned char)bufint;
    ptemp = in_pStats->FirstChildElement("wgt");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->wgt = (unsigned char)bufint;
    ptemp = in_pStats->FirstChildElement("uses");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->uses = (unsigned char)bufint;
    ptemp = in_pStats->FirstChildElement("prof");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->prof = (unsigned char)bufint;
    ptemp = in_pStats->FirstChildElement("minrange");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->range[0] = (unsigned char)bufint;
    ptemp = in_pStats->FirstChildElement("maxrange");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->range[1] = (unsigned char)bufint;
    ptemp = in_pStats->FirstChildElement("hand");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->hand[0] = (unsigned char)bufint;
    ptemp = in_pStats->LastChildElement("hand");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->hand[1] = (unsigned char)bufint;
    ptemp = in_pStats->FirstChildElement("dmg_type");
    ptemp->QueryBoolText(&in_stats->dmg_type);
    ptemp = in_pStats->FirstChildElement("price");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->price = (unsigned short int)bufint;
}

void readXML_stats(tinyxml2::XMLElement * in_pStats, Unit_stats * in_stats) {
    tinyxml2::XMLElement * ptemp = in_pStats->FirstChildElement("hp");
    unsigned int bufint;
    ptemp->QueryUnsignedText(&bufint);
    in_stats->hp = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("str");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->str = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("mag");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->mag = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("agi");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->agi = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("dex");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->dex = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("luck");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->luck = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("def");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->def = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("res");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->res = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("con");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->con = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("move");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->move = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("prof");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->prof = (unsigned char)bufint;
}

void writeXML_stats(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pStats, Weapon_stats * in_stats) {
    // Pmight, Mmight, hit, dodge, crit, favor, wgt, uses, prof, range, hand, dmg_type, cost
    Combat_stats combat;
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

void readXML_equipment(Inventory_item * equipment, tinyxml2::XMLElement * in_pEquipment) {
    tinyxml2::XMLElement * pItem = in_pEquipment->FirstChildElement("Item");
    tinyxml2::XMLElement * pUsed;
    int bufint;
    for (int i = 0; i < DEFAULT::EQUIPMENT_SIZE; i++) {
        pUsed = pItem->FirstChildElement("Used");
        equipment[i].id = pItem->IntAttribute("id");
        pUsed->QueryIntText(&bufint);
        equipment[i].used = bufint;
        pItem = pItem->NextSiblingElement("Item");
    }
}


