#ifndef UNITCONTAINER_HPP
#define UNITCONTAINER_HPP

#include "ECS.hpp"

class UnitContainer : public Component {
    private:
        short unsigned int index;
    public:
        UnitContainer() = default;
        UnitContainer(short unsigned int in_index) {
            index = in_index;
        }

        short unsigned int getIndex() {
            return (index);
        }

        void setIndex(short unsigned int in_index) {
            index = in_index;
        }
};

#endif /* UNITCONTAINER_HPP */