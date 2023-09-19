#ifndef NSTR_H
#define NSTR_H

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --- NOTE --- */
// caller deals with memory

extern char * nstr_replaceSingle(char * in_str, const char replace, const char with);
extern char * nstr_camelCase(char * in_str, const char separator, uint8_t minwordlen);
extern char * nstr_slicefromStart(char * in_str, uint8_t toslice);
extern char * nstr_slicefromEnd(char * in_str, uint8_t toslice);
extern char * nstr_toLower(char * in_str);
extern char * nstr_toUpper(char * in_str);
extern char * nstr_Replace(char * line, char * replace, char * with);

extern char * nstr_Path_Extension(char * filename);
extern char * nstr_Path_Join(char * path, char * top);
extern char * nstr_Path_Remove_Top(char * path, const char separator);
extern char * nstr_Path_Split_Top(char * path, const char separator);

#endif /* NSTR_H */
