#include "nstr.h"

/* --- Pascal String s8 strings --- */
// 3x-4x faster than null-terminated strings!
s8 s8_mut(char *string) {
    s8 s8_string;
    s8_string.len  = strlen(string);
    s8_string.data = malloc(s8_string.len);
    memcpy(s8_string.data, string, s8_string.len);
    return(s8_string);
}

void s8_free(s8 string) {
    if (string.data != NULL)
        free(string.data);
}

b32 s8equal(s8 s1, s8 s2) {
    if(s1.len != s2.len)
        return(false);

    for (int i = 0; i < s1.len; i++)
        if (s1.data[i] != s1.data[i])
            return(false);

    return(true);
}

void s8_slicefromStart(s8 *string, size_t toslice) {
    for (size_t i = toslice; i < string->len; i++)
        *(string->data + i - toslice) = (u8) *(string->data + i);
    string->len -= toslice;
}

void s8_slicefromEnd(s8 *string, size_t toslice) {
    for (size_t i = 0; i < (string->len - toslice) ; i++)
        *(string->data + i) = (u8) *(string->data + i);
    string->len -= toslice;
}

void s8_toLower(s8 *string) {
    for (size_t i = 0; i < string->len; i++)
        *(string->data + i) = (u8)tolower(*(string->data + i));
}

void s8_toUpper(s8 *string) {
    for (size_t i = 0; i < string->len; i++)
        *(string->data + i) = (u8)toupper(*(string->data + i));
}

void s8_camelCase(s8 *str8, const char separator, size_t minwordlen) {
    size_t wlen = 0;
    for (size_t i = 0; i <= str8->len; i++) {
        int word_end = (*(str8->data + i) == separator) || (i == str8->len);
        if (!word_end) {
            /* Next character in word. */
            wlen++;
            continue;
        }
        /* Word end found. */
        if (wlen > minwordlen)
            /* Upper First letter of word, if longer than min */
            *(str8->data + i - wlen) = (u8)toupper(*(str8->data + i - wlen));
        wlen = 0;
    }
}

void s8_replaceSingle(s8 *string, const char replace, const char with) {
    for (size_t i = 0; i < string->len; i++) {
        if (*(string->data + i) == replace)
            *(string->data + i) = with;
    }
}

void s8_Path_Remove_Top(s8 *path, const char separator) {
    char *folder        = strrchr(path->data, separator) + 1;
    size_t len_folder   = strlen(folder);
    *(path->data + (path->len - len_folder - 1)) = '\0';
    path->len -= len_folder;
}

void s8_Replace(s8 *string, const char *replace, const char *with) {
    /* find replace pos */
    char *found = strstr(string->data, replace);
    if (found != NULL) {
        // size_t len_f  = found - string->data;
        // size_t len_r  = strlen(replace);
        // size_t len_w  = strlen(with);
        // size_t len_l  = strlen(string->data);
        // size_t len_nl = len_l + len_w - len_r;
        // /* accomodate new str len */
        // memmove(string->data + len_f + len_w,
        //         string->data + len_f + len_r,
        //         len_l - (len_f + len_r));
        // /* overwrite replace with */
        // strncpy(string->data + len_f, with, len_w);
        // string->data[len_nl] = '\0';
        // string->len          = len_nl;
    }
}

/* --- Null-terminated strings --- */
// NOTE: caller deals with memory
char *nstr_slicefromStart(char *in_str, size_t toslice) {
    for (size_t i = toslice; i < strlen(in_str) ; i++) {
        *(in_str + i - toslice) = (u8) * (in_str + i);
    }
    *(in_str - toslice + strlen(in_str)) = '\0';
    return (in_str);
}

char *nstr_slicefromEnd(char *in_str, size_t toslice) {
    for (size_t i = 0; i < (strlen(in_str) - toslice) ; i++) {
        *(in_str + i) = (u8) * (in_str + i);
    }
    *(in_str - toslice + strlen(in_str)) = '\0';
    return (in_str);
}

char *nstr_toLower(char *in_str) {
    for (size_t i = 0; i < strlen(in_str) ; i++) {
        *(in_str + i) = tolower(*(in_str + i));
    }
    return (in_str);
}

char *nstr_toUpper(char *in_str) {
    for (size_t i = 0; i < strlen(in_str); i++) {
        *(in_str + i) = (u8)toupper(*(in_str + i));
    }
    return (in_str);
}

char *nstr_camelCase(char *in_str, const char separator, size_t minwordlen) {
    size_t wordlen = 0;
    for (size_t i = 0; i <= strlen(in_str) ; i++) {
        if ((*(in_str + i) == separator) || (i == strlen(in_str))) {
            if (wordlen > minwordlen) {
                *(in_str + i - wordlen) = (u8)toupper(*(in_str + i - wordlen));
            }
            wordlen = 0;
        } else {
            wordlen++;
        }
    }
    return (in_str);
}

char *nstr_replaceSingle(char *in_str, const char replace, const char with) {
    for (size_t i = 0; i < strlen(in_str) ; i++) {
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
