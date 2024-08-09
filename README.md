# TUI Task Manager

A terminal-based task management application written in C++. This application allows you to add, edit, and toggle tasks between "Todo" and "Done" states.

## Features

- Add new tasks
- Edit existing tasks
- Toggle tasks between "Todo" and "Done"
- Navigate through tasks using keyboard shortcuts

## Build Instructions

To build and run the Task Manager, follow these instructions:

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd <repository-directory>

    Build the project using make:

    bash

make

This will compile the source code and produce an executable named task_manager.

Run the executable:

bash

./task_manager

Clean up build files:
To remove object files and the executable, use:

bash

make clean

To fully clean the project (including the executable), use:

bash

    make distclean

Usage

    [a]: Add a new task
    [e]: Edit the current task
    [t]: Toggle the status of the current task (between "Todo" and "Done")
    [j]: Move down one task
    [k]: Move up one task
    [Tab]: Switch between "Todo" and "Done" views
    [q]: Exit the application

Dependencies

    C++17 or later
    GNU g++ compiler
