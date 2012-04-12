#include "scrollable_menu.hpp"
#include "gui.hpp"
#include <sstream>
#include <cstring>

#define DEFAULT_HEIGHT 6

ScrollableMenu::ScrollableMenu(std::string inTitle) {
    this->sz.x = this->title.length() + 6;
    this->sz.y = DEFAULT_HEIGHT;
    this->window = GUI::NewCentredWindow(sz.x, sz.y);
    this->SetTitle(inTitle);
    this->max_size = 0;
}

void
ScrollableMenu::Reset(void) {
    this->pointer = 0;
    this->scroll_offset = 0;
    this->Options.clear();
    this->Resize(this->title.length() + 6, DEFAULT_HEIGHT);
}

void
ScrollableMenu::Resize(int x, int y) {
    this->sz.x = x;
    this->sz.y = y;
    delwin(this->window);
    this->window = GUI::NewCentredWindow(this->sz.x, this->sz.y);
}

void
ScrollableMenu::AddItem(std::string str) {
    this->Options.push_back(str);

    if (this->sz.x < (int) str.length() + 10)
        sz.x = str.length() + 10;

    if (this->sz.y < LINES && this->max_size == 0)
        this->sz.y++;
    else if (this->sz.y < this->max_size)
        this->sz.y++;

    this->Resize(this->sz.x, this->sz.y);
}

void
ScrollableMenu::Show(void) {
    int i;
    std::string str;
    std::stringstream ss;
    static char const * const empty_string = "* E M P T Y *";

    box(this->window, 0, 0);

    ss << "::" << this->title << "::";
    str = ss.str();

    mvwprintw(this->window, 0, (this->sz.x - str.length()) / 2, str.c_str());

    if (this->Options.size() == 0) {
        mvwprintw(this->window, 2, (this->sz.x - strlen(empty_string)) / 2, empty_string);
    }
    else {
        i = 0;
        for (std::list<std::string>::iterator it = this->Options.begin();
          it != this->Options.end(); ++it, ++i) {
            str = *it;

            if (i == this->pointer - this->scroll_offset)
                mvwprintw(this->window, 3+i, 2, "-->  ");
            else
                mvwprintw(this->window, 3+i, 2, "    ");

            wprintw(this->window, str.c_str());

            for (int e = str.length(); e < this->sz.x - 9; ++e)
                waddch(this->window, ' ');
        }
    }

    wrefresh(this->window);
    refresh();
}

void
ScrollableMenu::PtrUp(void) {
    this->pointer--;
    if (pointer < 0)
        pointer = 0;
    this->Show();
}

void
ScrollableMenu::PtrDown(void) {
    this->pointer++;
    if (pointer >= (int) this->Options.size())
        pointer = this->Options.size() - 1;
    this->Show();
}

void
ScrollableMenu::PageUp(void) {

}

void
ScrollableMenu::PageDown(void) {

}

void
ScrollableMenu::SetTitle(std::string str) {
    this->title = str;
}

int
ScrollableMenu::Selection(void) {
    if (this->Options.size() == 0)
        return -1;
    return pointer;
}

ScrollableMenu::~ScrollableMenu(void) {
    delwin(this->window);
}
