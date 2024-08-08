#include "task.h"

#define TODO() throw std::logic_error("TODO: Not yet implemented")

void TaskStorage::add_task(std::string title) {
    tasks.push_back(Task{ .title = title, .todo = true});
}

void TaskStorage::remove_task(std::string& title) {
    tasks.erase(std::remove_if(
                tasks.begin(), 
                tasks.end(), 
                [&title](const Task& task) { 
                return task.title == title;
                }
                ), 
            tasks.end()
            );
}

void TaskStorage::edit_task(Task old_task, int index) {
    TODO();
}

int TaskStorage::get_tasks_count() {
    return tasks.size();
}

void TaskStorage::display_tasks(int col, bool todo) {
    std::ostringstream oss;
    
    for(int i = 0; i < (int) tasks.size(); i++) {
        if (todo && !tasks[i].todo) {
            continue;
        }
        if (!todo && tasks[i].todo) {
            continue;
        }

        if(col == i) {
            oss << "[x] " << tasks[i].title << "\n";
        } else {
            oss << "[] " << tasks[i].title << "\n";
        }
    }

    std::cout << oss.str();
}
