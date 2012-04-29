#ifndef GUI_H
#define GUI_H

#include "game.hpp"
#include "character.hpp"
#include "geometry.hpp"
#include <string>
#include <list>

class GUI {
    public:
    static void Init(void);
    static void End(void);
    static void AttachTo(Game *g);
    static void SetStatus(std::string);
    static void RedrawStatus(void);
    static void AddMessage(std::string str);
    static void ProcessMessages(Game *g);
    static Character* CharacterCreation(void);
    static Character::Race SelectRace(void);
    static Character::Class SelectClass(void);
    static std::string GetString(std::string prompt);
    static void ShowSplash(void);
    static void ShowSplash(Colour col);
    static void StartScreen(void);
    static void ScreenNoise(void);
    static WINDOW* NewCentredWindow(int w, int h);
    static void Alert(std::string str);
    static void Alert2(char const * const * const msg);
    static bool BinaryChoice(std::string str, char a, char b);
    static void FancyClear(void);
    static void ShowCharacterScreen(Character *c);
    static void ShowInventoryScreen(Character *c);
    static void CharacterStatusLine(Character *c);
    static void ShowInfoScreen(Game *g);
    static void CharacterRename(Character *c);
    static void QuitDialogue(Game *g);
    static void DrawAsOverlay(Point p, char c, int col);
    static bool isOnScreen(Point p);
    static void MoveCamera(Direction::Type d);
    static void DoRedraw(void);
    static void DrawLevel(Level *l);
    static void DrawObjectRelative(Point p, char c);
    static void CentreCam(Point p);
    static void InfoScreen(Item *i);
    static void InfoScreen(Equippable *e);
    static void InfoScreen(Potion *p);
    static void InfoScreen(StatTome *st);
    static void InfoScreen(Spell *sp);
    static void HardRedraw(void);
    static void ShowTombstone(Game *g);
    static Direction::Type GetDirection(void);

    static int NUM_COLOURS;

    private:
    static Game *g;
    static std::string status_line;
    static std::vector<std::string> messages;
    static const char* SplashStr;
    static const char* TombStr;
    static Point cam;
};

#endif
