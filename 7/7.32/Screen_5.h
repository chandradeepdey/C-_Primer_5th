#ifndef SCREEN_5_H_
#define SCREEN_5_H_

#include "Window_mgr.h"
#include <string>

class Screen {
    friend void Window_mgr::clear(ScreenIndex i);

public: // type members
    using pos = std::string::size_type;
public: // constructors
    Screen() = default;

    Screen(pos ht, pos wd) : height(ht), width(wd), contents(ht * wd, ' ') {}

    Screen(pos ht, pos wd, char ch) : height(ht), width(wd), contents(ht * wd, ch) {}

public: // other interface
    char get() const { return contents[cursor]; }

    char get(pos r, pos c) const { return contents[r * width + c]; }

    Screen move(pos r, pos c);

    Screen set(char ch);

    Screen set(pos r, pos c, char ch);

    Screen display(std::ostream &os);

    const Screen display(std::ostream &os) const;

private: // implementation
    void do_display(std::ostream &os) const { os << contents; }

private: // data members
    pos cursor = 0;
    pos height = 0, width = 0;
    std::string contents;
};

inline Screen Screen::move(pos r, pos c) {
    cursor = r * width + c;
    return *this;
}

inline Screen Screen::set(char ch) {
    contents[cursor] = ch;
    return *this;
}

inline Screen Screen::set(pos r, pos c, char ch) {
    contents[r * width + c] = ch;
    return *this;
}

inline Screen Screen::display(std::ostream &os) {
    do_display(os);
    return *this;
}

inline const Screen Screen::display(std::ostream &os) const {
    do_display(os);
    return *this;
}

inline Window_mgr::Window_mgr() : screens{Screen(24, 80, ' ')} {}

#endif
