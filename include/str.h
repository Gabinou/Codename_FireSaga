#ifndef STR_H
#define STR_H

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Note: all str_ functions malloc ->  free(output char *)
extern char * str_replaceSingle(char * in_str, const char replace, const char with);
extern char * str_camelCase(char * in_str, const char separator, uint8_t minwordlen);
extern char * str_slicefromStart(char * in_str, uint8_t toslice);
extern char * str_slicefromEnd(char * in_str, uint8_t toslice);
extern char * str_toLower(char * in_str);
extern char * str_toUpper(char * in_str);

extern char * path_extension(char * filename);
extern char * path_join(char * path, char * top);
extern char * path_Top_Remove(char * path, const char separator);
extern char * path_Top_Split(char * path, const char separator);

#endif /* STR_H */
