First, we need to agree that having a high-level perspective of the project's structure is essential. Even for a medium-sized project like this, using some design patterns—even simple ones—can make development much smoother and more manageable.

At first glance, we can see that tokenization is a key part of the project, so I’ll start by creating a tokenizer, which will be our first step.

Next, since we’re building an interactive shell, we need to display a prompt and read user input from the terminal. To achieve this, I created a function to construct the prompt, using getcwd to fetch the current directory and generate a Bash-like prompt.

I've also added two files:

    parsing.c (to handle parsing logic)
    utils.c (for helper functions, placed inside a utils/ folder with clear function names)

Additionally, I introduced some necessary header includes, which should all be placed inside an include/ folder. I also set up a Makefile to manage compilation.

Right now, the structure is basic and rough, but we should build on top of it and refine it step by step.

Solaiman, check out this repo, clone it to your device, and start pushing your changes. GG! 🚀
