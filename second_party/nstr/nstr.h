#ifndef NSTR_H
#define NSTR_H

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef int32_t b32;

/* --- Pascal String s8 strings --- */
typedef struct {
    u8      *data;
    size_t   len;
} s8;

/* -- Immutable s8 -- */
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define lengthof(s)  (countof(s) - 1)

#define s8_literal(s) (s8){(u8 *)s, lengthof(s)}
#define s8_var(s) s8_var_(s)
#define s8_var_(s) (s8){(u8 *)s, strlen(s)}

/* -- Mutable s8 -- */
s8  *s8_Init(char *string);
void s8_Free(s8   *string);

void s8_slicefromEnd(  s8 *string, size_t toslice);
void s8_slicefromStart(s8 *string, size_t toslice);

void s8_toLower(s8 *string);
void s8_toUpper(s8 *string);

// TODO: use s8equal instead of strcmp 
b32 s8equal(s8 *s1, s8 *s2);

void s8_camelCase(s8 *str8, const char separator, size_t minwordlen);

void s8_Replace(      s8 *string, const char *replace, const char *with);
void s8_replaceSingle(s8 *string, const char  replace, const char  with);


void s8_Path_Remove_Top(s8 *path, const char separator);


/* --- Null-terminated strings --- */
/* - Note: caller deals with memory - */
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
