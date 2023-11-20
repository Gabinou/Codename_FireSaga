
#include "filesystem.h"

/* --- INIT --- */


void Filesystem_Mount(s8 folder) {
    SDL_Log("Mounting build dir: '%s'", folder.data);
    if (!PHYSFS_mount(folder.data, NULL, 1)) {
        SDL_Log("Could not mount %s", folder.data);
        exit(ERROR_PHYSFSCannotMount);
    };
}

int Filesystem_Init(char *argv0) {
    /* -- Creating buffers for paths -- */
    s8 temp     = s8_mut("");

    /* -- Getting base path from SDL -- */
    char *temp_base = SDL_GetBasePath();
    s8 srcDir       = s8_mut(temp_base);
    s8 assetsDir    = s8_mut(temp_base);

    /* -- Finalize paths -- */
    if (srcDir.data[srcDir.num - 1] == DIR_SEPARATOR[0])
        srcDir = s8_Path_Remove_Top(srcDir,    DIR_SEPARATOR[0]);
    srcDir = s8_Path_Remove_Top(srcDir,    DIR_SEPARATOR[0]);

    assetsDir = s8_Path_Remove_Top(assetsDir, DIR_SEPARATOR[0]);
    assetsDir = s8cat(assetsDir, s8_literal(DIR_SEPARATOR"assets"));

    /* -- PhysFS init -- */
    if (PHYSFS_init(argv0) <= 0) {
        SDL_Log("Could not initialize PhysFS");
        exit(ERROR_Generic);
    }
    PHYSFS_permitSymbolicLinks(1);
    /* bsa: bear strategic archive */
    PHYSFS_setSaneConfig("AvgBear", "SotA", "bsa", 0, 0);

    /* -- Mounting srcDir -- */
    if (srcDir.num > 0) {
        temp = s8cpy(temp, srcDir);
        /* We later append to this path and assume it ends in a slash */
        if (temp.data[temp.num - 1] != DIR_SEPARATOR[0])
            s8cat(temp, s8_literal(DIR_SEPARATOR));
    }
    Filesystem_Mount(temp);
    PHYSFS_setWriteDir(temp.data);
    SDL_Log("Base directory: %s\n", temp.data);

    /* -- Mounting saves directory -- */
    temp = s8cat(temp, s8_literal(DIR_SEPARATOR"saves"));
    if (PHYSFS_stat(temp.data, NULL) == 0) {
        SDL_Log("mkdir %s", temp.data);
        sota_mkdir(temp.data);
    }

    SDL_Log("Mounting saves dir: '%s'", temp.data);
    Filesystem_Mount(temp);

    /* -- Mounting build directory -- */
    temp = s8_Path_Remove_Top(temp, DIR_SEPARATOR[0]);
    temp = s8cat(temp, s8_literal(DIR_SEPARATOR SOTA_BUILD_DIR));
    Filesystem_Mount(temp);

    /* -- Mounting tiles directory -- */
    temp = s8_Path_Remove_Top(temp, DIR_SEPARATOR[0]);
    temp = s8cat(temp, s8_literal(DIR_SEPARATOR"tiles"));
    Filesystem_Mount(temp);

    /* -- Mounting scenes directory -- */
    temp = s8_Path_Remove_Top(temp, DIR_SEPARATOR[0]);
    temp = s8cat(temp, s8_literal(DIR_SEPARATOR"scenes"));
    Filesystem_Mount(temp);

    /* -- Mounting items directory -- */
    temp = s8_Path_Remove_Top(temp, DIR_SEPARATOR[0]);
    temp = s8cat(temp, s8_literal(DIR_SEPARATOR "items"));
    Filesystem_Mount(temp);

    /* -- Mounting units directory -- */
    temp = s8_Path_Remove_Top(temp, DIR_SEPARATOR[0]);
    temp = s8cat(temp, s8_literal(DIR_SEPARATOR"units"));
    Filesystem_Mount(temp);

    /* -- Mounting assets directory -- */
    temp = s8_Path_Remove_Top(temp, DIR_SEPARATOR[0]);
    temp = s8cat(temp, s8_literal(DIR_SEPARATOR"assets"));
    Filesystem_Mount(temp);

    /* -- Mounting assets/Maps -- */
    temp = s8cat(temp, s8_literal(DIR_SEPARATOR"maps"));
    Filesystem_Mount(temp);

    /* -- Mounting assets/Tiles -- */
    temp = s8_Path_Remove_Top(temp, DIR_SEPARATOR[0]);
    temp = s8cat(temp, s8_literal(DIR_SEPARATOR"tiles"));
    Filesystem_Mount(temp);

    /* -- Mounting assets/Map_Units -- */
    temp = s8_Path_Remove_Top(temp, DIR_SEPARATOR[0]);
    temp = s8cat(temp, s8_literal(DIR_SEPARATOR"map_units"));
    Filesystem_Mount(temp);

    /* -- Mount assets.binou -- */
    temp = s8_Path_Remove_Top(temp, DIR_SEPARATOR[0]);
    temp = s8cat(temp, s8_literal(DIR_SEPARATOR"assets.binou"));
    Filesystem_Mount(temp);

    /* -- Cleanup -- */
    s8_free(&srcDir);
    s8_free(&assetsDir);
    s8_free(&temp);
    return 1;
}

int Filesystem_Free(void) {
    return (PHYSFS_deinit());
}

/* --- UTILITIES --- */
void Filesystem_Load_Bytes( char *name, u8 **mem, size_t *len) {
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

bool Filesystem_fequal( char *filename1,  char *filename2) {
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
SDL_Surface *Filesystem_Surface_Load( char *filename,  u32 format) {
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

SDL_Texture *Filesystem_Texture_Load(struct SDL_Renderer *renderer,  char *file, u32 format) {
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

void Filesystem_Texture_Dump( char *filename, SDL_Renderer *renderer, SDL_Texture *texture,
                              u32 format, SDL_Texture *render_target) {
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

/* --- SCENE --- */
s8 Filesystem_Scene_Chapter(s8 filename, int chapter) {
    if (chapter < CHAPTER_1) {
        filename = s8cat(filename, s8_literal("Prologue"));
    } else if (chapter == CHAPTER_FOLDER_TEST) {
        filename = s8cat(filename, s8_literal("Test"));
    } else if (chapter == CHAPTER_EPILOGUE) {
        filename = s8cat(filename, s8_literal("Epilogue"));
    } else {
        filename = s8cat(filename, s8_literal("Chapter_"));
        char chapter_num[2];
        stbsp_sprintf(chapter_num, "%d", chapter);
        filename = s8cat(filename, s8_literal(chapter_num));
    }
    filename = s8cat(filename, s8_literal(DIR_SEPARATOR));
    return (filename);
}
