/**
 * @file dbg.c
 * @brief Implement debug related functions.
 */

#include "rvcc.h"

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
dbg_print_ast (Node *node)
{
  if (node == NULL)
    return;

  switch (node->kind)
    {
    case ND_NUM:
      printf ("%d", node->val);
      break;
    case ND_NEG:
      printf ("(-");
      dbg_print_ast (node->lhs);
      printf (")");
      break;
    case ND_ADD:
    case ND_SUB:
    case ND_MUL:
    case ND_DIV:
      printf ("(");
      dbg_print_ast (node->lhs);
      printf (" %s ", node->kind == ND_ADD   ? "+"
                      : node->kind == ND_SUB ? "-"
                      : node->kind == ND_MUL ? "*"
                                             : "/");
      dbg_print_ast (node->rhs);
      printf (")");
      break;
    default:
      printf ("?unknown?");
      break;
    }
}

void
dbg_print_tree (Node *node, int level)
{
  if (node == NULL)
    return;

  for (int i = 0; i < level; i++)
    printf ("  ");

  switch (node->kind)
    {
    case ND_NUM:
      printf ("NUM: %d\n", node->val);
      break;
    case ND_NEG:
      printf ("NEG\n");
      dbg_print_tree (node->lhs, level + 1);
      break;
    case ND_ADD:
      printf ("ADD\n");
      dbg_print_tree (node->lhs, level + 1);
      dbg_print_tree (node->rhs, level + 1);
      break;
    case ND_SUB:
      printf ("SUB\n");
      dbg_print_tree (node->lhs, level + 1);
      dbg_print_tree (node->rhs, level + 1);
      break;
    case ND_MUL:
      printf ("MUL\n");
      dbg_print_tree (node->lhs, level + 1);
      dbg_print_tree (node->rhs, level + 1);
      break;
    case ND_DIV:
      printf ("DIV\n");
      dbg_print_tree (node->lhs, level + 1);
      dbg_print_tree (node->rhs, level + 1);
      break;
    default:
      printf ("UNKNOWN NODE\n");
      break;
    }
}
