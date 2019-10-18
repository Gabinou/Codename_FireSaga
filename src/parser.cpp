#include <parser.hpp>

#include <string>
#include <stdio.h>

int findinchar(const char * string, const char * search) {
    for(int n=0;n<sizeof(string)/sizeof(string[0]);n++) {
        if(string[n] == * search) {
            return(n);
        }
    }
    return(-1);
}


void read(const char *filename) {
    
    FILE *fp;
    char line_c[100];
    const char* elem = "@";
    const char* open = "{";
    const char* close = "}";
    unsigned int curLine = 0;
    std::string line_str;
    fp = fopen(filename, "r");
    if (fp == NULL) perror ("Error opening file");
    else {
        // while
        if (fgets(line_c, 100, fp) != NULL ) {
            puts(line_c);
            printf("%d \n", findinchar(line_c, elem));
            if(findinchar(line_c, elem) != -1) {
                
            }
            
            std::string line_str(line_c);
        }
    }
}