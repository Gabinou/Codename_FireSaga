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


int nextcharinfile() {
    return(-1);
}



void read(const char *filename) {
    
    FILE *fp;
    char line_c[100];
    const char* elem = "@";
    const char* open = "{";
    const char* close = "}";
    unsigned int curLine = 0;
    fpos_t pos;
    std::string line_str;
    fp = fopen(filename, "r");
    if (fp == NULL) perror ("Error opening file");
    else {
        if (fgets(line_c, 100, fp) != NULL ) {
            puts(line_c);
            printf("%d \n", findinchar(line_c, elem));
            fgetpos(fp,&pos);
            printf("%d \n", pos);
            if(findinchar(line_c, elem) != -1) {
                
            }
            
            std::string line_str(line_c);
        }
    }
}