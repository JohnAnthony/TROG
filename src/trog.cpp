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
        g->need_hard_redraw = true;
    else
        refresh();
}

static void
interrupt_handler(int sign) {
    erase();
    endwin();
    g->running = false;
    GUI::Alert("SIGINT received. The game will quit cleanly ASAP.");
    exit(0);
}

static void
handle_args(int argc, char **argv) {
    int i;

    colours = 8;
    while ((i = getopt (argc, argv, "86h")) != -1)
        switch (i) {
            case '8':
                colours = 8;
                break;
            case '6':
                colours = 16;
                break;
            case '?':
                if (isprint (optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else {
                    fprintf(stderr, "Unknown option character `\\x%x'.\n\n",
                        optopt);
                }
            case 'h':
                fprintf(stderr, "Switches:\n  -8: 8 colors\n");
                fprintf(stderr, "  -6: 16 colors\n");
                fprintf(stderr, "  -h: print this message\n\n");
                fprintf(stderr, "Keys:\n  `   = Switch to Movement Mode\n");
                fprintf(stderr, "  l   = Look around\n");
                fprintf(stderr, "  @   = Character Screen\n");
                fprintf(stderr, "  i   = Inventory Screen\n");
                fprintf(stderr, "  p   = Drink a potion\n");
                fprintf(stderr, "  r   = Read book/scroll\n");
                fprintf(stderr, "  g   = Gear Screen (Switching up equipment)\n");
                fprintf(stderr, "  ?   = Game Info\n");
                fprintf(stderr, "  F1  = Help text (Not implemented)\n");
                fprintf(stderr, "  F4  = Force Redraw\n");
                fprintf(stderr, "  F12 = Quit\n");
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
