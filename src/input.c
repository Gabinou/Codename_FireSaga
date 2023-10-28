
#include "input.h"

struct Input_Arguments Input_Arguments_default = {
    .map_index     = CHAPTER_TEST,
    .startup_mode  = STARTUP_TESTING,
    .cutScene      = 0,
    .talkScene     = 0,
    .save_filename = NULL,
};

static struct parg_opt longopts[] = {
    {"help",    PARG_NOARG,     0, 'h', NULL,   "display this help text and exit"},
    {"version", PARG_NOARG,     0, 'v', NULL,   "display version and exit"},
    {NULL,      PARG_NOARG,     0,  0,  NULL,   "Debug options:"},
    {"state",   PARG_REQARG,    0, 's', "FILE", "load save state   (ignored unless -m specified)"},
    {"map",     PARG_REQARG,    0, 'm', "INT",  "load game to map  (ignored unless -s specified)"}
};

struct Input_Arguments Input_parseInputs(int argc, char *argv[]) {
    struct Input_Arguments out_args = Input_Arguments_default;
    struct parg_state ps = parg_state_default;
    int *longindex;
    int c;
    while ((c = parg_getopt_long(&ps, argc, argv, "vhs:m:", longopts, longindex)) != -1) {
        switch (c) {
            case 1:
                printf("argument '%s' is ignored\n", ps.optarg);
                break;
            case 'h':
                parg_usage(SOTA_EXECUTABLE_STRING, longopts);
                exit(0);
                break;
            case 'm':
                out_args.map_index = atoi(ps.optarg);
                // printf("out_args.map_index %d\n", out_args.map_index);
                break;
            case 's':
                if (out_args.save_filename != NULL)
                    free(out_args.save_filename);
                out_args.save_filename = (char *) SDL_malloc(strlen(ps.optarg));
                strncpy(out_args.save_filename, ps.optarg, sizeof(*ps.optarg));
                // printf("out_args.save_filename %s\n", out_args.save_filename);
                break;
            case 'v':
                printf("SotA version %s\n", SOTA_VER_STRING);
                exit(0);
            case '?':
                if (ps.optopt == 's') {
                    printf("option -s/--state requires an argument\n");
                } else if (ps.optopt == 'm') {
                    printf("option -m/--map requires an argument\n");
                } else {
                    printf("unknown option -%c\n", ps.optopt);
                }
                exit(ERROR_Generic);
                break;
            default:
                printf("error: unhandled option -%c\n", c);
                exit(ERROR_Generic);
                break;
        }
    }

    for (c = ps.optind; c < argc; ++c)
        printf("argument '%s'\n", argv[c]);

    return (out_args);
}
