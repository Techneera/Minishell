# Minishell — Architecture Reference

> **Audience:** Developers who want to understand, extend, or debug the minishell codebase.
> This document follows the data flow from a raw input line all the way to process exit.

---

## Table of Contents

1. [High-Level Pipeline](#1-high-level-pipeline)
2. [Module Map](#2-module-map)
3. [Phase 1 — Lexer (Tokenisation)](#3-phase-1--lexer-tokenisation)
4. [Phase 2 — Parser (AST Construction)](#4-phase-2--parser-ast-construction)
5. [AST Node Types](#5-ast-node-types)
6. [Phase 3 — Expansion](#6-phase-3--expansion)
7. [Phase 4 — Execution Engine](#7-phase-4--execution-engine)
8. [Pipe Model](#8-pipe-model)
9. [Redirection Model](#9-redirection-model)
10. [Bonus Features: `&&`, `||`, `()`](#10-bonus-features---and-)
11. [Wildcard Expansion](#11-wildcard-expansion)
12. [Built-in Dispatch](#12-built-in-dispatch)
13. [Signal Handling](#13-signal-handling)
14. [Memory Management](#14-memory-management)
15. [Key Data Structures](#15-key-data-structures)

---

## 1. High-Level Pipeline

```
User input (readline)
        │
        ▼
  ┌─────────────┐
  │   Lexer     │  ft_state_lexer() + get_next_token()  →  token stream
  └──────┬──────┘
         │
         ▼
  ┌─────────────┐
  │   Parser    │  ft_parser() / ft_parse_and_or()      →  AST (t_ast *)
  └──────┬──────┘
         │
         ▼
  ┌─────────────┐
  │  Expansion  │  expand_args()                        →  resolved arguments
  └──────┬──────┘
         │
         ▼
  ┌─────────────┐
  │  Execution  │  ft_exec_tree() dispatch              →  exit status
  └─────────────┘
```

The REPL lives in `shell.c:loop()`. Each iteration:

1. Reads a line with `readline()`.
2. Skips whitespace-only input.
3. Calls `process_rl()` which drives all four phases above.
4. Frees the AST and lexer state with `cleanup_loop()`.

---

## 2. Module Map

| Directory | Responsibility |
|---|---|
| `shell.c` | `main()`, REPL loop, `env_init()`, `envlist_to_array()` |
| `src/lexer/` | Tokeniser: raw string → `t_token` linked list |
| `src/ast/` | Recursive-descent parser: tokens → `t_ast` tree |
| `src/expansion/` | Variable expansion, quote stripping, wildcard masking |
| `src/execution/` | Process management, pipes, redirections, built-in dispatch |
| `src/builtins/` | `echo`, `exit`, `exit_status`, `unset` implementations |
| `src/wildcard/` | Glob pattern matching (`*`) against the filesystem |
| `include/` | Public API headers and shared type definitions |
| `libft/` | Custom reimplementation of libc utilities |

---

## 3. Phase 1 — Lexer (Tokenisation)

**Source:** `src/lexer/`  
**Entry point:** `ft_state_lexer(char *line)` → `get_next_token(t_lexer *l)`

### How it works

The lexer operates as a **stateful cursor** over the input string. It stores the input
pointer, the current position (`pos`), and the total length (`len`) in `t_lexer`.

`get_next_token()` is called once per token. On each call it:

1. Skips leading whitespace.
2. Checks whether the current character is a **meta-character** (`|`, `&`, `<`, `>`, `(`, `)`).
   - If yes → delegates to `ft_handle_operator()`.
   - If no  → delegates to `ft_handle_word()`.
3. Returns a heap-allocated `t_token` with a `tok_label` and a `str`.

### Operator resolution

| Input | Token label |
|---|---|
| `\|` | `TOKEN_PIPE` |
| `\|\|` | `TOKEN_OR` |
| `&` (alone) | `TOKEN_ERROR` |
| `&&` | `TOKEN_AND` |
| `<` | `TOKEN_REDIR_IN` |
| `>` | `TOKEN_REDIR_OUT` |
| `>>` | `TOKEN_REDIR_APPEND` |
| `<<` + delimiter | `TOKEN_REDIR_HEREDOC` (the `str` field holds the raw delimiter) |
| `(` | `TOKEN_LEFT_PAR` |
| `)` | `TOKEN_RIGHT_PAR` |
| word / quoted word | `TOKEN_WORD` |
| end of input | `TOKEN_EOF` |

### Quote handling in the lexer

`ft_handle_word()` tracks an `in_quotes` flag. While inside quotes the loop only exits
when the matching closing quote is found, preventing any meta-character from breaking the
word token early. Unclosed quotes return `TOKEN_ERROR`.

### Heredoc delimiter extraction

`ft_get_unquoted_str()` / `ft_heredoc_quotes()` strip quoting characters from the heredoc
delimiter so that the executor knows whether to expand `$` variables inside heredoc body
lines.

---

## 4. Phase 2 — Parser (AST Construction)

**Source:** `src/ast/`  
**Entry point:** `ft_parser(t_lexer *l)`

### Recursive-descent grammar

The parser is a classic **recursive-descent** implementation. The grammar (in precedence
order, lowest to highest) is:

```
and_or        ::= pipeline ( ('&&' | '||') pipeline )*
pipeline      ::= grain_with_redirs ( '|' grain_with_redirs )*
grain_with_redirs ::= subshell | simple_command
subshell      ::= '(' and_or ')' redirect*
simple_command ::= (WORD | redirect)+
redirect      ::= ('<' | '>' | '>>' | '<<') WORD
```

### Token look-ahead

`t_parser` keeps two tokens: `current_token` (the token being examined) and `peek` (the
next token). `ft_parser_iter()` advances both by fetching the next token from the lexer.
This one-token look-ahead is sufficient for the grammar above.

### Parser call chain

```
ft_parser()
  └─ ft_parse_and_or()        ← handles && / ||
       └─ ft_parse_pipeline() ← handles |
            └─ ft_parse_grain_with_redirs()
                 ├─ ft_parse_subshell()       (if TOKEN_LEFT_PAR)
                 └─ ft_parse_simple_command() (if TOKEN_WORD / redir)
```

### Simple command accumulation

`ft_parse_simple_command()` uses a `t_simp_ctx` accumulator:

- Arguments are pushed onto a `t_list *args` linked list.
- Redirections are pushed onto a `t_list *redirs` linked list.
- `ft_finalize_simp_cmd()` converts both lists into flat arrays and wraps them in a `t_cmd`,
  which is then stored in a `NODE_CMD` AST node.

---

## 5. AST Node Types

Every node in the tree is a `t_ast` struct. The `type` field determines which union
members are valid:

| `t_node_type` | Meaningful fields | Semantics |
|---|---|---|
| `NODE_CMD` | `cmd` | A simple command with argument array and redirections |
| `NODE_PIPE` | `left`, `right` | `left \| right` — pipe between two subtrees |
| `NODE_AND` | `left`, `right` | `left && right` — short-circuit AND |
| `NODE_OR` | `left`, `right` | `left \|\| right` — short-circuit OR |
| `NODE_SUBSHELL` | `body`, `cmd` | `( body )` — subshell; `cmd` holds external redirs |

---

## 6. Phase 3 — Expansion

**Source:** `src/expansion/`, `src/wildcard/`  
**Entry point:** `expand_args(t_data *data)`

Expansion is performed **at execution time**, after the AST is built, on each command node
just before it is executed. This matches bash behaviour (no pre-expansion during parsing).

### Expansion states

`expand_word()` is a finite-state machine with three states (see `t_exp_state`):

| State | Behaviour |
|---|---|
| `EXP_GENERAL` | Normal mode: expand `$`, detect quotes, pass `*` through (masked) |
| `EXP_SQUOTE` | Inside `'...'`: copy literally, no expansion |
| `EXP_DQUOTE` | Inside `"..."`: expand `$` but suppress all other meta-characters |

### Variable expansion

`ft_get_expanded_value()` handles three cases:

- `$?` → converts the integer from `ft_exit_status(0,0,0)` to a string.
- `$n` (digit) → expands to an empty string (positional parameters not supported).
- `$VARNAME` → looks up the variable in the `envp` array via `ft_expand_env_var_aux()`.

### Wildcard masking

To prevent a `*` inside quotes from being treated as a glob, `mask_wildcards()` replaces
literal `*` characters with the control byte `\001` before variable expansion. After
expansion, `unmask_wildcards()` restores them. This ensures only unquoted `*` characters
reach the glob engine.

### Wildcard expansion

`ft_handle_wildcard()` is called for each argument that contains a `*`. It calls
`get_wildcard_matches()` (in `src/wildcard/ft_wildcard.c`), which opens the relevant
directory and uses `ft_match_wildcard()` / `ft_match_asterisk()` to filter entries. The
matching filenames replace the original argument in the `args` array via
`insert_wildcard_args()`.

---

## 7. Phase 4 — Execution Engine

**Source:** `src/execution/`  
**Entry point:** `ft_execution(t_data *data)`

### Initialisation

Before any forking, `ft_create_fds()` traverses the AST to:

1. Count commands (`n_cmds`), regular file redirections (`n_files`), and heredocs (`n_docs`).
2. Allocate the `t_fds` structure: `fd_files[]`, `heredoc_fds[][]`, `c_pids[]`.
3. Open all non-heredoc redirect files up front (`fill_fd_file()`).
4. Set up all heredoc pipes (`init_heredoc()` → `here_doc()`).

This pre-allocation strategy means every child process can look up its `fd` by a simple
index, avoiding race conditions around file opening.

### Dispatch — `ft_exec_tree()`

```c
if (node->type == NODE_CMD)      → ft_execute_cmd()
if (node->type == NODE_PIPE)     → ft_execute_pipe()
if (node->type == NODE_SUBSHELL) → ft_execute_sshell()
if (node->type == NODE_AND)      → execute_and()
if (node->type == NODE_OR)       → execute_or()
```

---

## 8. Pipe Model

**Source:** `src/execution/ft_execute_pipe.c`

`ft_execute_pipe()` creates a `pipe(pfd)`, then forks **two children**:

- **Left child**: `dup2(pfd[1], STDOUT_FILENO)` → executes the left subtree.
- **Right child**: `dup2(pfd[0], STDIN_FILENO)` → executes the right subtree.

The parent closes both pipe ends and returns immediately. The `ft_execution()` loop at the
top level calls `waitpid()` for all recorded PIDs after the entire tree has been dispatched.

For nested pipelines (e.g., `a | b | c`), `ft_parse_pipeline()` builds a left-leaning
binary tree: `((a | b) | c)`. Each `NODE_PIPE` node represents one `|` stage. When the
left child of an outer pipe itself contains a pipe, `child_pipe()` recurse-dispatches the
inner `ft_exec_tree()` call in the child process.

---

## 9. Redirection Model

**Source:** `src/execution/ft_fill_fds_file.c`, `src/execution/ft_here_doc.c`

All file descriptors are pre-opened before any fork. Each command node carries a
`t_redir[]` array. At execution time, `ft_child_cmd()` / `ft_child_sshell()` iterate over
the array and `dup2()` the pre-opened fds onto stdin/stdout.

Heredocs are implemented as anonymous pipes: `here_doc()` reads lines from the terminal
in a forked process and writes them to the write end; the read end is stored in
`heredoc_fds[doc_id]` for later `dup2()` onto stdin.

---

## 10. Bonus Features — `&&`, `||`, `()`

### Logical operators

`execute_and()` and `execute_or()` implement **short-circuit evaluation**:

- `&&` (AND): execute left subtree; if exit status is **0**, execute right subtree.
- `||` (OR): execute left subtree; if exit status is **non-zero**, execute right subtree.

Both functions call `ft_create_fds_bonus()` to allocate fresh `t_fds` for the current
subtree, then delegate to `ft_exec_tree()` for any `NODE_CMD`, `NODE_PIPE`, or
`NODE_SUBSHELL` nodes they encounter. The exit status obtained from `wait_bonus()` /
`ft_exit_status()` determines whether to continue execution.

### Subshells

`ft_execute_sshell()` forks a child process. The child calls `ft_child_sshell()`, which:

1. Applies any external redirections attached to the `NODE_SUBSHELL` node.
2. Recursively calls `ft_exec_tree()` on the `body` sub-tree, which may itself contain
   pipes, logical operators, or further subshells.
3. Calls `secure_exit()` so the child never returns to the parent's REPL loop.

### Operator precedence

Because the parser handles `&&` / `||` at the lowest precedence level
(`ft_parse_and_or()`), and pipelines at a higher level (`ft_parse_pipeline()`), operator
precedence is encoded structurally in the AST. Parentheses override this by treating the
grouped expression as a `NODE_SUBSHELL` leaf at the `grain` level — the highest precedence
in the grammar.

---

## 11. Wildcard Expansion

**Source:** `src/wildcard/ft_wildcard.c`, `src/execution/wildcards_utils.c`

`get_wildcard_matches(char *pattern)` splits the pattern on the last `/` to separate the
directory prefix from the filename pattern. It opens the target directory (or `.` if no
prefix) and iterates with `readdir()`. Each entry is tested with `ft_match_wildcard()`,
which delegates to the recursive `ft_match_asterisk()`.

`ft_match_asterisk()` works as follows:

- Consecutive `*` characters are collapsed to one.
- If the remaining pattern is empty after consuming all `*`, everything matches.
- Otherwise it tries to match the remaining pattern against every possible suffix of `str`.

Hidden files (names starting with `.`) are never matched unless the pattern also starts
with `.`.

---

## 12. Built-in Dispatch

**Source:** `src/execution/ft_is_builtin.c`, `src/execution/ft_cd.c`,
`src/execution/ft_env.c`, `src/execution/ft_export.c`, `src/execution/ft_pwd.c`,
`src/builtins/`

`ft_is_builtin()` is called from `ft_execute_cmd()` **before** any `fork()`. Built-ins
run in the shell process itself so that they can modify the shell's environment and current
directory. The function string-matches the command name and calls the corresponding
implementation.

| Built-in | Implementation file |
|---|---|
| `echo` | `src/builtins/ft_echo.c` |
| `cd` | `src/execution/ft_cd.c` |
| `pwd` | `src/execution/ft_pwd.c` |
| `export` | `src/execution/ft_export.c` |
| `unset` | `src/builtins/ft_unset.c` |
| `env` | `src/execution/ft_env.c` |
| `exit` | `src/builtins/ft_exit_status.c` |

---

## 13. Signal Handling

**Source:** `src/execution/handle_signal.c`

| Signal | Context | Handler | Effect |
|---|---|---|---|
| `SIGINT` (`Ctrl-C`) | Prompt | `handle_sigstop` | Prints newline, resets readline, sets status 130 |
| `SIGINT` | Waiting for child | `handle_sigint_wait` | Prints newline, sets status 130 |
| `SIGINT` | Heredoc input | `handle_sigstop_heredoc` | Closes stdin, marks heredoc interrupted |
| `SIGQUIT` (`Ctrl-\`) | Prompt | `SIG_IGN` | Ignored |
| `SIGQUIT` | Child process | `SIG_DFL` | Default kernel behaviour |

The signal strategy is set in `ft_sig_and_line()` before every `readline()` call, and
reset to `SIG_DFL` inside child processes before `execve()`.

`heredoc_status()` is a static-variable helper that acts as a thread-safe flag: when a
heredoc is interrupted by `SIGINT`, the flag is set to 1, and the parent can detect this
to abort heredoc processing without relying on additional global variables.

---

## 14. Memory Management

The shell maintains two primary resource trees per input line:

- `data->lexer` — freed by `free_lexer()` in `cleanup_loop()`.
- `data->root` — the AST, freed recursively by `ft_free_ast()` in `cleanup_loop()`.

`t_fds` (pre-opened file descriptors and PID array) is freed by `free_data()` at the end
of `ft_execution()`. Bonus-mode `t_fds` allocations are freed by `free_fds_bonus()` before
each logical-operator step.

The environment is stored as a `t_list *env_list` and a parallel `char **envp` array.
`refresh_env_array()` re-synchronises the array whenever a variable is added or removed.

---

## 15. Key Data Structures

```c
/* Core shell state — passed by pointer through almost every function */
typedef struct s_data {
    t_ast   *root;       /* root of the current AST */
    t_ast   *tree;       /* cursor pointing to the current execution node */
    t_fds   *fds;        /* pre-allocated file descriptors and PIDs */
    char   **envp;       /* flat environment array for execve() */
    t_list  *env_list;   /* linked list of t_env nodes */
    char    *rl;         /* raw readline input string */
    t_lexer *lexer;      /* lexer state for the current input line */
} t_data;

/* AST node — forms the execution tree */
typedef struct s_ast {
    t_cmd        *cmd;   /* command data (args + redirs), valid for CMD/SUBSHELL */
    t_node_type   type;  /* NODE_CMD | NODE_PIPE | NODE_AND | NODE_OR | NODE_SUBSHELL */
    struct s_ast *body;  /* subshell body (NODE_SUBSHELL only) */
    struct s_ast *left;  /* left operand (PIPE / AND / OR) */
    struct s_ast *right; /* right operand (PIPE / AND / OR) */
} t_ast;

/* Pre-allocated file descriptor table for one execution cycle */
typedef struct s_fds {
    int    **heredoc_fds; /* [doc_id][0=read,1=write] for each heredoc */
    int     *fd_files;    /* opened fds for each non-heredoc redirection */
    int     *c_pids;      /* PIDs of forked children */
    t_get    get;         /* counts: n_files, n_cmds, n_docs */
    t_pos    pos;         /* current indexes: file_id, doc_id, fork_id */
} t_fds;
```
