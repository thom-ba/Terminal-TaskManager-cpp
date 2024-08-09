// Copyright (C) 2024 Thomas "thom-ba" Baumeister
// See GitHub for License

#ifndef TASK_STORAGE_IO_H
#define TASK_STORAGE_IO_H

#include <fstream>  
#include <sstream>
#include <string>

#include "task.h"

#define TODO_FILE "./todos.txt"


void save_tasks(const TaskStorage& task_storage);

void load_tasks(TaskStorage* task_storage);

#endif // TASK_STORAGE_IO_H

