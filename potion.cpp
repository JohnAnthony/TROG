#include "potion.hpp"

Potion::Potion(const char *inName, Potion::Key inKey, Potion::Category inCat,
  int inMin, int inMax) {
    this->name = inName;
    this->key = inKey;
    this->category = inCat;
    this->minPOTENCY = inMin;
    this->maxPOTENCY = inMax;
    this->type = Item::POTION;
}

Potion*
Potion::GetPotion(Potion::Key k) {
    return &PotionList[k];
}
