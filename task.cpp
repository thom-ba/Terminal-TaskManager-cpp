#include "task.h"

#define TODO() throw std::logic_error("TODO: Not yet implemented")

void TaskStorage::add_task(std::string title, bool todo) {
    if(todo) {
        todos.push_back(Task{ .title = title, .todo = true});
    } else {
        dones.push_back(Task{ .title = title, .todo = false});
    }
}

int TaskStorage::get_tasks_count() const {
    return todos.size() + dones.size();
}

// Return todo_count ; done_count
std::pair<int, int> TaskStorage::get_indiv_count() const {
    return {todos.size(), dones.size() };
}

void TaskStorage::remove_task(int col, bool todo) {
    if(todo) {
        todos.erase(todos.begin() + col);
    } else {
        dones.erase(dones.begin() + col);
    }
}

void TaskStorage::toggle_task(int col, bool todo) {
    if(todo) { // ist in todo
        add_task(todos[col].title, !todo);
        remove_task(col, todo);

    } else { // ist nicht in todo
        add_task(dones[col].title, !todo);
        remove_task(col, todo);
    }
}

void TaskStorage::display_tasks(int col, bool todo) const {
    std::ostringstream oss;
    const std::vector<Task>& tasks = todo ? todos : dones;
    int tasks_size = tasks.size();

    for (int i = 0; i < tasks_size; ++i) {
        // Determine the sign based on whether this index is the current one
        std::string curr_sign = (col == i) ? CURR_BOX : BOX;

        // Append the task title
        oss << curr_sign << tasks[i].title << "\n";
    }

    std::cout << oss.str();
}

