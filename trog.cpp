#include <ncurses.h>
#include "game.hpp"

int main(int argc, char** argv) {
    Game *g;

    initscr();
	cbreak();
	keypad(stdscr, TRUE);
    noecho();
    raw();
    curs_set(0);

    g = new Game();
    g->ShowMap();
    g->Run();

    endwin();

    return 0;
}
