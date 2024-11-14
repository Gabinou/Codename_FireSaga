
#include "input.h"

static struct parg_opt longopts[] = {
    {"help",    PARG_NOARG,     NULL, 'h', NULL,   "display this help text and exit"},
    {"version", PARG_NOARG,     NULL, 'v', NULL,   "display version and exit\n Debug options:"},
    {"scene",   PARG_REQARG,    NULL, 'S', "INT",  "play scene with input index"},
    {"state",   PARG_REQARG,    NULL, 's', "FILE", "load save state   (ignored unless -m specified)"},
    {"map",     PARG_REQARG,    NULL, 'm', "INT",  "load game to map  (ignored unless -s specified)"}
};

struct Input_Arguments Input_parseInputs(int argc, char *argv[]) {
    struct Input_Arguments  out_args    = Input_Arguments_default;
    struct parg_state       ps          = parg_state_default;
    int longindex[1] = {-1};
    int c;
    while ((c = parg_getopt_long(&ps, argc, argv, "hvS:s:m:", longopts, longindex)) != -1) {
        // while ((c = parg_getopt_long(&ps, argc, argv, NULL, longopts, longindex)) != -1) {
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
            case 'S':
                // Play scene with input index
                out_args.scene = atoi(ps.optarg);
                printf("out_args.scene %d\n", out_args.scene);
                getchar();
                break;
            case 's':
                if (out_args.save_filename != NULL)
                    SDL_free(out_args.save_filename);
                out_args.save_filename = (char *) SDL_malloc(strlen(ps.optarg));
                memcpy(out_args.save_filename, ps.optarg, sizeof(*ps.optarg));
                // printf("out_args.save_filename %s\n", out_args.save_filename);
                break;
            case 'v':
                printf("SotA version %s\n", SOTA_VER_STRING);
                exit(0);
            case '?':
                if (longindex[0] >= 0) {
                    printf("option -%c/--%s requires an argument\n", longopts[longindex[0]].val,
                           longopts[longindex[0]].name);
                } else if (ps.optopt == 's') {
                    printf("option -s/--state requires an argument\n");
                } else if (ps.optopt == 'S') {
                    printf("option -S/--scene requires an argument\n");
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

    return (out_args);
}
