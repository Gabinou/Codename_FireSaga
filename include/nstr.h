#ifndef NSTR_H
#define NSTR_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "types.h"

enum NSTR {
    NSTR_MIN_LEN = 16,
};

/* --- Pascal String s8 strings --- */
// - String concatenation
// - Assignment copiable
//      - Can't use flexible array member (FAM).
//        User would need to malloc and memcpy.

typedef struct s8 {
    u8      *data;  /* First element -> &s8 == &s8.data */
    size_t   len;   /* allocated */
    size_t   num;   /* used, < len*/
} s8;

/* -- Utilities -- */
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define lengthof(s)  (countof(s) - 1)

/* -- s8 from string literal, string array -- */
#define s8_literal(s) (s8){(u8 *)s, lengthof(s), lengthof(s)}

/* -- s8 from pointer to string -- */
// -> Needed cause sizeof() gives pointer size...
// TODO: max length
#define s8_var(s)   s8_var_(s)
#define s8_var_(s) (s8){(u8 *)s, strlen(s), strlen(s)}


/* -- Mutable s8 functions -- */
// s8 function input notes
//  - foo(s8 *str8) -> foo can change all member variables
//  - bar(s8  str8) -> bar can only change *str8.data,
//                     ie what str8.data points to
s8   s8_mut( char *string);
void s8_free(s8   *str8);

s8 s8_slicefromEnd(  s8 str8, size_t toslice);
s8 s8_slicefromStart(s8 str8, size_t toslice);

s8 s8_toLower(s8 str8);
s8 s8_toUpper(s8 str8);

b32 s8equal(s8 s1, s8 s2);
b32 s8valid(s8 str8);

s8 s8cat(s8 s1, s8 s2);
s8 s8cpy(s8 s1, s8 s2);

s8 s8_camelCase(s8 str8,  char separator, size_t minwordlen);

s8 s8_Replace(      s8 str8, char *replace, char *with);
s8 s8_replaceSingle(s8 str8, char  replace, char  with);

s8 s8_Path_Remove_Top(s8 path, char separator);

/* --- Null-terminated strings --- */
/* - Note: caller deals with memory - */
char *nstr_toLower(       char *str);
char *nstr_toUpper(       char *str);
char *nstr_Replace(       char *line, char *replace, char *with);
char *nstr_camelCase(     char *str,  char separator, size_t minwordlen);
char *nstr_slicefromEnd(  char *str, size_t toslice);
char *nstr_replaceSingle( char *str,  char replace,  char with);
char *nstr_slicefromStart(char *str, size_t toslice);

char *nstr_Path_Join(      char *path, char *top);
char *nstr_Path_Extension( char *filename);
char *nstr_Path_Split_Top( char *path,  char separator);
char *nstr_Path_Remove_Top(char *path,  char separator);

#endif /* NSTR_H */
