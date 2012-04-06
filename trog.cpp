#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include "game.hpp"
#include "geometry.hpp"

int main(int argc, char** argv) {
    Game *g;

    srand(time(NULL));
    initscr();
	cbreak();
	keypad(stdscr, TRUE);
    noecho();
    raw();
    curs_set(0);
    start_color();

    for (int i = 0; i < 8; ++i) 
        init_pair(i, i, COLOR_BLACK);

    g = new Game();
    g->Run();
    delete g;
    endwin();

    return 0;
}
