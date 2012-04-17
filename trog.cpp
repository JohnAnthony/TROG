#include <cstdlib>
#include <ctime>
#include "game.hpp"
#include "character.hpp"
#include "gui.hpp"
#include <signal.h>
#include <unistd.h>

Game *g = NULL;
Character *c;
static int colours;

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

static void
handle_args(int argc, char **argv) {
    int i;

    colours = 16;
    while ((i = getopt (argc, argv, "8")) != -1)
        switch (i) {
            case '8':
                colours = 8;
                break;
            case '?':
                if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                exit(1);
            default:
                exit(1);
        }
    GUI::NUM_COLOURS = colours;
}

int main(int argc, char **argv) {
    bool playagain;

    srand(time(NULL));
    handle_args(argc, argv);
    GUI::Init();

    signal(SIGINT, interrupt_handler);
    signal(SIGWINCH, resize_handler);

    do {
        erase();
        GUI::StartScreen();
        c = GUI::CharacterCreation();
        g = new Game(c);
        playagain = g->Run();
    } while (playagain);

    GUI::End();
    delete g;
    delete c;

    return 0;
}
