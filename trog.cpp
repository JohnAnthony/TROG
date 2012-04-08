#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include "game.hpp"
#include "geometry.hpp"
#include <signal.h>

Game *g = NULL;

static void
resize_handler(int sign) {
    refresh();
    if (g)
        g->DoRedraw();
}

static void
interrupt_handler(int sign) {
    g->QuitDialogue();
    if (!g->running) {
        endwin();
        exit(0);
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));
    initscr();
	cbreak();
	keypad(stdscr, TRUE);
    noecho();
    // raw();
    curs_set(0);
    start_color();

    for (int i = 0; i < 8; ++i) 
        init_pair(i, i, COLOR_BLACK);

    signal(SIGWINCH, resize_handler);
    signal(SIGINT, interrupt_handler);

    g = new Game();
    g->Run();
    delete g;
    endwin();

    return 0;
}
