#include "SDL2/SDL.h" 
#include <stdio.h>

int main( int argc, char* args[] ) {
  SDL_Init( SDL_INIT_EVERYTHING ); //Start SDL
  SDL_Quit(); //Quit SDL
  printf("allo\n");
  return 0;
}