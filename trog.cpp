#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include "game.hpp"

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

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    g = new Game();
    g->Run();
    delete g;
    endwin();

    return 0;
}
