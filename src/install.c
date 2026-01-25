/*
**  Copyright 2023-2026 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
**  IES installer.
**      1. Copies game to pref dir
**      2. Copies asset archive to pref dir
**  Note: uses names folder includes
**
*/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "physfs.h"

#include "nstr.h"
#include "names/game.h"
#include "names/zip_archive.h"

#define STRINGIFY(x) #x
#define STRINGIZE(x) STRINGIFY(x)

void physfs_searchpath(void) {
    /* -- Debug: printing search path -- */
    char **i;
    for (i = PHYSFS_getSearchPath(); *i != NULL; i++) {
        printf("[%s] is in the search path.\n", *i);
    }
}

void physfs_copy(s8 from, s8 to) {
    /* Note: does not copy permissions */
    
    if (from.data == NULL) {
        printf("from is NULL\n");
    }
    if (to.data == NULL) {
        printf("to is NULL\n");
    }
    /* -- read file -- */
    PHYSFS_file *ffrom = PHYSFS_openRead(from.data);
    if (ffrom == NULL) {
        printf("Could not open '%s' for reading\n", from.data);
        return;
    }

    i64 flen = PHYSFS_fileLength(ffrom);
    if (flen <= 0) {
        printf("Problem with file length %d\n", flen);
        return;
    }
    /* Note: calloc in case too big for stack */
    char *fbuff = calloc(1, flen);
    if (fbuff == NULL) {
        printf("Could not allocate %d bytes\n", flen);
        return;
    }

    PHYSFS_readBytes(ffrom, fbuff, flen);
    PHYSFS_close(ffrom);

    /* -- write file -- */
    PHYSFS_file *fto = PHYSFS_openWrite(to.data);
    if (fto == NULL) {
        printf("Could not open '%s' for writing\n", to.data);
        free(fbuff);
        return;
    }

    PHYSFS_writeBytes(fto, fbuff, flen);
    PHYSFS_close(fto);
    free(fbuff);
}

int main(int argc, char *argv[]) {
    const char *app;
    const char *org;
    const char *sep;
    const char *prefdir;
    const char *writedir;

    /* -- 0- physfs init -- */
    if (PHYSFS_init(argv[0]) <= 0) {
        printf("Could not initialize PhysFS \n");
        exit(1);
    }
    s8 archive      = s8_mut(STRINGIFY(ZIP_ARCHIVE_NAME));
    s8 extension    = s8_mut(archive.data);
    s8_Path_Remove_Bottom(extension, '.');

    PHYSFS_setSaneConfig(   STRINGIZE(GAME_COMPANY),   
                            STRINGIZE(GAME_TITLE_ABREV),
                            extension.data,  0, 0);

    /* -- 1- Read org, app name from central location -- */
    app = STRINGIZE(GAME_TITLE_ABREV);
    org = STRINGIZE(GAME_COMPANY);
    printf("Installing %s \n", app);

    /* -- 2- get prefdir -- */
    prefdir = PHYSFS_getPrefDir(org, app);
    if (NULL == PHYSFS_setWriteDir(prefdir)) {
        printf("Could not set write dir '%s' \n", prefdir);
    }

    /* -- 3- Copy exe to prefdir -- */
    /* Note: 
    **  - Reads from search path, writes to write dir
    **    so -> (from, from) works
    */
    printf("Copying game to '%s%s' \n", 
            prefdir, STRINGIZE(GAME_TITLE_ABREV));
    s8 exe_from = s8_literal(STRINGIZE(GAME_TITLE_ABREV));
    physfs_copy(exe_from, exe_from);

    /* -- 4- Copy data.bsa to prefdir -- */
    printf("Copying assets to '%s%s' \n", 
            prefdir, STRINGIZE(ZIP_ARCHIVE_NAME));
    s8 ar_from = s8_literal(STRINGIZE(ZIP_ARCHIVE_NAME));
    physfs_copy(ar_from, ar_from);

    /* -- 5- make saves dir -- */
    if (0 == PHYSFS_mkdir("saves")) {
        printf("Could not mkdir '%s' \n", "saves");
    }

    return (0);
}
