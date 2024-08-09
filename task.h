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
        std::vector<Task> todos;
        std::vector<Task> dones;
        
        void edit_task(std::string title, int index, bool todo);

        void remove_task(int col, bool todo);
        void remove_todo(const std::string& title);
        void remove_done(const std::string& title);

        void add_task(std::string title, bool todo);
        void toggle_task(int col, bool todo);

        int get_tasks_count();
        std::pair<int, int> get_indiv_count();


        void display_tasks(int col, bool todo);

    private:
        
};

#endif // TASK_H
