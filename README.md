# OS Shell — Shello

A terminal-based Linux-style shell and file browser implemented in C. Shello allows users to browse directories, navigate between folders, edit or execute files, run system commands, and sort directory entries.

## Features

* Displays files and directories from the current working directory
* Distinguishes files (`F`) from directories (`D`)
* Displays directory entries five at a time using pagination
* Navigate between directory pages
* Change directories using either an entry name or displayed index
* Open files using the `nano` terminal editor
* Execute files directly from the shell
* Run Linux/system commands with arguments
* Sort directory entries by name, file size, or modification date
* Displays the current working directory and system time
* Supports starting Shello in a directory passed as a command-line argument

## How It Works

1. Shello reads the contents of the current directory using `opendir()` and `readdir()`.
2. Information about each file or directory is stored in a `DirEntry` structure, including its name, type, size, and modification time.
3. The Action Menu allows the user to display entries, change directories, edit files, execute files, run commands, or sort the directory listing.
4. Directory entries are displayed five at a time and can be selected using either their names or numeric indexes.
5. Sorting is performed using `qsort()` with comparison functions for name, size, and modification date.
6. The menu repeats until the user chooses to quit.

## Action Menu

| Command | Action                    |
| ------- | ------------------------- |
| `q`     | Quit Shello               |
| `d`     | Display directory entries |
| `e`     | Edit a file using `nano`  |
| `r`     | Run a system command      |
| `f`     | Execute a file            |
| `c`     | Change directory          |
| `s`     | Sort directory entries    |

## Requirements

* GCC or another compatible C compiler
* Linux or a Linux-like/POSIX environment
* `nano` for the file-editing feature

The project uses standard C and POSIX libraries and does not require additional packages.

## How to Compile

From the directory containing `shello.c`, run:

```bash
gcc shello.c -o shello
```

Compiler warnings can also be enabled with:

```bash
gcc -Wall -Wextra shello.c -o shello
```

## How to Run

After compiling, run Shello from the same directory:

./shello

By default, Shello starts in the terminal's current working directory and displays the current time, current directory, and Action Menu.

You can also provide a directory when starting the program:

./shello <directory>

For example:

./shello Documents

or with a full path:

./shello /home/user/Documents

If a directory path contains spaces, place it inside quotation marks:

./shello "My Documents"

The program attempts to change to the supplied directory using chdir() before reading and displaying its contents.

Once Shello is running, use the Action Menu commands to display directory entries, change directories, edit or execute files, run commands, sort entries, or quit the program.

## What I Learned

While building this project, I practiced:

* Working with Linux/POSIX directory functions such as `opendir()`, `readdir()`, and `chdir()`
* Retrieving file metadata using `stat()`
* Representing file-system information using C structs
* Processing command-line arguments with `argc` and `argv`
* Sorting structures using `qsort()` and custom comparison functions
* Handling user input in a menu-driven C program
* Executing external commands and files
* Building an interactive terminal-based application

## Stack & Concepts

C, System Calls, File I/O, Directory Handling, Structs, Sorting, Command Execution
