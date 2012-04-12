#ifndef ITEM_H
#define ITEM_H

#include "geometry.hpp"

class Item {
    public:
    enum Type {
        GENERIC,
        POTION,
        TREASURE_T,
        LAST_TYPE
    };

    Item(void);
    void SetPosition(int x, int y);

    const char *name;
    Type type;
    Point pos;
    char symbol;
    Color colour;
};

#endif
