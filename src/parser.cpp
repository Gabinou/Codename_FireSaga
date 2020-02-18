#include <parser.hpp>

int findinchar(const char * string, const char * search) {
    for (int n = 0; string[n] != '\0'; n++) {
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

std::vector<int> parse_line_vec(char * line, const char * until = ",", const int out_size = 255) {
    std::vector<int> out;
    int start = 0;
    char * end;
    char * current;
    end = strchr(line, *until);

    while (end != NULL) {
        current = slice_char(line, start, (int)(end - line));
        start = end + 1 - line;
        end = strchr(end + 1, *until);
        out.push_back(atoi(current));
    }

    current = slice_char(line, start, (int)(end));
    out.push_back(atoi(current));
    return (out);
}

int count(char * line, const char * counted = ",") {
    int out = 0;
    char * pch;
    pch = strchr(line, *counted);

    while (pch != NULL) {
        pch = strchr(pch + 1, *counted);
        out++;
    }

    out++;
    return (out);
}

// How to use read_csv_vec for different datatypes?
std::vector<std::vector<int>> readcsv_vec(const char * filename, const int header, const char * delim, const int out_size) {
    FILE * fp;
    char line_c[255];
    long int current_line = 0;
    static int line_length;
    std::vector<int> col;
    std::vector<std::vector<int>> matrix;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Error opening file");
    }

    while (current_line < header) {
        fgets(line_c, sizeof(line_c), fp);
        current_line++;
    }

    while (!feof(fp)) {
        fgets(line_c, sizeof(line_c), fp);
        line_length = count(line_c);
        col = parse_line_vec(line_c);
        matrix.push_back(col);
    }

    return (matrix);
}