// Copyright (C) 2024 Thomas "thom-ba" Baumeister
// See GitHub for License

#include "task_storage_io.h"

void erase_content() {
    std::ofstream ofs;
    ofs.open(TODO_FILE, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

void save_task_list(const std::vector<Task> tasks) {
    std::ofstream file(TODO_FILE, std::ios_base::app);
    for (const auto& task : tasks) { 
        file << (task.todo ? "[TODO]: " : "[DONE]: ") << task.title << "\n";
    }
}

void save_tasks(const TaskStorage& task_storage) {
    save_task_list(task_storage.todos);
    save_task_list(task_storage.dones);
}

void load_tasks(TaskStorage* task_storage) {
    std::ifstream todo(TODO_FILE); 
    
    int line_count = 1; std::string line;
    while(std::getline(todo, line)) {
        std::istringstream iss(line);
        
        std::string prefix;
        if(!(iss >> prefix)) { break; } 
        
        std::string task;
        std::getline(iss, task);

        if(!task.empty() && task[0] == ' ') {
            task = task.substr(1);
        }

        std::string trimmed_prefix;
        std::copy_if(prefix.begin(), prefix.end(), std::back_inserter(trimmed_prefix), [](char c) {
                return !std::isspace(c);
        });
        trimmed_prefix.pop_back();

        if(trimmed_prefix == "[TODO]") {
            task_storage->add_task(task, true);
        } else if (trimmed_prefix == "[DONE]") {
            task_storage->add_task(task, false);
        } else  {
            printf("Error in todo.txt in line number: %d\n --%s", line_count, line.c_str());
            break;
        }

        line_count++;
    }

    erase_content();
}


