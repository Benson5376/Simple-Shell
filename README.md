# Simple Shell Implementation

## Introduction

This project is a custom implementation of a simple Unix shell in C. It supports basic command execution, input/output redirection, background process management, and simple piping functionality. 

## Table of Contents

1. [Features](#features)
2. [Installation](#installation)
3. [Usage](#usage)
4. [Dependencies](#dependencies)
5. [Project Structure](#project-structure)
6. [Contributors](#contributors)

---

## Features

- **Command Execution**: Execute standard Unix commands.
- **Background Process Management**: Commands can be executed in the background using the `&` symbol.
- **Piping**: Supports a single pipe (`|`) for connecting two commands.
- **Input/Output Redirection**: Redirect output to files using the `>` operator.
- **Whitespace Management**: Handles commands with extra spaces gracefully.

---

## Installation

### Prerequisites
- GCC compiler
- A Unix-like operating system (Linux/MacOS)

### Steps
1. Clone this repository:
   ```bash
   git clone <repository-url>
   cd <repository-folder>
   
2. Compile the source code:
   ```bash
   gcc Simple_Shell.c -o simple_shell
   
3. Run the compiled program:
   ```bash
   ./simple_shell
   
---

### Usage
1. Start the shell:
   ```bash
   ./simple_shell
   
2. Type a command and press Enter. Examples:

  - **Basic command**:
     ```bash
     > ls -l
  
  - **Piped command**:
     ```bash
     > ls | grep ".c"
   
  - **Background process**:
     ```bash
     > sleep 10 &
  
  - **Output redirection**:
     ```bash
     > echo "Hello, World" > output.txt
     
To exit, use ```Ctrl+C``` or terminate the terminal.

---

### Dependencies
This program relies only on the standard C libraries and Linux system calls:
- ```<stdio.h>```
- ```<stdlib.h>```
- ```<string.h>```
- ```<fcntl.h>```
- ```<unistd.h>```
- ```<sys/wait.h>```

---

### Project Structure
#### Key Functions
1. ```main```: The entry point for the shell loop.
2. ```Input```: Handles user input.
3. ```Process_Command```: Decides the type of command (piped, redirected, or basic) and processes it.
4. ```Execute```: Executes non-piped commands.
5. ```Execute_Pipe```: Executes commands connected by a single pipe.
6. ```IO_Redirection```: Handles output redirection.
7. ```Check_Background_Execution```: Detects and manages background processes.

---

### Contributors
- **Name:** Hua-En (Benson) Lee
- **Email:** enen1015@gmail.com




   
