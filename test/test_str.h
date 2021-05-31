#include "minctest.h"
#include "platform.h"
#include "str.h"

void test_str() {
    SDL_Log("test_str");

    char * temp_str = (char *)malloc(DEFAULT_BUFFER_SIZE);
    strncpy(temp_str, "Hello_Anna", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_replaceSingle(temp_str, '_', ' '), "Hello Anna") == 0);
    strncpy(temp_str, "Hello=Anna", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_replaceSingle(temp_str, '=', ' '), "Hello Anna") == 0);
    strncpy(temp_str, "Hello Anna", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_replaceSingle(temp_str, ' ', '_'), "Hello_Anna") == 0);

    strncpy(temp_str, "Hello_Anna", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_toLower(temp_str), "hello_anna") == 0);
    strncpy(temp_str, "My Mama Told Me So", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_toLower(temp_str), "my mama told me so") == 0);
    strncpy(temp_str, "My Mama Told Me So", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_toUpper(temp_str), "MY MAMA TOLD ME SO") == 0);

    strncpy(temp_str, "my mama told me so", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_camelCase(temp_str, ' ', 1), "My Mama Told Me So") == 0);
    strncpy(temp_str, "my mama told me so", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_camelCase(temp_str, ' ', 2), "my Mama Told me so") == 0);
    strncpy(temp_str, "my mama told me so", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_camelCase(temp_str, '_', 3), "My mama told me so") == 0);

    // if ((PLATFORM == WINDOWS_64) || (PLATFORM == WINDOWS_32)) {
    //     lok(strcmp(path_extension("C:"DIR_SEPARATOR"myfile.txt"), ".txt") == 0);
    //     lok(strcmp(path_extension("C:"DIR_SEPARATOR"Sync"DIR_SEPARATOR"myfile.csv"), ".csv") == 0);

    //     lok(strcmp(path_Top_Remove("C:"DIR_SEPARATOR"Sync"DIR_SEPARATOR"pictures"DIR_SEPARATOR"step.txt", DIR_SEPARATOR), "C:"DIR_SEPARATOR"Sync"DIR_SEPARATOR"pictures") == 0);
    //     lok(strcmp(path_Top_Remove("C:"DIR_SEPARATOR"Sync"DIR_SEPARATOR"pictures"DIR_SEPARATOR"", DIR_SEPARATOR), "C:"DIR_SEPARATOR"Sync"DIR_SEPARATOR"") == 0);

    //     lok(strcmp(path_Top_Split("C:"DIR_SEPARATOR"Sync"DIR_SEPARATOR"pictures"DIR_SEPARATOR"", DIR_SEPARATOR), "pictures") == 0);
    //     lok(strcmp(path_Top_Split("C:"DIR_SEPARATOR"Sync"DIR_SEPARATOR"pictures2a", DIR_SEPARATOR), "pictures2a") == 0);
    //     lok(strcmp(path_Top_Split("D:"DIR_SEPARATOR"Gamebinours"DIR_SEPARATOR"Tagueule"DIR_SEPARATOR"COnnard"DIR_SEPARATOR"Demerde"), "Demerde") == 0);
    // }
    strncpy(temp_str, "This is a beautiful day", DEFAULT_BUFFER_SIZE);
    lok(strcmp("is a beautiful day", str_slicefromStart(temp_str, 5)) == 0);
    strncpy(temp_str, "This is a beautiful day", DEFAULT_BUFFER_SIZE);
    lok(strcmp("This is a beautifu", str_slicefromEnd(temp_str, 5)) == 0);
}
