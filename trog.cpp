#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include "game.hpp"
#include "character.hpp"
#include "geometry.hpp"
#include "gui.hpp"
#include <signal.h>

Game *g = NULL;
Character *c;

static void
resize_handler(int sign) {
    if (g)
        g->HandleResize(sign);
    else
        refresh();
}

static void
interrupt_handler(int sign) {
    erase();
    endwin();
    puts("SIGINT received; exiting cleanly.");
    delete g;
    delete c;
    exit(0);
}

int main(int argc, char** argv) {
    bool playagain;

    srand(time(NULL));
    initscr();
	cbreak();
	keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();

    signal(SIGINT, interrupt_handler);
    signal(SIGWINCH, resize_handler);

    for (int i = 0; i < 8; ++i) 
        init_pair(i, i, COLOR_BLACK);

    do {
        erase();
        GUI::StartScreen();
        c = GUI::CharacterCreation();
        g = new Game(c);
        playagain = g->Run();
        delete g;
        delete c;
    } while (playagain);

    endwin();

    return 0;
}
