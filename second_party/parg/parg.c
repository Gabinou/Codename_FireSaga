
#include "parg.h"

struct parg_state parg_state_default = {
    .optarg = NULL,
    .optind = 1,
    .optopt = '?',
    .nextchar = NULL
};

/* Automatic usage/help printing */
void parg_usage(const char * name, const struct parg_opt * longopts) {
    assert(longopts);
    printf("\nUsage: %s [OPTIONS]\n", name);
    for (int i = 0; longopts[i].doc; ++i) {
        if (longopts[i].val)
            printf(" -%c", longopts[i].val);
 
        if (longopts[i].name)
            printf(",  --%s", longopts[i].name);

        if (longopts[i].arg)
            printf("\t[=%s]\t\t", longopts[i].arg);
        else if (longopts[i].val || longopts[i].name)
            printf("\t\t");

        if ((!longopts[i].arg) && ((longopts[i].val) || (longopts[i].name)))
            printf("\t");

        if (longopts[i].doc)
            printf("%s", longopts[i].doc);
        printf("\n");
    }
}

void reverse(char * v[], int i, int j) {
    while (j - i > 1) {
        char * tmp = v[i];
        v[i] = v[j - 1];
        v[j - 1] = tmp;
        ++i;
        --j;
    }
}

/* Check if state is at end of argv */
int is_argv_end(const struct parg_state * ps, int argc, char * const argv[]) {
    return ps->optind >= argc || argv[ps->optind] == NULL;
}


/* Match string at nextchar against longopts. */
int match_long(struct parg_state * ps, int argc, char * const argv[], const char * optstring,
               const struct parg_opt * longopts, int * longindex) {
    int i, match = -1, num_match = 0;
    size_t len = strcspn(ps->nextchar, "=");

    for (i = 0; longopts[i].name; ++i) {
        if (strncmp(ps->nextchar, longopts[i].name, len) == 0) {
            match = i;
            num_match++;
            /* Take if exact match */
            if (longopts[i].name[len] == '\0') {
                num_match = 1;
                break;
            }
        }
    }

    /* Return '?' on no or ambiguous match */
    if (num_match != 1) {
        ps->optopt = 0;
        ps->nextchar = NULL;
        return '?';
    }

    assert(match != -1);

    if (longindex) {
        *longindex = match;
    }

    if (ps->nextchar[len] == '=') {
        /* Option argument present, check if extraneous */
        if (longopts[match].has_arg == PARG_NOARG) {
            ps->optopt = longopts[match].flag ? 0 : longopts[match].val;
            ps->nextchar = NULL;
            return optstring[0] == ':' ? ':' : '?';
        } else {
            ps->optarg = &ps->nextchar[len + 1];
        }
    } else if (longopts[match].has_arg == PARG_REQARG) {
        /* Option argument required, so return next argv element */
        if (is_argv_end(ps, argc, argv)) {
            ps->optopt = longopts[match].flag ? 0 : longopts[match].val;
            ps->nextchar = NULL;
            return optstring[0] == ':' ? ':' : '?';
        }

        ps->optarg = argv[ps->optind++];
    }

    ps->nextchar = NULL;

    if (longopts[match].flag != NULL) {
        *longopts[match].flag = longopts[match].val;
        return 0;
    }

    return longopts[match].val;
}


/* Match nextchar against optstring */
int match_short(struct parg_state * ps, int argc, char * const argv[], const char * optstring) {
    const char * p = strchr(optstring, *ps->nextchar);

    if (p == NULL) {
        ps->optopt = *ps->nextchar++;
        return '?';
    }

    /* If no option argument, return option */
    if (p[1] != ':') {
        return *ps->nextchar++;
    }

    /* If more characters, return as option argument */
    if (ps->nextchar[1] != '\0') {
        ps->optarg = &ps->nextchar[1];
        ps->nextchar = NULL;
        return *p;
    }

    /* If option argument is optional, return option */
    if (p[2] == ':') {
        return *ps->nextchar++;
    }

    /* Option argument required, so return next argv element */
    if (is_argv_end(ps, argc, argv)) {
        ps->optopt = *ps->nextchar++;
        return optstring[0] == ':' ? ':' : '?';
    }

    ps->optarg = argv[ps->optind++];
    ps->nextchar = NULL;
    return *p;
}

/* Parse next long or short option in `argv`.
 * Check GNU getopt_long example for details:
 * https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
 */
int parg_getopt_long(struct parg_state * ps, int argc, char * const argv[], const char * optstring,
                     const struct parg_opt * longopts, int * longindex) {
    assert(ps != NULL);
    assert(argv != NULL);
    assert(optstring != NULL);

    ps->optarg = NULL;

    if (argc < 2) {
        return -1;
    }

    /* Advance to next element if needed */
    if (ps->nextchar == NULL || *ps->nextchar == '\0') {
        if (is_argv_end(ps, argc, argv)) {
            return -1;
        }

        ps->nextchar = argv[ps->optind++];

        /* Check for argument element (including '-') */
        if (ps->nextchar[0] != '-' || ps->nextchar[1] == '\0') {
            ps->optarg = ps->nextchar;
            ps->nextchar = NULL;
            return 1;
        }

        /* Check for '--' */
        if (ps->nextchar[1] == '-') {
            if (ps->nextchar[2] == '\0') {
                ps->nextchar = NULL;
                return -1;
            }

            if (longopts != NULL) {
                ps->nextchar += 2;

                return match_long(ps, argc, argv, optstring,
                                  longopts, longindex);
            }
        }

        ps->nextchar++;
    }

    /* Match nextchar */
    return match_short(ps, argc, argv, optstring);
}

/*
 * Reorder elements of `argv` with no special cases.
 *
 * This function assumes there is no `--` element, and the last element
 * is not an option missing a required argument.
 *
 * The algorithm is described here:
 * http://hardtoc.com/2016/11/07/reordering-arguments.html
 */
int parg_reorder_simple(int argc, char * argv[], const char * optstring,
                        const struct parg_opt * longopts) {
    struct parg_state ps;
    int change, l = 0, m = 0, r = 0;

    if (argc < 2) {
        return argc;
    }

    do {
        int c, nextind;
        ps = parg_state_default;
        nextind = ps.optind;

        /* Parse until end of argument */
        do {
            c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
        } while (ps.nextchar != NULL && *ps.nextchar != '\0');

        change = 0;

        do {
            /* Find next non-option */
            for (l = nextind; c != 1 && c != -1;) {
                l = ps.optind;

                do {
                    c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
                } while (ps.nextchar != NULL && *ps.nextchar != '\0');
            }

            /* Find next option */
            for (m = l; c == 1;) {
                m = ps.optind;

                do {
                    c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
                } while (ps.nextchar != NULL && *ps.nextchar != '\0');
            }

            /* Find next non-option */
            for (r = m; c != 1 && c != -1;) {
                r = ps.optind;

                do {
                    c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
                } while (ps.nextchar != NULL && *ps.nextchar != '\0');
            }

            /* Find next option */
            for (nextind = r; c == 1;) {
                nextind = ps.optind;

                do {
                    c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
                } while (ps.nextchar != NULL && *ps.nextchar != '\0');
            }

            if (m < r) {
                change = 1;
                reverse(argv, l, m);
                reverse(argv, m, r);
                reverse(argv, l, r);
            }
        } while (c != -1);
    } while (change != 0);

    return l + (r - m);
}

/**
 * Parse next short option in `argv`.
 * Check GNU getopt example for details:
 * https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
 */
int parg_getopt(struct parg_state * ps, int argc, char * const argv[], const char * optstring) {
    return parg_getopt_long(ps, argc, argv, optstring, NULL, NULL);
}

/**
 * Reorder elements of `argv` so options appear first.
 *
 * If there are no long options, `longopts` may be `NULL`.
 *
 * The return value can be used as `argc` parameter for `parg_getopt()` and
 * `parg_getopt_long()`.
 *
 * @param argc number of elements in `argv`
 * @param argv array of pointers to command-line arguments
 * @param optstring string containing option characters
 * @param longopts array of `parg_option` structures
 * @return index of first argument in `argv` on success, `-1` on error
 */
int parg_reorder(int argc, char * argv[], const char * optstring,
                 const struct parg_opt * longopts) {
    struct parg_state ps;
    int lastind, optend, c;

    assert(argv != NULL);
    assert(optstring != NULL);

    if (argc < 2) {
        return argc;
    }

    ps = parg_state_default;

    /* Find end of normal arguments */
    do {
        lastind = ps.optind;

        c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);

        /* Check for trailing option with error */
        if ((c == '?' || c == ':') && is_argv_end(&ps, argc, argv)) {
            lastind = ps.optind - 1;
            break;
        }
    } while (c != -1);

    optend = parg_reorder_simple(lastind, argv, optstring, longopts);

    /* Rotate `--` or trailing option with error into position */
    if (lastind < argc) {
        reverse(argv, optend, lastind);
        reverse(argv, optend, lastind + 1);
        ++optend;
    }

    return optend;
}

int parg_zgetopt_long(struct parg_state * ps, int argc, char * const argv[], const char * optstring,
                      const struct parg_opt * longopts, int * longindex) {
    assert(ps != NULL);
    assert(argv != NULL);
    assert(optstring != NULL);

    ps->optarg = NULL;

    if (argc < 2) {
        return -1;
    }

    /* Advance to next element if needed */
    if (ps->nextchar == NULL || *ps->nextchar == '\0') {
        if (is_argv_end(ps, argc, argv)) {
            return -1;
        }

        ps->nextchar = argv[ps->optind++];

        /* Check for argument element (including '-') */
        if (ps->nextchar[0] != '-' || ps->nextchar[1] == '\0') {
            ps->optarg = ps->nextchar;
            ps->nextchar = NULL;
            return 1;
        }

        /* Check for '--' */
        if (ps->nextchar[1] == '-') {
            if (ps->nextchar[2] == '\0') {
                ps->nextchar = NULL;
                return -1;
            }

            if (longopts != NULL) {
                ps->nextchar += 2;

                return match_long(ps, argc, argv, optstring,
                                  longopts, longindex);
            }
        }
        ps->nextchar++;
    }

    /* Match nextchar */
    return match_short(ps, argc, argv, optstring);
}
