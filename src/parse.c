/**
 * @file parse.c
 * @brief Implement parsing related functions.
 */

#include "rvcc.h"

// expr = mul ("+" mul | "-" mul)*
Node *
expr (Token **rest, Token *tok)
{
  Node *node = mul (&tok, tok);

  // ("+" mul | "-" mul)*
  for (;;)
    {
      // "+" mul
      if (equal (tok, "+"))
        {
          node = new_binary (ND_ADD, node, mul (&tok, tok->next));
          continue;
        }
      // "-" mul
      if (equal (tok, "-"))
        {
          node = new_binary (ND_SUB, node, mul (&tok, tok->next));
          continue;
        }

      *rest = tok;
      return node;
    }
}

// Multiplication
// mul = unary ("*" unary | "/" unary)*
Node *
mul (Token **rest, Token *tok)
{
  // unary
  Node *node = unary (&tok, tok);

  // ("*" unary | "/" unary)*
  for (;;)
    {
      if (equal (tok, "*"))
        {
          node = new_binary (ND_MUL, node, unary (&tok, tok->next));
          continue;
        }

      if (equal (tok, "/"))
        {
          node = new_binary (ND_DIV, node, unary (&tok, tok->next));
          continue;
        }

      *rest = tok;
      return node;
    }
}

// unary = ("+" | "-") unary | primary
Node *
unary (Token **rest, Token *tok)
{
  // "+" unary
  if (equal (tok, "+"))
    return unary (rest, tok->next);

  // "-" unary
  if (equal (tok, "-"))
    return new_unary (ND_NEG, unary (rest, tok->next));

  return primary (rest, tok);
}

Node *
primary (Token **rest, Token *tok)
{
  if (equal (tok, "("))
    {
      Node *node = expr (&tok, tok->next);
      *rest = skip (tok, ")");
      return node;
    }

  if (tok->kind == TK_NUM)
    {
      Node *node = new_num (tok->val);
      *rest = tok->next;
      return node;
    }

  error_tok (tok, "expected an expression");
  return NULL;
}
