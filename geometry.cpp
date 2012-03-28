#include "geometry.hpp"
#include <ncurses.h>

void
ScreenNoise(void) {
    for (int e = 0; e < LINES; ++e)
        for (int i = 0; i < COLS; ++i) {
            mvaddch(e, i, NOISE_CHARACTER);
        }
}
