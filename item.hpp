#ifndef ITEM_H
#define ITEM_H

class Item {
    public:
    enum Type {
        GENERIC,
        POTION,
        LAST_TYPE
    };

    Item(void);

    const char *name;
    Type type;
};

#endif
