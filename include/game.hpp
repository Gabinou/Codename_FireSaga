#ifndef GAME_HPP
#define GAME_HPP

#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "shared.hpp"
#include "ECS.hpp"
#include "map.hpp"

struct KeyboardInputMap {
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

struct GamepadInputMap {
    std::vector<SDL_GameControllerAxis> mainxaxis{SDL_CONTROLLER_AXIS_LEFTX};
    std::vector<SDL_GameControllerAxis> mainyaxis{SDL_CONTROLLER_AXIS_LEFTY};
    std::vector<SDL_GameControllerAxis> secondxaxis{SDL_CONTROLLER_AXIS_RIGHTX};
    std::vector<SDL_GameControllerAxis> secondyaxis{SDL_CONTROLLER_AXIS_RIGHTY};
    std::vector<SDL_GameControllerAxis> triggerleft{SDL_CONTROLLER_AXIS_TRIGGERLEFT};
    std::vector<SDL_GameControllerAxis> triggerright{SDL_CONTROLLER_AXIS_TRIGGERRIGHT};

    std::vector<SDL_GameControllerButton> moveright{SDL_CONTROLLER_BUTTON_DPAD_RIGHT};
    std::vector<SDL_GameControllerButton> moveup{SDL_CONTROLLER_BUTTON_DPAD_UP};
    std::vector<SDL_GameControllerButton> movedown{SDL_CONTROLLER_BUTTON_DPAD_DOWN};
    std::vector<SDL_GameControllerButton> moveleft{SDL_CONTROLLER_BUTTON_DPAD_LEFT};

    std::vector<SDL_GameControllerButton> accept{SDL_CONTROLLER_BUTTON_A};
    std::vector<SDL_GameControllerButton> cancel{SDL_CONTROLLER_BUTTON_B};
    std::vector<SDL_GameControllerButton> stats{SDL_CONTROLLER_BUTTON_X};
    std::vector<SDL_GameControllerButton> menu_right{SDL_CONTROLLER_BUTTON_RIGHTSHOULDER};
    std::vector<SDL_GameControllerButton> menu_left{SDL_CONTROLLER_BUTTON_LEFTSHOULDER};
    std::vector<SDL_GameControllerButton> minimap{SDL_CONTROLLER_BUTTON_Y};
    std::vector<SDL_GameControllerButton> faster{SDL_CONTROLLER_BUTTON_START};
    std::vector<SDL_GameControllerButton> pause{SDL_CONTROLLER_BUTTON_START};


};

class Game {

    private:
        void moveUnit(Entity & cursor);
        void destroyUnitmenu();
        void createUnitmenu(Entity & setting_entity);
        KeyboardInputMap keyboardInputMap;
        GamepadInputMap gamepadInputMap;
        // Manager & manager;
        int fontsize;
        bool isRunning;
        int count;
        int tilesize[2];
        SDL_Window * window;
        std::string state;
        // I think these should be used as stacks: the last created entity should be the first destroyed:
        stack <int> menu_entities;
        stack <int> unit_entities;

        /* Possible states (make all letters lowercase when checking):
        *       worldmap: Story exposition, small animations of units moving around,
        *           maybe even player control sometimes?
        *       map: Cursor traveling on the map. Main button configuration. Units fight.
        *       unitmove: Unit found on map after checking accept.
        *           Draws the unit move overlay, draws arrows on map, pathfinds,
        *           Goes to unit menu if accept or back to map if cancel.
        *       unitmenu: After moving unit, decide what to do.
        *       items: s/t
        *       attack: s/t
        *       trade: s/t
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
        *     State hierarchy:
        *       map
                    unitmove
                    unitmenu
                options
                preparation:
                    chooseunits
                    equipment
                    forge
                    storage
                    bonusexp
        */


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

        KeyboardInputMap getKeyboardInputMap();
        GamepadInputMap getGamepadInputMap();

        void setState(Entity & in_entity, std::string new_state);
        void setState(const char * new_state);

        std::string getState();

        bool running();

        static SDL_Renderer * renderer;
        static SDL_Event event;
        static TTF_Font * font;


};



#endif /* GAME_HPP */