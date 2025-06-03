/**
 * @file ast.c
 * @brief Implement ast related functions.
 */

#include "rvcc.h"

Node *
new_node (NodeKind kind)
{
  Node *node = calloc (1, sizeof (Node));
  node->kind = kind;
  return node;
}

Node *
new_unary (NodeKind kind, Node *expr)
{
  Node *node = new_node (kind);
  node->lhs = expr;

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
