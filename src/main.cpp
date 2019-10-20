#include <parser.hpp>
#include <stdio.h>

using namespace std;



// What should the data base structure be?
// JSON: {"a":"1"}
// bibtex: @unit{Marth, stats={},....}
// I think bibtext style is good cause then the database will be combined weapons and units and other things, if necessary.


int main(){
    puts("TESTING THIS BITCH.");
    // read("..\\units_FE1_bibstyle.txt");
    readcsv("..\\testarr.txt", 2);
    
    return 0;
}
