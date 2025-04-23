#include "nstr.h"
#include "SDL.h"

/* --- Pascal String s8 strings --- */
/* 3x-4x faster than null-terminated strings! */
/* -- s8_mut ONLY, unless noted otherwise -- */

s8 s8_mut(char *string) {
    s8 s8_string;
    size_t len = strlen(string) + 1;
    s8_string.num  = len - 1;
    s8_string.len  = len < NSTR_MIN_LEN ? NSTR_MIN_LEN : len;
    s8_string.data = SDL_calloc(1, s8_string.len);
    memcpy(s8_string.data, string, s8_string.num);
    return (s8_string);
}

b32 s8valid(s8 str8) {
    return (str8.data != NULL);
}

void s8_free(s8 *str8) {
    /* str8 is a pointer to modify to member variables */
    if (str8->data != NULL) {
        SDL_free(str8->data);
        str8->data = NULL;
    }
    str8->num = 0;
    str8->len = 0;
}

b32 s8equal(s8 s1, s8 s2) {
    /* -- 8_literal and s8_var OK -- */
    if (s1.num != s2.num)
        return (false);

    for (int i = 0; i < s1.num; i++)
        if (s1.data[i] != s2.data[i])
            return (false);

    return (true);
}

s8 s8cat(s8 s1, s8 s2) {
    /* Grow buffer */
    while ((s1.num + s2.num) >= (s1.len - 1)) {
        size_t newlen = s1.len * 2;
        s1.data = SDL_realloc(s1.data, newlen * sizeof(*s1.data));
        memset(s1.data + s1.num, 0, newlen - s1.num);
        s1.len = newlen;
    }

    /* Concatenate */
    SDL_assert(s1.data != NULL);
    SDL_assert(s2.data != NULL);
    memcpy(s1.data + s1.num, s2.data, s2.num);
    s1.num += s2.num;
    SDL_assert(strlen(s1.data) == s1.num);
    return (s1);
}

s8 s8cpy(s8 s1, s8 s2) {
    /* If null, allocate a copy */
    if (s1.data == NULL)
        return (s8_mut(s2.data));

    /* Increase buffer size for string to copy */
    while (s2.num >= (s1.len - 1)) {
        size_t newlen = s1.len < NSTR_MIN_LEN ? NSTR_MIN_LEN * 2 : s1.len * 2;
        s1.data = SDL_realloc(s1.data, newlen * sizeof(*s1.data));
        memset(s1.data + s1.num, 0, newlen - s1.num);
        s1.len = newlen;
    }

    /* Copy */
    memcpy(s1.data, s2.data, s2.num);
    s1.num = s2.num;
    return (s1);
}

s8 s8_slicefromStart(s8 str8, size_t toslice) {
    for (size_t i = toslice; i < str8.num; i++)
        *(str8.data + i - toslice) = (u8) * (str8.data + i);
    str8.num -= toslice;
    return (str8);
}

s8 s8_slicefromEnd(s8 str8, size_t toslice) {
    for (size_t i = 0; i < (str8.num - toslice) ; i++)
        *(str8.data + i) =  *(str8.data + i);
    str8.num -= toslice;
    return (str8);
}

s8 s8_toLower(s8 str8) {
    SDL_assert(str8.num < str8.len);
    for (size_t i = 0; i < str8.num; i++)
        *(str8.data + i) = (u8)tolower(*(str8.data + i));
    return (str8);
}

s8 s8_toUpper(s8 str8) {
    SDL_assert(str8.num < str8.len);
    for (size_t i = 0; i < str8.num; i++)
        *(str8.data + i) = (u8)toupper(*(str8.data + i));
    return (str8);
}

s8 s8_camelCase(s8 str8, char separator, size_t minwordlen) {
    size_t wlen = 0;
    for (size_t i = 0; i <= str8.num; i++) {
        int word_end = (*(str8.data + i) == separator) || (i == str8.num);
        if (!word_end) {
            /* Next character in word. */
            wlen++;
            continue;
        }
        /* Word end found. */
        if (wlen > minwordlen)
            /* Upper First letter of word, if longer than min */
            *(str8.data + i - wlen) = (u8)toupper(*(str8.data + i - wlen));
        wlen = 0;
    }
    return (str8);
}

s8 s8_Path_Remove_Top(s8 str8, char separator) {
    char *folder = strrchr(str8.data, separator);

    /* Skip if no separator found */
    if (folder == NULL)
        return (str8);

    /* Set all bytes to 0 after top separator */
    size_t len_folder   = strlen(folder);
    str8.num -= len_folder;
    memset(str8.data + str8.num, 0, str8.len - str8.num);
    return (str8);
}

s8 s8_replaceSingle(s8 str8,  char replace,  char with) {
    SDL_assert(str8.num < str8.len);
    for (size_t i = 0; i < str8.num; i++) {
        if (*(str8.data + i) == replace)
            *(str8.data + i) = with;
    }
    return (str8);
}

s8 s8_Replace(s8 str8,  char *replace,  char *with) {
    /* find replace pos */
    u8 *found = strstr(str8.data, replace);
    if (found != NULL) {
        size_t len_f  = found - str8.data;
        size_t len_r  = strlen(replace);
        size_t len_w  = strlen(with);
        size_t len_l  = strlen(str8.data);
        size_t len_nl = len_l + len_w - len_r;

        /* accomodate new str len */
        while (len_nl > str8.len) {
            size_t newlen = str8.len * 2;
            str8.data = SDL_realloc(str8.data, newlen * sizeof(*str8.data));
            memset(str8.data + str8.num, 0, newlen - str8.num);
            str8.len = newlen;
        }

        /* move text after replace to after with */
        memmove(str8.data + len_f + len_w,
                str8.data + len_f + len_r,
                len_l - (len_f + len_r));

        /* overwrite replace with */
        strncpy(str8.data + len_f, with, len_w);
        str8.data[len_nl] = '\0';
        str8.num          = len_nl;
    }
    return (str8);
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

char *nstr_camelCase(char *in_str,  char separator, size_t minwordlen) {
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

char *nstr_replaceSingle(char *in_str,  char replace,  char with) {
    for (size_t i = 0; i < strlen(in_str) ; i++) {
        if (*(in_str + i) == replace) {
            *(in_str + i) = with;
        }
    }
    return (in_str);
}

char *nstr_Path_Remove_Top(char *in_path,  char separator) {
    char *folder = strrchr(in_path, separator) + 1;
    *(in_path + (strlen(in_path) - strlen(folder) - 1)) = '\0';
    if (strlen(folder) == 0) {
        folder = strrchr(in_path, separator) + 1;
        *(in_path + (strlen(in_path) - strlen(folder) - 1)) = '\0';
    }
    return (in_path);
}

char *nstr_Path_Split_Top(char *in_path,  char separator) {
    char *temp = (char *) SDL_malloc(strlen(in_path) + 1);
    strcpy(temp, in_path);
    * (temp + strlen(in_path)) = '\0';
    char *folder = strrchr(temp, separator) + 1;
    if (strlen(folder) == 0) {
        *(temp + (strlen(temp) - strlen(folder) - 1)) = '\0';
        folder = strrchr(temp, separator) + 1;
    }
    strcpy(in_path, temp + strlen(temp) - strlen(folder));
    SDL_free(temp);
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
    return (line);
}
