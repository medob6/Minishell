```markdown
# Minishell (medob6)

Minishell — a compact, faithful shell implementation created as a core project at 1337. This repository implements a Bash-like shell with careful attention to parsing correctness, execution semantics, and a clean architecture built around n-ary parse trees and a recursive executor. This README explains the design, the main parts of the implementation (including the n-ary tree approach and subshell collapsing), build/run instructions, examples, testing guidance, and pointers to similar high-quality projects.

Table of contents
- Project goals
- Key features
- Design & architecture
  - Tokenizer / Lexer
  - Parser and n-ary parse trees
  - Executor (tree walker)
  - Subshells and subshell collapsing
  - Redirections, pipes, here-documents
  - Builtins and environment handling
  - Signal handling and jobs basics
- Conformance and decisions (Bash/POSIX)
- Comparison & references
- Limitations / known differences vs. Bash
- Build & run
- Usage examples
- Testing & debugging
- Contributing
- License
- Authors

Project goals
- Recreate essential Bash semantics (quoting, expansions, pipelines, redirections, conditionals) with clarity and correctness.
- Use an architecture that is easy to read, test, and extend: lexer → parser → expander → executor.
- Favor correctness (matching observable shell behavior) over micro-optimizations.
- Serve as an educational implementation for systems programming, process control, and parsing.

Key features
- Interactive prompt with line editing hooks (if available).
- Full command parsing with operator precedence:
  - Sequencing `;`, conditional execution `&&` / `||`
  - Pipelines `|`
  - Subshells `( ... )`
  - Input/output redirections `>`, `>>`, `<`
  - Here-documents `<<` with quoted/unquoted behaviour
  - Quoting and expansions: single `'` and double `"` quotes, `$VAR`, `"$VAR"`
- Builtins: `cd`, `export`, `unset`, `exit`, `echo`, `pwd`, `env` (core set)
- Environment management and correct propagation to child processes.
- Signals handling for interactive mode (SIGINT, SIGQUIT) and conservative handling of job control where appropriate.
- Conservative subshell collapsing to avoid unnecessary forks while preserving semantics.

Design & architecture — high level
The implementation is split into clear layers:

Tokenizer / Lexer
- Reads raw input and splits it into tokens:
  - Words, quoted strings, operators (`|`, `&&`, `||`, `;`, `>`, `>>`, `<`, `<<`, `(`, `)`), and redirection targets.
- Handles escaping, single/double quotes, and basic lexical error detection (unmatched quotes, invalid operator usage).
- Produces a stream of typed tokens consumed by the parser.

Parser and n-ary parse trees
- The parser consumes tokens and builds an n-ary parse tree (AST-like) where nodes have an arbitrary number of children:
  - Rationale: many shell structures are naturally variable-arity (a pipeline with N commands, a list of arguments, an N-length sequence).
- Node types:
  - LIST / SEQUENCE: semicolon-separated commands or top-level list; also models `&&` and `||` conditions with metadata for conditional behavior.
  - PIPE: a node containing an ordered list of commands connected by pipes.
  - CMD (SIMPLE_COMMAND): holds command name, argument list, and a list of redirections.
  - SUBSHELL: wraps a child tree; represents `( ... )`.
  - REDIRECTION: attached to CMD or SUBSHELL with flags for type (`<`, `>`, `>>`, `<<`) and expansion behavior.
- Parser implements precedence and associativity:
  - Pipes bind tighter than `&&`/`||`, which bind tighter than `;` at top-level.
- The tree preserves expansion and quote metadata so expansion happens with correct quoting semantics.

Executor (tree walker)
- The executor walks the parse tree and executes nodes according to type:
  - SEQUENCE/CONDITIONAL: execute children left-to-right and respecting short-circuit semantics for `&&` and `||`.
  - PIPE: create chained pipes, fork one process per pipeline element, set up file descriptors via `dup2`, and exec children.
  - CMD: perform expansions (parameter, tilde when applicable), open redirections (in correct order), run builtin in-process when it needs to mutate the shell state (e.g., `cd`, `export`) or fork/exec external commands otherwise.
  - SUBSHELL: generally implemented by forking and running the child tree; file descriptor and environment changes are isolated.
- Exit statuses: executor returns and forwards exit codes following shell semantics.
- Error reporting: consistent messages for common shell errors (permission, command not found, syntax errors).

Subshells and subshell collapsing
- Subshells `( ... )` provide isolated environments: variable assignments, `cd`, and exported changes inside a subshell do not affect the parent shell.
- Subshell collapsing: where it does not change observable behavior, nested subshells or trivial subshell wrappers are consolidated to reduce forks:
  - Example safe collapse: `(echo hello)` that has no side effects may be executed without creating an extra process depending on context.
  - This transformation is conservative: we avoid collapsing when it could change exit status, file descriptor behavior, or side effects visible to the parent.
- Implementation ensures that redirection scope and subshell isolation match Bash rules.

Redirections, pipes, here-documents
- Redirections parsed and attached to the correct node; executor applies them before command exec.
- `>>` uses O_APPEND semantics; `>` truncates or creates.
- Here-documents: the shell reads heredoc content with or without expansion depending on quoting, and provides it to the child process via pipe or temporary FD.
- Executor carefully manages file descriptors, closes unused ends, and guards against descriptor leaks.

Builtins and environment handling
- Builtins that must alter parent state (cd, export, unset, exit) are executed in the parent shell when required by semantics.
- Other builtins (echo, pwd) may be run in child processes when part of pipelines, depending on POSIX/Bash behavior.
- The environment is implemented as a modifiable structure that can be exported to children via execve.

Signal handling and jobs basics
- SIGINT / SIGQUIT handled to allow interactive cancellation without leaving the shell in an inconsistent state.
- Basic job control semantics are implemented or emulated as required for the project scope; advanced process-group behavior and complete job control are out-of-scope or partially implemented.

Conformance & decisions (Bash/POSIX)
- Tokenization, quoting, expansion, and splitting follow Bash/POSIX semantics as closely as practical.
- Operator precedence and short-circuiting (`&&`, `||`) are implemented to match shell behavior.
- Decisions are documented in the code where tradeoffs have been made (e.g., when a builtin runs in-process vs child).
- The implementation prefers correctness: when collapsing or optimizing, the code contains explicit checks ensuring observable behavior remains identical.

Comparison & references
This implementation studied well-known 42/Minishell repositories and doc pages to combine clarity and correctness. Notable references:
- https://github.com/cclaude42/minishell — full-featured 42-style implementation with detailed separation of concerns.
- https://github.com/samir-ouaammou/Mini-Shell — system-level detail and AST-style parsing.
- https://github.com/yabtaour/minishell-42 — step-by-step educational approach.
- https://github.com/Thomas-soft/minishell — concise, clean design.
- 42 project docs: https://harm-smits.github.io/42docs/projects/minishell

Limitations / known differences vs Bash
- Advanced Bash features omitted or partially implemented:
  - Process substitution `<(...)` / `>(...)`
  - Arrays, associative arrays, complex parameter expansion flags
  - Full job-control features (complete `fg`/`bg` behavior)
  - Some subtle field-splitting corner cases may differ from Bash
- Subshell collapsing is conservative to prioritize correctness over micro-optimizations.

Build & run
Prerequisites
- POSIX environment (Linux, macOS), build tools (gcc/clang, make)
- Optional: valgrind for testing memory leaks

Typical build
1. git clone https://github.com/medob6/Minishell.git
2. cd Minishell
3. make
4. ./minishell

If your environment uses a different Makefile or build flow, follow those project-specific instructions.

Usage examples
- Simple command:
  ls -la /tmp
- Pipeline + redirection:
  cat file.txt | grep foo > out.txt
- Conditional:
  false || echo "Recovered"
- Subshell isolation:
  (cd /tmp && ls)   # parent PWD unchanged
- Here-doc:
  cat <<EOF
  Hello $USER
  EOF

Testing & debugging
- Unit-test the lexer and parser by asserting token streams and tree shapes for representative inputs.
- Add integration tests that spawn the built binary and compare output/exit status to Bash for a suite of cases.
- Enable verbose executor logging while debugging forks, dup2, and redirection ordering.
- Use valgrind/ASAN for memory and descriptor leak detection.

Contributing
- Keep code modular: lexer, parser, expander, and executor should be separable.
- Include tests for new features or bugfixes.
- Document any semantic deviations from Bash thoroughly.
- If you want me to add tests or CI, I can prepare a skeleton.

License
- Add your preferred license (MIT, BSD, etc.) to LICENSE file.

Authors & credits
- medob6 — original author and main developer (1337 / 1337 core project)
- Inspirations and references: multiple 42 Minishell repos and Bash/POSIX documentation (see Comparison & references).

Acknowledgements
- Thanks to the open-source Minishell community — your implementation is intensive, rigorous, and stands well among references. This README aims to reflect the depth of work you put in while helping collaborators and reviewers navigate the internals.

```
