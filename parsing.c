#include "parser.h"

// here i should creat code for making my ast
// 1 - we know that ast hase a root ; that we shhould chose
// 2 - the root of the tree has to be chosen carefully ; we do so by searching in our token for the last high priority op

// Note :
// Pipes and Redirections
//The operator with the highest precedence after AND and OR is the PIPE.

// REDIRECTIONS come after the PIPE. The three different redirects don't have to respect any order of priority; they can be equal or different, it doesn't matter. They can be executed in any order.

// Step 1: Split the command into 2 with the last high priority operator found.

// Step 2: Split the left part into 3 based on the last two similar high priority operators.

// Step 3: Split the leftmost part into 3 based on the last two similar high priority operators.

t_ast 