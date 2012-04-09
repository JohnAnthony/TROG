#ifndef GUI_H
#define GUI_H

#include "game.hpp"
#include "character.hpp"
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
    static void ScreenNoise(void);

    private:
    static std::string status_line;
    static std::list<std::string> messages;
    static const char* SplashStr;
};

#endif
