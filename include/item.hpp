#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "geometry.hpp"

/*
=================================================
TROG
Item qualities and various methods
=================================================
*/
class Item {
    public:
    enum Type {
        GENERIC,
        POTION,
        TREASURE_T,
        STAT_TOME,
        EQUIPPABLE,
        LAST_TYPE
    };

    Item(void);
    ~Item(void);
    void SetPosition(int x, int y);
    std::string GetName(void);

    Type type;
    Point pos;
    char symbol;
    Colour colour;

    private:
    const char *name;
};

#endif
