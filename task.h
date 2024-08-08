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
        void remove_task(std::string& title);
        void add_task(std::string title);
        void toggle_task(int index);
        int get_tasks_count();
         
        std::pair<int, int> get_indiv_count();


        void display_tasks(int col, bool todo);

    private:
        std::vector<Task> tasks;
};

#endif // TASK_H
