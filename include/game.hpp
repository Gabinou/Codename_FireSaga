#ifndef GAME_HPP
#define GAME_HPP

#include "shared.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "ECS.hpp"
#include "map.hpp"
#include "weapon.hpp"
#include "tile.hpp"
#include "linalg.hpp"
#include "script.hpp"

class Game {

    private:
        void moveUnit(Entity & cursor);
        void destroyUnitmenu();
        void createUnitmenu(Entity & setting_entity);
        KeyboardInputMap keyboardInputMap;
        GamepadInputMap gamepadInputMap;
        int fontsize;
        bool isRunning;
        int count;
        int tilesize[2];
        SDL_Window * window;
        std::string state;
        std::stack <int> menu_entities;
        std::stack <int> unit_entities;

    public:
        static SDL_Renderer * renderer;
        static SDL_Event event;
        static TTF_Font * font;
        static Manager manager;
        static Settings settings;

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

        void baseUnitComponents();
};

#endif /* GAME_HPP */