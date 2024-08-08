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

// Return todo_count ; done_count
std::pair<int, int> TaskStorage::get_indiv_count() {
    int done_count; 

    int todo_count = std::count_if(tasks.begin(), tasks.end(),
            [](const Task& task) {
                return task.todo;
            });
   
    done_count = tasks.size() - todo_count; 
   
    return {todo_count, done_count};
}

void TaskStorage::toggle_task(int index) {
    tasks[index].todo = !tasks[index].todo;
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
