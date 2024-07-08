# Minishell

As beautiful as a shell

## About

This project is about creating a simple shell, akin to a basic version of bash. <br>
The main goal is to have a good understanding of process creation and file descriptors using the C programmming language.

## Allowed External Functions
Input/Output: printf write read perror<br>
Memory Management: malloc free<br>
File System: access open read close unlink opendir readdir closedir stat lstat fstat getcwd chdir<br>
Process Control: fork wait waitpid wait3 wait4 execve exit<br>
Signal Handling: signal sigaction sigemptyset sigaddset kill<br>
Terminal I/O: isatty ttyname ttyslot ioctl tcsetattr tcgetattr tgetent tgetflag tgetnum tgetstr tgoto tputs<br>
Readline Library: readline rl_clear_history rl_on_new_line rl_replace_line rl_redisplay add_history

## Shell Requirements
Display a prompt when waiting for a new command.<br>
Maintain a working history.<br>
Execute the correct executable based on the PATH variable or relative/absolute paths.<br>
Limit global variable usage to indicate a received signal.<br>
Handle special characters and quotes correctly.<br>
Implement input/output redirections and pipes.<br>
Manage environment variables and special variables like $?.<br>
Handle ctrl-C, ctrl-D, and ctrl-\ appropriately in interactive mode.<br>
Implement built-in commands: echo with -n, cd, pwd, export, unset, env, and exit.<br>
Implement logical operators && and || with parenthesis for priority.<br>
Handle wildcards * in the current working directory.<br>

# How It Work

## Overview
It parses and executes commands using a structured approach involving recursive descent parsing and a command execution tree. This method ensures efficient parsing and execution of complex command sequences.

## Parsing with Recursive Descent

### Lexical Analysis/Tokenization:
The first step involves breaking the input command string into tokens (e.g., keywords, operators, arguments).<br>
Tokens are typically identified using a tokenizer which recognizes elements like commands, arguments, pipes, and redirection operators.

### Recursive Descent Parsing:
This parsing technique is a top-down method where each grammar rule is represented by a function in the parser.<br>
The parser functions call each other recursively to process the input tokens and build a syntax tree.<br>
The grammar rules define how commands are structured, including how pipes and redirections are handled.<br>

### Syntax Tree Construction:
As the parser processes the input, it constructs a syntax tree, also known as a parse tree.<br>
Each node in the tree represents a command, an argument, or an operator.<br>
For instance, a pipe operator would create a node with two child nodes representing the commands on either side of the pipe.

## Command Execution

### Tree Traversal:
Once the syntax tree is built, the execution phase begins by traversing this tree.<br>
The traversal method depends on the type of operation represented by each node (e.g., simple command, pipe, redirection).

### Command Expansion:
During traversal, any necessary expansions are performed (e.g., replacing $VAR with its value or expanding *).

### Execution:

The commands are executed in a bottom-up manner, starting from the leaf nodes of the tree.<br>
For simple commands, the shell uses system calls like execve to run the program.<br>
For pipelines, the output of one command is passed as input to the next using pipes.<br>
For redirections, file descriptors are manipulated to redirect input/output as specified.

### Handling Built-ins and External Commands:
The shell differentiates between built-in commands (like cd, echo) and external programs.<br>
Built-in commands are handled internally within the shell process.<br>
External commands are executed by forking a new process and using execve.

### Error Handling
The shell ensures robust(^_-) error handling by checking for common errors such as syntax errors, command not found, and invalid arguments.<br>
It provides meaningful error messages to the user to aid in debugging command input issues.

## Example Workflow
Input: ls -l | grep "test" > output.txt<br>
Tokens: [ls, -l, |, grep, "test", >, output.txt]<br>
Parsing:<br>
>ls -l parsed as a command node.<br>
>grep "test" parsed as another command node.<br>
>| creates a pipe node with ls -l and grep "test" as children.<br>
>\> creates a redirection node parent to grep "test".<br>
Tree Construction:<br>
```
Copy code
   |   
  / \
ls   > output.txt
      \
   grep "test"
```

Execution:<br>
Traverse the tree and execute ls -l, pipe its output to grep "test", and redirect the final output to output.txt.<br>

