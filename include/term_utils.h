#ifndef TERMUTILS_H
#define TERMUTILS_H

#include <termios.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <string>


class Terminal {
    public:
        Terminal();
        ~Terminal();

        void enable_raw_mode();
        void disable_raw_mode();
        void clear_screen() const;


    private:
        termios orig_term;
        bool raw_mode_enabled;
};

#endif // TERMUTILS_H
