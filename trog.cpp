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

    g = new Game();
    g->ShowMap();
    g->Run();
    delete g;
    endwin();

    return 0;
}
