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

  // 打印右子树，增加层级
  dbg_print_tree (node->rhs, level + 1);

  // 打印当前节点的内容，按层级进行缩进
  for (int i = 0; i < level; i++)
    {
      printf ("   "); // 每个层级缩进四个空格
    }

  // 打印节点值
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

  // 打印左子树，增加层级
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
