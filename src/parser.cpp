#include <parser.hpp>

#include <string>
#include <stdio.h>
#include <tuple>

int findinchar(const char * string, const char * search) {
    puts(string);
    for(int n=0;string[n] != '\0';n++) {
        printf("%d\n", n);
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
    buffer[i] = '\0';
    printf("buffer");
    printf("%s\n", buffer);                
    return(single_char);
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