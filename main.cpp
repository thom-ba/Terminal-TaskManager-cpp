#include <termios.h>
#include <cstdlib>
#include <unistd.h>
#include <string.h>

#include "task.h"

termios orig_termios;
void on_exit() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void enable_raw_mode() {
    termios raw;

    tcgetattr(STDIN_FILENO, &orig_termios);
    raw = orig_termios;

    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    atexit(on_exit);
}

struct Pos {
    int row;
    int col;
};

const char* yellow_bg = "\033[43m";
const char* reset = "\033[0m";

void display_topbar(bool todo) {
    if(todo) {
        printf("%sTodo%s | ", yellow_bg, reset);
        printf("Done\n");
    } else {
        printf("Todo | ");
        printf("%sDone%s\n", yellow_bg, reset);
    }
}

void display_botbar() {
    printf("\n-- [a] to add a task | [d] to change tasks state --");
    std::cout.flush();
}

void display_interface(Pos pos, TaskStorage task_storage, bool todo) {
    display_topbar(todo);
    task_storage.display_tasks(pos.col, todo);   
    display_botbar();
}

void clear_screen() {
    std::cout << "\033[2J\033[H"; 
}

#define MAX_TASK_LEN 30
void add_task(TaskStorage *task_storage) {
    char title[MAX_TASK_LEN];
    clear_screen();
    printf("What task: ");
    on_exit(); 

    if(fgets(title, MAX_TASK_LEN, stdin) != NULL) {
        size_t len = strlen(title);
        if(len > 0 && title[len-1] == '\n') {
            title[len-1] = '\0';
        }
    }
    enable_raw_mode();

    task_storage->add_task(title);
}

int handle_input(TaskStorage task_storage) {
    bool todo = true;
    Pos pos{.row = 0, .col = 0 };

    clear_screen();
    enable_raw_mode();
    display_interface(pos, task_storage, todo); 

    char c;
    while(true) {
        read(STDIN_FILENO, &c, 1);

        switch (c) {
            case 'q': // Exit the TaskManager
                return 0;

            case 'a':
                add_task(&task_storage);
                break;

            case 'j': // Move Down one
                if (pos.col < task_storage.get_tasks_count()-1) 
                    pos.col++;
                break;

            case 'k': // Move up one
                if (pos.col > 0)
                    pos.col--;
                break;

            case 9: // Pressing Tab changes the tasks to show
                todo = !todo;
        }
        clear_screen();
        display_interface(pos, task_storage, todo);
    }    
}

int main() {
    TaskStorage taskStorage;
    taskStorage.add_task("Test 1");
    taskStorage.add_task("Test 2");

    handle_input(taskStorage);
}
