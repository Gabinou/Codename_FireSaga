#ifndef CONVOY_HPP
#define CONVOY_HPP

#include "enums.hpp"
#include "structs.hpp"
#include "physfs.h"
#include "tinyxml2.h"
#include "filesystem.hpp"
#include "utilities.hpp"
#include "string.h"
#include <string>

class Convoy {
    private:
        Inventory_item convoy[DEFAULT::CONVOY_SIZE];

    public:
        Convoy();

        void write(const char * filename, const char * mode = "a");
        void writeXML(const char * filename, const bool append = false);
        void readXML(const char * filename);
};

#endif /* CONVOY_HPP */