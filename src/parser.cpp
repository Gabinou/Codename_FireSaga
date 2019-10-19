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
int between(FILE * fp, const char *open, const char *close, const char *interrupt = "@", const char * filelength = "short") {
    
    char single;
    int open_num = 0;
    int open_pos;
    int close_num = 0;
    int close_pos;
    if (filelength == "long") {
        fpos_t pos;
    } else {
        long int pos;
    }
    
    while((!feof(fp)) && (single != * interrupt) && ((open_num!=close_num)) || (open_num == 0)) {
        single = fgetc(fp);
        if (single == * open) {
            if (open_num == 0) {open_pos = ftell(fp);}
            open_num++;
        }
        if (single == * close) {close_num++;}
        printf("%c", single);
    }
    // single = fseek(fp, ftell(fp), SEEK_SET);
    close_pos = ftell(fp);
    rewind(fp);
    printf("\n%d\n", open_pos);
    printf("%d\n", close_pos);
    fseek(fp, open_pos-1, 0);
    single  = fgetc(fp);
    printf("%c\n", single);    
    fseek(fp, close_pos-1, 0);
    single = fgetc(fp);
    printf("%c\n", single);
    printf("exit\n");
    return(-1);
}

void read(const char *filename) {
    
    FILE *fp;
    char line_c[100];
    char single;
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
            single = fgetc(fp);
            if (single == * elem) {
                printf("%c\n", single);
                between(fp, open, close);
                break;
                // printf("%d \n", findinchar(line_c, elem));
                // fgetpos(fp,&pos);
                // printf("%d \n", pos);
                // if(findinchar(line_c, elem) != -1) {
                   
            } else {
                // puts(single);
            }
                
            std::string line_str(line_c);
            
        }
    }
}