#include "str.h"
#include "SDL2/SDL.h"

// str functions assumes the caller deals with memory
char * str_slicefromStart(char * in_str, uint8_t toslice) {
    for (uint16_t i = toslice; i < strlen(in_str) ; i++) {
        *(in_str + i - toslice) = (char) * (in_str + i);
    }
    *(in_str - toslice + strlen(in_str)) = '\0';
    return (in_str);
}

char * str_slicefromEnd(char * in_str, uint8_t toslice) {
    for (uint16_t i = 0; i < (strlen(in_str) - toslice) ; i++) {
        *(in_str + i) = (char) * (in_str + i);
    }
    *(in_str - toslice + strlen(in_str)) = '\0';
    return (in_str);
}

char * str_toLower(char * in_str) {
    for (uint16_t i = 0; i < strlen(in_str) ; i++) {
        *(in_str + i) = tolower(*(in_str + i));
    }
    return (in_str);
}

char * str_toUpper(char * in_str) {
    for (uint16_t i = 0; i < strlen(in_str) ; i++) {
        *(in_str + i) = (char)toupper(*(in_str + i));
    }
    return (in_str);
}

char * str_camelCase(char * in_str, const char separator, uint8_t minwordlen) {
    uint8_t wordlen = 0;
    uint16_t i = 0;
    for (uint16_t i = 0; i <= strlen(in_str) ; i++) {
        if ((*(in_str + i) == separator) || (i == strlen(in_str))) {
            if (wordlen > minwordlen) {
                *(in_str + i - wordlen) = (char)toupper(*(in_str + i - wordlen));
            }
            wordlen = 0;
        } else {
            wordlen++;
        }
    }
    return (in_str);
}

char * str_replaceSingle(char * in_str, const char replace, const char with) {
    for (uint16_t i = 0; i < strlen(in_str) ; i++) {
        if (*(in_str + i) == replace) {
            *(in_str + i) = with;
        }
    }
    return (in_str);
}

char * path_Top_Remove(char * in_path, const char separator) {
    char * folder = strrchr(in_path, separator) + 1;
    *(in_path + (strlen(in_path) - strlen(folder) - 1)) = '\0';
    if (strlen(folder) == 0) {
        folder = strrchr(in_path, separator) + 1;
        *(in_path + (strlen(in_path) - strlen(folder) - 1)) = '\0';
    }
    return (in_path);
}

char * path_Top_Split(char * in_path, const char separator) {
    char * temp = (char *) malloc(strlen(in_path) + 1);
    strcpy(temp, in_path);
    * (temp + strlen(in_path)) = '\0';
    char * folder = strrchr(temp, separator) + 1;
    if (strlen(folder) == 0) {
        *(temp + (strlen(temp) - strlen(folder) - 1)) = '\0';
        folder = strrchr(temp, separator) + 1;
    }
    strcpy(in_path, temp + strlen(temp) - strlen(folder));
    free(temp);
    return (in_path);
}

char * path_extension(char * filename) {
    char * dot = strrchr(filename, '.');
    if (!dot || dot == filename) { return ""; }
    return (dot);
}
