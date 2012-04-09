#ifndef GUI_H
#define GUI_H

#include "game.hpp"
#include "character.hpp"
#include <string>
#include <list>

class GUI {
    public:
    void SetStatus(std::string);
    void RedrawStatus(void);
    void AddMessage(std::string str);
    void ProcessMessages(Game *g);
    Character* CharacterCreation(void);
    Character::Race SelectRace(void);
    Character::Class SelectClass(void);
    std::string GetString(std::string prompt);

    private:
    static std::string status_line;
    static std::list<std::string> messages;
};

#endif
