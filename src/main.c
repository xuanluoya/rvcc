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

  // Declare a main segment.
  printf ("  .globl main\n");
  // main section label.
  printf ("main:\n");

  // li is aliases of addi, load a immediate into the register.
  // `strtol` will find the first number.
  printf ("  li a0, %d\n", get_number (tok));
  tok = tok->next;

  // Analyze
  while (tok->kind != TK_EOF)
    {
      if (equal (tok, "+"))
        {
          tok = tok->next;
          printf ("  addi a0, a0, %d\n", get_number (tok));
          tok = tok->next;
          continue;
        }

      tok = skip (tok, "-");
      printf ("  addi a0, a0, -%d\n", get_number (tok));
      tok = tok->next;
    }

  // ret is aliases od `jalr x0, x1, 0`,used of return subroutines
  printf ("  ret\n");

  return 0;
}

Token *
new_token (TokenKind kind, char *start, char *end)
{
  Token *tok = calloc (1, sizeof (Token));
  tok->kind = kind;
  tok->loc = start;
  tok->len = end - start;
  return tok;
}

int
get_number (Token *tok)
{
  if (tok->kind != TK_NUM)
    error_tok (tok, "expect a number");
  return tok->val;
}

bool
equal (Token *tok, char *str)
{
  return memcmp (tok->loc, str, tok->len) == 0 && str[tok->len] == '\0';
};

Token *
skip (Token *tok, char *str)
{
  if (!equal (tok, str))
    error_tok (tok, "expect '%s'", str);
  return tok->next;
}

Token *
tokenize (char *p)
{
  // (HEAD) -> 1 -> 2 -> 3
  Token head = {};
  Token *cur = &head;

  while (*p)
    {
      // skip whitespace.
      if (isspace (*p))
        {
          ++p;
          continue;
        }

      // Analysis number.
      if (isdigit (*p))
        {
          cur->next = new_token (TK_NUM, p, p);
          cur = cur->next;
          const char *oldptr = p;
          cur->val = strtol (p, &p, 10);
          cur->len = p - oldptr;
          continue;
        }

      // Analysis identifier.
      if (*p == '+' || *p == '-')
        {
          cur->next = new_token (TK_PUNCT, p, p + 1);
          cur = cur->next;
          ++p;
          continue;
        }

      // Handing unrecognizable character.
      error_at (p, "invalid token");
    }

  // Analysis end.
  // Add EOF
  cur->next = new_token (TK_EOF, p, p);

  // Debug
  // debug_token (head.next);

  // return head -> cur 1 (next) -> 2  ...
  return head.next;
}

// [NOTE]
// * Function in rvcc.h:106.
// * Debug function for token.
/**
void
debug_token (Token *tok)
{
  int tok_num = 0;
  while (tok->kind != TK_EOF)
    {
      printf ("(Num: %d, Kind: %d, Val: %d)\n", tok_num, tok->kind,
      tok->val); tok_num++; tok = tok->next;
    }
}
*/
