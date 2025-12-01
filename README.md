# Minishell - Core Shell Program

## Summary
This project involves creating a simple shell. It is a mini-Bash implementation that provides extensive knowledge about processes and file descriptors.

## Features

### Mandatory Part
- **Prompt:** Displays a prompt when waiting for a new command.
- **History:** Maintains a working command history.
- **Command Execution:** Searches and launches executables based on the `PATH` variable or using absolute/relative paths.
- **Signal Handling:**
  - `Ctrl-C`: Displays a new prompt on a new line.
  - `Ctrl-D`: Exits the shell.
  - `Ctrl-\`: Does nothing.
- **Quote Handling:**
  - `'` (Single quote): Prevents interpretation of meta-characters in the quoted sequence.
  - `"` (Double quote): Prevents interpretation of meta-characters except for `$` (dollar sign).
- **Redirections:**
  - `<`: Redirect input.
  - `>`: Redirect output.
  - `<<`: Heredoc (read input until delimiter).
  - `>>`: Redirect output in append mode.
- **Pipes (`|`):** Connects the output of one command to the input of the next.
- **Environment Variables:** Expands `$` followed by a sequence of characters to their values.
- **Exit Status:** Handles `$?` which expands to the exit status of the most recently executed foreground pipeline.

### Built-in Commands
Your shell must implement the following built-ins:
- `echo` (with option `-n`)
- `cd` (with only a relative or absolute path)
- `pwd` (with no options)
- `export` (with no options)
- `unset` (with no options)
- `env` (with no options or arguments)
- `exit` (with no options)

### Bonus Part
- **Logical Operators:** `&&` and `||` with parenthesis for priorities.
- **Wildcards:** `*` works for the current working directory.

## Technical Requirements
- **Language:** C
- **Norm:** Written in accordance with the Norm.
- **Memory Management:** All heap-allocated memory is properly freed. No memory leaks tolerated.
- **Global Variables:** At most one global variable allowed (for signal handling).

## Installation & Usage

Test the program as if it was bash.

To compile the project, run:
```bash
make
./minishell
