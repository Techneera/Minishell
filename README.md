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
- **Logical Operators:** `&&` and `||` with parenthesis for priorities.
- **Wildcards:** Glob pattern `*`, works for the current working directory.

### Built-in Commands
Your shell must implement the following built-ins:
- `echo` (with option `-n`)
- `cd` (with only a relative or absolute path)
- `pwd` (with no options)
- `export` (with no options)
- `unset` (with no options)
- `env` (with no options or arguments)
- `exit` (with no options)

## Technical Requirements
- **Language:** C
- **Memory Management:** All heap-allocated memory is properly freed. No memory leaks tolerated.
- **Global Variables:** One global variable allowed (for signal handling).

## Installation & Usage

Test the program as if it was bash.

To compile the project, run:

1. Clone the repository's main branch.
2. Run the following command:

```bash
make && ./minishell
