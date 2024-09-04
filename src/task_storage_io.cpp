// Copyright (C) 2024 Thomas "thom-ba" Baumeister
// See GitHub for License

#include "../include/task_storage_io.h"
#include <filesystem>

#include <thread>
#include <chrono>

const std::string TODO_FILE = std::string(getenv("HOME")) + "/.local/share/tui_task_manager/todo.txt";

void ensure_file_exists() {
    printf("%s\n", TODO_FILE.c_str());
    std::this_thread::sleep_for(std::chrono::seconds(2));
    if (!std::filesystem::exists(TODO_FILE)) {
        std::ofstream file(TODO_FILE);
        if (!file) {
            std::cerr << "Failed to create the file: " << TODO_FILE << std::endl;
        } else {
            std::cout << "File created: " << TODO_FILE << std::endl;
            file.close();
        }
    }
}
void create_dir() {
    const char *home_dir = getenv("HOME");
    if (!home_dir) {
        std::cerr << "Unable to get home directory" << std::endl;
        return;
    }

    std::string todo_dir = std::string(home_dir) + "/.local/share/tui_task_manager/";

    // Ensure the directory exists, create it if not present
    try {
        if (!std::filesystem::exists(todo_dir)) {
            if (std::filesystem::create_directories(todo_dir)) {
                std::cout << "Directory created: " << todo_dir << std::endl;
            } else {
                std::cerr << "Failed to create directory: " << todo_dir << std::endl;
            }
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}

// Clears content of the TODO file
void erase_content() {
    std::ofstream ofs(TODO_FILE, std::ofstream::out | std::ofstream::trunc);
    if (!ofs) {
        std::cerr << "Failed to open file for erasing: " << TODO_FILE << std::endl;
        return;
    }
    ofs.close();
}

// Saves a list of tasks to the TODO file
void save_task_list(const std::vector<Task>& tasks) {
    std::ofstream file(TODO_FILE, std::ios_base::app);
    if (!file) {
        std::cerr << "Failed to open file for saving tasks: " << TODO_FILE << std::endl;
        return;
    }

    for (const auto& task : tasks) {
        file << (task.todo ? "[TODO]: " : "[DONE]: ") << task.title << "\n";
    }
    file.close();
}

// Saves tasks to the TODO file
void save_tasks(const TaskStorage& task_storage) {
    save_task_list(task_storage.todos);
    save_task_list(task_storage.dones);
}

// Loads tasks from the TODO file
void load_tasks(TaskStorage* task_storage) {
    std::ifstream todo(TODO_FILE);
    if (!todo) {
        std::cerr << "Failed to open file for loading tasks: " << TODO_FILE << std::endl;
        return;
    }

    int line_count = 1;
    std::string line;
    while (std::getline(todo, line)) {
        std::istringstream iss(line);

        std::string prefix;
        if (!(iss >> prefix)) {
            break;
        }

        std::string task;
        std::getline(iss, task);

        if (!task.empty() && task[0] == ' ') {
            task = task.substr(1);
        }

        std::string trimmed_prefix;
        std::copy_if(prefix.begin(), prefix.end(),
                     std::back_inserter(trimmed_prefix),
                     [](char c) { return !std::isspace(c); });
        trimmed_prefix.pop_back();

        if (trimmed_prefix == "[TODO]") {
            task_storage->add_task(task, true);
        } else if (trimmed_prefix == "[DONE]") {
            task_storage->add_task(task, false);
        } else {
            std::cerr << "Error in todo.txt on line number: " << line_count
                      << "\n -- " << line << std::endl;
            break;
        }

        line_count++;
    }

    todo.close();
    erase_content();
}

