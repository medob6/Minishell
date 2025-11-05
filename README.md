# MiniShell

A small POSIX‑like command-line shell implemented as a learning project. Mini Shell is intended for education and experimentation and demonstrates core shell concepts: parsing, job control, pipelines, I/O redirection, builtins, process creation, and basic signal handling. The codebase is kept compact and readable so you can study and extend it.

Table of contents
- Project overview
- Features
- Requirements
- Build & run
- Usage examples
- Supported grammar & behavior
- Built-in commands
- Implementation notes
- Testing & debugging
- Known limitations
- Contributing
- License & credits
- Contact

Project overview
----------------
Mini Shell is a simplified reimplementation of a Unix shell suitable for learning. It focuses on predictable behavior, clarity, and correctness for common shell operations.

Goals
- Implement the shell loop: read → parse → execute → wait → repeat
- Support pipelines and file redirection
- Provide essential builtins: cd, exit, export, unset, env, echo, pwd
- Support background jobs with `&`
- Handle basic signals (Ctrl‑C / SIGINT, Ctrl‑Z / SIGTSTP)

Not a goal
- Full POSIX conformance
- Advanced job control features (complex job lists, sophisticated fg/bg handling)
- Full interactive line editing (use readline/libedit if needed)

Features
--------
- Customizable interactive prompt
- Tokenization with support for single and double quotes and escapes
- Environment variable expansion (e.g., $HOME, ${VAR})
- PATH resolution for executables
- I/O redirection: `< file`, `> file`, `>> file`
- Pipelines (e.g., `ls -l | grep '^d' | wc -l`)
- Background execution with `&`
- Builtins: `cd`, `exit`, `pwd`, `env`, `export`, `unset`, `echo`
- Basic signal handling: Ctrl‑C interrupts the foreground job; Ctrl‑Z suspends (basic)
- Helpful error messages for common failures

Requirements
------------
- POSIX-compatible OS (Linux, macOS)
- C compiler (gcc/clang) if implemented in C (or the appropriate language toolchain)
- make (if a Makefile is provided)

Build & run
-----------
Typical (C project) steps:

1. Clone:
   git clone https://github.com/medob6/Minishell.git
   cd Minishell

2. Build:
   make

3. Run:
   ./minishell
   mini$ echo "hello world"
   mini$ exit

If your project uses a different build system, adapt these steps accordingly.

Usage examples
--------------
- Simple command:
  mini$ /bin/ls -l

- Path lookup:
  mini$ ls -l

- Redirection:
  mini$ echo "line" > out.txt
  mini$ cat < out.txt

- Append:
  mini$ echo "another" >> out.txt

- Piping:
  mini$ ps aux | grep python | wc -l

- Background:
  mini$ sleep 30 &

- Environment:
  mini$ export GREETING="hi"
  mini$ echo $GREETING

Built-in commands
-----------------
- cd [dir] — change directory; no args → $HOME
- exit [n] — exit shell with status n (default 0)
- pwd — print working directory
- env — print environment variables
- export NAME[=value] — set environment variable
- unset NAME — remove environment variable
- echo [args...] — print text (supports `-n`)

Supported grammar & behavior
----------------------------
- Tokens separated by whitespace unless quoted
- Single quotes (') are literal; double quotes (") allow expansion
- Backslash `\` escapes the next character
- Multiple redirections allowed; last redirection for an FD wins
- Pipes connect stdout → stdin across commands
- Trailing `&` backgrounds the whole pipeline
- Shell exit status is the status of the last foreground pipeline

Implementation notes (high level)
--------------------------------
- Read loop:
  - Read a line (getline or equivalent)
  - Tokenize respecting quotes/escapes
  - Parse tokens into commands/pipelines with redirections
  - Execute: run builtins in the main process when appropriate; fork/exec external commands with proper redirections and pipe wiring; wait for foreground jobs
- Signal handling:
  - Parent ignores SIGINT/SIGTSTP while waiting; children use default handlers
  - Background processes: optionally report PID
- Error handling:
  - Clear messages for file-open, fork/exec, and parse errors

Testing & debugging
-------------------
- Unit-test tokenizer/parser where feasible
- Manual tests:
  - Redirections: `cat < infile > outfile`
  - Pipelines: `echo a b c | tr ' ' '\n' | wc -l`
  - Quotes/expansion: `echo "a $HOME 'b'"`
  - Background: `sleep 1 &`
- Tools:
  - valgrind (C) to detect leaks
  - strace -f to inspect forks/execs

Known limitations
-----------------
- Not fully POSIX-compliant
- No advanced line editing (readline recommended)
- Limited job control (fg/bg/jobs may be absent)
- Partial support for complex parameter expansions and process substitution

Contributing
------------
Contributions welcome: bug fixes, tests & CI, new features (heredoc, improved job control), docs improvements. Open issues or PRs with clear descriptions and tests.

