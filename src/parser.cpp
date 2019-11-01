#include <parser.hpp>
#include <string>
#include <stdio.h>
#include <tuple>
#include <vector>

int findinchar(const char * string, const char * search) {
    for(int n=0;string[n] != '\0';n++) {
        if(string[n] == * search) {
            return(n);
        }
    }
    return(-1);
}

/// \var int between
/// \brief Find start and end position between first 'open' and last 'close'.
/// Intended to find start and end position between a bunch of nested bracket pairs. 
/// I.E. Should find these ->{ foo{ bar}}<-. Or these: ->{ }<- foo{ bar}}. 
std::tuple<fpos_t, fpos_t> between(FILE * fp, const char *open, const char *close, const char *interrupt = "@") {
    char single_char;
    int open_num = 0;
    int close_num = 0;
    fpos_t fopen_pos;
    fpos_t fclose_pos;
    while((!feof(fp)) && (single_char != * interrupt) && ((open_num!=close_num)) || (open_num == 0)) {
        single_char = fgetc(fp);
        if (single_char == * open) {
            if (open_num == 0) {
                fgetpos(fp, &fopen_pos);
                }
            open_num++;
        }
        if (single_char == * close) {close_num++;}
        // printf("%c", single_char);
    }
    fgetpos(fp, &fclose_pos);
    
    rewind(fp);
    char line_c[100];
    fsetpos(fp, &fopen_pos);
    fgets(line_c, sizeof(line_c), fp);
    printf("HERE THOUGH2\n");
    printf("%s\n", line_c);
    return std::make_tuple(fopen_pos, fclose_pos);
}
/// \var int until
/// \brief Find position in file of next 'until' character.
char until(FILE * fp, const char * until = ",",  const char * interrupt = "@"){
    char single_char;
    char buffer[255];
    int i = 0;
    while((!feof(fp)) && (single_char != * until)){
        single_char = fgetc(fp);
        printf("%c\n", single_char);
        buffer[i] = single_char;
        i++;
    }
    buffer[i] = *"\0";
    // printf("buffer");
    // printf("%s\n", buffer);                
    return(single_char);
}

char * slice_char(const char * in, int start, int end){
    char* out;
    if (end <= start){ 
        end = (int)strcspn(in, "\n");
    }
    out = (char*)malloc((end-start)*sizeof(char*));
    for (int i = start; i<end; i++) {
        out[i-start] = in[i];
    }
    out[end-start] = *"\0";
    return(out);
}

int * parse_line(char * line, const char * until = ",", const int out_size=255){
    int* out;
    int start = 0;
    int i = 0;
    char* end;
    char* current;
    end = strchr(line, *until);
    out = (int*)malloc((out_size)*sizeof(int*));
    while (end!=NULL)
    {
        current = slice_char(line, start, (int)(end-line));
        start = end + 1 - line;
        end = strchr(end+1, *until);
        out[i] = atoi(current);
        i++;
    }
    current = slice_char(line, start, (int)(end));
    out[i] = atoi(current);
    return(out);
}

std::vector<int> parse_line_vec(char * line, const char * until = ",", const int out_size=255){
    std::vector<int> out;
    int start = 0;
    char* end;
    char* current;
    end = strchr(line, *until);
    while (end!=NULL)
    {
        current = slice_char(line, start, (int)(end-line));
        start = end + 1 - line;
        end = strchr(end+1, *until);
        out.push_back(atoi(current));
    }
    current = slice_char(line, start, (int)(end));
    out.push_back(atoi(current));
    return(out);
}


int count(char * line, const char * counted = ","){
    int out = 0;
    char* pch;
    pch = strchr(line, *counted);
    while (pch!=NULL)
    {
        pch = strchr(pch+1, *counted);
        out++;
    }
    out++;
    return(out);
}


// Proxy (https://stackoverflow.com/questions/2717513/c-choose-function-by-return-type) 
// int x=f();
// double x=f(); // different behaviour from above
// by making f() return a proxy with an overloaded cast operator.

// struct Proxy
// {
  // operator double() const { return 1.1; }
  // operator int() const { return 2; }
// };

// Proxy f()
// {
  // return Proxy();
// }

void print2Dvec(std::vector<std::vector<int>> in_mat){
    for (int row = 0; row<in_mat.size(); row++) {
        for (int col = 0; col<in_mat[row].size(); col++) {
            printf("%d,",in_mat[row][col]);
            // printf("%d,%d",row, col);
        }
        printf("\n");
    }
}

// How to determine datatype from string? Simple criteria: if there are '.' on the first line.
void readcsv(const char *filename, const int header, const char * delim, const int out_size){
    FILE *fp;
    char line_c[255];
    char * pch;
    long int current_line = 0;
    int* readline;
    std::vector<int> col;
    std::vector<std::vector<int>> matrix;
    // std::vector<std::vector<int>> matrix;
    readline = (int*)malloc((out_size)*sizeof(int*));
    std::vector<int> rowi;
    std::vector<float> rowf;
    fp = fopen(filename, "r");
    std::string::size_type sz;
    if (fp == NULL) {
        perror("Error opening file");
    }
    while (current_line < header) {
        fgets(line_c, sizeof(line_c), fp);
        printf(line_c);
        current_line++;
    }
    while(!feof(fp)) {
        fgets(line_c, sizeof(line_c), fp);
        printf("%s \n", line_c);
        // const char * until = ",";
        // strchr(line_c, *until);
        int line_length = count(line_c);
        printf("Line length %d \n", line_length);
        readline = parse_line(line_c);
        col = parse_line_vec(line_c);
        matrix.push_back(col);
        // for (int i = 0; i<line_length; i++) {
            // printf("%d \n", readline[i]);
        // }
        
        // matrix.push_back(col);
        printf("a \n");
        // printf(line_c);
        
        // current_line++;
    }
    print2Dvec(matrix);
    getchar();
            // fgets(line_c, sizeof(line_c), fp);
        // // printf("%s\n", line_c);
        
        // if ((findinchar(line_c, ".") >= -1) && (current_line==header)){
            // std::vector<std::vector<int>> out;
        // } else {
            // std::vector<std::vector<float>> out;
        // }

        // // printf("%d\n", findinchar(line_c, "."));
        // pch = strtok (line_c, delim);
        // while (pch != NULL) {
            // printf ("%s\n", pch);
            // printf ("a");
            // pch = strtok (NULL, delim);
            // if (findinchar(line_c, ".") >= -1) {
                // rowi.push_back(atoi(pch));
            // } else {
                // rowf.push_back(atof(pch));
            // }
        // }
        // if (findinchar(line_c, ".") >= -1){
            // out.push_back(rowi);
        // } else {
            // out.push_back(rowf);
        // }
        // rowi.clear();
        // rowf.clear();
        // break;
    // }
    // return(out);
}

void read(const char *filename) {
    FILE *fp;
    char line_c[100];
    char single_char;
    const char* elem = "@";
    const char* open = "{";
    const char* close = "}";
    unsigned int curLine = 0;
    fpos_t fopen_pos;
    fpos_t fclose_pos;
    
    fpos_t pos; // for arbitrarily long files
    std::string line_str;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
    } else {
        while(!feof(fp)) {
            single_char = fgetc(fp);
            if (single_char == * elem) {
                // printf("%c\n", single_char);
                // printf("until\n");
                rewind(fp);
                std::tie(fopen_pos, fclose_pos) = between(fp, open, close);
                rewind(fp);
                fsetpos(fp, &fopen_pos);
                fgets(line_c, sizeof(line_c), fp);
                // fgets(line_c, sizeof(line_c), fp);
                // fgets(line_c, sizeof(line_c), fp);
                // fgets(line_c, sizeof(line_c), fp);
                // fgets(line_c, sizeof(line_c), fp);
                // fgets(line_c, sizeof(line_c), fp);
                // fgets(line_c, sizeof(line_c), fp);<
                // fgets(line_c, sizeof(line_c), fp);
                printf("HERE THOUGH\n");
                printf("%s\n", line_c);

                // until(fp, ",");
                break;
                // printf("%d \n", findinchar(line_c, elem));
                // fgetpos(fp,&pos);
                // printf("%d \n", pos);
                // if(findinchar(line_c, elem) != -1) {
                   
            } else {
                // puts(single_char);
            }
                
            std::string line_str(line_c);
            
        }
    }
}