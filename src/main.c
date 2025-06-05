/**
 * @file main.c
 * @brief Contains the program's entry and main function.
 *
 * This file achieved the main compiler function. */

#include "rvcc.h"

// @brief Input's string.
char *CurrentInput;

/**
 * @brief The program's main entry.
 *
 * Achieved compiler function.
 *
 * @param[in] argc The number of command lin arguments
 * @param[in] argv Point to an array of parameter strings
 * @return int Program exit code. */
int
main (int argc, char *argv[])
{
  if (argc != 2)
    {
      error ("Usage: %s <expression>", argv[0]);
    }

  CurrentInput = argv[1];
  // Analyze token
  Token *tok = tokenize (CurrentInput);

  // Analyze AST
  Node *node = parse (tok);

  /** @brife Stack deepth */
  int stack_deep = 0;

  codegen (node, &stack_deep);

  return 0;
}
