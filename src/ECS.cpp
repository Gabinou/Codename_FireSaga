
#include <ECS.hpp>

void Entity::addGroup(Group in_Group) {
    groupbitset[in_Group] = true;
    manager.addtogroup(this, in_Group);
    
}