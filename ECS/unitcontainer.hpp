#ifndef UNITCONTAINER_HPP
#define UNITCONTAINER_HPP

#include "ECS.hpp"

class UnitComponent : public Component {
    private:
        std::string name;
    public:
        UnitComponent() = default;

        std::string getName() {
            return (name)
        }

        void setName(std::string in_name) {
            name = in_name;
        }
};

#endif /* UNITCONTAINER_HPP */