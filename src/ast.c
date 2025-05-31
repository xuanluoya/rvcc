/**
 * @file ast.c
 * @brief Implement ast related functions.
 */

#include "rvcc.h"

void
dbg_print_ast (Node *node)
{
  if (node == NULL)
    {
      return;
    }

  if (node->kind == ND_NUM)
    {
      printf ("%d", node->val);
      return;
    }

  printf ("(");
  dbg_print_ast (node->lhs);

  switch (node->kind)
    {
    case ND_ADD:
      printf (" + ");
      break;
    case ND_SUB:
      printf (" - ");
      break;
    case ND_MUL:
      printf (" * ");
      break;
    case ND_DIV:
      printf (" / ");
      break;
    default:
      printf (" ? ");
      break;
    }

  dbg_print_ast (node->rhs);
  printf (")");
}

void
dbg_print_tree (Node *node, int level)
{
  if (node == NULL)
    {
      return;
    }

  dbg_print_tree (node->rhs, level + 1);

  for (int i = 0; i < level; i++)
    {
      printf ("   ");
    }

  switch (node->kind)
    {
    case ND_NUM:
      printf ("%d\n", node->val);
      break;
    case ND_ADD:
      printf ("+\n");
      break;
    case ND_SUB:
      printf ("-\n");
      break;
    case ND_MUL:
      printf ("*\n");
      break;
    case ND_DIV:
      printf ("/\n");
      break;
    default:
      printf ("Unknown\n");
      break;
    }

  dbg_print_tree (node->lhs, level + 1);
}

Node *
new_node (NodeKind kind)
{
  Node *node = calloc (1, sizeof (Node));
  node->kind = kind;
  return node;
}

Node *
new_binary (NodeKind kind, Node *lhs, Node *rhs)
{
  Node *node = new_node (kind);
  node->lhs = lhs;
  node->rhs = rhs;

  return node;
}

Node *
new_num (int val)
{
  Node *node = new_node (ND_NUM);
  node->val = val;
  return node;
}
