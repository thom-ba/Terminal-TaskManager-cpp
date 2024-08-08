#include <termios.h>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <thread>
#include <chrono>
#include <thread>

#include "task.h"

termios orig_termios;

void clear_screen() {
    std::cout << "\033[2J\033[H"; 
}

void on_exit() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    clear_screen();
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
    printf("\n-- [a] to add a task | "\
            "[t] toggle tasks state | "\
            "[e] edit task --");
    std::cout.flush();
}

void display_interface(Pos pos, TaskStorage task_storage, bool todo) {
    display_topbar(todo);
    task_storage.display_tasks(pos.col, todo);   
    display_botbar();
}



#define MAX_TASK_LEN 30
bool validate(TaskStorage task_storage, std::string title, int len) {
    bool validated = true;
    if(len >= MAX_TASK_LEN)
        validated = false;
    
    if ((std::find_if(task_storage.todos.begin(), 
                task_storage.todos.end(), 
                [&title](const Task& task) {
                    return task.title == title;
                }) != task_storage.todos.end()
       ) || 
        (std::find_if(task_storage.dones.begin(), 
                      task_storage.dones.end(),
                      [&title](const Task& task) {
                        return task.title == title;
                      }) != task_storage.dones.end()))
        validated = false;

    if (validated == false) {
        printf("Title too long or exists already!");
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return validated;
}

void add_task(TaskStorage *task_storage) {
    std::string title;
    on_exit(); 
    printf("What task: ");
    
    getline(std::cin, title);

    size_t len = title.length();

    if(validate(*task_storage, title, len)) {
        if(len > 0 && title[len-1] == '\n') {
            title[len-1] = '\0';
        }
        enable_raw_mode();
        task_storage->add_task(title, true);

    } else {
        add_task(task_storage);
    }
}

void edit_task(TaskStorage *task_storage, int index, bool todo) {
    std::string title;

    std::string old_title = todo ? task_storage->todos[index].title : task_storage->dones[index].title;
    clear_screen();
    on_exit();
    printf("Edit Task: %s -> ", old_title.c_str());

    getline(std::cin, title);
    size_t len = title.length();

    if(validate(*task_storage, title, len)) {
        if(len > 0 && title[len-1] == '\n') {
            title[len-1] = '\0';
        }
        task_storage->remove_task(index, todo); 
        task_storage->add_task(title, todo);

    } else {
        edit_task(task_storage, index, todo);
    }

    enable_raw_mode();
}

int handle_input(TaskStorage task_storage) {
    bool todo = true;
    Pos pos{.row = 0, .col = 0 };

    clear_screen();
    enable_raw_mode();
    display_interface(pos, task_storage, todo); 

    char c;
    while(true) {
        auto counts = task_storage.get_indiv_count();
        read(STDIN_FILENO, &c, 1);

        switch (c) {
            case 'q': // Exit the TaskManager
                clear_screen();
                return 0;

            case 'a':
                add_task(&task_storage);
                break;

            case 'j': // Move Down one
                if(todo) {
                    if(pos.col < counts.first -1) 
                        pos.col++;
                } else {
                    if(pos.col < counts.second -1) 
                        pos.col++;
                }
                break;

            case 'k': // Move up one
                if (pos.col > 0)
                    pos.col--;
                break;

            case 't': // Change status
                if ((todo) && (pos.col < counts.first || pos.col < 0)) {
                    task_storage.toggle_task(pos.col, todo);

                    if(pos.col == counts.first -1 && pos.col > 0) {
                        pos.col--;
                    }
                } 

                if ((todo == false) && (pos.col < counts.second || pos.col < 0)) {
                    task_storage.toggle_task(pos.col, todo);

                    if(pos.col == counts.second -1 && pos.col > 0) {
                        pos.col--;
                    }
                }

                break;

            case 'e': // Edit Task
                edit_task(&task_storage, pos.col, todo);

                break;

            case 9: // Pressing Tab changes the tasks to show
                pos.col = 0;
                todo = !todo;
        }
        clear_screen();
        display_interface(pos, task_storage, todo);
    }    
}

int main() {
    TaskStorage taskStorage;
    taskStorage.add_task("Test 1", true);
    taskStorage.add_task("Test 2", true);
    taskStorage.add_task("Test 3", false);

    handle_input(taskStorage);
}
