#include <parser.hpp>

#include <string>
#include <stdio.h>

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
int between(FILE * fp, const char *open, const char *close, const char *interrupt = "@") {
    char single_char;
    int open_num = 0;
    int close_num = 0;
    current_pos = ftell(fp);
    fpos_t fopen_pos;
    fpos_t fclose_pos;
    while((!feof(fp)) && (single_char != * interrupt) && ((open_num!=close_num)) || (open_num == 0)) {
        // printf("%d,%d\n", open_num, close_num);
        single_char = fgetc(fp);
        if (single_char == * open) {
            if (open_num == 0) {
                open_pos = ftell(fp);
                fgetpos(fp, &fopen_pos);
                }
            open_num++;
        }
        if (single_char == * close) {close_num++;}
        printf("%c", single_char);
    }
    close_pos = ftell(fp);
    fgetpos(fp, &fclose_pos);
    return(fopen_pos, fclose_pos);
}

char fgetsuntil(FILE * fp, const char *open, const char *close, const char *interrupt = "@", const char * filelength = "short"){
    
}


void read(const char *filename) {
    
    FILE *fp;
    char line_c[100];
    char single_char;
    const char* elem = "@";
    const char* open = "{";
    const char* close = "}";
    unsigned int curLine = 0;

    fpos_t pos; // for arbitrarily long files
    std::string line_str;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
    } else {
        while(!feof(fp)) {
            single_char = fgetc(fp);
            if (single_char == * elem) {
                printf("%c\n", single_char);
                between(fp, open, close);
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