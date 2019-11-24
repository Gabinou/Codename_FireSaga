#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include "vector2D.hpp"
#include "shared.hpp"

struct InputMap {
    std::vector<SDL_Scancode> moveright{SDL_SCANCODE_RIGHT, SDL_SCANCODE_D, SDL_SCANCODE_L};
    std::vector<SDL_Scancode> moveup{SDL_SCANCODE_W, SDL_SCANCODE_UP, SDL_SCANCODE_I};
    std::vector<SDL_Scancode> movedown{SDL_SCANCODE_S, SDL_SCANCODE_DOWN, SDL_SCANCODE_K};
    std::vector<SDL_Scancode> moveleft{SDL_SCANCODE_A, SDL_SCANCODE_LEFT, SDL_SCANCODE_J};
    std::vector<SDL_Scancode> accept{SDL_SCANCODE_SPACE};
    std::vector<SDL_Scancode> cancel{SDL_SCANCODE_BACKSPACE};
    std::vector<SDL_Scancode> stats{};
    std::vector<SDL_Scancode> menu_right{}; // Switches between units of same affilition, when in 'map' states.
    std::vector<SDL_Scancode> menu_left{}; // Switches between units of same affilition, when in 'map' states.
    std::vector<SDL_Scancode> minimap{}; // Same controls as when in map state.
    std::vector<SDL_Scancode> faster{}; // Toggle or Hold?
    std::vector<SDL_Scancode> pause{SDL_SCANCODE_RETURN};
};


class Game {

    public:
        Game();
        ~Game();

        void init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen);

        void handleEvents();
        void update();
        void render();
        void clean();

        void setFontsize(int in_fontsize);

        int getFontsize();

        InputMap getInputMap();

        void setState(std::string in_state);
        void setState(const char * in_state);

        std::string getState();

        bool running();

        static SDL_Renderer * renderer;
        static SDL_Event event;
        static TTF_Font * font;

    private:

        InputMap inputMap;
        int fontsize;
        bool isRunning;
        int count;
        SDL_Window * window;
        std::string state;
        /* Possible states (make all letters lowercase when checking):
        *       worldmap: Story exposition, small animations of units moving around,
        *           maybe even player control sometimes?
        *       map: Cursor traveling on the map. Main button configuration. Units fight.
        *       unitmove: Unit found on map after checking accept.
        *           Draws the unit move overlay, draws arrows on map, pathfinds,
        *           Goes to unit menu if accept or back to map if cancel.
        *       unitmenu: After moving unit, decide what to do.
        *       options: Clicking accept on map. Then  goes into options submenues.
        *       pause: Clicking pause. Reduces sounds. Makes the screen black.
        *       minimap: Makes minimap appear on screen. Map control still enabled.
        *           Goes to map if accept or cancel, or to other menus.
        *       stats: Shows unit statistics screen. changes some buttons around.
        *       introvideo: When start menu is idle too long.
        *       startmenu: s/t. Submenus:
        *           saves: where you can delete or load files, start new games, etc.
        *           soundroom: play songs from the game.
        *           options: s/t. Should also be accessible from the map.
        *       conversation: s/t
        *       battle: s/t
        *       cutscene: for scripted events/battles.
        *       gameover: s/t
        *       preparation: s/t. Submenus:
        *           placement: goes to the map and places units.
        *           chooseunits: s/t
        *           equipment: s/t
        *           forge: s/t
        *           storage: s/t
        *           bonusexp: s/t
        *       shop: s/t
        */
};



#endif /* GAME_HPP */