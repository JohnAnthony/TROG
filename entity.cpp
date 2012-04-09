#include "entity.hpp"

void
Entity::TakeDamage(int amt) {
    this->curHP -= amt;
}

bool
Entity::isAlive(void) {
    return (this->curHP > 0);
}
