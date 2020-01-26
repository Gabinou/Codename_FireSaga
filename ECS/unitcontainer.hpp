#ifndef UNITCONTAINER_HPP
#define UNITCONTAINER_HPP

#include "ECS.hpp"

class UnitContainer : public Component {
    private:
        std::string name;
    public:
        UnitContainer() = default;
        UnitContainer(std::string in_name) {
            name = in_name;
        }

        std::string getName() {
            return (name);
        }

        void setName(std::string in_name) {
            name = in_name;
        }
};

#endif /* UNITCONTAINER_HPP */