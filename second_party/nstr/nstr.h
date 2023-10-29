#ifndef NSTR_H
#define NSTR_H

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --- NOTE --- */

/* --- caller deals with memory --- */
typedef uint8_t u8;

#define countof(a)  (sizeof(a) / sizeof(*(a)))  /* Includes \0 */
#define lengthof(s) (countof(s) - 1)            /* Excludes \0 */

// #define s8(s) (s8){(u8 *)s, strlen(s)-1}
typedef struct {
    u8      *data;
    size_t   len;
} s8;
s8  *s8_Init(char *string);
void s8_Free(s8  *string);

void s8_toLower(s8 *string);
void s8_toUpper(s8 *string);

/* caller deals with memory */
char *nstr_toLower(        char *str);
char *nstr_toUpper(        char *str);
char *nstr_Replace(        char *line, char * replace, char * with);
char *nstr_camelCase(      char *str, const char separator, size_t minwordlen);
char *nstr_slicefromEnd(   char *str, size_t toslice);
char *nstr_replaceSingle(  char *str, const char replace, const char with);
char *nstr_slicefromStart( char *str, size_t toslice);

char *nstr_Path_Join(      char *path, char * top);
char *nstr_Path_Extension( char *filename);
char *nstr_Path_Split_Top( char *path, const char separator);
char *nstr_Path_Remove_Top(char *path, const char separator);

#endif /* NSTR_H */
