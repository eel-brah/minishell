# 🐚 Minishell  
> As beautiful as a shell

---

## 📘 About

**Minishell** is a simplified shell project, built in C, that mimics core features of `bash`. The goal is to gain a deep understanding of:
- Process creation & management  
- File descriptors  
- Shell parsing & execution  

It's part of the curriculum at **42**, designed to teach you how the internals of a Unix shell work.

---

## 🛠️ Building & Running

```bash
make              # Compile minishell (release mode)
make MODE=debug   # Compile with debug flags and sanitizers
./minishell       # Run the shell
```

---

## ✅ Allowed External Functions

**Input/Output:**  
`printf`, `write`, `read`, `perror`

**Memory Management:**  
`malloc`, `free`

**File System:**  
`access`, `open`, `read`, `close`, `unlink`, `opendir`, `readdir`, `closedir`, `stat`, `lstat`, `fstat`, `getcwd`, `chdir`

**Process Control:**  
`fork`, `wait`, `waitpid`, `wait3`, `wait4`, `execve`, `exit`

**Signal Handling:**  
`signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill`

**Terminal I/O:**  
`isatty`, `ttyname`, `ttyslot`, `ioctl`, `tcsetattr`, `tcgetattr`, `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`

**Readline Library:**  
`readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`

---

## 🧩 Shell Features

- Custom interactive prompt
- Command history (via `readline`)
- Execution of binaries via `PATH` or absolute/relative paths
- Environment variable handling (`$VAR`, `$?`)
- Input/output redirection (`<`, `>`, `>>`)
- Pipelines (`|`)
- Logical operators: `&&`, `||`
- Built-in commands:
  - `echo` (with `-n`)
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Proper signal handling:
  - `Ctrl-C` → interrupts current command
  - `Ctrl-D` → exits the shell
  - `Ctrl-\` → ignored or handled appropriately
- Wildcard expansion (`*`) in the current directory
- Syntax support for parentheses to manage operator precedence

---

## ⚙️ How It Works

### 🧩 Overview  
Minishell parses and executes commands using a structured approach involving **recursive descent parsing** and a **command execution tree**. This method enables efficient parsing and execution of even complex command sequences like pipelines, redirections, and logical operators.

---

### 🧱 Parsing with Recursive Descent

#### 🔹 Lexical Analysis / Tokenization  
The first step involves breaking the input command string into **tokens** — keywords, operators, arguments, and more.  
Tokens are typically identified using a **tokenizer**, which recognizes elements like:
- Commands (e.g. `ls`)
- Arguments (e.g. `-l`)
- Operators (`|`, `>`, `&&`, etc.)
- Redirection symbols
- Quotes and subshells

#### 🔹 Recursive Descent Parsing  
This technique is a **top-down parsing** method where each grammar rule is represented by a function in the parser.  
Each parser function may call other functions recursively to process nested or dependent components.

The grammar defines the structure of valid shell input — including how sequences of commands, pipelines, and redirections must be formed.  
This allows support for nested constructs like:

```bash
(cmd1 && cmd2) || cmd3 | cmd4 > file
```

#### 🔹 Syntax Tree Construction  
As the parser processes the tokens, it builds a **syntax tree** (also known as an **abstract syntax tree** or AST).  
Each node represents:
- A command or argument
- A redirection
- A logical operator (`&&`, `||`)
- A pipe (`|`)

For example, a pipe operator creates a node with **two child nodes**, one for each command it connects.  
This tree represents the **logical structure** of the input and dictates the order of execution.

---

### 🚀 Command Execution

#### 🔹 Tree Traversal  
Once the syntax tree is built, the shell enters the **execution phase**.  
It traverses the tree recursively — the method of traversal depends on the type of node:
- **Simple command**: Execute directly
- **Pipe**: Connect two subcommands via pipe
- **Redirection**: Adjust file descriptors
- **Logical op**: Evaluate based on previous command's result

#### 🔹 Command Expansion  
During traversal, expansions are performed:
- Replace `$VAR` with its value from the environment
- Expand `*` (wildcards) to match filenames in the current directory
- Replace special variables like `$?` (last command exit code)

#### 🔹 Execution  

The execution follows a **bottom-up** approach:
- Leaf nodes (simple commands) are executed first
- Their outputs are passed through pipes or redirected as specified

Execution uses low-level system calls like:
- `fork()` – to create child processes
- `execve()` – to run external programs
- `dup2()` – for I/O redirection
- `pipe()` – to connect commands via pipes

#### 🔹 Handling Built-ins vs External Commands  
- **Built-ins** (`cd`, `echo`, `export`, etc.) are executed **inside the shell process** (no `fork`)
- **External commands** (`ls`, `cat`, etc.) are executed in a **child process** using `execve()`

The shell must first **check** if a command is a built-in before deciding how to execute it.

#### 🔹 Error Handling  
The shell implements robust (and maybe even a bit cheeky 😉) error handling:
- Syntax errors (e.g., unmatched quotes, invalid operator usage)
- Command not found
- Invalid arguments or paths
- Permission denied
- Invalid redirections or file descriptor usage

Meaningful error messages are printed to help the user debug issues easily.

---

### 🧪 Example Workflow

**Input:**  
```bash
ls -l | grep "test" > output.txt
```

**Tokens:**  
```text
[ls, -l, |, grep, "test", >, output.txt]
```

**Parsing:**  
- `ls -l` is parsed as a command node  
- `grep "test"` is parsed as another command node  
- `|` creates a pipe node with `ls -l` and `grep "test"` as children  
- `>` creates a redirection node parent to `grep "test"`  

**Tree Construction:**
```
       |   
      / \
    ls   >
          \
         grep "test"
              \
           output.txt
```

**Execution Steps:**
1. Execute `ls -l`
2. Pipe its output to `grep "test"`
3. Redirect the final result to `output.txt`

Each step is managed by traversing and interpreting the syntax tree, respecting the structure, order, and redirections implied by the parsed input.

---

**Note** 
Minishell was developed on a system running bash version 3.2.57, and it mimics its core features. 
As a result, some behaviors — particularly the exit statuses of built-in commands — may differ slightly from those in newer versions of Bash.

---

> This project was built as part of the 42 curriculum to learn system programming, shell design, and C fundamentals.

