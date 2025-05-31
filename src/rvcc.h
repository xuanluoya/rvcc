/**
 * @file rvcc.h
 * @brief Include base head file and public function.
 */

#ifndef RVCC_H
#define RVCC_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @defgroup Token
 *
 * @{
 */

/**
 * @brief Token struct.
 *
 * This enumeration is used to define token types.
 * @enum TokenKind
 */
typedef enum
{
  TK_PUNCT, /**< Operator: `+`, `-`... */
  TK_NUM,   /**< Number */
  TK_EOF,   /**< End of file */
} TokenKind;

/**
 * @brief Token struct.
 *
 * This struct defines a token data structure.
 *
 * @struct Token
 */
typedef struct Token
{
  TokenKind kind;     /**< Token kind. */
  struct Token *next; /**< The next node. */
  int val;            /**< Value. */
  char *loc;          /**< The position in string. */
  int len;            /**< Length of token. */
} Token;

/** @} */

// @brief Input's string.
extern char *CurrentInput;

/**
 * @defgroup AST
 * @{
 */

/**
 * @brief AST node kinds.
 *
 * @enum NodeKind
 */
typedef enum
{
  ND_ADD, /**< + */
  ND_SUB, /**< - */
  ND_MUL, /**< * */
  ND_DIV, /**< / */
  ND_NUM, /**< Int */
} NodeKind;

/**
 * @brief AST node.
 *
 * @struct Node
 */
typedef struct Node
{
  NodeKind kind;    /**< Node kinds */
  struct Node *lhs; /**< left-hand side of tree */
  struct Node *rhs; /**< roght-hand side of tree */
  int val;          /**< Save value of ND_NUM */
} Node;

/** @} */

/**
 * @defgroup Error
 *
 * @brief Error Funtions.
 *
 * @{
 */

/**
 * @brief Print an error message and exit.
 *
 * @param fmt Format string for the error message.
 * @param ... Additional arguments for the format string.
 */
void error (char *fmt, ...);

/**
 * @brief Output the location where the error occurred.
 *
 * @param input Sourse input.
 * @param loc Location for the error message.
 * @param fmt Format string for the error message.
 * @param ap Variable argument list for the format string.
 */
void verror_at (char *loc, char *fmt, va_list ap);

/**
 * @brief Output string analysos error, and quit.
 *
 * @param loc Location for the error message.
 * @param fmt Format string for the error message.
 */
void error_at (char *loc, char *fmt, ...);

/**
 * @brief Output token analysos error, and quit.
 *
 * @param loc Location for the error message.
 * @param fmt Format string for the error message.
 */
void error_tok (Token *tok, char *fmt, ...);

/** @} */

/**
 * @defgroup Lexer
 *
 * @brief Lexer functions.
 *
 * @{
 */

/**
 * @brief Initialize new token.
 *
 * @param kind The token kind.
 * @param start position of pointer.
 * @param end the end position of pointer.
 * @return Token* Pointer to the newly created token.
 */
Token *new_token (TokenKind kind, char *start, char *end);

/**
 * @brief Get token number if kinds is TK_NUM.
 *
 * @param tok token
 * @return int token number
 */
int get_number (Token *tok);

/**
 * @brief Determines whether the value is a given value.
 *
 * @param tok token
 * @param str string to compare
 * @return bool true if equal, false otherwise
 */
bool equal (Token *tok, char *str);

/**
 * @brief If whether the value is given value then skip.
 *
 * @param tok token
 * @param str string to compare
 * @return Token* Pointer to the next token.
 */
Token *skip (Token *tok, char *str);

/**
 * @brief Analysis string to the token.
 *
 * This function is used to analyze tokens.
 *
 * @param p Is pointer to the string.
 * @return Token linked list.
 */
Token *tokenize (char *p);

/**
 * @brief Debug token.
 *
 * Show token information.
 *
 * @param tok Pointer to the token.
 */
void dbg_print_token (Token *tok);

/** @} */

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
Node *new_node (NodeKind kind);

/**
 * @brief Create new binary tree node.
 *
 * @param kind Node kind.
 * @param lhs left-hand side of tree.
 * @param rhs right-hand side of tree.
 * @return binary tree fork
 */
Node *new_binary (NodeKind kind, Node *lhs, Node *rhs);

/**
 * @brief Create new number node.
 *
 * @param val number value.
 * @return number node.
 */
Node *new_num (int val);

/**
 * @brief Print AST.
 *
 * @param node
 */
void dbg_print_ast (Node *node);

/**
 * @brief Print AST tree.
 *
 * @param node
 * @param level
 */
void dbg_print_tree (Node *node, int level);

/** @} */

/**
 * @defgroup Parser
 *
 * @brief Recursive Descent Parser
 * @{
 */

/**
 * @brief Parse addition and subtraction expressions.
 *
 * expr = mul ("+" mul | "-" mul)*
 *
 * @param Rest returns the unparsed Token
 * @param Tok the current parsing start Token
 * @return the root node of the abstract syntax tree
 */
Node *expr (Token **rest, Token *tok);

/**
 * @brief Parse multiplication and division expressions.
 *
 * mul = primary ("*" primary | "/" primary)*
 *
 * @param Rest Returns the unparsed Token
 * @param Tok The current parsing start Token
 * @return The node of the abstract syntax tree
 */
Node *mul (Token **rest, Token *tok);

/**
 * @brief Parse bracket expressions or numbers.
 *
 * primary = "(" expr ")" | num
 *
 * @param Rest Returns the unparsed Token
 * @param Tok The current parsing start Token
 * @return Leaf node or subtree of the abstract syntax tree
 */
Node *primary (Token **rest, Token *tok);

/** @} */

/**
 * @defgroup Assembly
 *
 * @brief generate assembly.
 */

/**
 * @brief Push noto stack.
 *
 * @param stack_deep Stack deep
 */
void push (int *stack_deep);

/**
 * @brief Pop stack.
 *
 * Make value of sp pointer pop onto reg
 *
 * @param reg Register name.
 * @param stack_deep Stack deep
 */
void pop (char *reg, int *stack_deep);

/**
 * @brief Traverse the AST tree to generate assembly.
 *
 * @param node Tree node.
 */
void gen_exper (Node *node, int *stack_deep);

/** @} */

#endif
