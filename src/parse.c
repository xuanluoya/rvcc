/**
 * @file parse.c
 * @brief Implement parsing related functions.
 */

#include "rvcc.h"

Node *
expr (Token **rest, Token *tok)
{
  Node *node = mul (&tok, tok);

  for (;;)
    {
      if (equal (tok, "+"))
        {
          node = new_binary (ND_ADD, node, mul (&tok, tok->next));
          continue;
        }
      if (equal (tok, "-"))
        {
          node = new_binary (ND_SUB, node, mul (&tok, tok->next));
          continue;
        }

      *rest = tok;
      return node;
    }
}

Node *
mul (Token **rest, Token *tok)
{
  Node *node = primary (&tok, tok);

  for (;;)
    {
      if (equal (tok, "*"))
        {
          node = new_binary (ND_MUL, node, primary (&tok, tok->next));
          continue;
        }

      if (equal (tok, "/"))
        {
          node = new_binary (ND_DIV, node, primary (&tok, tok->next));
          continue;
        }

      *rest = tok;
      return node;
    }
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
