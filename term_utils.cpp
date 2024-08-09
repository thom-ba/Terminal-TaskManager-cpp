#include "term_utils.h"

Terminal::Terminal() : raw_mode_enabled(false) {
    enable_raw_mode();
}

Terminal::~Terminal() {
    disable_raw_mode();
}

void Terminal::enable_raw_mode() {
    termios raw;
    tcgetattr(STDIN_FILENO, &orig_term);
    raw = orig_term;

    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    raw_mode_enabled = true;
}

void Terminal::disable_raw_mode() {
    if(raw_mode_enabled) {
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
        raw_mode_enabled = false;
        clear_screen();
    }
}

void Terminal::clear_screen() const {
    std::cout << "\033[2J\033[H"; 
}
