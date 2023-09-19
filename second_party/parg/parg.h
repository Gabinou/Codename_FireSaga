/*
 * parg - parse argv
 *
 * Written in 2015-2016 by Joergen Ibsen
 * Modified in 2023 by Gabriel Taillon for SotA
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to the
 * public domain worldwide. This software is distributed without any
 * warranty. <http://creativecommons.org/publicdomain/zero/1.0/>
 */

#ifndef PARG_INCLUDED
#define PARG_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PARG_VER_MAJOR 1
#define PARG_VER_MINOR 0
#define PARG_VER_PATCH 2
#define PARG_VER_STRING "1.0.2"

enum PARG_HAS_ARG {
    PARG_NOARG,
    PARG_REQARG, /* required */
    PARG_OPTARG  /* optional */
};

struct parg_state {
    const char * optarg;  /* option argument*/
    int optind;           /* next index */
    int optopt;           /* error option */
    const char * nextchar;
};
extern struct parg_state parg_state_default;

/* Long options for `parg_getopt_long()` */
struct parg_opt {
    const char * name;
    int has_arg;
    int * flag;
    int val;
    const char * arg;
    const char * doc;
};

/* - parg help - */
extern void parg_usage(const char * n, const struct parg_opt * lo);

/* - option matching - */
extern int match_long(struct parg_state * ps, int c, char * const v[], const char * o,
                      const struct parg_opt * lo, int * li);
extern int match_short(struct parg_state * ps, int c, char * const v[], const char * os);

/* - utilities - */
extern void reverse(char * v[], int i, int j);
extern int is_argv_end(const struct parg_state * ps, int c, char * const v[]);

/* - argv reordering - */
extern int parg_reorder_simple(int c, char * v[], const char * os, const struct parg_opt * lo);
extern int parg_reorder(int c, char * v[], const char * os, const struct parg_opt * lo);

/* - parg public API: getopt and getopt_long - */
extern int parg_getopt(struct parg_state * ps, int c, char * const v[], const char * os);
extern int parg_getopt_long(struct parg_state * ps, int c, char * const v[],
                            const char * os, const struct parg_opt * lo, int * li);

#endif /* PARG_INCLUDED */
