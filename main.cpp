#include <termios.h>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <chrono>
#include <thread>

#include "task.h"
#include "term_utils.h"

struct Pos {
    int row;
    int col;
};

#define MAX_TASK_LEN 30
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

void display_interface(TaskStorage task_storage, Pos pos, bool todo, Terminal term) {
    term.clear_screen();
    display_topbar(todo);
    task_storage.display_tasks(pos.col, todo);   
    display_botbar();
    term.enable_raw_mode();
}

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

void add_task(TaskStorage *task_storage, Terminal term) {
    std::string title;
    
    term.clear_screen();
    printf("What task: ");
    
    getline(std::cin, title);

    size_t len = title.length();

    if(validate(*task_storage, title, len)) {
        if(len > 0 && title[len-1] == '\n') {
            title[len-1] = '\0';
        }
        task_storage->add_task(title, true);

    } else {
        add_task(task_storage, term);
    }
}

void edit_task(TaskStorage *task_storage, int index, bool todo, Terminal term) {
    std::string title;

    std::string old_title = todo ? task_storage->todos[index].title : task_storage->dones[index].title;
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
        edit_task(task_storage, index, todo, term);
    }
}

int handle_input(TaskStorage& task_storage, Terminal& term) {
    bool todo = true;
    Pos pos{0, 0};

    display_interface(task_storage, pos, todo, term);

    char c;
    while(true) {
        auto counts = task_storage.get_indiv_count();
        read(STDIN_FILENO, &c, 1);

        switch (c) {
            case 'q': // Exit the TaskManager
                term.clear_screen();
                return 0;

            case 'a':
                term.disable_raw_mode();
                add_task(&task_storage, term);
                term.enable_raw_mode();
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
                term.clear_screen();
                term.disable_raw_mode();
                edit_task(&task_storage, pos.col, todo, term);
                term.enable_raw_mode();

                break;

            case 9: // Pressing Tab changes the tasks to show
                pos.col = 0;
                todo = !todo;
        }
        term.clear_screen();
        display_interface(task_storage, pos, todo, term);
    }    
}

int main() {
    Terminal term;
    TaskStorage taskStorage;

    taskStorage.add_task("Test 1", true);
    taskStorage.add_task("Test 2", true);
    taskStorage.add_task("Test 3", false);

    handle_input(taskStorage, term);
}
