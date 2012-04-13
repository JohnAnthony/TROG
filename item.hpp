#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "geometry.hpp"

class Item {
    public:
    enum Type {
        GENERIC,
        POTION,
        TREASURE_T,
        STAT_TOME,
        LAST_TYPE
    };

    Item(void);
    void SetPosition(int x, int y);
    std::string GetName(void);

    Type type;
    Point pos;
    char symbol;
    Color colour;

    private:
    const char *name;
};

#endif
