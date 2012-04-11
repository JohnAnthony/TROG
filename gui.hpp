#ifndef GUI_H
#define GUI_H

#include "game.hpp"
#include "character.hpp"
#include <ncurses.h>
#include <string>
#include <list>

class GUI {
    public:
    static void SetStatus(std::string);
    static void RedrawStatus(void);
    static void AddMessage(std::string str);
    static void ProcessMessages(Game *g);
    static Character* CharacterCreation(void);
    static Character::Race SelectRace(void);
    static Character::Class SelectClass(void);
    static std::string GetString(std::string prompt);
    static void ShowSplash(void);
    static void ShowSplash(Color col);
    static void StartScreen(void);
    static void ScreenNoise(void);
    static WINDOW* NewCentredWindow(int w, int h);
    static void Alert(std::string str);
    static bool BinaryChoice(std::string str, char a, char b);

    private:
    static std::string status_line;
    static std::list<std::string> messages;
    static const char* SplashStr;
};

#endif
