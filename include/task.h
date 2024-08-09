// Copyright (C) 2024 Thomas "thom-ba" Baumeister
// See GitHub for License

#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <iostream>

const std::string CURR_BOX = "[x] ";
const std::string BOX = "[ ] ";


struct Task {
    std::string title;
    bool todo;
};

class TaskStorage {
    public:
        std::vector<Task> todos;
        std::vector<Task> dones;
        
        void remove_task(int col, bool todo);

        void add_task(std::string title, bool todo);
        void toggle_task(int col, bool todo);

        int get_tasks_count() const;
        std::pair<int, int> get_indiv_count() const;
        void display_tasks(int col, bool todo) const;

    private:
        
};

#endif // TASK_H
