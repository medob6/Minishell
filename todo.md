# Minishell Project Todo List

## Project Structure
- [x] Create directory structure
- [x] Create header file with necessary includes and data structures
- [x] Create Makefile
- [x] Set up libft directory

## Core Components
- [x] Initialize shell structure (init.c)
- [x] Environment variable handling (env.c)
- [x] Signal handling (signals.c)
- [x] Utility functions (utils.c)

## Command Parsing
- [x] Implement lexical analyzer (lexer.c)
  - [x] Handle quotes properly
  - [x] Tokenize input into words, pipes, redirections
- [ ] Implement parser (parser.c)
  - [ ] Create command structures from tokens
  - [ ] Handle syntax errors
- [ ] Implement variable expansion (expand.c)
  - [ ] Handle environment variables
  - [ ] Handle $? for exit status

## Command Execution
- [ ] Implement command executor (executor.c)
  - [ ] Execute external commands
  - [ ] Handle pipes between commands
- [ ] Implement redirections (redirections.c)
  - [ ] Input redirection (<)
  - [ ] Output redirection (>)
  - [ ] Append redirection (>>)
  - [ ] Heredoc (<<)

## Builtins
- [ ] Implement echo with -n option
- [ ] Implement cd with relative/absolute paths
- [ ] Implement pwd
- [ ] Implement export
- [ ] Implement unset
- [ ] Implement env
- [ ] Implement exit

## Signal Handling
- [ ] Handle Ctrl+C (SIGINT)
- [ ] Handle Ctrl+D (EOF)
- [ ] Handle Ctrl+\ (SIGQUIT)

## Testing and Debugging
- [ ] Test basic command execution
- [ ] Test pipes and redirections
- [ ] Test builtins
- [ ] Test signal handling
- [ ] Test error handling
- [ ] Memory leak checks

## Bonus (if applicable)
- [ ] Implement && and || operators with parentheses
- [ ] Implement wildcards (*) for current directory
