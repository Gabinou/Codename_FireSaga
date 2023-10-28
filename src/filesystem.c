
#include "filesystem.h"

/* --- INIT --- */
int Filesystem_Init(char *argv0) {
    /* -- Creating buffers for paths -- */
    char *srcDir    = SDL_malloc(DEFAULT_BUFFER_SIZE);
    char *assetsDir = SDL_malloc(DEFAULT_BUFFER_SIZE);
    char temp[DEFAULT_BUFFER_SIZE]   = "";
    char output[DEFAULT_BUFFER_SIZE] = "";

    /* -- Getting base path from SDL -- */
    char *temp_base = SDL_GetBasePath();
    strncpy(srcDir,    temp_base, DEFAULT_BUFFER_SIZE);
    strncpy(assetsDir, temp_base, DEFAULT_BUFFER_SIZE);
    free(temp_base);

    /* -- Finalize paths -- */
    srcDir    = nstr_Path_Remove_Top(srcDir,    DIR_SEPARATOR[0]);
    assetsDir = nstr_Path_Remove_Top(assetsDir, DIR_SEPARATOR[0]);
    strcat(assetsDir, DIR_SEPARATOR"assets");

    /* -- PhysFS init -- */
    if (PHYSFS_init(argv0) <= 0) {
        SDL_Log("Could not initialize PhysFS");
        exit(ERROR_Generic);
    }
    PHYSFS_permitSymbolicLinks(1);
    /* bsa: bear strategic archive */
    PHYSFS_setSaneConfig("AvgBear", "CodenameFiresaga", "bsa", 0, 0);

    /* -- Mounting srcDir -- */
    if (strlen(srcDir) > 0) {
        strcpy(output, srcDir);
        /* We later append to this path and assume it ends in a slash */
        if (output[strlen(output) - 1] != DIR_SEPARATOR[0])
            strcat(output, DIR_SEPARATOR);
    }

    SDL_Log("Mounting %s", output);
    if (!PHYSFS_mount(output, NULL, 1)) {
        SDL_Log("Could not mount %s", output);
        exit(ERROR_PHYSFSCannotMount);
    };
    PHYSFS_setWriteDir(output);
    SDL_Log("Base directory: %s\n", output);

    /* -- Mounting saves directory -- */
    strcpy(temp, output);
    strcat(temp, "saves"DIR_SEPARATOR);
    if (PHYSFS_stat(temp, NULL) == 0) {
        SDL_Log("mkdir %s", temp);
        sota_mkdir(temp);
    }

    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };

    /* -- Mounting build directory -- */
    strcpy(temp, output);
    strcat(temp, SOTA_BUILD_DIR DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };

    /* -- Mounting tiles directory -- */
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "tiles"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };

    /* -- Mounting scenes directory -- */
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "scenes"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };

    /* -- Mounting items directory -- */
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "items"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };

    /* -- Mounting units directory -- */
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "units"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };

    /* -- Mounting assets directory -- */
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "assets");
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };

    /* -- Mounting assets/Maps -- */
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "assets"DIR_SEPARATOR"Maps"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };

    /* -- Mounting assets/Tiles -- */
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "assets"DIR_SEPARATOR"Tiles"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };

    /* -- Mounting assets/Map_Units -- */
    temp[0] = '\0';
    strcpy(temp, output);
    strcat(temp, "assets"DIR_SEPARATOR"Map_Units"DIR_SEPARATOR);
    SDL_Log("Mounting %s", temp);
    if (!PHYSFS_mount(temp, NULL, 1)) {
        SDL_Log("Could not mount %s", temp);
        exit(ERROR_PHYSFSCannotMount);
    };

    /* -- Mount assets.binou -- */
    // TODO: Remove -> use .bsa instead
    temp[0] = '\0';
    if (assetsDir) {
        strcpy(output, assetsDir);
        strcat(output, DIR_SEPARATOR"assets.binou\0");
    } else {
        strcpy(output, PHYSFS_getBaseDir());
        strcat(output, DIR_SEPARATOR"assets.binou");
    }
    SDL_Log("Path to assets: %s\n", output);
    if (!PHYSFS_mount(output, NULL, 1)) {
        SDL_Log("Mount 1 failed");
        if (!PHYSFS_mount("../assets.binou", NULL, 1)) {
            SDL_Log("Missing assets.binou: PHYSFS_ERROR: %s\n",
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing assets.binou", "Missing assets.binou",
                                     NULL);
            exit(ERROR_MissingAssets);
        }
    }

    /* -- Cleanup -- */
    SDL_free(srcDir);
    SDL_free(assetsDir);
    SDL_Log("Filesystem_Init");
    return 1;
}

int Filesystem_Free() {
    return (PHYSFS_deinit());
}

/* --- UTILITIES --- */
void Filesystem_Load_Bytes(const char *name, u8 **mem, size_t *len) {
    /* Open file */
    PHYSFS_File *physfs_file = PHYSFS_openRead(name);
    SDL_assert(physfs_file != NULL);

    /* Alloc buffer with file length */
    PHYSFS_uint32 length = PHYSFS_fileLength(physfs_file);
    if (len != NULL) {
        *len = length;
        *mem = (u8 *) SDL_malloc(length + 1);
        (*mem)[length] = 0;
    } else
        *mem = (u8 *) SDL_malloc(length);

    /* Read file, then close it */
    PHYSFS_readBytes(physfs_file, *mem, length);
    PHYSFS_close(physfs_file);
}

bool Filesystem_fequal(const char *filename1, const char *filename2) {
    bool out = false;
    PHYSFS_file *fp1  = PHYSFS_openRead(filename1);
    PHYSFS_file *fp2  = PHYSFS_openRead(filename2);
    SDL_assert(fp1);
    SDL_assert(fp2);
    u32 filelen1 = PHYSFS_fileLength(fp1);
    u32 filelen2 = PHYSFS_fileLength(fp2);
    char filebuffer1[filelen1], filebuffer2[filelen2];
    PHYSFS_readBytes(fp1, filebuffer1, filelen1);
    PHYSFS_readBytes(fp2, filebuffer2, filelen2);
    PHYSFS_close(fp1);
    PHYSFS_close(fp2);
    if (filelen1 == filelen2)
        out = Utilities_charArr_Equal(filebuffer1, filebuffer2, filelen1);

    return (out);
}

/* --- SURFACES AND TEXTURES --- */
SDL_Surface *Filesystem_Surface_Load(const char *filename, const u32 format) {
    SDL_Log("%s\n", filename);
    SDL_Surface *loadedsurface  = NULL, *outsurface   = NULL;
    SDL_Surface *conv1surface   = NULL, *conv2surface = NULL;
    SDL_Surface *indexedsurface = NULL;
    /* IMG_Load leaves some pixels non-init -> SDL_ConvertSurfaceFormat */
    loadedsurface = IMG_Load(filename);
    SDL_SaveBMP(loadedsurface, "loadedsurface.png");

    SDL_assert(loadedsurface != NULL);
    if (SDL_ISPIXELFORMAT_INDEXED(format)) {
        SDL_Log("is indexed %d\n", SDL_ISPIXELFORMAT_INDEXED(format));
        /* align bits for Filesystem_Surface_Pixels2Indices allocs */
        conv1surface = SDL_ConvertSurfaceFormat(loadedsurface, SDL_PIXELFORMAT_ABGR8888, SDL_IGNORE);
        SDL_FreeSurface(loadedsurface);

        SDL_SaveBMP(conv1surface, "conv1surface.png");
        SDL_assert(conv1surface != NULL);
        /* alloc: */
        indexedsurface = Filesystem_indexedSurface_Init(conv1surface->w, conv1surface->h);
        SDL_assert(indexedsurface != NULL);

        /* WEIRDNESS: SDL_ConvertSurface messes up colors when converting to indexed.
            -> affects bridge tileset <-
            use Filesystem_Surface_Pixels2Indices solves it */
        SDL_assert(indexedsurface != NULL);
        SDL_LockSurface(indexedsurface);
        SDL_LockSurface(conv1surface);
        /* no alloc: */
        Filesystem_Surface_Pixels2Indices(conv1surface, indexedsurface);
        SDL_SaveBMP(indexedsurface, "indexedsurface.png");
        /*makes surfaces faster allocs? */
        // conv2surface = SDL_ConvertSurface(indexedsurface, indexedsurface->format, SDL_IGNORE);
        // SDL_assert(conv2surface != NULL);

        // SDL_SaveBMP(conv2surface, "conv2surface.png");
        // SDL_assert(conv2surface != NULL);
        SDL_UnlockSurface(indexedsurface);
        SDL_UnlockSurface(conv1surface);

        outsurface = indexedsurface;
        // conv2surface = NULL;
        SDL_SaveBMP(indexedsurface, "indexedsurface2.bmp");
        SDL_FreeSurface(conv1surface);
    } else
        outsurface = loadedsurface;

    // SDL_SaveBMP(outsurface, "outsurface.bmp");
    if (outsurface == NULL) {
        SDL_Log("Could not open: %s\n", filename);
        exit(ERROR_CannotOpenFile);
    }
    SDL_assert(outsurface != NULL);
    return (outsurface);
}

SDL_Surface *Filesystem_Surface_Palette_Swap(SDL_Surface *surface, SDL_Palette *palette) {
    /* allocates memory for new surface with switched palette */
    /* Caller deals with memory */

    SDL_assert(surface);
    SDL_assert(surface->w > 0);
    SDL_assert(surface->h > 0);
    /* Duplicate input surface*/
    SDL_Surface *out_surface = SDL_DuplicateSurface(surface);
    SDL_assert(out_surface != NULL);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(out_surface->format->format));
    SDL_assert(palette);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(surface->format->format));

    /* Swap palette */
    SDL_LockSurface(out_surface);
    SDL_LockSurface(surface);
    SDL_SetSurfacePalette(out_surface, palette);
    SDL_SetColorKey(out_surface, SDL_TRUE, PALETTE_COLORKEY);
    SDL_UnlockSurface(out_surface);
    SDL_UnlockSurface(surface);

    return (out_surface);
}

SDL_Texture *Filesystem_Texture_Load(struct SDL_Renderer *renderer, const char *file, u32 format) {
    SDL_Log("%s\n", file);
    SDL_Surface *tempsurface = Filesystem_Surface_Load(file, format);
    // SDL_SaveBMP(tempsurface, "readsurface.png");

    SDL_Texture *texture     = SDL_CreateTextureFromSurface(renderer, tempsurface);
    SDL_assert(texture);
    SDL_FreeSurface(tempsurface);
    SDL_assert(texture);
    return (texture);
}

/* Note: pixels are in a separate buffer that needs to be freed separately. */
SDL_Surface *Filesystem_TexturetoSurface(SDL_Renderer *renderer, SDL_Texture *texture,
                                         u32 format, SDL_Texture *render_target) {
    SDL_assert(texture         != NULL);
    SDL_Surface *out_surface    = NULL;
    SDL_Texture *render_texture = NULL;

    int w, h, success;
    void *pixels = NULL;

    /* Get information about texture we want to save */
    success = SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_assert(success == 0);
    render_texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_assert(render_texture);

    /* Initialize our canvas, then copy texture to a target whose pixel data we can access */
    success = SDL_SetRenderTarget(renderer, render_texture);
    SDL_assert(success == 0);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    success = SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_assert(success == 0);

    /* Create buffer to hold texture data and load it */
    int bits    = SDL_BITSPERPIXEL(format), bytes = SDL_BYTESPERPIXEL(format);
    pixels      = malloc(w * h * bytes);
    SDL_assert(pixels);
    success = SDL_RenderReadPixels(renderer, NULL, format, pixels, w * bytes);
    SDL_assert(success == 0);

    /* Copy pixel data over to surface */
    out_surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, bits, w * bytes, format);
    SDL_assert(out_surface);
    SDL_SetRenderTarget(renderer, render_target);
    SDL_DestroyTexture(render_texture);
    return (out_surface);
}

SDL_Surface *Filesystem_indexedSurface_Init(size_t w, size_t h) {
    SDL_Surface *surface;

    surface = SDL_CreateRGBSurfaceWithFormat(SDL_IGNORE, w, h, 8, SDL_PIXELFORMAT_INDEX8);

    if (PALETTE_DEFAULT == NULL) {
        SDL_Log("palette_NES not loaded");
        exit(ERROR_NullPointer);
    }

    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(surface->format->format));
    SDL_assert(surface);
    SDL_LockSurface(surface);
    int success = SDL_SetColorKey(surface, SDL_TRUE, PALETTE_COLORKEY);
    SDL_assert(success == 0);
    success = SDL_SetSurfacePalette(surface, PALETTE_DEFAULT);
    SDL_assert(success == 0);
    SDL_assert(surface->format->palette != NULL);

    /* Init pixels value */
    memset(surface->pixels, 0, surface->h * surface->pitch);

    SDL_UnlockSurface(surface);
    return (surface);
}

/* Convert ABGR color image to indexed images (NES colors) */
void Filesystem_Surface_Pixels2Indices(SDL_Surface *abgr_surf, SDL_Surface *index_surf) {
    /* VERY SLOW */
    SDL_assert(abgr_surf    != NULL);
    SDL_assert(index_surf   != NULL);
    SDL_assert(abgr_surf->w == index_surf->w);
    SDL_assert(abgr_surf->h == index_surf->h);
    SDL_assert(abgr_surf->w  > 0);
    SDL_assert(abgr_surf->h  > 0);
    SDL_assert(index_surf->format->palette      != NULL);
    SDL_assert(index_surf->format->BitsPerPixel == 8);
    SDL_assert(index_surf->w == abgr_surf->w);
    SDL_assert(index_surf->h == abgr_surf->h);

    u32 pixel, color;
    u8 bitsmin = 24, bitsmax = 32; /* Bits for A channel? */
    SDL_LockSurface(abgr_surf);
    SDL_LockSurface(index_surf);
    u8 *arbg_pixels  = (u8 *)abgr_surf->pixels;
    u8 *index_pixels = (u8 *)index_surf->pixels;
    for (size_t y = 0; y < abgr_surf->h; y++) {/* this loop cache friendly */
        for (size_t x = 0; x < abgr_surf->w; x++) {
            size_t index = Util_SDL_Surface_Index(abgr_surf, x, y);
            SDL_assert(index >= 0);
            SDL_assert(index < (abgr_surf->pitch * abgr_surf->h));
            pixel = *(u32 *)(arbg_pixels + index);

            /* Swap byte order if big endian */
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                pixel = SDL_SwapBE32(pixel);

            /* Get rid of A channel? */
            for (size_t j = bitsmin; j < bitsmax; j++)
                pixel &= ~(1 << j);

            /* Find which color this pixel corresponds to */
            for (size_t i = 0; i < index_surf->format->palette->ncolors; i++) {
                color = *(u32 *)(&index_surf->format->palette->colors[i]);

                if (pixel == color) {
                    index = Util_SDL_Surface_Index(index_surf, x, y);
                    SDL_assert(index >= 0);
                    SDL_assert(index < (index_surf->pitch * index_surf->h));

                    index_pixels[index] = i;
                    break;
                }
            }
        }
    }
    SDL_UnlockSurface(abgr_surf);
    SDL_UnlockSurface(index_surf);
}

void Filesystem_Texture_Dump(const char *filename, SDL_Renderer *renderer, SDL_Texture *texture,
                             const u32 format, SDL_Texture *render_target) {
    if (!texture) {
        SDL_Log("Warning: Input texture is NULL");
    }

    SDL_Surface *surface = Filesystem_TexturetoSurface(renderer, texture, format, render_target);
    /* Save result to an image */
    if (IMG_SavePNG(surface, filename) != 0) {
        SDL_Log("PHYSFS_setBuffer failed");
        exit(ERROR_CannotWriteFile);
    }
    SDL_free(surface->pixels);
    SDL_FreeSurface(surface);
}

/* --- JSON READ & WRITE --- */
// TODO: rewrite with input cjson
void Filesystem_readJSON_Shop(const char *filename, struct Shop *shop) {
    SDL_Log("%s", filename);
    struct cJSON *jfile = jsonio_parseJSON(filename);
    SDL_assert(jfile != NULL);
    struct cJSON *jshop = cJSON_GetObjectItem(jfile, "Shop");
    if (jshop == NULL) {
        SDL_Log("No Shop element in shop json");
        exit(ERROR_JSONParsingFailed);
    }
    struct cJSON *jitems = cJSON_GetObjectItem(jshop, "Items");
    if (jitems == NULL) {
        SDL_Log("No Items array in shop json");
        exit(ERROR_JSONParsingFailed);
    }
    size_t items_num = cJSON_GetArraySize(jitems);
    shop->qty = calloc(items_num, sizeof(*shop->qty));
    shop->items = calloc(items_num, sizeof(*shop->items));
    for (int i = 0; i < items_num; i++) {
        struct cJSON *jitem = cJSON_GetArrayItem(jitems, i);
        shop->qty[i] = cJSON_GetNumberValue(jitem);
        shop->items[i] = Hashes_itemName2ID(jitem->string);
    }
    struct cJSON *jshopkeeper = cJSON_GetObjectItem(jshop, "Shopkeeper");
    shop->shopkeeper = Hashes_shopkeeperName2ID(cJSON_GetStringValue(jshopkeeper));
    if (jfile != NULL)
        cJSON_Delete(jfile);
}

void Filesystem_readJSON_Promotion(const char *filename, struct Promotion *promotion) {
    SDL_Log("%s", filename);
    struct cJSON *jfile = jsonio_parseJSON(filename);
    SDL_assert(jfile != NULL);
    struct cJSON *jpromotion = cJSON_GetObjectItem(jfile, "Promotion");
    if (jpromotion == NULL) {
        SDL_Log("No Promotion element in promotion json");
        exit(ERROR_JSONParsingFailed);
    }
    struct cJSON *jstats = cJSON_GetObjectItem(jpromotion, "Stats");
    if (jstats == NULL) {
        SDL_Log("No Stats element in promotion json");
        exit(ERROR_JSONParsingFailed);
    }
    Filesystem_readJSON_Unitstats(jstats, &promotion->bonus);
    struct cJSON *jskill = cJSON_GetObjectItem(jpromotion, "Skill");
    if (jskill != NULL) {
        struct cJSON *jname = cJSON_GetObjectItem(jskill, "Name");
        if (jname == NULL) {
            SDL_Log("No Name element in skills element of promotion json");
            exit(ERROR_JSONParsingFailed);
        }
        promotion->skill = Hashes_skillName2ID(cJSON_GetStringValue(jname));

        struct cJSON *jlvl = cJSON_GetObjectItem(jskill, "level");
        if (jlvl != NULL)
            promotion->level = cJSON_GetNumberValue(jlvl);
    }

    if (jfile != NULL)
        cJSON_Delete(jfile);

}

void Filesystem_readJSON_Palette(const char *filename, struct SDL_Palette *palette) {
    SDL_Log("%s", filename);
    struct cJSON *jfile = jsonio_parseJSON(filename);
    SDL_assert(jfile != NULL);
    struct cJSON *jpalette = cJSON_GetObjectItem(jfile, "Palette");
    if (jpalette == NULL) {
        SDL_Log("No Palette element in palette json");
        exit(ERROR_JSONParsingFailed);
    }
    struct cJSON *jrgbs = cJSON_GetObjectItem(jpalette, "rgb");
    if (jrgbs == NULL) {
        SDL_Log("No rgb array in palette json");
        exit(ERROR_JSONParsingFailed);
    }

    size_t colors_num = cJSON_GetArraySize(jrgbs);
    if (colors_num != PALETTE_NES_COLOR_NUM) {
        SDL_Log("Invalid number of colors in Palette");
        exit(ERROR_JSONParsingFailed);
    }

    for (int i = 0; i < colors_num; i++) {
        struct cJSON *jrgb   = cJSON_GetArrayItem(jrgbs, i);
        palette->colors[i].r = cJSON_GetNumberValue(cJSON_GetArrayItem(jrgb, 0));
        palette->colors[i].g = cJSON_GetNumberValue(cJSON_GetArrayItem(jrgb, 1));
        palette->colors[i].b = cJSON_GetNumberValue(cJSON_GetArrayItem(jrgb, 2));
        palette->colors[i].a = 0;
    }
    if (jfile != NULL)
        cJSON_Delete(jfile);

}

void Filesystem_readJSON_PaletteTable(const char *filename, u8   *table) {
    SDL_Log("%s", filename);
    struct cJSON *jfile = jsonio_parseJSON(filename);
    SDL_assert(jfile != NULL);
    struct cJSON *jpalette_table = cJSON_GetObjectItem(jfile, "Palette Table");
    if (jpalette_table == NULL) {
        SDL_Log("No 'Palette Table' element in palette table json");
        exit(ERROR_JSONParsingFailed);
    }

    struct cJSON *jfrom_tos = cJSON_GetObjectItem(jpalette_table, "from_to");
    if (jfrom_tos == NULL) {
        SDL_Log("No from_to array in palette table json");
        exit(ERROR_JSONParsingFailed);
    }

    size_t colors_num = cJSON_GetArraySize(jfrom_tos);
    if (colors_num != PALETTE_NES_COLOR_NUM) {
        SDL_Log("Invalid number of colors in Palette Table");
        exit(ERROR_JSONParsingFailed);
    }

    for (int i = 0; i < colors_num; i++) {
        struct cJSON *jfrom_to =  cJSON_GetArrayItem(jfrom_tos, i);
        table[i] = cJSON_GetNumberValue(cJSON_GetArrayItem(jfrom_to, 1));
    }
    if (jfile != NULL)
        cJSON_Delete(jfile);

}

void Filesystem_readJSON_MObj_Link(const struct cJSON *const j, struct Mobj_Link *restrict b) {

}

void Filesystem_readJSON_Breakable(const struct cJSON *const jbreak,
                                   struct Breakable *restrict breaka) {
    struct cJSON *jhp  = cJSON_GetObjectItem(jbreak, "hp");
    struct cJSON *jdef = cJSON_GetObjectItem(jbreak, "def");
    struct cJSON *jres = cJSON_GetObjectItem(jbreak, "res");

    if (jres != NULL)
        breaka->res = cJSON_GetNumberValue(jres);
    if (jdef != NULL)
        breaka->def = cJSON_GetNumberValue(jdef);
    if (jhp != NULL)
        breaka->hp  = cJSON_GetNumberValue(jhp);
}

void Filesystem_readJSON_Door(const struct cJSON *const jdoor, struct Door *restrict door) {
    struct cJSON *jtile          = cJSON_GetObjectItem(jdoor, "tile");
    struct cJSON *jevent         = cJSON_GetObjectItem(jdoor, "event");
    struct cJSON *jscene         = cJSON_GetObjectItem(jdoor, "scene");
    struct cJSON *jchapter_open  = cJSON_GetObjectItem(jdoor, "chapter_open");
    struct cJSON *jchapter_close = cJSON_GetObjectItem(jdoor, "chapter_close");

    if (jchapter_open != NULL)
        door->chapter_open  = cJSON_GetNumberValue(jchapter_open);
    if (jchapter_close != NULL)
        door->chapter_close = cJSON_GetNumberValue(jchapter_close);
    if (jevent != NULL)
        door->event         = cJSON_GetNumberValue(jevent);
    if (jscene != NULL)
        door->scene         = cJSON_GetNumberValue(jscene);
    if (jtile != NULL)
        door->tile          = cJSON_GetNumberValue(jtile);
}

void Filesystem_readJSON_Chest(const struct cJSON *const jchest, struct Chest *restrict chest) {
    SDL_assert(jchest != NULL);
    SDL_assert(chest != NULL);
    struct cJSON *jgold = cJSON_GetObjectItem(jchest, "gold");
    struct cJSON *jitem = cJSON_GetObjectItem(jchest, "gold");
    struct cJSON *jtile = cJSON_GetObjectItem(jchest, "tile");

    if (jtile != NULL)
        chest->tile = cJSON_GetNumberValue(jtile);
    if (jgold != NULL)
        chest->gold = cJSON_GetNumberValue(jgold);
    if (jitem != NULL) {
        char *name  = cJSON_GetStringValue(jitem);
        if (name != NULL)
            chest->item = Hashes_itemName2ID(name);
    }
}

void Filesystem_readJSON_Position(const struct cJSON *const jpos, struct Point *pos) {
    SDL_assert(jpos != NULL);
    SDL_assert(pos != NULL);
    if (cJSON_IsArray(jpos)) {
        pos->x = cJSON_GetNumberValue(cJSON_GetArrayItem(jpos, 0));
        pos->y = cJSON_GetNumberValue(cJSON_GetArrayItem(jpos, 1));
    } else {
        struct cJSON *jcol = cJSON_GetObjectItem(jpos, "col");
        struct cJSON *jrow = cJSON_GetObjectItem(jpos, "row");
        if (jcol != NULL)
            pos->x = cJSON_GetNumberValue(jcol);
        if (jrow != NULL)
            pos->y = cJSON_GetNumberValue(jrow);
    }
}

void Filesystem_readJSON_mvtcost(const struct cJSON          *const    jcost,
                                 struct fMovement_cost *restrict cost) {
    SDL_assert(cost != NULL);
    struct cJSON *jmages       = cJSON_GetObjectItem(jcost, "mages");
    struct cJSON *jfliers      = cJSON_GetObjectItem(jcost, "fliers");
    struct cJSON *jarmors      = cJSON_GetObjectItem(jcost, "armors");
    struct cJSON *jpirates     = cJSON_GetObjectItem(jcost, "pirates");
    struct cJSON *jbandits     = cJSON_GetObjectItem(jcost, "bandits");
    struct cJSON *jfoot_slow   = cJSON_GetObjectItem(jcost, "foot_slow");
    struct cJSON *jfoot_fast   = cJSON_GetObjectItem(jcost, "foot_fast");
    struct cJSON *jriders_slow = cJSON_GetObjectItem(jcost, "riders_slow");
    struct cJSON *jriders_fast = cJSON_GetObjectItem(jcost, "riders_fast");
    cost->mages       = cJSON_GetNumberValue(jmages);
    cost->fliers      = cJSON_GetNumberValue(jfliers);
    cost->armors      = cJSON_GetNumberValue(jarmors);
    cost->pirates     = cJSON_GetNumberValue(jpirates);
    cost->bandits     = cJSON_GetNumberValue(jbandits);
    cost->foot_slow   = cJSON_GetNumberValue(jfoot_slow);
    cost->foot_fast   = cJSON_GetNumberValue(jfoot_fast);
    cost->riders_slow = cJSON_GetNumberValue(jriders_slow);
    cost->riders_fast = cJSON_GetNumberValue(jriders_fast);
}

void Filesystem_readJSON_Array(const struct cJSON *const jarray, i32 *restrict array) {
    SDL_assert(array != NULL);
    struct cJSON *jnum;
    size_t i = 0;
    cJSON_ArrayForEach(jnum, jarray) {
        array[i++] = (i32)cJSON_GetNumberValue(jnum);
    }
}

void Filesystem_readJSON_2DArray(const struct cJSON *const jarr, i32 *restrict arr2D,
                                 u8 row_len, u8 col_len) {
    /* caller deals with memory */
    SDL_assert(jarr != NULL);
    SDL_assert(cJSON_IsArray(jarr));
    SDL_assert(cJSON_GetArraySize(jarr) == row_len);
    for (u8 row = 0; row < cJSON_GetArraySize(jarr); row++) {
        struct cJSON *jrow  = cJSON_GetArrayItem(jarr, row);
        SDL_assert(cJSON_GetArraySize(jrow) == col_len);
        for (u8 col = 0; col < cJSON_GetArraySize(jrow); col++) {
            struct cJSON *jnum  = cJSON_GetArrayItem(jrow, col);
            arr2D[(row * col_len + col)] = (i32)cJSON_GetNumberValue(jnum);
        }
    }
}

void Filesystem_writeJSON_Array(struct cJSON *jarr, const i32 *restrict arr, size_t num) {
    SDL_Log("%zu", num);
    SDL_assert(arr != NULL);
    for (u8 i = 0; i < num; i++) {
        struct cJSON *jnum = cJSON_CreateNumber(arr[i]);
        cJSON_AddItemToArray(jarr, jnum);
    }
}

void Filesystem_writeJSON_2DArray(struct cJSON *arr, const i32 *restrict arr2D, u8 row_len,
                                  u8 col_len) {
    SDL_Log("%d %d", row_len, col_len);
    SDL_assert(arr != NULL);
    struct cJSON *jrow, *jnum;
    char rowname[DEFAULT_BUFFER_SIZE / 8];
    for (u8 row = 0; row < row_len; row++) {
        jrow = cJSON_CreateArray();
        for (u8 col = 0; col < col_len; col++) {
            jnum = cJSON_CreateNumber(arr2D[(row * col_len + col)]);
            cJSON_AddItemToArray(jrow, jnum);
        }
        stbsp_sprintf(rowname, "row %d", row);
        cJSON_AddItemToObject(arr, rowname, jrow);
    }
}

void Filesystem_writeJSON_mvtcost(struct cJSON *jcost, const struct fMovement_cost *restrict cost) {
    SDL_assert(jcost != NULL);
    struct cJSON *jmages       = cJSON_CreateNumber(cost->mages);
    struct cJSON *jfliers      = cJSON_CreateNumber(cost->fliers);
    struct cJSON *jarmors      = cJSON_CreateNumber(cost->armors);
    struct cJSON *jpirates     = cJSON_CreateNumber(cost->pirates);
    struct cJSON *jbandits     = cJSON_CreateNumber(cost->bandits);
    struct cJSON *jfoot_slow   = cJSON_CreateNumber(cost->foot_slow);
    struct cJSON *jfoot_fast   = cJSON_CreateNumber(cost->foot_fast);
    struct cJSON *jriders_slow = cJSON_CreateNumber(cost->riders_slow);
    struct cJSON *jriders_fast = cJSON_CreateNumber(cost->riders_fast);
    cJSON_AddItemToObject(jcost, "mages",       jmages);
    cJSON_AddItemToObject(jcost, "fliers",      jfliers);
    cJSON_AddItemToObject(jcost, "armors",      jarmors);
    cJSON_AddItemToObject(jcost, "pirates",     jpirates);
    cJSON_AddItemToObject(jcost, "bandits",     jbandits);
    cJSON_AddItemToObject(jcost, "foot_slow",   jfoot_slow);
    cJSON_AddItemToObject(jcost, "foot_fast",   jfoot_fast);
    cJSON_AddItemToObject(jcost, "riders_slow", jriders_slow);
    cJSON_AddItemToObject(jcost, "riders_fast", jriders_fast);
}

void Filesystem_readJSON_Tilestats(const struct cJSON      *const    jstats,
                                   struct Tile_stats *restrict stats) {
    SDL_assert(jstats != NULL);
    struct cJSON *jdodge = cJSON_GetObjectItem(jstats, "Dodge");
    struct cJSON *jPprot = cJSON_GetObjectItem(jstats, "Pprot");
    struct cJSON *jMprot = cJSON_GetObjectItem(jstats, "Mprot");
    struct cJSON *jHeal  = cJSON_GetObjectItem(jstats, "Heal");
    stats->dodge         = cJSON_GetNumberValue(jdodge);
    stats->Pprot         = cJSON_GetNumberValue(jPprot);
    stats->Mprot         = cJSON_GetNumberValue(jMprot);
    stats->heal          = cJSON_GetNumberValue(jHeal);
}

void Filesystem_readJSON_Reinforce(const struct cJSON         *const jarrival,
                                   struct Reinforcement *restrict arrival) {
    SDL_assert(arrival  != NULL);
    SDL_assert(jarrival != NULL);
    struct cJSON *jid       = cJSON_GetObjectItem(jarrival,  "id");
    struct cJSON *jarmy     = cJSON_GetObjectItem(jarrival,  "army");
    struct cJSON *jturn     = cJSON_GetObjectItem(jarrival,  "turn");
    struct cJSON *jlevelups = cJSON_GetObjectItem(jarrival,  "levelups");
    struct cJSON *jposition = cJSON_GetObjectItem(jarrival,  "Position");

    struct cJSON *jrow      = cJSON_GetObjectItem(jposition, "row");
    struct cJSON *jcol      = cJSON_GetObjectItem(jposition, "col");

    arrival->id         = cJSON_GetNumberValue(jid);
    arrival->army       = cJSON_GetNumberValue(jarmy);
    arrival->turn       = cJSON_GetNumberValue(jturn);
    arrival->levelups   = cJSON_GetNumberValue(jlevelups);
    arrival->position.x = cJSON_GetNumberValue(jrow);
    arrival->position.y = cJSON_GetNumberValue(jcol);
}

void Filesystem_writeJSON_arrival(      struct cJSON                  *jarrival,
                                        const struct Reinforcement *restrict arrival) {
    SDL_assert(jarrival != NULL);
    struct cJSON *jid       = cJSON_CreateNumber(arrival->id);
    struct cJSON *jrow      = cJSON_CreateNumber(arrival->position.x);
    struct cJSON *jcol      = cJSON_CreateNumber(arrival->position.y);
    struct cJSON *jturn     = cJSON_CreateNumber(arrival->turn);
    struct cJSON *jarmy     = cJSON_CreateNumber(arrival->army);
    struct cJSON *jlevelups = cJSON_CreateNumber(arrival->levelups);
    struct cJSON *jposition = cJSON_CreateObject();
    cJSON_AddItemToObject(jposition, "row",      jrow);
    cJSON_AddItemToObject(jposition, "col",      jcol);
    cJSON_AddItemToObject(jarrival,  "id",       jid);
    cJSON_AddItemToObject(jarrival,  "army",     jarmy);
    cJSON_AddItemToObject(jarrival,  "levelups", jlevelups);
    cJSON_AddItemToObject(jarrival,  "turn",     jturn);
    cJSON_AddItemToObject(jarrival,  "Position", jposition);
}

void Filesystem_writeJSON_Tilestats(      struct cJSON      *restrict jstats,
                                          const struct Tile_stats *restrict stats) {
    SDL_assert(jstats != NULL);
    struct cJSON *jdodge = cJSON_CreateNumber(stats->dodge);
    struct cJSON *jPprot = cJSON_CreateNumber(stats->Pprot);
    struct cJSON *jMprot = cJSON_CreateNumber(stats->Mprot);
    struct cJSON *jheal  = cJSON_CreateNumber(stats->heal);
    cJSON_AddItemToObject(jstats, "Dodge", jdodge);
    cJSON_AddItemToObject(jstats, "Pprot", jPprot);
    cJSON_AddItemToObject(jstats, "Mprot", jMprot);
    cJSON_AddItemToObject(jstats, "Heal",  jheal);
}

void Filesystem_writeJSON_Unitstats(struct cJSON *restrict jstats,
                                    const struct Unit_stats *restrict stats) {
    SDL_assert(jstats != NULL);
    struct cJSON *php   = cJSON_CreateNumber(stats->hp);
    struct cJSON *pstr  = cJSON_CreateNumber(stats->str);
    struct cJSON *pmag  = cJSON_CreateNumber(stats->mag);
    struct cJSON *pagi  = cJSON_CreateNumber(stats->agi);
    struct cJSON *pdex  = cJSON_CreateNumber(stats->dex);
    struct cJSON *pluck = cJSON_CreateNumber(stats->luck);
    struct cJSON *pdef  = cJSON_CreateNumber(stats->def);
    struct cJSON *pres  = cJSON_CreateNumber(stats->res);
    struct cJSON *pcon  = cJSON_CreateNumber(stats->con);
    struct cJSON *pmove = cJSON_CreateNumber(stats->move);
    struct cJSON *pprof = cJSON_CreateNumber(stats->prof);
    cJSON_AddItemToObject(jstats, "hp",   php);
    cJSON_AddItemToObject(jstats, "str",  pstr);
    cJSON_AddItemToObject(jstats, "mag",  pmag);
    cJSON_AddItemToObject(jstats, "agi",  pagi);
    cJSON_AddItemToObject(jstats, "dex",  pdex);
    cJSON_AddItemToObject(jstats, "luck", pluck);
    cJSON_AddItemToObject(jstats, "def",  pdef);
    cJSON_AddItemToObject(jstats, "res",  pres);
    cJSON_AddItemToObject(jstats, "con",  pcon);
    cJSON_AddItemToObject(jstats, "move", pmove);
    cJSON_AddItemToObject(jstats, "prof", pprof);
}

void Filesystem_writeJSON_Itemstats(struct cJSON *restrict jstats,
                                    const struct Item_stats *restrict stats) {
    SDL_assert(jstats != NULL);
    struct cJSON *jAP    = cJSON_CreateNumber(stats->AP);
    struct cJSON *juses  = cJSON_CreateNumber(stats->uses);
    struct cJSON *jprice = cJSON_CreateNumber(stats->price);
    cJSON_AddItemToObject(jstats, "uses", juses);
    cJSON_AddItemToObject(jstats, "price", jprice);
    cJSON_AddItemToObject(jstats, "AP", jAP);
}

void Filesystem_writeJSON_Wpnstats(      struct cJSON                *jstats,
                                         const struct Weapon_stats *restrict stats) {
    SDL_assert(jstats != NULL);
    struct cJSON *pprot   = cJSON_CreateArray();
    struct cJSON *prange  = cJSON_CreateArray();
    struct cJSON *pattack = cJSON_CreateArray();
    cJSON_AddItemToArray(pprot,   cJSON_CreateNumber(stats->protection[DAMAGE_TYPE_PHYSICAL]));
    cJSON_AddItemToArray(pprot,   cJSON_CreateNumber(stats->protection[DAMAGE_TYPE_MAGICAL]));
    cJSON_AddItemToArray(prange,  cJSON_CreateNumber(stats->range.min));
    cJSON_AddItemToArray(prange,  cJSON_CreateNumber(stats->range.max));
    cJSON_AddItemToArray(pattack, cJSON_CreateNumber(stats->attack[DAMAGE_TYPE_PHYSICAL]));
    cJSON_AddItemToArray(pattack, cJSON_CreateNumber(stats->attack[DAMAGE_TYPE_MAGICAL]));
    cJSON_AddItemToArray(pattack, cJSON_CreateNumber(stats->attack[DAMAGE_TYPE_TRUE]));
    struct cJSON *phit   = cJSON_CreateNumber(stats->hit);
    struct cJSON *pdodge = cJSON_CreateNumber(stats->dodge);
    struct cJSON *pcrit  = cJSON_CreateNumber(stats->crit);
    struct cJSON *pfavor = cJSON_CreateNumber(stats->favor);
    struct cJSON *pwgt   = cJSON_CreateNumber(stats->wgt);
    struct cJSON *pprof  = cJSON_CreateNumber(stats->prof);
    // struct cJSON * pminhand = cJSON_CreateNumber(stats->hand);
    // struct cJSON * pismagic = cJSON_CreateNumber(stats->ismagic);
    cJSON_AddItemToObject(jstats, "Attack",     pattack);
    cJSON_AddItemToObject(jstats, "Protection", pprot);
    cJSON_AddItemToObject(jstats, "hit",        phit);
    cJSON_AddItemToObject(jstats, "dodge",      pdodge);
    cJSON_AddItemToObject(jstats, "crit",       pcrit);
    cJSON_AddItemToObject(jstats, "favor",      pfavor);
    cJSON_AddItemToObject(jstats, "wgt",        pwgt);
    cJSON_AddItemToObject(jstats, "prof",       pprof);
    cJSON_AddItemToObject(jstats, "Range",      prange);
    // cJSON_AddItemToObject(jstats, "hand", pminhand);
    // cJSON_AddItemToObject(jstats, "dmg_type", pdmg_type);
}

void Filesystem_readJSON_Item(const struct cJSON *const jitem,
                              struct Inventory_item *restrict item) {
    SDL_assert(jitem != NULL);
    struct cJSON *jid       = cJSON_GetObjectItem(jitem, "id");
    struct cJSON *jused     = cJSON_GetObjectItem(jitem, "used");
    struct cJSON *jinfusion = cJSON_GetObjectItem(jitem, "infusion");
    item->id       = cJSON_GetNumberValue(jid);
    item->used     = cJSON_GetNumberValue(jused);
    item->infusion = cJSON_GetNumberValue(jinfusion);
}

void Filesystem_writeJSON_item(struct cJSON *jitem, const struct Inventory_item *restrict item) {
    SDL_assert(jitem != NULL);
    char buffer[DEFAULT_BUFFER_SIZE];
    cJSON *jid       = cJSON_CreateNumber(item->id);
    cJSON *jused     = cJSON_CreateNumber(item->used);
    cJSON *jinfusion = cJSON_CreateNumber(item->infusion);
    cJSON_AddItemToObject(jitem, "id",       jid);
    cJSON_AddItemToObject(jitem, "used",     jused);
    cJSON_AddItemToObject(jitem, "infusion", jinfusion);
}

void Filesystem_readJSON_Itemstats(const struct cJSON *const jstats,
                                   struct Item_stats *restrict stats) {
    SDL_assert(jstats != NULL);
    struct cJSON *jAP    = cJSON_GetObjectItem(jstats, "AP");
    struct cJSON *juses  = cJSON_GetObjectItem(jstats, "uses");
    struct cJSON *jprice = cJSON_GetObjectItem(jstats, "price");
    stats->AP    = cJSON_GetNumberValue(jAP);
    stats->uses  = cJSON_GetNumberValue(juses);
    stats->price = cJSON_GetNumberValue(jprice);
}

void Filesystem_readJSON_Wpnstats(const struct cJSON        *const    jstats,
                                  struct Weapon_stats *restrict  stats) {
    SDL_assert(jstats != NULL);
    struct cJSON *jnum;
    size_t i;
    struct cJSON *jattack = cJSON_GetObjectItem(jstats, "Attack");
    i = 0;
    cJSON_ArrayForEach(jnum, jattack) {
        stats->attack[i++] = (u8)cJSON_GetNumberValue(jnum);
    }

    struct cJSON *jprot = cJSON_GetObjectItem(jstats, "Protection");
    i = 0;
    cJSON_ArrayForEach(jnum, jprot) {
        stats->protection[i++] = (u8)cJSON_GetNumberValue(jnum);
    }

    struct cJSON *jrange = cJSON_GetObjectItem(jstats, "Range");
    stats->range.min = (u8)cJSON_GetNumberValue(cJSON_GetArrayItem(jrange, RANGE_MIN));
    stats->range.max = (u8)cJSON_GetNumberValue(cJSON_GetArrayItem(jrange, RANGE_MAX));
    SDL_assert(stats->range.max >= stats->range.min);
    struct cJSON *jhit   = cJSON_GetObjectItem(jstats, "hit");
    struct cJSON *jwgt   = cJSON_GetObjectItem(jstats, "wgt");
    struct cJSON *jcrit  = cJSON_GetObjectItem(jstats, "crit");
    struct cJSON *jprof  = cJSON_GetObjectItem(jstats, "prof");
    struct cJSON *jhand  = cJSON_GetObjectItem(jstats, "hand");
    struct cJSON *jfavor = cJSON_GetObjectItem(jstats, "favor");
    struct cJSON *jdodge = cJSON_GetObjectItem(jstats, "dodge");
    stats->hit   = cJSON_GetNumberValue(jhit);
    stats->wgt   = cJSON_GetNumberValue(jwgt);
    stats->crit  = cJSON_GetNumberValue(jcrit);
    stats->prof  = cJSON_GetNumberValue(jprof);
    stats->favor = cJSON_GetNumberValue(jfavor);
    stats->dodge = cJSON_GetNumberValue(jdodge);
    // stats->hand = cJSON_GetNumberValue(jhand);
}

void Filesystem_readJSON_Unitstats(const struct cJSON      *const    jstats,
                                   struct Unit_stats *restrict stats) {
    SDL_assert(stats != NULL);
    SDL_assert(jstats != NULL);
    struct cJSON *jhp   = cJSON_GetObjectItem(jstats, "hp");
    struct cJSON *jstr  = cJSON_GetObjectItem(jstats, "str");
    struct cJSON *jmag  = cJSON_GetObjectItem(jstats, "mag");
    struct cJSON *jagi  = cJSON_GetObjectItem(jstats, "agi");
    struct cJSON *jdex  = cJSON_GetObjectItem(jstats, "dex");
    struct cJSON *jfth  = cJSON_GetObjectItem(jstats, "fth");
    struct cJSON *jluck = cJSON_GetObjectItem(jstats, "luck");
    struct cJSON *jdef  = cJSON_GetObjectItem(jstats, "def");
    struct cJSON *jres  = cJSON_GetObjectItem(jstats, "res");
    struct cJSON *jcon  = cJSON_GetObjectItem(jstats, "con");
    struct cJSON *jmove = cJSON_GetObjectItem(jstats, "move");
    struct cJSON *jprof = cJSON_GetObjectItem(jstats, "prof");
    stats->hp   = cJSON_GetNumberValue(jhp);
    stats->str  = cJSON_GetNumberValue(jstr);
    stats->mag  = cJSON_GetNumberValue(jmag);
    stats->agi  = cJSON_GetNumberValue(jagi);
    stats->dex  = cJSON_GetNumberValue(jdex);
    stats->fth  = cJSON_GetNumberValue(jfth);
    stats->luck = cJSON_GetNumberValue(jluck);
    stats->def  = cJSON_GetNumberValue(jdef);
    stats->res  = cJSON_GetNumberValue(jres);
    stats->con  = cJSON_GetNumberValue(jcon);
    stats->move = cJSON_GetNumberValue(jmove);
    stats->prof = cJSON_GetNumberValue(jprof);
}

void Filesystem_printJSON(PHYSFS_file *fp, const struct cJSON *const json) {
    /* Write json element bytes to buffer */
    char *buffer = cJSON_Print(json);

    /* Set up buffering for a PhysicsFS file handle. */
    size_t length = strlen(buffer);
    if (!PHYSFS_setBuffer(fp, length)) {
        SDL_Log("PHYSFS_setBuffer failed");
        exit(ERROR_CannotWriteFile);
    }

    /* Write bytes to file with Physfs */
    PHYSFS_writeBytes(fp, buffer, length);

    /* Free buffer */
    cJSON_free(buffer);
}

/* --- SCENE --- */
void Filesystem_Scene_Chapter(char *filename, int chapter) {
    if (chapter < CHAPTER_1) {
        strcat(filename, "Prologue");
    } else if (chapter == CHAPTER_FOLDER_TEST) {
        strcat(filename, "Test");
    } else if (chapter == CHAPTER_EPILOGUE) {
        strcat(filename, "Epilogue");
    } else {
        strcat(filename, "Chapter_");
        char chapter_num[2];
        stbsp_sprintf(chapter_num, "%d", chapter);
        strcat(filename, chapter_num);
    }
    strcat(filename, DIR_SEPARATOR);
}
