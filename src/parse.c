/**
 * @file parse.c
 * @brief Implement parsing related functions.
 */

#include "rvcc.h"

/**
 * @defgroup AST
 *
 * @brief AST Funtions
 *
 * @{
 */

/**
 * @brief Create new AST node.
 *
 * @param node Node kinds.
 * @return Create new AST node.
 */
static Node *
new_node (NodeKind kind)
{
  Node *node = calloc (1, sizeof (Node));
  node->kind = kind;
  return node;
}

/**
 * @brief Create new unary tree.
 *
 * @param node Node kinds.
 * @param expr The left child of the current node.
 */
static Node *
new_unary (NodeKind kind, Node *expr)
{
  Node *node = new_node (kind);
  node->lhs = expr;

  return node;
}

/**
 * @brief Create new binary tree node.
 *
 * @param kind Node kind.
 * @param lhs left-hand side of tree.
 * @param rhs right-hand side of tree.
 * @return binary tree fork
 */
static Node *
new_binary (NodeKind kind, Node *lhs, Node *rhs)
{
  Node *node = new_node (kind);
  node->lhs = lhs;
  node->rhs = rhs;

  return node;
}

/**
 * @brief Create new number node.
 *
 * @param val number value.
 * @return number node.
 */
static Node *
new_num (int val)
{
  Node *node = new_node (ND_NUM);
  node->val = val;
  return node;
}

/** @} */

/**
 * @defgroup Parser
 *
 * @brief Recursive Descent Parser
 * @{
 */

// expr = equality
// equality = relational ("==" relational | "!=" relational)*
// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
// add = mul ("+" mul | "-" mul)*
// mul = unary ("*" unary | "/" unary)*
// unary = ("+" | "-") unary | primary
// primary = "(" expr ")" | num
static Node *unary (Token **rest, Token *tok);
static Node *mul (Token **rest, Token *tok);
static Node *add (Token **rest, Token *tok);
static Node *relational (Token **rest, Token *tok);
static Node *equality (Token **rest, Token *tok);
static Node *primary (Token **rest, Token *tok);

/**
 * @brief Parse addition and subtraction expressions.
 *
 * expr = equality
 *
 * @param Rest returns the unparsed Token
 * @param Tok the current parsing start Token
 * @return the root node of the abstract syntax tree
 */
static Node *
expr (Token **rest, Token *tok)
{
  return equality (rest, tok);
}

/**
 * @brife Parse equality
 *
 * equality = relational ("==" relational | "!=" relational)*
 *
 * @param Rest returns the unparsed Token
 * @param Tok the current parsing start Token
 * @return the root node of the abstract syntax tree
 */
static Node *
equality (Token **rest, Token *tok)
{
  Node *node = relational (&tok, tok);

  for (;;)
    {
      if (equal (tok, "=="))
        {
          node = new_binary (ND_EQ, node, relational (&tok, tok->next));
          continue;
        }

      if (equal (tok, "!="))
        {
          node = new_binary (ND_NE, node, relational (&tok, tok->next));
          continue;
        }

      *rest = tok;
      return node;
    }
}

/**
 * @brife Parse equality
 *
 * relational = add ("<" add | "<=" add | ">" add | ">=" add)*
 *
 * @param Rest returns the unparsed Token
 * @param Tok the current parsing start Token
 * @return the root node of the abstract syntax tree
 */
static Node *
relational (Token **rest, Token *tok)
{
  Node *node = add (&tok, tok);

  for (;;)
    {
      if (equal (tok, "<"))
        {
          node = new_binary (ND_LT, node, add (&tok, tok->next));
          continue;
        }

      // "<=" add
      if (equal (tok, "<="))
        {
          node = new_binary (ND_LE, node, add (&tok, tok->next));
          continue;
        }

      // ">" add
      // X>Y is Y<X
      if (equal (tok, ">"))
        {
          node = new_binary (ND_LT, add (&tok, tok->next), node);
          continue;
        }

      // ">=" add
      // X>=Y is Y<=X
      if (equal (tok, ">="))
        {
          node = new_binary (ND_LE, add (&tok, tok->next), node);
          continue;
        }

      *rest = tok;
      return node;
    }
}

/**
 * @brife Parse add
 *
 * add = mul ("+" mul | "-" mul)*
 *
 * @param Rest returns the unparsed Token
 * @param Tok the current parsing start Token
 * @return the root node of the abstract syntax tree
 */
static Node *
add (Token **rest, Token *tok)
{
  // mul
  Node *node = mul (&tok, tok);

  // ("+" mul | "-" mul)*
  while (true)
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

/**
 * @brief Parse multiplication and division expressions.
 *
 * mul = unary ("*" unary | "/" unary)*
 *
 * @param Rest Returns the unparsed Token
 * @param Tok The current parsing start Token
 * @return The node of the abstract syntax tree
 */
static Node *
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

/**
 * @brief Handling unary operations.
 *
 * unary = ("+" | "-") unary | primary
 *
 * @param Rest Returns the unparsed Token
 * @param Tok The current parsing start Token
 * @return The node of the abstract syntax tree
 */
static Node *
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

/**
 * @brief Parse bracket expressions or numbers.
 *
 * primary = "(" expr ")" | num
 *
 * @param Rest Returns the unparsed Token
 * @param Tok The current parsing start Token
 * @return Leaf node or subtree of the abstract syntax tree
 */
static Node *
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

Node *
parse (Token *tok)
{
  Node *Nd = expr (&tok, tok);
  if (tok->kind != TK_EOF)
    error_tok (tok, "extra token");
  return Nd;
}

/** @} */
