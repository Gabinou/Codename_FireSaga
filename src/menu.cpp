
#include "menu.hpp"

entityx::ComponentHandle<Unit> getUnit() {
	return(unit);
}

void Clickable::setUnit(entityx::ComponentHandle<Unit> in_unit) {
	unit = in_unit;
}

void Clickable::setEvent(void * event);
