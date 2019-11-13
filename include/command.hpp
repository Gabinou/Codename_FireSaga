#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "components.hpp"

class Command {

    public:
        virtual ~Command() {}
        virtual void execute(Entity & cursor) = 0; // cause in FIreSaga, the player controls the cursor exclusively.
};

class InputHandler {
    public:
        void handleInput();

        // Methods to bind command...

    private:
        Command * buttonUp_;
        Command * buttonDown_;
        Command * buttonLeft_;
        Command * buttonRight_;
};

class MoveCommand : public Command {

};



#endif /* COMMAND_HPP */