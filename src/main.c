/**
 * @file main.c
 * @brief Contains the program's entry and main function.
 *
 * This file achieved the main compiler function. */

#include "rvcc.h"

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
      fprintf (stderr, "%s: invalid number of arguments", argv[0]);
      return 1;
    }

  // `p` save the script from command line.
  char *p = argv[1];

  // Declare a main segment.
  printf ("  .globl main\n");
  // main section label.
  printf ("main:\n");
  // li is aliases of addi, load a immediate into the register.
  // `strtol` will find the first number.
  printf ("  li a0, %ld\n", strtol (p, &p, 10));

  // analysis `p`.
  while (*p)
    {
      if (*p == '+')
        {
          // skip symble
          ++p;
          // addi rd, rs1, imm => rd = rs1 + imm
          printf ("  addi a0, a0, %ld\n", strtol (p, &p, 10));
          continue;
        }
      else if (*p == '-')
        {
          ++p;
          // rd = rs1 + (-imm)
          printf ("  addi a0, a0, -%ld\n", strtol (p, &p, 10));
          continue;
        }
      else
        {
          fprintf (stderr, "unexpected character: `%c`\n", *p);
          return 1;
        }
    }

  // ret is aliases od `jalr x0, x1, 0`,used of return subroutines
  printf ("  ret\n");

  return 0;
}
