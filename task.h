#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <iostream>

struct Task {
    std::string title;
    bool todo;
};

class TaskStorage {
    public:
        void edit_task(Task old_task, int index);

        void remove_task(int col, bool todo);
        void remove_todo(std::string& title);
        void remove_done(std::string& title);

        void add_task(std::string title, bool todo);
        void toggle_task(int col, bool todo);

        int get_tasks_count();
        std::pair<int, int> get_indiv_count();


        void display_tasks(int col, bool todo);

    private:
        std::vector<Task> todos;
        std::vector<Task> dones;
};

#endif // TASK_H
