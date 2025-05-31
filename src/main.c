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
  Node *node = expr (&tok, tok);
  if (tok->kind != TK_EOF)
    error_tok (tok, "extra token");

  // dbg_print_tree (node, 0);
  // return 1;

  printf ("  .globl main\n");
  printf ("main:\n");

  /** @brife Stack deepth */
  int stack_deep = 0;

  gen_exper (node, &stack_deep);

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
      if (ispunct (*p))
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

void
dbg_print_token (Token *tok)
{
  int tok_num = 0;
  while (tok->kind != TK_EOF)
    {
      printf ("(Num: %d, Kind: %d, Val: %d)\n", tok_num, tok->kind, tok->val);
      tok_num++;
      tok = tok->next;
    }
}

void
push (int *stack_deep)
{
  // The stack usually grows downward, that is, the value of the stack pointer
  // decreases. Grow downwards to make room for data: sp
  // sp: `Stack Pointer`,  pointer stack head.
  printf ("  addi sp, sp, -8\n");
  // sd: used to store 64 bits of data in the register to a specified location
  // in memory.
  // sd rs2, offset(rs1)
  // a0 -> data => sp location
  printf ("  sd a0, 0(sp)\n");
  stack_deep++;
}

void
pop (char *reg, int *stack_deep)
{
  printf ("  ld %s, 0(sp)\n", reg);
  // Back stack top.
  printf ("  addi sp, sp, 8\n");
  stack_deep--;
}

void
gen_exper (Node *node, int *stack_deep)
{
  // Load number onto a0.
  if (node->kind == ND_NUM)
    {
      printf ("  li a0, %d\n", node->val);
      return;
    }

  gen_exper (node->rhs, stack_deep);
  push (stack_deep);

  gen_exper (node->lhs, stack_deep);
  pop ("a1", stack_deep);

  // generate assembly.
  switch (node->kind)
    {
    case ND_ADD: // + a0=a0+a1
      printf ("  add a0, a0, a1\n");
      return;
    case ND_SUB: // - a0=a0-a1
      printf ("  sub a0, a0, a1\n");
      return;
    case ND_MUL: // * a0=a0*a1
      printf ("  mul a0, a0, a1\n");
      return;
    case ND_DIV: // / a0=a0/a1
      printf ("  div a0, a0, a1\n");
      return;
    default:
      break;
    }

  error ("invalid expression");
}
