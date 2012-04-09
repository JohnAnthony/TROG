#include "entity.hpp"

void
Entity::TakeDamage(int amt) {
    this->curHP -= amt;
}
