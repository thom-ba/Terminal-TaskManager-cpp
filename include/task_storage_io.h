// Copyright (C) 2024 Thomas "thom-ba" Baumeister
// See GitHub for License

#ifndef TASK_STORAGE_IO_H
#define TASK_STORAGE_IO_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "task.h"

void ensure_file_exists();

void save_tasks(const TaskStorage &task_storage);

void load_tasks(TaskStorage *task_storage);

#endif // TASK_STORAGE_IO_H
