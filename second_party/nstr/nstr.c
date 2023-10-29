#include "nstr.h"

/* --- NOTE --- */
// caller deals with memory

char *nstr_slicefromStart(char *in_str, uint_fast8_t toslice) {
    for (uint_fast16_t i = toslice; i < strlen(in_str) ; i++) {
        *(in_str + i - toslice) = (char) * (in_str + i);
    }
    *(in_str - toslice + strlen(in_str)) = '\0';
    return (in_str);
}

char *nstr_slicefromEnd(char *in_str, uint_fast8_t toslice) {
    for (uint_fast16_t i = 0; i < (strlen(in_str) - toslice) ; i++) {
        *(in_str + i) = (char) * (in_str + i);
    }
    *(in_str - toslice + strlen(in_str)) = '\0';
    return (in_str);
}

char *nstr_toLower(char *in_str) {
    for (uint_fast16_t i = 0; i < strlen(in_str) ; i++) {
        *(in_str + i) = tolower(*(in_str + i));
    }
    return (in_str);
}

char *nstr_toUpper(char *in_str) {
    for (uint_fast16_t i = 0; i < strlen(in_str); i++) {
        *(in_str + i) = (char)toupper(*(in_str + i));
    }
    return (in_str);
}

char *nstr_camelCase(char *in_str, const char separator, uint_fast8_t minwordlen) {
    uint_fast8_t wordlen = 0;
    for (uint_fast16_t i = 0; i <= strlen(in_str) ; i++) {
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

char *nstr_replaceSingle(char *in_str, const char replace, const char with) {
    for (uint_fast16_t i = 0; i < strlen(in_str) ; i++) {
        if (*(in_str + i) == replace) {
            *(in_str + i) = with;
        }
    }
    return (in_str);
}

char *nstr_Path_Remove_Top(char *in_path, const char separator) {
    char *folder = strrchr(in_path, separator) + 1;
    *(in_path + (strlen(in_path) - strlen(folder) - 1)) = '\0';
    if (strlen(folder) == 0) {
        folder = strrchr(in_path, separator) + 1;
        *(in_path + (strlen(in_path) - strlen(folder) - 1)) = '\0';
    }
    return (in_path);
}

char *nstr_Path_Split_Top(char *in_path, const char separator) {
    char *temp = (char *) malloc(strlen(in_path) + 1);
    strcpy(temp, in_path);
    * (temp + strlen(in_path)) = '\0';
    char *folder = strrchr(temp, separator) + 1;
    if (strlen(folder) == 0) {
        *(temp + (strlen(temp) - strlen(folder) - 1)) = '\0';
        folder = strrchr(temp, separator) + 1;
    }
    strcpy(in_path, temp + strlen(temp) - strlen(folder));
    free(temp);
    return (in_path);
}

char *nstr_Path_Extension(char *filename) {
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) {
        return "";
    }
    return (dot);
}

char *nstr_Replace(char *line, char *replace, char *with) {
    /* find replace pos */
    char *found = strstr(line, replace);
    if (found != NULL) {
        size_t len_f = found - line;
        size_t len_r = strlen(replace);
        size_t len_w = strlen(with);
        size_t len_l = strlen(line);
        size_t len_nl = len_l + len_w - len_r;
        /* accomodate new str len */
        memmove(line + len_f + len_w,
                line + len_f + len_r,
                len_l - (len_f + len_r));
        /* overwrite replace with */
        strncpy(line + len_f, with, len_w);
        line[len_nl] = '\0';
    }
    return(line);
}
