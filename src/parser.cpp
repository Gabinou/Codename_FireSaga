#include <parser.hpp>

short unsigned int count_words(char line[DEFAULT::LINE_LENGTH], const char * counted) {
    short unsigned int out = 0;
    char * pch;
    pch = strchr(line, *counted);

    while (pch != NULL) {
        pch = strchr(pch + 1, *counted);
        out++;
    }

    out++;
    return (out);
}

int findinchar(const char * string, const char * search) {
    for (short unsigned int n = 0; string[n] != '\0'; n++) {
        if (string[n] == * search) {
            return (n);
        }
    }
    return (-1);
}

char until(FILE * fp, const char * until = ",",  const char * interrupt = "@") {
    char single_char;
    char buffer[255];
    int i = 0;

    while ((!feof(fp)) && (single_char != * until)) {
        single_char = fgetc(fp);
        printf("%c\n", single_char);
        buffer[i] = single_char;
        i++;
    }

    buffer[i] = *"\0";
    return (single_char);
}

char * slice_char(const char * in, int start, int end) {
    char * out;

    if (end <= start) {
        end = (int)strcspn(in, "\n");
    }

    out = (char *)malloc((end - start) * sizeof(char *));

    for (int i = start; i < end; i++) {
        out[i - start] = in[i];
    }

    out[end - start] = *"\0";
    return (out);
}

int * parse_line(char * line, const char * until = ",", const int out_size = 255) {
    int * out;
    int start = 0;
    int i = 0;
    char * end;
    char * current;
    end = strchr(line, *until);
    out = (int *)malloc((out_size) * sizeof(int *));

    while (end != NULL) {
        current = slice_char(line, start, (int)(end - line));
        start = end + 1 - line;
        end = strchr(end + 1, *until);
        out[i] = atoi(current);
        i++;
    }

    current = slice_char(line, start, (int)(end));
    out[i] = atoi(current);
    return (out);
}
