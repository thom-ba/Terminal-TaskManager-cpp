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
};

class TaskStorage {
    public:
        void edit_task(Task old_task, int index);
        void remove_task(std::string& title);
        void add_task(std::string title);
        int get_tasks_count();

        void display_tasks(int col);

    private:
        std::vector<Task> tasks;
};

#endif // TASK_H
