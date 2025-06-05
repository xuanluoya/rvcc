/**
 * @file codegen.h
 *
 * @brief AST to risc-v Assembly
 */

#include "rvcc.h"

/**
 * @brief Push noto stack.
 *
 * @param stack_deep Stack deep
 */
static void
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

/**
 * @brief Pop stack.
 *
 * Make value of sp pointer pop onto reg
 *
 * @param reg Register name.
 * @param stack_deep Stack deep
 */
static void
pop (char *reg, int *stack_deep)
{
  printf ("  ld %s, 0(sp)\n", reg);
  // Back stack top.
  printf ("  addi sp, sp, 8\n");
  stack_deep--;
}

/**
 * @brief Traverse the AST tree to generate assembly.
 *
 * @param node Tree node.
 */
static void
gen_expr (Node *node, int *stack_deep)
{
  // root node
  switch (node->kind)
    {
    // load number onto a0
    case ND_NUM:
      printf ("  li a0, %d\n", node->val);
      return;
    // Invert a register
    case ND_NEG:
      gen_expr (node->lhs, stack_deep);
      // neg a0, a0 is an alias for sub a0, x0, a0, i.e. a0=0-a0
      printf ("  neg a0, a0\n");
      return;
    default:
      break;
    }

  gen_expr (node->rhs, stack_deep);
  push (stack_deep);

  gen_expr (node->lhs, stack_deep);
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
    case ND_EQ:
    case ND_NE:
      // a0=a0^a1
      printf ("  xor a0, a0, a1\n");

      if (node->kind == ND_EQ)
        // a0==a1
        // a0=a0^a1, sltiu a0, a0, 1
        printf ("  seqz a0, a0\n");
      else
        // a0!=a1
        // a0=a0^a1, sltu a0, x0, a0
        printf ("  snez a0, a0\n");
      return;
    case ND_LT:
      printf ("  slt a0, a0, a1\n");
      return;
    case ND_LE:
      // a0<=a1
      // a0=a1<a0, a0=a1^1
      printf ("  slt a0, a1, a0\n");
      printf ("  xori a0, a0, 1\n");
      return;
    default:
      break;
    }

  if (stack_deep != 0)
    error ("The stack is not cleared");

  error ("invalid expression");
}

void
codegen (Node *node, int *stack_deep)
{
  printf ("  .globl main\n");
  printf ("main:\n");

  gen_expr (node, stack_deep);
  printf ("  ret\n");
}
